#include "PluginLoader.h"
#include <iostream>
#include <filesystem>
#include <stdexcept>


namespace fs = std::filesystem;
using namespace std;



PluginHandle PluginLoader::loadLibrary(const string& path) {
    return LoadLibraryA(path.c_str());
}
void* PluginLoader::getFunction(PluginHandle handle, const string& funcName) {
    return (void*)GetProcAddress(handle, funcName.c_str());
}
void PluginLoader::freeLibrary(PluginHandle handle) {
    if (handle) FreeLibrary(handle);
}


PluginLoader::PluginLoader() {}

PluginLoader::~PluginLoader() {
    
    plugins.clear();
    
    for (PluginHandle handle : handles) {
        freeLibrary(handle);
    }
}

void PluginLoader::loadPlugins(const std::string& pluginDir) {
    if (!fs::exists(pluginDir) || !fs::is_directory(pluginDir)) {
        
        throw std::runtime_error("Package of plugins wasn't founded " + pluginDir);
    }

    plugins.clear();
    for (PluginHandle handle : handles) freeLibrary(handle);
    handles.clear();

    for (const auto& entry : fs::directory_iterator(pluginDir)) {
        
        //Проверяем, что это DLL-файл
        if (!entry.is_regular_file() || entry.path().extension() != ".dll") {
            continue;
        }
        
        std::string path = entry.path().string();
        PluginHandle handle = loadLibrary(path);

        //Проверяем загрузку библиотеки
        if (!handle) {
            std::cerr << "Error, can't load library " << path << std::endl;
            continue;
        }
        
        handles.push_back(handle);

        using CreatePluginFunc = IPlugin * (*)();
        CreatePluginFunc creator = (CreatePluginFunc)getFunction(handle, "createPlugin");

        //Проверяем наличие функции createPlugin
        if (!creator) {
            std::cerr << "Error, not found function 'createPlugin' in " << path << std::endl;
            continue; 
        }

        IPlugin* pluginPtr = creator();
        
        //Проверяем успешность создания плагина
        if (!pluginPtr) {
            std::cerr << "Error  createPlugin returned nullptr for " << path << std::endl;
            continue; 
        }
        
        std::string name = pluginPtr->getName();
        
        //Проверяем дубликат
        if (plugins.count(name)) {
            std::cerr << "Plugin '" << name << "' already loaded. Skip." << std::endl;
            delete pluginPtr;
            continue;
        }

        
        plugins[name] = std::unique_ptr<IPlugin>(pluginPtr);
         
    }
}

const map<string, unique_ptr<IPlugin>>& PluginLoader::getAvailablePlugins() const {
    return plugins;
}