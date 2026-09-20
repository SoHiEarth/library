#include <ncurses.h>
#include <optional>
#include <pugixml.hpp>

#include "library/interface.h"
#include "library/library.h"
#include "library/tabs/home.h"

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
  Interface interface({});
  Tab current_tab = Tab::kHome;

  while (true) {
    interface.Reset();
    switch (current_tab) {
    case Tab::kHome:
      DrawHome(interface, library.value());
      break;
    default:
      break;
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
