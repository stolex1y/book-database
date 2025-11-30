#include "statsistics.hpp"

#include <iostream>
#include <random>

#include <benchmark/benchmark.h>

namespace bookdb::test {
class StatisticsBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(benchmark::State &st) override {
        db_.Clear();
        const auto book_count = st.range(0);

        const auto author_count = st.range(1);
        std::unordered_map<std::string, size_t> authors;
        authors.reserve(author_count);
        const auto avg_book_per_author = book_count / author_count + 1;
        std::ranges::generate_n(std::inserter(authors, authors.end()), author_count,
                                [&] { return std::make_pair(GenerateAuthor(), avg_book_per_author); });

        for (auto [i, author_it] = std::make_tuple(0, authors.begin()); i < book_count; ++i) {
            db_.EmplaceBack(GenerateTitle(), author_it->first, GenerateYear(), GenerateGenre(), GenerateRating(),
                            GenerateReadCount());
            if (--author_it->second == 0) {
                author_it = authors.erase(author_it);
            }
        }
    }

    std::string GenerateString(size_t min_len, size_t max_len) {
        std::uniform_int_distribution len_dist(min_len, max_len);
        std::uniform_int_distribution char_dist('a', 'z');

        std::string s;
        s.reserve(max_len);

        std::ranges::generate_n(std::back_inserter(s), static_cast<ssize_t>(len_dist(generator_)),
                                [&] { return char_dist(generator_); });

        return s;
    }

    std::string GenerateTitle() { return GenerateString(10, 20); }

    std::string GenerateAuthor() { return GenerateString(10, 20); }

    Genre GenerateGenre() {
        std::uniform_int_distribution<> genre_dist{0, static_cast<int>(Genre::Count) - 1};
        return static_cast<Genre>(genre_dist(generator_));
    }

    double GenerateRating() {
        std::uniform_real_distribution<> rating_dist{1, 5};
        return rating_dist(generator_);
    }

    uint32_t GenerateYear() {
        std::uniform_int_distribution<> year_dist{1900, 2025};
        return year_dist(generator_);
    }

    uint32_t GenerateReadCount() {
        std::uniform_int_distribution<> read_count_dist{0, 500};
        return read_count_dist(generator_);
    }

    BookDatabase<> db_;
    std::mt19937 generator_{2025};
};

BENCHMARK_DEFINE_F(StatisticsBenchmarkTest, build_author_histogram_flat)(benchmark::State &state) {
    for (auto _ : state) {
        auto result = BuildAuthorHistogramFlat(db_);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(StatisticsBenchmarkTest, build_author_histogram_flat)
    ->ArgNames({"book_count", "author_count"})
    ->ArgsProduct({{10'000, 100'000, 1'000'000}, {1'000, 10'000, 100'000}})
    ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(StatisticsBenchmarkTest, build_author_histogram_flat_v2)(benchmark::State &state) {
    for (auto _ : state) {
        auto result = BuildAuthorHistogramFlatV2(db_);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(StatisticsBenchmarkTest, build_author_histogram_flat_v2)
    ->ArgNames({"book_count", "author_count"})
    ->ArgsProduct({{10'000, 100'000, 1'000'000}, {1'000, 10'000, 100'000}})
    ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(StatisticsBenchmarkTest, build_author_histogram)(benchmark::State &state) {
    for (auto _ : state) {
        auto result = BuildAuthorHistogram(db_);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(StatisticsBenchmarkTest, build_author_histogram)
    ->ArgNames({"book_count", "author_count"})
    ->ArgsProduct({{10'000, 100'000, 1'000'000}, {1'000, 10'000, 100'000}})
    ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(StatisticsBenchmarkTest, calculate_genre_ratings)(benchmark::State &state) {
    for (auto _ : state) {
        auto result = CalculateGenreRatings(db_);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(StatisticsBenchmarkTest, calculate_genre_ratings)
    ->ArgNames({"book_count", "author_count"})
    ->ArgsProduct({{10'000, 100'000, 1'000'000}, {1'000, 10'000, 100'000}})
    ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(StatisticsBenchmarkTest, calculate_genre_ratings_v2)(benchmark::State &state) {
    for (auto _ : state) {
        auto result = CalculateGenreRatingsV2(db_);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(StatisticsBenchmarkTest, calculate_genre_ratings_v2)
    ->ArgNames({"book_count", "author_count"})
    ->ArgsProduct({{10'000, 100'000, 1'000'000}, {1'000, 10'000, 100'000}})
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

}  // namespace bookdb::test
