#include <iostream> //cout,endl
#include <cassert> //assert()
#include <ctime> //clock()
#include <vector> //std::vector<int>

#include "MempryPool.hpp" //MemoryPool<T>
#include "StackAlloc.hpp" //StackAlloc<T,Alloc>

#define ELEMS 10e7 //插入元素个数
#define REPS 100 //重复次数

int main()
{
  clock_t start;
  //默认分配器
  StackAlloc<int,std::allocator<int>> stackDefault;
  start = clock();
  for(int i=0;i<REPS;i++){
    assert(stackDefault.empty());
    for(int j=0;j<ELEMS;j++){
      stackDefault.push(i);
    }
    for(int j=0;j<ELEMS;j++){
      stackDefault.pop();
    }
  }
  std::cout<<"Default Allocator Time: ";
  std::cout<< (((double)clock()-start)/CLOCKS_PER_SEC)<<std::endl;

  //使用内存池
  StackAlloc<int,MemoryPool<int>> stackPool;
  start = clock();
  for(int i=0;i<REPS;i++){
    assert(stackPool.empty());
    for(int j=0;j<ELEMS;j++){
      stackPool.push(i);
    }
    for(int j=0;j<ELEMS;j++){
      stackPool.pop();
    }
  }

  std::cout<<"MemoryPool Allocator Time: ";
  std::cout<< (((double)clock()-start)/CLOCKS_PER_SEC)<<std::endl;

  return 0;
}
