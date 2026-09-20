#ifndef LIBRARY_INTERFACE_H
#define LIBRARY_INTERFACE_H

#include <functional>
#include <memory>
#include <string>

enum class Tab { kHome, kShelf, kSearch, kNotes, kSettings };

struct Position {
  int x = 0, y = 0;
};

enum SpecialFlags { kDefault, kIgnoreSettings };

struct Text {
  Position position;
  std::string text;
  int style;
  int flags;
  Text() = default;
  Text(Position position_, std::string text_, int style_, int flags_ = 0)
      : position(position_), text(text_), style(style_), flags(flags_) {}
};

struct Button : public Text {
  std::function<void()> func;
  Button() = default;
  Button(Position position_, std::string text_, int style_,
         const std::function<void()> &func_, int flags_ = 0)
      : Text(position_, text_, style_, flags_), func(func_) {}
};

class InterfaceSettings {
public:
  int left_padding = 3;
  int right_padding = 3;
  int top_padding = 1;
};

class Interface {
private:
  InterfaceSettings settings;
  std::vector<std::shared_ptr<Text>> content_;
  std::vector<std::shared_ptr<Button>> buttons_;
  int selected = 0;

public:
  void AddText(Position position, std::string text, int style, int flags = 0);
  void AddButton(Position position, std::string text, int style,
                 const std::function<void()> &func, int flags = 0);
  void AddSeparator(int y, std::string character = "-");
  void HandleInput(int ch);
  void Draw();
  void Reset();

  InterfaceSettings GetSettings();
  void SetSettings(InterfaceSettings settings_);
  Interface(InterfaceSettings settings_) : settings(settings_) {}
};

#endif // LIBRARY_INTERFACE_H
