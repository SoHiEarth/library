#ifndef LIBRARY_LIBRARY_H
#define LIBRARY_LIBRARY_H

#include <filesystem>
#include <pugixml.hpp>
#include <vector>

constexpr std::string library_path = "library.xml";

struct Book {
  pugi::xml_node book_node;
  std::string title;
  std::string author;
  Book() = default;
  Book(std::string title_, std::string author_)
      : title(title_), author(author_) {}

  // Do not compare the XML nodes
  bool operator==(const Book &book_) const {
    return (title == book_.title) && (author == book_.author);
  }
};

class Library {
private:
  pugi::xml_document doc;
  pugi::xml_node library_node;
  pugi::xml_node books_node;
  std::vector<Book> books_;

public:
  std::vector<Book> &GetBooks() { return books_; }

  void NewBook(std::string title, std::string author) {
    if (std::find(books_.begin(), books_.end(), Book(title, author)) !=
        books_.end()) {
      throw std::runtime_error("A book with the same data exists!");
    }
    auto book_node = books_node.append_child("book");
    book_node.append_attribute("title") = title.c_str();
    book_node.append_attribute("author") = author.c_str();
    books_.push_back(Book(title, author));
    books_.back().book_node = book_node;
  }

  void RemoveBook(Book book) {
    books_node.remove_child(book.book_node);
    std::erase(books_, book);
  }

  Library() {
    if (!std::filesystem::exists(library_path)) {
      pugi::xml_document doc;
      auto library_node = doc.append_child("library");
      library_node.append_child("books");
      doc.save_file(library_path.c_str());
    }

    auto result = doc.load_file(library_path.c_str());
    if (!result) {
      throw std::runtime_error("Failed to load library.xml");
    }

    library_node = doc.child("library");
    books_node = library_node.child("books");
    for (auto book_node : books_node.children("book")) {
      books_.push_back(Book(book_node.attribute("title").as_string(),
                            book_node.attribute("author").as_string()));
      books_.back().book_node = book_node;
    }
  }
  ~Library() { doc.save_file(library_path.c_str()); }
};

#endif // LIBRARY_LIBRARY_H
