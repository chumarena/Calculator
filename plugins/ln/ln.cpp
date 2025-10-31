// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

#include "ln.h" // Использует свой заголовочный файл

double LnPlugin::calculate(double a, double b) {
    if (a <= 0) {
        throw std::runtime_error("ln: аргумент должен быть > 0.");
    }
    return std::log(a);
}

extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new LnPlugin();
}