#ifndef LIBRARY_LIBRARY_H
#define LIBRARY_LIBRARY_H

#include <filesystem>
#include <pugixml.hpp>
#include <vector>

constexpr std::string library_path = "library.xml";

struct Book {
  std::string title;
  std::string author;
  bool operator==(const Book &) const = default;
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
    auto book_node = books_node.append_child("book");
    book_node.append_attribute("title") = title.c_str();
    book_node.append_attribute("author") = author.c_str();
    books_.push_back({title, author});
  }

  void RemoveBook(Book book) { std::erase(books_, book); }

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
      books_.push_back({book_node.attribute("title").as_string(),
                        book_node.attribute("author").as_string()});
    }
  }
  ~Library() {
    books_node.remove_children();
    for (const auto &book : books_) {
      auto book_node = books_node.append_child("book");
      book_node.append_attribute("title") = book.title;
      book_node.append_attribute("author") = book.author;
    }
    doc.save_file(library_path.c_str());
  }
};

#endif // LIBRARY_LIBRARY_H
