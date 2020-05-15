#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include <climits>
#include <cstddef>

//这里定义了每一个block的大小，默认为4096 bytes
template<typename T,size_t BlockSize = 4096>
class MemoryPool
{
public:
  //定义一些别名简化类型书写
  typedef T* pointer;

  //定义rebind<U>::other 接口
  template <typename U> struct rebind{
    typedef MemoryPool<U> other;
  };

  //构造函数 初始化所有指针
  //noexcept表示不抛出异常
  MemoryPool() noexcept
  {
    currentBlock_ = nullptr;
    currentSlot_ = nullptr;
    lastSlot_ = nullptr;
    freeSlot_ = nullptr;
  }

  //析构
  ~MemoryPool() noexcept
  {
    //需要释放所有的block
    slot_pointer_ curr = currentBlock_;
    while(curr!=nullptr){
      slot_pointer_ prev = curr->next;
      operator delete(reinterpret_cast<void*>(curr));
      curr = prev;
    }
  }

  //分配空间
  //同一时间只分配一个对象
  pointer allocate(size_t n=1,const T* hint=0)
  {
    //如果有空闲的节点，则直接分配出去
    if(freeSlot_!=nullptr){
      pointer result = reinterpret_cast<pointer>(freeSlot_);
      freeSlot_ = freeSlot_->next;
      return result;
    }else{
      //否则需要新建一个block
      if(currentSlot_ >= lastSlot_){
        //分配一个新的block，使用开头n个字节作为一个指针指向前一个block
        data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
        reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
        currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);

        //已使用了block中的前slot_pointer的字节存放指向上一个block的指针
        //将block中剩下的空间切分

        //这是数据链表的起点
        data_pointer_ body = newBlock+sizeof(slot_pointer_);
        //将起点地址转换为unsigned long int类型
        uintptr_t result = reinterpret_cast<uintptr_t>(body);
        //计算链表节点要从哪里开始，基于内存对齐系数
        //先算差值
        size_t bodyPadding = (alignof(slot_type_)-result)%alignof(slot_type_);
        //再算真正起点地址
        currentSlot_ = reinterpret_cast<slot_pointer_>(body+bodyPadding);
        //计算最后一个节点的地址
        lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock+BlockSize-sizeof(slot_type_)+1);
      }
      return reinterpret_cast<pointer>(currentSlot_++);
    }
  }

  //回收空间
  void deallocate(pointer p,size_t n=1)
  {
    //这里我们需要把p指向的位置加入到空闲链表的头部
    if(p!=nullptr){
      //这里需要把p的类型强制转换成slot_pointer
      reinterpret_cast<slot_pointer_>(p)->next = freeSlot_;
      freeSlot_ = reinterpret_cast<slot_pointer_>(p);
    }
  }

  //调用构造函数
  template <typename U,typename... Args>
  void construct(U* p, Args&&... args)
  {
    //这里其实实在已分配的空间上调用构造函数
    //p指向已分配的空间
    new (p) U(std::forward<Args>(args)...);
  }

  //调用析构函数
  template <typename U>
  void destroy(U* p)
  {
    p->~U();
  }

private:
  //每一个block里面为一个链表
  //链表的每一个节点要么存放对象，要么存放指向下一个节点的指针
  //所以这里用了union
  union Slot_{
    T element;
    Slot_* next;
  };

  //数据指针
  typedef char* data_pointer_;
  //对象槽
  typedef Slot_ slot_type_;
  //对象槽指针
  typedef Slot_* slot_pointer_;

  //指向当前block的指针
  slot_pointer_ currentBlock_;
  //指向当前block中的一个链表节点（对象槽）
  slot_pointer_ currentSlot_;
  //指向当前block中的最后一个链表节点
  slot_pointer_ lastSlot_;
  //指向当前block中空闲的链表节点
  slot_pointer_ freeSlot_;

  //检查内存池是否过小
  static_assert(BlockSize>=2*sizeof(slot_type_),"BlockSize too small!");
};

#endif
