#include "plugin.h" 
#include <stdexcept>

class DividePlugin : public IPlugin {
public:
    std::string getName() const override { return "/"; }
    OpType getType() const override { return OpType::BINARY; }
    int getPrecedence() const override { return 2; } // Приоритет 2 (средний)
    
    double calculate(double a, double b = 0.0) override {
        if (b == 0.0) {
            throw std::runtime_error("Division by zero is not impossible.");
        }
        return a / b; 
    }
};

extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new DividePlugin();
}