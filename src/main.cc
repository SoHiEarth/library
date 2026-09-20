#include <format>
#include <ncurses.h>
#include <optional>
#include <pugixml.hpp>
#include <string>

#include "library/interface.h"
#include "library/library.h"

int main() {
  std::optional<Library> library;
  try {
    library.emplace();
  } catch (const std::exception &e) {
    printf("Caught an exception while loading library. Reason: %s", e.what());
    return 1;
  }

  std::setlocale(LC_ALL, "");
  initscr();
  keypad(stdscr, TRUE);

  int ch = 0;
  Interface interface;
  while (true) {
    interface.Reset();
    int line = 0;
    interface.AddButton({0, line++}, "Add New Book", A_BOLD, [&library]() {
      library->NewBook("NewTitle", "NewAuthor");
    });

    interface.AddText({0, line}, "Title", A_UNDERLINE);
    interface.AddText({20, line++}, "Author", A_UNDERLINE);
    for (auto book : library->GetBooks()) {
      interface.AddButton({0, line}, std::format("{}", book.title), A_BOLD,
                          [&library, book]() { library->RemoveBook(book); });
      interface.AddText({20, line++}, std::format("{}", book.author), A_ITALIC);
    }
    interface.Draw();

    auto ch = getch();
    if (ch == 'q')
      break;
    interface.HandleInput(ch);
  }

  library.reset();
  endwin();
}
