#include "library/tabs/home.h"
#include "library/interface.h"
#include <ncurses.h>

void DrawHome(Interface &interface, Library &library) {
  int line = 0;
  interface.AddText({0, line}, "Welcome Back.", 0);
  line += 2;
  interface.AddText({0, line}, "Your Shelf", A_BOLD | A_ITALIC);
  interface.AddButton({15, line++}, "[ Add New Book ]", 0, [&library]() {
    library.NewBook("NewTitle", "NewAuthor");
  });
  interface.AddSeparator(line);
  line++;
  for (auto book : library.GetBooks()) {
    interface.AddButton({0, line}, book.title, A_BOLD,
                        [&library, book]() { library.RemoveBook(book); });
    interface.AddText({20, line++}, book.author, A_ITALIC);
  }
}
