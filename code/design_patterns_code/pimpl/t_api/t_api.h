#pragma once

#include <memory>
#include <string>

class TApi {
public:
  TApi();
  ~TApi(); // 在实现文件中定义，其中 Implementation 是完整类型

public:
  // 公开 API，将被转发给实现
  void SetPath(const std::string &path);
  void ShowPath() const;

private:
  class Implementation;
  std::unique_ptr<Implementation> impl_;
};
