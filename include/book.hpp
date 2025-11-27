#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre {
    Fiction,
    NonFiction,
    SciFi,
    Biography,
    Mystery,
    Unknown,

    Count,
};

constexpr Genre GenreFromString(std::string_view s) {
    if (s == "Fiction")
        return Genre::Fiction;
    if (s == "NonFiction")
        return Genre::NonFiction;
    if (s == "SciFi")
        return Genre::SciFi;
    if (s == "Biography")
        return Genre::Biography;
    if (s == "Mystery")
        return Genre::Mystery;

    return Genre::Unknown;
}

constexpr std::string_view GenreToString(Genre genre) {
    switch (genre) {
    case Genre::Fiction:
        return "Fiction";
    case Genre::NonFiction:
        return "NonFiction";
    case Genre::SciFi:
        return "SciFi";
    case Genre::Biography:
        return "Biography";
    case Genre::Mystery:
        return "Mystery";
    case Genre::Unknown:
        return "Unknown";
    default:
        throw std::logic_error{std::format("Unsupported bookdb::Genre - {}", static_cast<int>(genre))};
    }
}

struct Book {
    constexpr Book(std::string title, std::string_view author, int year, Genre genre, double rating, int read_count)
        : author(author), title(std::move(title)), year(year), genre(genre), rating(rating), read_count(read_count) {}

    constexpr Book(std::string title, std::string_view author, int year, std::string_view genre, double rating,
                   int read_count)
        : author(author), title(std::move(title)), year(year), genre(GenreFromString(genre)), rating(rating),
          read_count(read_count) {}

    auto operator<=>(const Book &) const = default;

    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;
};

}  // namespace bookdb

template <>
struct std::formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        return format_to(fc.out(), "{}", bookdb::GenreToString(g));
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct std::formatter<bookdb::Book, char> {

    template <typename FormatContext>
    auto format(const bookdb::Book &book, FormatContext &fc) const {
        return format_to(fc.out(), "{{author: {}, title: {}, year: {}, genre: {}, rating: {}, read count: {}}}",
                         book.author, book.title, book.year, book.genre, book.rating, book.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
