#include "library/library.h"

std::vector<Book> &Library::GetBooks() { return books_; }

void Library::NewBook(std::string title, std::string author) {
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

void Library::RemoveBook(Book book) {
  books_node.remove_child(book.book_node);
  std::erase(books_, book);
}

Library::Library() {
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

Library::~Library() { doc.save_file(library_path.c_str()); }
