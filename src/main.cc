#include <filesystem>
#include <ncurses.h>
#include <optional>
#include <pugixml.hpp>
#include <string>
#include <vector>

const std::string library_path = "library.xml";

struct Book {
  std::string title;
  std::string author;
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

  Library() {
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

  ~Library() { doc.save_file(library_path.c_str()); }
};

int main() {
  if (!std::filesystem::exists(library_path)) {
    pugi::xml_document doc;
    auto library_node = doc.append_child("library");
    library_node.append_child("books");
    doc.save_file(library_path.c_str());
  }

  std::optional<Library> library;
  try {
    library.emplace();
  } catch (const std::exception &e) {
    printf("Caught an exception while loading library. Reason: %s", e.what());
    return 1;
  }

  initscr();

  while (true) {
    int line = 0;
    clear();
    attron(A_BOLD);
    mvprintw(line++, 0, "Books");
    attroff(A_BOLD);

    attron(A_REVERSE);
    mvprintw(line++, 0, "Add New Book");
    attroff(A_REVERSE);

    for (auto book : library->GetBooks()) {
      mvprintw(line++, 0, "%s, %s", book.title.c_str(), book.author.c_str());
    }

    auto ch = getch();
    if (ch == '\n') {
      library->NewBook("New Book", "No Author");
    }
    if (ch == 'q') {
      break;
    }
  }

  library.reset();

  endwin();
}
