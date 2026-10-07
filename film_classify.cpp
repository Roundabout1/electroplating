#include "film_classify.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include "csv.hpp"

std::vector<FilmClassification> ReadFilmClassesCsv(const std::string& path, const bool includeBgr, const size_t lambdaRangesNum) {
    csv::CSVFormat fmt;
    fmt.trim({ ' ', '\t' });            // аккуратно обрезаем пробелы
    fmt.variable_columns(csv::VariableColumnPolicy::THROW); // строгая схема

    auto reader = [path, fmt] () {
        try {
            return csv::CSVReader(path, fmt);
        } catch (const std::exception& e) {
            throw std::runtime_error(
                "ReadFilmClassesCsv: не могу открыть/извлечь " + path + ": " + e.what());
        }
    } ();

    std::vector<FilmClassification> classes;
    classes.reserve(reader.n_rows());

    for (const csv::CSVRow& row : reader) {
        try {
            FilmClassification fc {
                // A!: TODO
            };
            classes.push_back(fc);
        } catch (const std::exception& e) {
            throw std::runtime_error(
                "ReadFilmClassesCsv: неправильная строка в " + path + ": " + e.what());
        }
    }
    return classes;
}

void WriteFilmClassificationCsv(const std::string& path, const std::vector<FilmClassification>& filmClassifications, const bool includeBgr, const size_t lambdaRangesNum) {
    // A!: TODO
}