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