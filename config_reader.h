#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>

#include <opencv2/core/types.hpp>

/**
 * @brief Структура, содержащая параметры конфигурации.
 */
struct Config {
    std::string imagePath;          /**< Путь к файлу изображения. */
    std::string csvFolderPath;      /**< Путь к папке с CSV-таблицами. */
    cv::Scalar backgroundBgr;     /**< Цвет фона для игнорирования (BGR). */
    double backgroundTolerance;     /**< Допуск для сравнения цвета фона. */
};

/**
 * @brief Читает конфигурацию из INI-файла.
 *
 * @param configPath Путь к файлу конфигурации.
 * @return Структура Config с параметрами.
 * @throws std::runtime_error Если файл не найден или параметры отсутствуют.
 */
Config readConfig(const std::string& configPath);
#endif // CONFIG_READER_H
