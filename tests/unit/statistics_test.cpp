#include "statsistics.hpp"

#include <gtest/gtest.h>

namespace bookdb::test {

using BookDb = BookDatabase<>;

class StatisticsTest : public testing::Test {};

TEST_F(StatisticsTest, build_author_histogram_flat) {
    using Comparator = std::greater<>;

    // clang-format off
    BookDatabase db{{
        {"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
        {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120},
        {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
        {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156},
        {"Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178},
        {"The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112},
        {"Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98},
        {"Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110},
        {"The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203},
        {"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89},
    }};
    // clang-format on

    const std::map<std::string_view, size_t, Comparator> expected_histogram{
        {"George Orwell", 2},    {"F. Scott Fitzgerald", 1}, {"Harper Lee", 1},
        {"Jane Austen", 1},      {"J.D. Salinger", 1},       {"Aldous Huxley", 1},
        {"Charlotte Brontë", 1}, {"J.R.R. Tolkien", 1},      {"William Golding", 1}};

    const auto actual_histogram = BuildAuthorHistogramFlat(db, Comparator{});
    ASSERT_EQ(expected_histogram.size(), actual_histogram.size());

    for (auto [expected_it, actual_it] = std::make_tuple(expected_histogram.begin(), actual_histogram.begin());
         expected_it != expected_histogram.end(); ++expected_it, ++actual_it) {
        EXPECT_EQ(*actual_it, *expected_it);
    }
}

TEST_F(StatisticsTest, build_author_histogram_flat_v2) {
    using Comparator = std::greater<>;

    // clang-format off
    BookDatabase db{{
        {"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
        {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120},
        {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
        {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156},
        {"Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178},
        {"The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112},
        {"Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98},
        {"Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110},
        {"The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203},
        {"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89},
    }};
    // clang-format on

    const std::map<std::string_view, size_t, Comparator> expected_histogram{
        {"George Orwell", 2},    {"F. Scott Fitzgerald", 1}, {"Harper Lee", 1},
        {"Jane Austen", 1},      {"J.D. Salinger", 1},       {"Aldous Huxley", 1},
        {"Charlotte Brontë", 1}, {"J.R.R. Tolkien", 1},      {"William Golding", 1}};

    const auto actual_histogram = BuildAuthorHistogramFlatV2(db, Comparator{});
    ASSERT_EQ(expected_histogram.size(), actual_histogram.size());

    for (auto [expected_it, actual_it] = std::make_tuple(expected_histogram.begin(), actual_histogram.begin());
         expected_it != expected_histogram.end(); ++expected_it, ++actual_it) {
        EXPECT_EQ(*actual_it, *expected_it);
    }
}

TEST_F(StatisticsTest, build_author_histogram) {
    using Comparator = std::greater<>;

    // clang-format off
    BookDatabase db{{
        {"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
        {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120},
        {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
        {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156},
        {"Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178},
        {"The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112},
        {"Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98},
        {"Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110},
        {"The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203},
        {"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89},
    }};
    // clang-format on

    const std::map<std::string_view, size_t, Comparator> expected_histogram{
        {"George Orwell", 2},    {"F. Scott Fitzgerald", 1}, {"Harper Lee", 1},
        {"Jane Austen", 1},      {"J.D. Salinger", 1},       {"Aldous Huxley", 1},
        {"Charlotte Brontë", 1}, {"J.R.R. Tolkien", 1},      {"William Golding", 1}};

    const auto actual_histogram = BuildAuthorHistogram(db, Comparator{});
    ASSERT_EQ(expected_histogram.size(), actual_histogram.size());

    for (auto [expected_it, actual_it] = std::make_tuple(expected_histogram.begin(), actual_histogram.begin());
         expected_it != expected_histogram.end(); ++expected_it, ++actual_it) {
        EXPECT_EQ(*actual_it, *expected_it);
    }
}

TEST_F(StatisticsTest, calculate_genre_ratings) {
    using Comparator = std::greater<>;

    // clang-format off
    BookDatabase db{{
        {"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
        {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120},
        {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
        {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156},
        {"Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178},
        {"The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112},
        {"Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98},
        {"Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110},
        {"The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203},
        {"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89},
    }};
    // clang-format on

    const std::map<Genre, double, Comparator> expected_genre_ratings{{Genre::SciFi, 4.25}, {Genre::Fiction, 4.55}};

    const auto actual_genre_ratings = CalculateGenreRatings(db, Comparator{});
    ASSERT_EQ(expected_genre_ratings.size(), actual_genre_ratings.size());

    for (auto [expected_it, actual_it] = std::make_tuple(expected_genre_ratings.begin(), actual_genre_ratings.begin());
         expected_it != expected_genre_ratings.end(); ++expected_it, ++actual_it) {
        EXPECT_EQ(actual_it->first, expected_it->first);
        EXPECT_DOUBLE_EQ(actual_it->second, expected_it->second);
    }
}

TEST_F(StatisticsTest, calculate_genre_ratings_v2) {
    using Comparator = std::greater<>;

    // clang-format off
    BookDatabase db{{
        {"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
        {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120},
        {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
        {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156},
        {"Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178},
        {"The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112},
        {"Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98},
        {"Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110},
        {"The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203},
        {"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89},
    }};
    // clang-format on

    const std::map<Genre, double, Comparator> expected_genre_ratings{{Genre::SciFi, 4.25}, {Genre::Fiction, 4.55}};

    const auto actual_genre_ratings = CalculateGenreRatingsV2(db, Comparator{});
    ASSERT_EQ(expected_genre_ratings.size(), actual_genre_ratings.size());

    for (auto [expected_it, actual_it] = std::make_tuple(expected_genre_ratings.begin(), actual_genre_ratings.begin());
         expected_it != expected_genre_ratings.end(); ++expected_it, ++actual_it) {
        EXPECT_EQ(actual_it->first, expected_it->first);
        EXPECT_DOUBLE_EQ(actual_it->second, expected_it->second);
    }
}

}  // namespace bookdb::test
