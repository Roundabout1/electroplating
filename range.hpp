#ifndef RANGE_H
#define RANGE_H

#include <ranges>

/**
 * @brief Замкнутый числовой диапазон [low, high].
 *
 * Используется для хранения диапазонов длин волн и толщин плёнок в
 * таблицах цветов, а также для проверки попадания значений в границы.
 *
 * @tparam T Тип границ диапазона (например, double).
 */
template <typename T>
struct Range {
    /// Нижняя граница диапазона (включительно).
    const T low;
    /// Верхняя граница диапазона (включительно).
    const T high;
};

/**
 * @brief Проверяет, лежит ли скалярное значение вне диапазона.
 *
 * @tparam T Тип границ диапазона.
 * @param value Проверяемое значение.
 * @param range Диапазон, относительно которого выполняется проверка.
 * @return true, если значение меньше low или больше high, иначе false.
 */
template <typename T>
bool OutOfRange(const auto& value, const Range<T>& range) {
    return value < range.low || value > range.high;
}

/**
 * @brief Проверяет, лежит ли хотя бы один элемент диапазона-контейнера вне
 *        заданного числового диапазона.
 *
 * @tparam T Тип границ числового диапазона.
 * @tparam R Тип контейнера, удовлетворяющий std::ranges::range.
 * @param iterable Контейнер значений для проверки.
 * @param range Числовой диапазон, относительно которого выполняется проверка.
 * @return true, если хотя бы один элемент выходит за границы, иначе false.
 */
template <typename T, typename R> requires std::ranges::range<R>
bool OutOfRange(const R& iterable, const Range<T>& range) {
    for (auto i : iterable) {
        if (OutOfRange(i, range)) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Ограничивает значение заданным диапазоном (clamp).
 *
 * @tparam T Тип границ диапазона.
 * @param value Значение, которое нужно ограничить.
 * @param range Диапазон, в границы которого ограничивается значение.
 * @return value, если оно внутри диапазона; low, если меньше low; high,
 *         если больше high.
 */
template <typename T>
T ToRange(const T& value, const Range<T>& range) {
    return std::min(std::max(range.low, value), range.high);
}

#endif // RANGE_H