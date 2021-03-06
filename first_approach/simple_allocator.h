#include <cstddef> //定义size_t

class simple_allocator
{
private:

  //嵌入式指针
  //借用小区块的前n个字节存储下一个空闲区块的地址
  //这里需要的空间取决于指针的大小
  //前提条件是小区块的大小要至少大于一个指针的大小
  //当小区块分配出去后就不再需要这个空间了所以这个空间可以正常存储

  struct free_element
  {
    struct free_element * next;
  };

  free_element * free_store = nullptr;
  //默认分配的链表长度
  //通常20-30之间
  static const int num_chunk = 4;

public:
  void * allocate(size_t size)
  {
    free_element * e;
    if(!free_store){
      //链表为空，创建一个新的大区块
      size_t big_chunk_size = num_chunk * size;
      free_store = reinterpret_cast<free_element *>(malloc(big_chunk_size));

      //构建链表
      for(int i=0;i<num_chunk-1;i++){
        //把每一个小区块连起来
        e->next = reinterpret_cast<free_element *>(reinterpret_cast<char *>(e)+size);
        e = e->next;
      }
      //最后一个小区块的下一个区块为null
      e->next = nullptr;
    }

    //返回下一个可用的区块并把指针往后移一位
    e = free_store;
    free_store = free_store->next;
    return e;
  }
  void deallocate(void * e,size_t size)
  {
    //释放内存
    //相当于把要释放的区块放在空闲区块链表的头部
    reinterpret_cast<free_element *>(e)->next = free_store;
    free_store = reinterpret_cast<free_element*>(e);
  }
};
