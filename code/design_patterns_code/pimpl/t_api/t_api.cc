
#include "t_api.h"
#include "t_event.h" // 具体功能

#include <utility>

// must TApi::Implementation 而不是仅仅 Implementation
class TApi::Implementation {
public:
  Implementation() = default;
  ~Implementation() = default;

  void SetPath(const std::string &path) { event_.SetPath(path); }
  void ShowPath() const { event_.ShowPath(); };

private:
  TEvent event_; // 私有数据
};

// 这里使用了现代make_unique语法，也可以直接new
TApi::TApi() : impl_(std::make_unique<Implementation>()) {}

// 实现！  注意！TApi的析构函数需要！// or TApi::~TApi() { }
TApi::~TApi() = default;

void TApi::SetPath(const std::string &path) { impl_->SetPath(path); }

void TApi::ShowPath() const { impl_->ShowPath(); }
