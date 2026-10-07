#ifndef FILM_CLASSIFY_H
#define FILM_CLASSIFY_H

#include <string>
#include <vector>
#include <optional>

#include "opencv2/core/matx.hpp"

#include "range.hpp"

/**
 * @brief Описание одного класса тонкоплёночной интерференционной окраски.
 *
 * Соответствует одной строке CSV-таблицы цветов. Для цветов первого порядка
 * используется один диапазон длин волн (`lambda` длины 1), для цветов второго
 * порядка — два диапазона (`lambda` длины 2), так как интерференционные
 * максимумы второго порядка наблюдаются на двух длинах волн одновременно.
 */
struct FilmClassification {
    /// Название цвета (например, "gold-yellow", "cyan").
    const std::string                name;
    /// Порядок интерференции: 1 или 2.
    const int                        order;
    /// Диапазоны длин волн (нм). Длина 1 для I порядка, 2 для II порядка.
    const std::vector<Range<double>> lambda;
    /// Диапазон толщин плёнки (нм), соответствующий данному цвету.
    const Range<double>              thickness;
    /// Эталонный цвет BGR, если он присутствует в таблице (иначе std::nullopt).
    const std::optional<cv::Vec3b>   refBgr;
};

/**
 * @brief Считывает таблицу классов окраски из CSV-файла.
 *
 * Формат файла зависит от порядка интерференции: для первого порядка
 * используется одна пара колонок `lambdaMin`/`lambdaMax`, для второго —
 * две пары (`lambdaMin1`/`lambdaMax1`, `lambdaMin2`/`lambdaMax2`). Колонка
 * с эталонным цветом BGR читается только если `includeBgr == true`.
 *
 * @param path Путь к CSV-файлу с таблицей цветов.
 * @param includeBgr Читать ли колонки с эталонным цветом BGR.
 * @param lambdaRangesNum Ожидаемое число диапазонов длин волн на строку
 *        (1 для I порядка, 2 для II порядка).
 * @return Вектор прочитанных классов окраски.
 * @throws std::runtime_error Если файл не удаётся открыть, схема колонок
 *         не соответствует ожидаемой или строка содержит некорректные данные.
 */
std::vector<FilmClassification> ReadFilmClassificationCsv(const std::string& path, const bool includeBgr, const size_t lambdaRangesNum);

/**
 * @brief Записывает таблицу классов окраски в CSV-файл.
 *
 * Формат записи обратен формату ReadFilmClassificationCsv: для первого
 * порядка пишется одна пара колонок длин волн, для второго — две. Колонки
 * с эталонным цветом BGR пишутся только если `includeBgr == true`.
 *
 * @param path Путь к выходному CSV-файлу.
 * @param filmClassifications Классы окраски для записи.
 * @param includeBgr Писать ли колонки с эталонным цветом BGR.
 * @param lambdaRangesNum Число диапазонов длин волн на строку
 *        (1 для I порядка, 2 для II порядка).
 * @throws std::runtime_error Если файл не удаётся открыть для записи или
 *         число диапазонов длин волн у класса не совпадает с ожидаемым.
 */
void WriteFilmClassificationCsv(const std::string& path, const std::vector<FilmClassification>& filmClassifications, const bool includeBgr, const size_t lambdaRangesNum);

#endif // FILM_CLASSIFY_H