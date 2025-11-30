#pragma once

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

#include <format>
#include <string>
#include <unordered_set>
#include <vector>

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using value_type = Book;
    using pointer = Book *;
    using const_pointer = const Book *;
    using reference = Book &;
    using const_reference = const Book &;
    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;
    using const_reverse_iterator = BookContainer::const_reverse_iterator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using AuthorContainer = std::unordered_set<std::string, TransparentStringHash, TransparentStringEqual>;

    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> books) {
        for (const auto &book : books) {
            EmplaceBack(book);
        }
    }

    template <typename Self>
    [[nodiscard]] auto begin(this Self &&self) {
        return self.books_.begin();
    }

    template <typename Self>
    [[nodiscard]] auto end(this Self &&self) {
        return self.books_.end();
    }

    template <typename Self>
    [[nodiscard]] auto rbegin(this Self &&self) {
        return self.books_.rbegin();
    }

    template <typename Self>
    [[nodiscard]] auto rend(this Self &&self) {
        return self.books_.rend();
    }

    [[nodiscard]] size_type size() const { return books_.size(); }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    [[nodiscard]] const BookContainer &GetBooks() const { return books_; }

    [[nodiscard]] const AuthorContainer &GetAuthors() const { return authors_; }

    template <typename... Args>
        requires requires(Args... args) { requires std::is_constructible_v<Book, Args...>; }
    Book &EmplaceBack(Args &&...args) {
        books_.emplace_back(std::forward<Args>(args)...);
        Book &book = *std::prev(books_.end());
        const auto [author_it, _] = authors_.emplace(book.author);
        book.author = *author_it;
        return book;
    }

    void PushBack(Book book) { EmplaceBack(std::move(book)); }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

template <typename T>
struct std::formatter<bookdb::BookDatabase<T>> {

    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<T> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
