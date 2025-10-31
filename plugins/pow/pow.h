#pragma once

#include "Iplugin.h"
#include <cmath>
#include <stdexcept>

class PowerPlugin : public IPlugin {
public:
    std::string getName() const override { return "^"; }
    OpType getType() const override { return OpType::BINARY; }
    double calculate(double a, double b = 0.0) override;
};