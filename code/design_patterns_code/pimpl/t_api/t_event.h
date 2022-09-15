#pragma once

#include <string>

class TEvent final {
 public:
  TEvent() = default;
  ~TEvent() = default;

  void SetPath(const std::string &path);
  void ShowPath() const;

 private:
  std::string model_path_;
};
