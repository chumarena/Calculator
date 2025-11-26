#include "plugin.h" 
#include <cmath>
#include <stdexcept>

class LnPlugin : public IPlugin {
public:
    std::string getName() const override { return "ln"; }
    OpType getType() const override { return OpType::UNARY; }
    int getPrecedence() const override { return 4; } // ДОБАВЛЕНО: Приоритет функций (самый высокий)
    

    double calculate(double a, double b = 0.0) override {
        if (a <= 0.0) {
            throw std::runtime_error("Error ln of a non-positive number");
        }
        return std::log(a); 
    }
};


extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new LnPlugin();
}