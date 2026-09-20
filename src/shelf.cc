#include "library/tabs/shelf.h"
#include <ncurses.h>

std::string FormatToString(BookFormat format) {
  switch (format) {
  case BookFormat::Physical:
    return "PHYS";
  case BookFormat::Digital:
    return "DIGI";
  }
}

void DrawShelf(Interface &interface, Library &library) {
  int line = 0;
  interface.AddButton({0, line++}, "[ Add New Book ]", A_BOLD, [&library]() {
    library.NewBook("NewTitle", "NewAuthor");
  });

  interface.AddSeparator(line++);

  for (auto book : library.GetBooks()) {
    interface.AddText({0, line}, "[ " + FormatToString(book.format) + " ]");
    interface.AddButton({10, line}, book.title, A_BOLD,
                        [&library, book]() { library.RemoveBook(book); });
    interface.AddText({25, line++}, book.author, A_ITALIC);
  }
}
