//定义一个类并重写他的new和delete函数
#include "simple_allocator.h"

#include <stdio.h>

struct foo
{
  uint64_t a[2];
  static simple_allocator alloc;

  static void * operator new(size_t size)
  {
    return alloc.allocate(size);
  }

  static void operator delete(void *p,size_t size)
  {
    alloc.deallocate(p,size);
  }
};

simple_allocator foo::alloc;
