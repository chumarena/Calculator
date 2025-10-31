#pragma once

#include "IPlugin.h"
#include <cmath>

class CosPlugin : public IPlugin {
public:
    std::string getName() const override { return "cos"; }
    OpType getType() const override { return OpType::UNARY; }
    // a = аргумент
    double calculate(double a, double b = 0.0) override;
};