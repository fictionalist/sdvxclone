#include "Configuration.hpp"

#include <map>
#include <string>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include "Logging.hpp"

namespace Configuration {
    nlohmann::json settings;

    bool generateDefaultSettings();
}

bool Configuration::generateDefaultSettings() {
    settings["graphics"]["resolution"] = "1280x720";
    settings["graphics"]["displayMode"] = "windowed";
    settings["gameplay"]["drawOffset"] = 0;
    settings["gameplay"]["soundOffset"] = 0;
    settings["gameplay"]["laneSpeed"] = 1.00;

    if (!save()) {
        Logging::error("Failed to create default settings file!");
        return false;
    }
    return true;
}

bool Configuration::init() {
    std::ifstream settingsFile("./settings.json");

    if (!settingsFile.is_open()) {
        if (!generateDefaultSettings()) {
            return false;
        }
    } else {
        settings = nlohmann::json::parse(settingsFile);
    }

    printf("%s\n", settings["graphics"]["resolution"].get<std::string>().c_str());

    return true;
}

bool Configuration::save() {
    std::ofstream settingsFile("./settings.json", std::ios::trunc);
    if (!settingsFile.is_open()) {
        Logging::error("Failed to save settings file!");
        return false;
    }
    settingsFile << settings.dump(4);
    settingsFile.close();
    return true;
}