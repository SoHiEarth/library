#ifndef LIBRARY_INTERFACE_H
#define LIBRARY_INTERFACE_H

#include <functional>
#include <map>
#include <string>

struct Position {
  int x = 0, y = 0;
};

inline bool operator<(const Position &p1, const Position &p2) {
  if (p1.y != p2.y) {
    return p1.y < p2.y;
  } else {
    return p1.x < p2.x;
  }
}

struct Text {
  Position position;
  std::string text;
  int style;
  Text() = default;
  Text(Position position_, std::string text_, int style_)
      : position(position_), text(text_), style(style_) {}
};

struct Button : public Text {
  std::function<void()> func;
  Button() = default;
  Button(Position position_, std::string text_, int style_,
         const std::function<void()> &func_)
      : Text(position_, text_, style_), func(func_) {}
};

class Interface {
private:
  std::vector<std::shared_ptr<Text>> content_;
  // <y, <x, button>
  std::map<int, std::map<int, std::shared_ptr<Button>>> buttons_;
  Position selected;

public:
  void AddText(Position position, std::string text, int style);
  void AddButton(Position position, std::string text, int style,
                 const std::function<void()> &func);
  void HandleInput(int ch);
  void Draw();
  void Reset();
};

#endif // LIBRARY_INTERFACE_H
