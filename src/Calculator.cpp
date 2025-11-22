#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <locale> 

#ifdef _WIN32
#include <windows.h>
#endif

#include "PluginLoader.h" 
#include "Parser.h"      
#include "ExpressionTree.h" 

using namespace std;

void print_initial_status(const PluginLoader& loader) {
    const auto& availablePlugins = loader.getAvailablePlugins();
    
    std::cout << "Calculator\n";
    std::cout << "Base operations: +, -, *, /\n";
    std::cout << "Loaded plugins: ";
    if (availablePlugins.empty()) {
        std::cout << "No\n";
    }
    else {
        for (const auto& pair : availablePlugins) {
            std::cout << pair.first << "(" << (pair.second->getType() == OpType::UNARY ? "unary" : "binary") << ") ";
        }
        std::cout << "\n";
    }
    std::cout << "Enter 'exit' or 'quit' to finish.\n";
}


int main() {
    
    #ifdef _WIN32
        SetConsoleOutputCP(65001); 
    #endif
    setlocale(LC_ALL, "ru");
    std::cout.imbue(std::locale(""));
    std::cerr.imbue(std::locale(""));
    
    PluginLoader loader;

  
    try {
        loader.loadPlugins("plugins"); 
    }
    catch (const std::exception& e) {
        std::cerr << "Error in loading plugins: " << e.what() << "\n";
    }
    
    print_initial_status(loader);

    std::string input;
    while (true) {
        
        //Сначала ждем ввод
        std::cout << "\nEnter the expression: ";
        std::getline(std::cin, input);

        std::string lower_input = input;
        std::transform(lower_input.begin(), lower_input.end(), lower_input.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (lower_input == "exit" || lower_input == "quit" || std::cin.eof()) {
            std::cout << "Completion of the Calculator.\n";
            break;
        }

        if (input.empty()) continue;
        
        
        try {
            loader.loadPlugins("plugins"); 
        }
        catch (const std::exception& e) {
            std::cerr << "Warning: Problem when updating plugins: " << e.what() << "\n";
        }
        
   
        const auto& currentPlugins = loader.getAvailablePlugins();
        Parser parser(currentPlugins); 

        try {
            std::vector<std::string> tokens = parser.tokenize(input);
            std::vector<std::string> rpn_tokens = parser.infix_to_rpn(tokens);

            ExpressionTree tree(currentPlugins); 
            tree.build(rpn_tokens);
            double result = tree.evaluate();

            std::cout << "Result: " << result << "\n";

        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        catch (...) {
            std::cerr << "Unknown error.\n";
        }
    }

    return 0;
}