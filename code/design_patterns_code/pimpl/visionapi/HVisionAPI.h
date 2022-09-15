#pragma once

#include <memory>
#include <string>

//  声明于此，因为HVisionEvent的实现比较复杂，这里不使用经典的pimpl写法
class HVisionEvent;

class HVisionAPI {
 public:
  HVisionAPI(); // 构造函数创建实现类的智能指针对象
  ~HVisionAPI(); // 析构必须要! 智能指针.reset();

 public:
  void SetPath(const std::string &path);
  void ShowPath() const;

 private:
  // class HVisionEvent;
  // 这里使用智能指针 原始的pimp手法起源于裸指针
  std::unique_ptr<HVisionEvent> d_;
};
