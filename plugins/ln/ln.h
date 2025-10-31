#pragma once

#include "IPlugin.h"
#include <cmath>
#include <stdexcept>

class LnPlugin : public IPlugin {
public:
    std::string getName() const override { return "ln"; }
    OpType getType() const override { return OpType::UNARY; }
    // a = аргумент
    double calculate(double a, double b = 0.0) override;
};