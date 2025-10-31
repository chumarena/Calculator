#pragma once

#include "IPlugin.h"
#include <cmath>

class SinPlugin : public IPlugin {
public:
    std::string getName() const override { return "sin"; }
    OpType getType() const override { return OpType::UNARY; }
    
    double calculate(double a, double b = 0.0) override;
};