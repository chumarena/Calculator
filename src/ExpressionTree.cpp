#include "ExpressionTree.h"
#include <stack>
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;

ExpressionTree::ExpressionTree(const map<string, unique_ptr<IPlugin>>& availablePlugins)
    : plugins(availablePlugins) {
}

bool ExpressionTree::is_operator_or_function(const string& token) const {
    if (token == "~") { 
        return true;
    }
    return plugins.count(token);
}

void ExpressionTree::build(const vector<string>& rpn_tokens) {
    stack<unique_ptr<Node>> node_stack;

    for (const string& token : rpn_tokens) {
        if (is_operator_or_function(token)) {
            OpType type;
            
            if (token == "~") {
                type = OpType::UNARY;
            }
            else if (plugins.count(token)) {
                type = plugins.at(token)->getType(); 
            }
            else {
                throw runtime_error("Unknown token type during build: " + token);
            }

            if (type == OpType::BINARY) {
                if (node_stack.size() < 2) throw runtime_error("Error not enough operators of binary operator " + token);

                unique_ptr<Node> right = move(node_stack.top()); node_stack.pop();
                unique_ptr<Node> left = move(node_stack.top()); node_stack.pop();
                node_stack.push(make_unique<Node>(token, move(left), move(right)));
            }
            else { 
                if (node_stack.empty()) throw runtime_error("Error not enough operangs for unary operator " + token);

                unique_ptr<Node> arg = move(node_stack.top()); node_stack.pop();
                node_stack.push(make_unique<Node>(token, nullptr, move(arg)));
            }

        }
        else {
            node_stack.push(make_unique<Node>(token));
        }
    }

    if (node_stack.size() != 1) {
        throw runtime_error("Error: Incorrect number of tokens in the expression.");
    }
    root = move(node_stack.top());
}

double ExpressionTree::evaluate() const {
    if (!root) return 0.0;
    return evaluate_recursive(root.get());
}

double ExpressionTree::evaluate_recursive(const Node* node) const {
    if (!node) throw runtime_error("Node is empty.");

    
    if (!node->left && !node->right) {
        double value;
        std::stringstream ss(node->value);

        if (ss >> value) {
            return value;
        }
        else {
            throw runtime_error("Invalid operand: " + node->value);
        }
    }

	//Унарный минус
    if (node->value == "~") {
        double arg = evaluate_recursive(node->right.get());
        return -arg;
    }

    
    auto it = plugins.find(node->value);
    if (it != plugins.end()) {
        IPlugin* plugin = it->second.get();

        if (plugin->getType() == OpType::UNARY) {
            double arg = evaluate_recursive(node->right.get());
            return plugin->calculate(arg);
        }
        else if (plugin->getType() == OpType::BINARY) {
            double left_val = evaluate_recursive(node->left.get());
            double right_val = evaluate_recursive(node->right.get());
            return plugin->calculate(left_val, right_val);
        }
    }

    throw runtime_error("Unknown operator or function: " + node->value);
}