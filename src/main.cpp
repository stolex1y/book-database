#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "formatters.h"
#include "statsistics.hpp"

#include <algorithm>
#include <print>

using namespace bookdb;

int main() {
    // Create a book database
    BookDatabase<> db;

    // Add some books
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    std::print("Books: {}\n\n", db);

    // Sorts
    std::ranges::sort(db, comp::LessByAuthor{});
    std::print("Books sorted by author: {}\n\n==================\n", db);

    std::ranges::sort(db, comp::LessByPopularity{});
    std::print("Books sorted by popularity: {}\n\n==================\n", db);

    // Author histogram
    auto histogram = BuildAuthorHistogramFlat(db);
    std::print("Author histogram: {}", histogram);

    // Ratings
    auto genre_ratings = CalculateGenreRatings(db);
    std::print("\n\nAverage ratings by genres: {}\n", genre_ratings);

    auto avrRating = CalculateAverageRating(db);
    std::print("Average books rating in library: {}\n", avrRating);

    // Samples
    auto samples = SampleRandomBooks(db, 3);
    std::print("\n\n3 random book samples: {}\n", samples);

    // Filters
    auto filtered_by_all_of = FilterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    std::print("\n\nBooks from the 20th century with rating ≥ 4.5: {}\n", filtered_by_all_of);

    auto filtered_by_any_of = FilterBooks(db.begin(), db.end(), any_of(YearBetween(1800, 1899), GenreIs(Genre::SciFi)));
    std::print("\n\nBooks from the 19th century or in the SciFi genre: {}\n", filtered_by_any_of);

    // Top 3 books
    auto top_books = GetTopNBy(db, 3, comp::LessByRating{});
    std::print("\n\nTop 3 books by rating: {}\n", top_books);

    const auto orwellBookIt = std::ranges::find_if(db, [](const auto &v) { return v.author == "George Orwell"; });
    if (orwellBookIt != db.end()) {
        std::print("\n\nTransparent lookup by authors. Found Orwell's book: {}\n", *orwellBookIt);
    }

    return 0;
}