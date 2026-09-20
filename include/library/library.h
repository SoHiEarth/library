#ifndef LIBRARY_LIBRARY_H
#define LIBRARY_LIBRARY_H

#include <filesystem>
#include <pugixml.hpp>
#include <vector>

constexpr std::string library_path = "library.xml";

enum class BookFormat { Physical, Digital };

struct Book {
  pugi::xml_node book_node;
  BookFormat format = BookFormat::Physical;
  std::string title;
  std::string author;

  Book() = default;
  Book(std::string title_, std::string author_)
      : title(title_), author(author_) {}

  bool operator==(const Book &book_) const {
    return (title == book_.title) && (author == book_.author);
  }
};

struct DigitalBook : public Book {
  BookFormat format = BookFormat::Digital;
  std::filesystem::path document_path;
};

class Library {
private:
  pugi::xml_document doc;
  pugi::xml_node library_node;
  pugi::xml_node books_node;
  std::vector<Book> books_;

public:
  std::vector<Book> &GetBooks();
  void NewBook(std::string title, std::string author);
  void RemoveBook(Book book);
  Library();
  ~Library();
};

#endif // LIBRARY_LIBRARY_H
