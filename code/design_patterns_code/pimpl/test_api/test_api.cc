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
//注意，析构函数需要
TestApi::~TestApi() = default;