#pragma once

#include "book_database.hpp"
#include "comparators.hpp"

#include <algorithm>
#include <flat_map>
#include <random>
#include <ranges>
#include <string_view>

namespace bookdb {

template <BookContainerLike T, Comparator<std::string_view> Comparator = TransparentStringLess>
auto BuildAuthorHistogramFlat(const BookDatabase<T> &db, Comparator comp = {}) {
    // N - books, M - authors
    // mem: O(M), time: O(N + M*log(M))

    std::flat_map<std::string_view, size_t, Comparator> histogram{comp};
    std::vector<std::string_view> authors{std::from_range, db.GetAuthors()};  // mem: O(M)
    std::ranges::sort(authors, comp);                                         // time: O(M*log(M))

    std::unordered_map<std::string_view, size_t, TransparentStringHash, TransparentStringEqual> book_counters{
        authors.size()};           // mem: O(M)
    for (const auto &book : db) {  // time: O(N)
        ++book_counters[book.author];
    }

    std::vector<size_t> book_count_by_author;  // mem: O(M)
    book_count_by_author.reserve(authors.size());
    for (const auto author : authors) {  // time: O(M)
        book_count_by_author.emplace_back(book_counters[author]);
    }

    histogram.replace(std::move(authors), std::move(book_count_by_author));
    return histogram;
}

template <BookContainerLike T, Comparator<Genre> Comparator = std::less<>>
auto CalculateGenreRatings(const BookDatabase<T> &db, Comparator comp = {}) {
    // N - books, M - genres
    // mem: O(M), time: O(N + M*log(M))

    std::flat_map<Genre, double, Comparator> histogram{comp};

    // получение отсортированного списка всех жанров
    std::vector<Genre> genres;  // mem: O(M)
    genres.reserve(static_cast<size_t>(Genre::Count));
    for (size_t i = 0; i < static_cast<size_t>(Genre::Count); ++i) {  // time: O(M)
        genres.emplace_back(static_cast<Genre>(i));
    }
    std::ranges::sort(genres, comp);  // time: O(M*log(M))

    struct Counter {
        [[nodiscard]] double Average() const {
            if (count == 0) {
                return 0;
            }
            return rating / count;
        }

        auto operator<=>(const Counter &) const = default;

        double rating;
        size_t count;
    };

    std::unordered_map<Genre, Counter, std::hash<Genre>, std::equal_to<>> genre_rating_counters{
        genres.size()};            // mem: O(M)
    for (const Book &book : db) {  // time: O(N)
        auto &counter = genre_rating_counters[book.genre];
        ++counter.count;
        counter.rating += book.rating;
    }

    std::erase_if(genres, [&](const auto genre) { return genre_rating_counters[genre].count == 0; });  // time: O(M)

    std::vector<double> average_rating_by_genre;  // mem: O(M)
    average_rating_by_genre.reserve(genres.size());
    for (const auto genre : genres) {  // time: O(M)
        average_rating_by_genre.emplace_back(genre_rating_counters[genre].Average());
    }

    histogram.replace(std::move(genres), std::move(average_rating_by_genre));
    return histogram;
}

template <BookContainerLike T>
double CalculateAverageRating(const BookDatabase<T> &db) {
    return std::transform_reduce(db.begin(), db.end(), 0.0, std::plus{}, [](const Book &book) { return book.rating; }) /
           db.size();
}

template <BookContainerLike T>
std::vector<std::reference_wrapper<const Book>> SampleRandomBooks(const BookDatabase<T> &db, size_t sample_count) {
    std::vector<std::reference_wrapper<const Book>> samples;
    samples.reserve(sample_count);
    std::sample(db.begin(), db.end(), std::back_inserter(samples), sample_count, std::mt19937{std::random_device{}()});
    return samples;
}

template <BookContainerLike T, BookComparator Comparator>
std::vector<std::reference_wrapper<const Book>> GetTopNBy(BookDatabase<T> &db, size_t count,
                                                          Comparator comparator = {}) {
    auto sorted_end = std::next(db.begin(), count);
    std::partial_sort(db.begin(), sorted_end, db.end(), comp::InvertedComparator{std::move(comparator)});
    return {db.begin(), sorted_end};
}

}  // namespace bookdb
