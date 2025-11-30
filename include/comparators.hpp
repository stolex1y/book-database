#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    using is_transparent = void;

    [[nodiscard]] auto operator()(const Book &lhs, const Book &rhs) const { return lhs.author < rhs.author; }
    [[nodiscard]] auto operator()(const Book &lhs, std::string_view rhs) const { return lhs.author < rhs; }
    [[nodiscard]] auto operator()(std::string_view lhs, const Book &rhs) const { return lhs < rhs.author; }
};

struct LessByPopularity {
    using is_transparent = void;

    [[nodiscard]] auto operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count < rhs.read_count; }
    [[nodiscard]] auto operator()(const Book &lhs, int rhs) const { return lhs.read_count < rhs; }
    [[nodiscard]] auto operator()(int lhs, const Book &rhs) const { return lhs < rhs.read_count; }
};

struct LessByRating {
    using is_transparent = void;

    [[nodiscard]] auto operator()(const Book &lhs, const Book &rhs) const { return lhs.rating < rhs.rating; }
    [[nodiscard]] auto operator()(const Book &lhs, double rhs) const { return lhs.rating < rhs; }
    [[nodiscard]] auto operator()(double lhs, const Book &rhs) const { return lhs < rhs.rating; }
};

template <BookComparator Comparator>
struct InvertedComparator {

    explicit InvertedComparator(Comparator c = {}) : comparator(std::move(c)) {}

    [[nodiscard]] auto operator()(const Book &lhs, const Book &rhs) const { return !comparator(lhs, rhs); }

private:
    Comparator comparator;
};

}  // namespace bookdb::comp