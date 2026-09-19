#include "library/interface.h"
#include "ncurses.h"
#include <ios>

void Interface::AddText(Position position, std::string text, int style) {
  auto text_object = std::make_shared<Text>(position, text, style);
  content_.push_back(text_object);
}

void Interface::AddButton(Position position, std::string text, int style,
                          const std::function<void()> &func) {
  auto button_object = std::make_shared<Button>(position, text, style, func);
  content_.push_back(button_object);
  if (!buttons_.contains(position.y)) {
    buttons_[position.y] = std::map<int, std::shared_ptr<Button>>();
  }
  buttons_[position.y][position.x] = button_object;
}

void Interface::HandleInput(int ch) {
  if (ch == KEY_UP) {
    selected.y = std::max(0, selected.y - 1);
  } else if (ch == KEY_DOWN) {
    selected.y =
        std::min(selected.y + 1, static_cast<int>(buttons_.size() - 1));
  } else if (ch == KEY_LEFT) {
    selected.x = std::max(0, selected.x - 1);
  } else if (ch == KEY_RIGHT) {
    selected.x = std::min(selected.x + 1,
                          static_cast<int>(buttons_[selected.y].size() - 1));
  } else if (ch == '\n') {
    // Find the current button
    if (!buttons_.empty()) {
      // Find the current button
      auto selected_x_map = std::next(buttons_.begin(), selected.y)->second;
      auto current_button =
          std::next(selected_x_map.begin(), selected.x)->second;
      if (current_button)
        current_button->func();
    }
  }

  if (selected.y > static_cast<int>(buttons_.size() - 1)) {
    selected.y = static_cast<int>(buttons_.size() - 1);
  }
  if (selected.x > static_cast<int>(buttons_[selected.y].size() - 1)) {
    selected.x = static_cast<int>(buttons_[selected.y].size() - 1);
  }
}

void Interface::Draw() {
  clear();
  std::shared_ptr<Button> current_button;
  if (selected.y > static_cast<int>(buttons_.size() - 1)) {
    selected.y = static_cast<int>(buttons_.size() - 1);
  }
  if (selected.x > static_cast<int>(buttons_[selected.y].size() - 1)) {
    selected.x = static_cast<int>(buttons_[selected.y].size() - 1);
  }

  if (!buttons_.empty()) {
    // Find the current button
    auto selected_x_map = std::next(buttons_.begin(), selected.y)->second;
    current_button = std::next(selected_x_map.begin(), selected.x)->second;
  }

  if (current_button)
    current_button->style |= A_REVERSE;

  for (const auto text : content_) {
    attron(text->style);
    mvprintw(text->position.y, text->position.x, text->text.c_str());
    attroff(text->style);
  }
  if (current_button)
    current_button->style &= ~A_REVERSE;
}

void Interface::Reset() {
  content_.clear();
  buttons_.clear();
}
