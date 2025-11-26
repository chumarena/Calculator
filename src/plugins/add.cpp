#include "plugin.h" 

class AddPlugin : public IPlugin {
public:
    std::string getName() const override { return "+"; }
    OpType getType() const override { return OpType::BINARY; }
    int getPrecedence() const override { return 1; } // Приоритет 1 (низший)
    
    double calculate(double a, double b = 0.0) override {
        return a + b; 
    }
};

extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new AddPlugin();
}