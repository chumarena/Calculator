#include "plugin.h"
#include <cmath>

class PowPlugin : public IPlugin {
public:
    std::string getName() const override { return "^"; } 
    OpType getType() const override { return OpType::BINARY; }
    int getPrecedence() const override { return 3; } // ДОБАВЛЕНО: Приоритет выше * и /
    
    double calculate(double a, double b) override {
        return std::pow(a, b); 
    }
};

extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new PowPlugin();
}