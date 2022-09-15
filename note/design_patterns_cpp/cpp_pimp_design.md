# PIMPL

PIMPL (Pointer to implementation) 由指针指向实现.

pimpl是CPP中很常用的设计手法，虽然不属于设计模式中的一种，但是这种设计“模式”挺多的，我姑且叫做设计手法吧。

- doctotext里面发挥的淋漓尽致 (刚开始参加工作时候阅读这个代码，很流畅，设计很好，推荐新手有空可以学习这个库的编码。)
- opencv里面有很多pimp设计手法
- Qt中的d指针也是pimpl的使用


> doctotext home page: http://silvercoders.com/en/products/doctotext/

## ref 

- https://zh.cppreference.com/w/cpp/language/pimpl (cpp手册上面的介绍，最后再看这个)
- https://herbsutter.com/gotw/_100/
- https://www.yanbinghu.com/2020/11/06/3161.html  守望写的已经很清晰, 这也是手册上面写法的复现 
- https://www.fluentcpp.com/2017/09/22/make-pimpl-using-unique_ptr/ (推荐，也是手册上面写法的复现)
- https://blog.csdn.net/big_yellow_duck/article/details/52351729 (Effective Modern C++)


# pimpl经典介绍博客的翻译

- 来自: https://www.fluentcpp.com/2017/09/22/make-pimpl-using-unique_ptr/ (推荐)

pimpl惯用法，pointer to implementation，就是用指针来拆分实现，这样改动不会导致所有文件都编译一遍，也是一种解耦

以前的实现

```c++
#include "Engine.h"

class Fridge
{
public:
   void coolDown();
private:
   Engine engine_;
};
```

实现文件
```cpp
#include "Fridge.h"

void Fridge::coolDown()
{
   /* ... */
}
```

这样改动Engine就会重编Fridge

**引入指针分离**

```c++
class Fridge
{
public:
   Fridge();
   ~Fridge();

   void coolDown();
private:
   class FridgeImpl;
   FridgeImpl* impl_;
};
```

**FridgeImpl封装一层Engine！！！**，使其不可见

```c++
#include "Engine.h"
#include "Fridge.h"

// 注意FridgeImpl的声明 Fridge::FridgeImpl
class Fridge::FridgeImpl
{
public:
   void coolDown()
   {
      /* ... */
   }
private:
   Engine engine_;
};

Fridge::Fridge() : impl_(new FridgeImpl) {}

Fridge::~Fridge()
{
   if (impl_)
   delete impl_;
}

void Fridge::coolDown()
{
   impl_->coolDown();
}
```

这样还是需要管理impl_生命周期，如果用unique_ptr管理声明周期就更好了

**改进的代码**

```c++
#include <memory>
 
class Fridge
{
public:
   Fridge();
   void coolDown();
private:
   class FridgeImpl;
   std::unique_ptr<FridgeImpl> impl_;
};
```

实现文件

```cpp
#include "Engine.h"
#include "Fridge.h"

// 修改了blog中的代码
class Fridge::FridgeImpl
{
public:
   void coolDown()
   {
      /* ... */
   }
private:
   Engine engine_;
};

Fridge::Fridge() : impl_(new FridgeImpl) {}
```

## 析构可见性--特别注意

这样会有新问题，编译不过

> use of undefined type 'FridgeImpl' can't delete an incomplete type

**因为unique_ptr需要知道托管对象的析构，最起码要保证可见性**

c++规则，以下两种情况，delete pointer会有未定义行为

- void* 类型
- 指针的类型不完整，比如这种前向声明类指针

由于unique_ptr检查，会在编译期直接拒绝 同理的还有boost::checked_delete

进一步讨论，Fridge 和FridgeImpl的析构函数都是没定义的，编译器会自动定义并内联，在Fridge的编译单元，就已经见到了Fridge的析构了，但是见不到FridgeImpl的析构，**解决办法就是加上Fridge的析构声明，并把实现放到实现文件中，让Fridge和FridgeImpl的析构同时可见**  (直接在声明为default好像也没啥问题，可能是这些文章都是参考了cppreference吧)


**改进代码**

```cpp
#include <memory>

class Fridge
{
public:
   Fridge();
   ~Fridge(); // 先声明 在实现文件中定义
   void coolDown();
private:
   class FridgeImpl;
   std::unique_ptr<FridgeImpl> impl_;
};
```

```cpp
#include "Engine.h"
#include "Fridge.h"

class Fridge::FridgeImpl
{
public:
   void coolDown()
   {
      /* ... */
   }
private:
   Engine engine_;
};

Fridge::Fridge() : impl_(new FridgeImpl) {}

// 定义
Fridge::~Fridge() = default;
```

> TODO: 为啥不直接在声明出 `~Fridge() =default;`


# 下面是守望的博客的原文

https://www.yanbinghu.com/2020/11/06/3161.html

## 前言

有时候我们需要提供对外的API，通常会以头文件的形式提供。举个简单的例子：  
提供一个从某个指定数开始打印的接口，

头文件内容如下：

```cpp
#ifndef _TEST_API_H
#define _TEST_API_H
//test_api.h
class TestApi{
  public:
    TestApi(int s):start(s){}
    void TestPrint(int num);
  private:
    int start_ = 0;
};
#endif //_TEST_API_H
```

实现文件如下：

```cpp
#include "test_api.h"
#include <iostream>
//test_api.cc
TestApi::TestPrint(int num){
  for(int i = start_; i < num; i++){
    std::cout<< i <<std::endl;
  }
}
```

类TestApi中有一个私有变量start_，头文件中是可以看到的。

```cpp
#include "test_api.h"
int main(){
  TestApi test_api{10};
  test_api.TestPrint(15);
  return 0;
}
```

## 常规实现缺点

从前面的内容来看， 一切都还正常，但是有什么问题呢？

- 头文件暴露了私有成员
- 实现与接口耦合
- 编译耦合

第一点可以很明显的看出来，其中的私有变量`start_`能否在头文件中看到，如果实现越来越复杂，这里可能也会出现更多的私有变量。有人可能会问，私有变量外部也不能访问，暴露又何妨？

不过你只是提供几个接口，给别人看到这么多信息干啥呢？这样就会导致实现和接口耦合在了一起。

另外一方面，如果有另外一个库使用了这个库，而你的这个库实现变了，头文件就会变，而头文件一旦变动，就需要所有使用了这个库的程序都要重新编译！

这个代价是巨大的。

所以，我们应该尽可能地保证头文件不变动，或者说，尽可能隐藏实现，隐藏私有变量。



## PIMPL

Pointer to implementation，由指针指向实现，而不过多暴露细节。废话不多说，上代码：

```cpp
#ifndef _TEST_API_H
#define _TEST_API_H
#include <memory>
//test_api.h
class TestApi{
  public:
    TestApi(int s);
    ~TestApi();
    void TestPrint(int num);
  private:
    class TestImpl;
    std::unique_ptr<TestImpl> test_impl_;
};
#endif //_TEST_API_H
```

从这个头文件中，我们可以看到：

- 实现都在TestImpl中，因为只有一个私有的TestImpl变量，可以预见到，实现变化时，这个头文件是基本不需要动的
- `testimpl`是一个`unique_ptr`，因为我们使用的是现代C++，**这里需要注意的一点是，它的构造函数和析构函数必须存在，否则会有编译问题**。


我们再来看下具体的实现：


```cpp
#include "test_api.h"
#include <iostream>
//test_api.cc
class TestApi::TestImpl{
  public:
    void TestPrint(int num);
    TestImpl(int s):start_(s){}
    TestImpl() = default;
    ~TestImpl() = default;
  private:
    int start_;
};

void TestApi::TestImpl::TestPrint(int num){
  for(int i = start_; i < num; i++){
    std::cout<< i <<std::endl;
  }
}

TestApi::TestApi(int s){
    test_impl_.reset(new TestImpl(s));
}

void TestApi::TestPrint(int num){
  test_impl_->TestPrint(num);
}

//注意，TestApi的析构函数需要！
TestApi::~TestApi() = default;
```

从实现中看到，TestApi中的TestPrint调用了TestImpl中的TestPrint实现，而所有的具体实现细节和私有变量都在TestImpl中，即便实现变更了，其他库不需要重新编译，而仅仅是在生成可执行文件时重新链接。

## 总结

从例子中，我们可以看到PIMPL模式中有以下优点：

- 降低耦合，头文件稳定，类具体实现改变不影响其他模块的编译，只影响可执行程序的链接
- 接口和实现分离，提高稳定性

当然了，由于实现在另外一个类中，所以会多一次调用，会有性能的损耗，但是这点几乎可以忽略。


# Code

[示例code](../../code/design_patterns_code/pimpl/)

文件分布说明

```shell
- test_api 是守望博客上的经典形式 (经典写法) 
- t_api 是一种常见写法 (经典写法，推荐看这个)
- visionapi 是我之前给以前写的变种形式 (现在不推荐使用了)
```

