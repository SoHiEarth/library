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

  initscr();
  mvprintw(0, 0, "lib - A simple home library manager.");
  mvprintw(1, 0, "Press any key to continue.");
  mvprintw(2, 0, "Press Q to quit.");
  keypad(stdscr, TRUE);

  int ch = 0;
  Interface interface;
  int line = 0;
  interface.AddButton({0, line++}, "Add New Book", A_BOLD,
                      [&library]() { library->NewBook("Title", "Author"); });
  for (auto book : library->GetBooks()) {
    interface.AddButton({0, line++},
                        std::format("{} ({})", book.title, book.author), A_BOLD,
                        [&library, book]() { library->RemoveBook(book); });
  }
  interface.Draw();
  while (true) {
    auto ch = getch();
    if (ch == 'q')
      break;
    interface.HandleInput(ch);

    interface.Reset();
    int line = 0;
    interface.AddButton({0, line++}, "Add New Book", A_BOLD,
                        [&library]() { library->NewBook("Title", "Author"); });
    for (auto book : library->GetBooks()) {
      interface.AddButton(
          {0, line++}, std::format("{} ({})", book.title, book.author), A_BOLD,
          [&library, book]() { library->RemoveBook(book); });
    }
    interface.Draw();
  }

  library.reset();
  endwin();
}
