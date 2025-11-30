#pragma once

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

template <BookIterator Iterator, BookPredicate Predicate>
[[nodiscard]] std::vector<std::reference_wrapper<Book>> FilterBooks(Iterator begin, Iterator end, Predicate predicate) {
    std::vector<std::reference_wrapper<Book>> result;
    std::copy_if(begin, end, std::back_inserter(result), std::move(predicate));
    return result;
}

[[nodiscard]] constexpr auto YearBetween(uint32_t from, uint32_t to) {
    return [from, to](const Book &book) { return book.year >= from and book.year <= to; };
}

[[nodiscard]] constexpr auto RatingAbove(double lower_bound) {
    return [lower_bound](const Book &book) { return book.rating >= lower_bound; };
}

[[nodiscard]] constexpr auto GenreIs(Genre genre) {
    return [genre](const Book &book) { return book.genre == genre; };
}

template <BookPredicate... Predicates>
[[nodiscard]] constexpr auto all_of(Predicates... preds) {
    return [predicates = std::make_tuple(std::move(preds)...)](const Book &book) {
        const auto impl = [&predicates, &book]<size_t... Ids>(std::index_sequence<Ids...>) {
            return (... && std::get<Ids>(predicates)(book));
        };
        return impl(std::make_index_sequence<sizeof...(Predicates)>{});
    };
}

template <BookPredicate... Predicates>
[[nodiscard]] constexpr auto any_of(Predicates... preds) {
    return [predicates = std::make_tuple(std::move(preds)...)](const Book &book) {
        const auto impl = [&predicates, &book]<size_t... Ids>(std::index_sequence<Ids...>) {
            return (... || std::get<Ids>(predicates)(book));
        };
        return impl(std::make_index_sequence<sizeof...(Predicates)>{});
    };
}

}  // namespace bookdb