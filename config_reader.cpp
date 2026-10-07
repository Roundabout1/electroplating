#include "config_reader.h"

#include <ini.h>

#include <stdexcept>
#include <string>

// A: Приватная функция-обработчик для inih.
static int configHandler(void* user, const char* section, const char* name, const char* value) {
    auto* config = static_cast<Config*>(user);
    std::string sSection(section);
    std::string sName(name);

    if (sSection == "paths") {
        if (sName == "image_path") {
            config->imagePath = value;
        } else if (sName == "csv_folder") {
            config->csvFolderPath = value;
        }
    } else if (sSection == "background") {
        if (sName == "color") {
            // A: Ожидается формат "R,G,B" или "B,G,R"? Для OpenCV используем BGR.
            // A: Примем формат "R,G,B" и преобразуем в BGR.
            int r, g, b;
            if (sscanf(value, "%d,%d,%d", &r, &g, &b) == 3) {
                config->backgroundBgr = cv::Scalar(b, g, r);
            } else {
                return 0; // A: Ошибка парсинга
            }
        } else if (sName == "tolerance") {
            config->backgroundTolerance = std::stod(value);
        }
    }
    return 1;
}

Config readConfig(const std::string& configPath) {
    Config config;
    // A: Значения по умолчанию
    config.backgroundTolerance = 10.0;

    if (ini_parse(configPath.c_str(), configHandler, &config) < 0) {
        throw std::runtime_error("Не удалось открыть файл конфигурации: " + configPath);
    }

    if (config.imagePath.empty() || config.csvFolderPath.empty()) {
        throw std::runtime_error("В конфигурации не указаны обязательные пути.");
    }

    return config;
}