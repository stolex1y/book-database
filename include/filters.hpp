#pragma once

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

template <BookIterator Iterator, BookPredicate Predicate>
std::vector<std::reference_wrapper<Book>> FilterBooks(Iterator begin, Iterator end, Predicate predicate) {
    std::vector<std::reference_wrapper<Book>> result;
    std::copy_if(begin, end, std::back_inserter(result), std::move(predicate));
    return result;
}

struct YearBetween {
    bool operator()(const Book &book) const { return book.year >= from and book.year <= to; }

    uint32_t from;
    uint32_t to;
};

struct RatingAbove {
    bool operator()(const Book &book) const { return book.rating >= lower_bound; }

    double lower_bound;
};

struct GenreIs {
    bool operator()(const Book &book) const { return book.genre == genre; }

    Genre genre;
};

template <BookPredicate... Predicates>
struct all_of {
    explicit all_of(Predicates... preds) : predicates{std::move(preds)...} {}

    bool operator()(const Book &book) const {
        const auto impl = [this, &book]<size_t... Ids>(std::index_sequence<Ids...>) {
            return (... && std::get<Ids>(predicates)(book));
        };
        return impl(std::make_index_sequence<sizeof...(Predicates)>{});
    }

    std::tuple<Predicates...> predicates;
};

template <BookPredicate... Predicates>
struct any_of {
    explicit any_of(Predicates... preds) : predicates{std::move(preds)...} {}

    bool operator()(const Book &book) const {
        const auto impl = [this, &book]<size_t... Ids>(std::index_sequence<Ids...>) {
            return (... || std::get<Ids>(predicates)(book));
        };
        return impl(std::make_index_sequence<sizeof...(Predicates)>{});
    }

    std::tuple<Predicates...> predicates;
};

}  // namespace bookdb