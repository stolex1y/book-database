#include "statsistics.hpp"

#include <gtest/gtest.h>

namespace bookdb::test {

using BookDb = BookDatabase<>;

class StatisticsTest : public testing::Test {
public:
    void SetUp() override {
        // clang-format off
        db_ = BookDatabase{{
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
    }

    BookDb db_;
};

TEST_F(StatisticsTest, build_author_histogram_flat) {
    using Comparator = std::greater<>;
    std::map<std::string_view, size_t, Comparator> expected_histogram;
    for (const auto &book : db_) {
        ++expected_histogram[book.author];
    }
    const auto actual_histogram = BuildAuthorHistogramFlat(db_, Comparator{});
    for (auto [expected_it, actual_it] = std::make_tuple(expected_histogram.begin(), actual_histogram.begin());
         expected_it != expected_histogram.end(); ++expected_it, ++actual_it) {
        EXPECT_EQ(*actual_it, *expected_it);
    }
}

TEST_F(StatisticsTest, calculate_genre_ratings) {
    using Comparator = std::greater<>;
    std::map<Genre, std::pair<double, size_t>, Comparator> expected_genre_ratings;
    for (const auto &book : db_) {
        auto &[summary_rating, count] = expected_genre_ratings[book.genre];
        summary_rating += book.rating;
        ++count;
    }
    const auto actual_genre_ratings = CalculateGenreRatings(db_, Comparator{});
    ASSERT_EQ(expected_genre_ratings.size(), actual_genre_ratings.size());
    for (auto [expected_it, actual_it] = std::make_tuple(expected_genre_ratings.begin(), actual_genre_ratings.begin());
         expected_it != expected_genre_ratings.end(); ++expected_it, ++actual_it) {
        EXPECT_EQ(actual_it->first, expected_it->first);
        EXPECT_EQ(actual_it->second, expected_it->second.first / expected_it->second.second);
    }
}

}  // namespace bookdb::test
