#pragma once

#include <string>

class HVisionEvent final {
 public:
  // 写为default是因为项目中是default，就想验证写为default的情况
  HVisionEvent() = default;
  ~HVisionEvent();

  void SetPath(const std::string &path);
  void ShowPath() const;

 private:
  std::string modelPath_;
};
