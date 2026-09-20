#ifndef LIBRARY_INTERFACE_H
#define LIBRARY_INTERFACE_H

#include <functional>
#include <memory>
#include <string>

struct Position {
  int x = 0, y = 0;
};

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
  std::vector<std::shared_ptr<Button>> buttons_;
  int selected = 0;

public:
  void AddText(Position position, std::string text, int style);
  void AddButton(Position position, std::string text, int style,
                 const std::function<void()> &func);
  void HandleInput(int ch);
  void Draw();
  void Reset();
};

#endif // LIBRARY_INTERFACE_H
