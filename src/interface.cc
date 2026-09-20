#include "library/interface.h"
#include "ncurses.h"
#include <format>

void Interface::AddText(Position position, std::string text, int style) {
  auto text_object = std::make_shared<Text>(position, text, style);
  content_.push_back(text_object);
}

void Interface::AddButton(Position position, std::string text, int style,
                          const std::function<void()> &func) {
  auto button_object = std::make_shared<Button>(position, text, style, func);
  content_.push_back(button_object);
  buttons_.push_back(button_object);
}

void Interface::HandleInput(int ch) {
  if (ch == KEY_UP) {
    selected--;
  } else if (ch == KEY_DOWN) {
    selected++;
  } else if (ch == KEY_LEFT) {
    selected--;
  } else if (ch == KEY_RIGHT) {
    selected++;
  }

  selected = std::clamp(selected, 0, static_cast<int>(buttons_.size()) - 1);

  if (ch == '\n') {
    auto current_button = buttons_.at(selected);
    if (current_button) {
      try {
        current_button->func();
      } catch (const std::exception &e) {
        move(LINES - 1, 0);
        clrtoeol();
        mvaddstr(LINES - 1, 0, std::format("Error: {}", e.what()).c_str());
        getch();
      }
    }
  }
}

void Interface::Draw() {
  clear();
  std::shared_ptr<Button> current_button;
  if (!buttons_.empty()) {
    selected = std::clamp(selected, 0, static_cast<int>(buttons_.size()) - 1);
    current_button = buttons_.at(selected);
  }

  if (current_button)
    current_button->style |= A_REVERSE;

  for (const auto text : content_) {
    if (text->position.y < 0 || text->position.y > LINES ||
        text->position.x < 0 || text->position.x > COLS) {
      continue;
    }
    attron(text->style);
    mvaddstr(text->position.y, text->position.x, text->text.c_str());
    attroff(text->style);
  }
  if (current_button)
    current_button->style &= ~A_REVERSE;
}

void Interface::Reset() {
  content_.clear();
  buttons_.clear();
}
