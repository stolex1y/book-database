#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    using is_transparent = void;

    auto operator()(const Book &lhs, const Book &rhs) const { return lhs.author < rhs.author; }
    auto operator()(const Book &lhs, std::string_view rhs) const { return lhs.author < rhs; }
    auto operator()(std::string_view lhs, const Book &rhs) const { return lhs < rhs.author; }
};

struct LessByPopularity {
    using is_transparent = void;

    auto operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count < rhs.read_count; }
    auto operator()(const Book &lhs, int rhs) const { return lhs.read_count < rhs; }
    auto operator()(int lhs, const Book &rhs) const { return lhs < rhs.read_count; }
};

struct LessByRating {
    using is_transparent = void;

    auto operator()(const Book &lhs, const Book &rhs) const { return lhs.rating < rhs.rating; }
    auto operator()(const Book &lhs, double rhs) const { return lhs.rating < rhs; }
    auto operator()(double lhs, const Book &rhs) const { return lhs < rhs.rating; }
};

template <BookComparator Comparator>
struct InvertedComparator {

    explicit InvertedComparator(Comparator c = {}) : comparator(std::move(c)) {}

    auto operator()(const Book &lhs, const Book &rhs) const { return !comparator(lhs, rhs); }

private:
    Comparator comparator;
};

}  // namespace bookdb::comp