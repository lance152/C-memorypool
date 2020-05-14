#include <iostream>

template<typename T,int num = 50>
class allocPool
{
public:
  allocPool(){}
  ~allocPool(){}

  //重载new
  static void* operator new(unsigned size)
  {
    //如果是第一次要求分配内存，就申请num个T的内存空间
    if(!m_pointer){
      myalloc();
    }
    unsigned char* ret = m_pointer;
    m_pointer = *((unsigned char**)ret);
    return ret;
  }

  //重载delete
  static void operator delete(void* p)
  {
    *((unsigned char**)p) = m_pointer;
    m_pointer = (unsigned char*)p;
  }

private:
  static unsigned char* m_pointer; //保存空闲区域内存的开始

  static void myalloc(){
    m_pointer = new unsigned char[sizeof(T)*num];
    //实现一个链表保存所有内存
    //这里使用了二级指针
    //强制转换m_pointer为二级指针,这样*cur就是m_pointer保存到的前n个字节，这里n取决与指针大小
    unsigned char** cur = (unsigned char**) m_pointer;
    unsigned char* nextNode = m_pointer;
    for(int i=0;i<num;i++){
      nextNode += sizeof(T);
      *cur = nextNode;
      cur = (unsigned char**) nextNode;
    }
    *cur = nullptr;
  }
};

class cmyInt:public allocPool<cmyInt,5>
{
public:
  cmyInt(){
    m_value = 0;
  }
private:
  int m_value;
};

template<typename T,int num>
unsigned char* allocPool<T,num>::m_pointer=nullptr;

int main()
{
  cmyInt* p = new cmyInt;
  delete p;
  return 0;
}
