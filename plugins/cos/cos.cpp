
#include "pch.h"

#include "Cos.h" // Использует свой заголовочный файл

double CosPlugin::calculate(double a, double b) {
    return std::cos(a);
}

extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new CosPlugin();
}