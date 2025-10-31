// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

#include "pow.h" // Использует свой заголовочный файл

double PowerPlugin::calculate(double a, double b) {
    return std::pow(a, b);
}

extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new PowerPlugin();
}