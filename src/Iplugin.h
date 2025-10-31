#pragma once

#include <string>
#include <memory>

enum class OpType {
    UNARY,  // sin, cos, ln (принимает 1 операнд)
    BINARY  // ^ (принимает 2 операнда)
};

class IPlugin {
public:
    virtual ~IPlugin() = default;

    virtual std::string getName() const = 0;

    virtual OpType getType() const = 0;

    virtual double calculate(double a, double b = 0.0) = 0;
};

// Точка входа в DLL
extern "C" __declspec(dllexport) IPlugin* createPlugin();