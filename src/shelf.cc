#include "library/tabs/shelf.h"
#include <ncurses.h>

void DrawShelf(Interface &interface, Library &library) {
  int line = 0;
  interface.AddButton({0, line++}, "Add New Book", A_BOLD, [&library]() {
    library.NewBook("NewTitle", "NewAuthor");
  });

  interface.AddText({0, line}, "Title", A_UNDERLINE);
  interface.AddText({20, line++}, "Author", A_UNDERLINE);

  for (auto book : library.GetBooks()) {
    interface.AddButton({0, line}, book.title, A_BOLD,
                        [&library, book]() { library.RemoveBook(book); });
    interface.AddText({20, line++}, book.author, A_ITALIC);
  }
}
