#pragma once

#include "book.hpp"

#include <concepts>
#include <iterator>

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T t, Book book) {
    t.emplace_back(book);
    requires std::ranges::range<T>;
    { t.size() } -> std::same_as<std::size_t>;
    t.clear();
};

template <typename T>
concept BookIterator = std::forward_iterator<T> && std::convertible_to<typename T::value_type, const Book>;

template <typename P>
concept BookPredicate = std::is_invocable_r_v<bool, P, const Book &>;

template <typename C, typename T>
concept Comparator = std::is_invocable_r_v<bool, C, const T &, const T &>;

template <typename C>
concept BookComparator = Comparator<C, Book>;

}  // namespace bookdb