// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

#include "sin.h" 

double SinPlugin::calculate(double a, double b) {
    return std::sin(a);
}

extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new SinPlugin();
}