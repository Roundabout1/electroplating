#include "config_reader.h"

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

#include <ini.h>

// A: Внутреннее состояние парсера: какие поля были заполнены.
// A: Нужно, чтобы после ini_parse проверить, что обязательные поля заданы.
struct ParserState {
    Config config;
    bool imagePathSet = false;
    bool csvFolderSet = false;
    bool colorSet = false;
    bool toleranceSet = false;
    std::string parseError; // A: Заполняется, если что-то пошло не так внутри handler.
};

// A: Парсит строку вида "R,G,B" в cv::Scalar(B, G, R).
// A: Возвращает true при успехе, false — при ошибке формата.
// A: Приватная функция, документируется здесь, т.к. не экспортируется.
static bool parseColor(const char* value, cv::Scalar& outColor) {
    int r = 0, g = 0, b = 0;
    int matched = std::sscanf(value, "%d,%d,%d", &r, &g, &b);
    if (matched != 3) {
        return false;
    }
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        return false;
    }
    outColor = cv::Scalar(b, g, r);
    return true;
}

// A: Callback для inih. void* user — это указатель на ParserState,
// A: переданный третьим аргументом в ini_parse.
static int configHandler(void* user, const char* section, const char* name, const char* value) {
    auto* state = static_cast<ParserState*>(user);
    const std::string sSection(section);
    const std::string sName(name);

    if (sSection == "paths") {
        if (sName == "image_path") {
            state->config.imagePath = value;
            state->imagePathSet = true;
        } else if (sName == "csv_folder") {
            state->config.csvFolderPath = value;
            state->csvFolderSet = true;
        }
    } else if (sSection == "background") {
        if (sName == "color") {
            if (!parseColor(value, state->config.backgroundBgr)) {
                state->parseError = "Неверный формат background.color (ожидается R,G,B в диапазоне 0..255): " + std::string(value);
                return 0; // A: Сигнал inih об ошибке.
            }
            state->colorSet = true;
        } else if (sName == "tolerance") {
            try {
                state->config.backgroundTolerance = std::stod(value);
            } catch (const std::exception&) {
                state->parseError = "Неверный формат background.tolerance: " + std::string(value);
                return 0;
            }
            if (state->config.backgroundTolerance < 0.0) {
                state->parseError = "background.tolerance не может быть отрицательным.";
                return 0;
            }
            state->toleranceSet = true;
        }
    }
    return 1;
}

Config readConfig(const std::string& configPath) {
    ParserState state;
    // A: Значение по умолчанию для необязательного параметра.
    state.config.backgroundTolerance = 10.0;

    const int parseResult = ini_parse(configPath.c_str(), configHandler, &state);

    if (parseResult < 0) {
        throw std::runtime_error("Не удалось открыть файл конфигурации: " + configPath);
    }
    if (parseResult > 0) {
        // A: parseResult > 0 — это номер строки с синтаксической ошибкой INI.
        throw std::runtime_error("Синтаксическая ошибка в " + configPath +
                                 " на строке " + std::to_string(parseResult));
    }
    if (!state.parseError.empty()) {
        throw std::runtime_error(state.parseError);
    }

    // A: Проверка обязательных полей.
    if (!state.imagePathSet) {
        throw std::runtime_error("В config.ini отсутствует обязательное поле [paths] image_path");
    }
    if (!state.csvFolderSet) {
        throw std::runtime_error("В config.ini отсутствует обязательное поле [paths] csv_folder");
    }
    if (!state.colorSet) {
        throw std::runtime_error("В config.ini отсутствует обязательное поле [background] color");
    }

    return state.config;
}