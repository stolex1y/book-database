#include "book_database.hpp"

#include <gtest/gtest.h>

namespace bookdb::test {

using BookDb = BookDatabase<>;

class BookDatabaseTest : public testing::Test {
public:
    BookDb db_;
};

TEST_F(BookDatabaseTest, empty_db) {
    ASSERT_EQ(db_.size(), 0);
    ASSERT_TRUE(std::ranges::empty(db_.GetBooks()));
    ASSERT_TRUE(std::ranges::empty(db_.GetAuthors()));
}

TEST_F(BookDatabaseTest, emplace_back) {
    const Book expected_book{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    const auto &actual_book = db_.EmplaceBack(expected_book.title, expected_book.author, expected_book.year,
                                              expected_book.genre, expected_book.rating, expected_book.read_count);
    ASSERT_EQ(actual_book, expected_book);
    ASSERT_EQ(db_.size(), 1);

    const auto &actual_books = db_.GetBooks();
    ASSERT_EQ(actual_books.size(), 1);
    ASSERT_EQ(*actual_books.begin(), expected_book);

    const auto &actual_authors = db_.GetAuthors();
    ASSERT_EQ(actual_authors.size(), 1);
    ASSERT_EQ(*actual_authors.begin(), expected_book.author);
}

TEST_F(BookDatabaseTest, book_author_string_view_validity) {
    std::string orig_author{"Author"};
    const auto &actual_book = db_.EmplaceBack(Book{"1984", orig_author, 1949, Genre::SciFi, 4., 190});
    ASSERT_EQ(actual_book.author, orig_author);

    const auto &actual_authors = db_.GetAuthors();
    ASSERT_EQ(actual_authors.size(), 1);
    ASSERT_EQ(*actual_authors.begin(), actual_book.author);

    orig_author += " Modified";
    ASSERT_NE(*actual_authors.begin(), orig_author);
}

TEST_F(BookDatabaseTest, push_back) {
    const Book expected_book{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    db_.PushBack(expected_book);

    ASSERT_EQ(db_.size(), 1);

    const auto &actual_books = db_.GetBooks();
    ASSERT_EQ(actual_books.size(), 1);
    ASSERT_EQ(*actual_books.begin(), expected_book);

    const auto &actual_authors = db_.GetAuthors();
    ASSERT_EQ(actual_authors.size(), 1);
    ASSERT_EQ(*actual_authors.begin(), expected_book.author);
}

TEST_F(BookDatabaseTest, create_from_initializer_list) {
    const Book expected_book1{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    const Book expected_book2{"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120};
    const Book expected_book3{"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143};
    std::unordered_map<std::string_view, const Book *> expected_books{{{expected_book1.title, &expected_book1},
                                                                       {expected_book2.title, &expected_book2},
                                                                       {expected_book3.title, &expected_book3}}};
    db_ = BookDatabase{{expected_book1, expected_book2, expected_book3}};
    std::unordered_set expected_authors{{expected_book1.author, expected_book2.author, expected_book3.author}};

    ASSERT_EQ(db_.size(), expected_books.size());

    const auto &actual_books = db_.GetBooks();
    ASSERT_EQ(actual_books.size(), expected_books.size());
    ASSERT_TRUE(std::all_of(actual_books.begin(), actual_books.end(), [&](const auto &actual_book) {
        const auto &expected_book = expected_books[actual_book.title];
        return expected_book && actual_book == *expected_book;
    }));

    const auto &actual_authors = db_.GetAuthors();
    ASSERT_EQ(actual_authors.size(), expected_authors.size());
    ASSERT_TRUE(std::all_of(actual_authors.begin(), actual_authors.end(), [&](const auto &actual_author) {
        const auto expected_author = expected_authors.find(actual_author);
        return expected_author != expected_authors.end() && actual_author == *expected_author;
    }));
}

TEST_F(BookDatabaseTest, constant_for_each_book) {
    const Book expected_book1{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    const Book expected_book2{"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120};
    const Book expected_book3{"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143};
    std::unordered_map<std::string_view, const Book *> expected_books{{{expected_book1.title, &expected_book1},
                                                                       {expected_book2.title, &expected_book2},
                                                                       {expected_book3.title, &expected_book3}}};
    db_ = BookDatabase{{expected_book1, expected_book2, expected_book3}};
    std::unordered_set expected_authors{{expected_book1.author, expected_book2.author, expected_book3.author}};

    size_t actual_book_count{};
    for (auto &actual_book : const_cast<const BookDb &>(db_)) {
        static_assert(std::is_same_v<decltype(actual_book), const Book &>);

        const auto &expected_book = expected_books[actual_book.title];
        ASSERT_TRUE(expected_book);
        ASSERT_EQ(actual_book, *expected_book);
        ++actual_book_count;
    }
    ASSERT_EQ(actual_book_count, expected_books.size());
}

TEST_F(BookDatabaseTest, for_each_book) {
    const Book expected_book1{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    const Book expected_book2{"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120};
    const Book expected_book3{"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143};
    std::unordered_map<std::string_view, const Book *> expected_books{{{expected_book1.title, &expected_book1},
                                                                       {expected_book2.title, &expected_book2},
                                                                       {expected_book3.title, &expected_book3}}};
    db_ = BookDatabase{{expected_book1, expected_book2, expected_book3}};
    std::unordered_set expected_authors{{expected_book1.author, expected_book2.author, expected_book3.author}};

    size_t actual_book_count{};
    for (auto &actual_book : db_) {
        static_assert(std::is_same_v<decltype(actual_book), Book &>);

        const auto &expected_book = expected_books[actual_book.title];
        ASSERT_TRUE(expected_book);
        ASSERT_EQ(actual_book, *expected_book);
        ++actual_book_count;
    }
    ASSERT_EQ(actual_book_count, expected_books.size());
}

TEST_F(BookDatabaseTest, clear_db) {
    const Book expected_book1{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    const Book expected_book2{"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120};
    const Book expected_book3{"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143};
    db_ = BookDatabase{{expected_book1, expected_book2, expected_book3}};

    db_.Clear();
    ASSERT_EQ(db_.size(), 0);
    ASSERT_EQ(db_.GetBooks().size(), 0);
    ASSERT_EQ(db_.GetAuthors().size(), 0);
}

}  // namespace bookdb::test
