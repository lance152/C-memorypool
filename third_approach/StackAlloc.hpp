#include <memory>

//用链表实现一个栈

//定义链表中的节点
template <typename T>
struct StackNode
{
  T data;
  StackNode* prev;
};

// T为存储对象类型，Alloc为使用的分配器，默认使用std::Allocator作为分配器
template <typename T,typename Alloc=std::allocator<T>>
class StackAlloc
{
public:
  //定义别名
  typedef StackNode<T> Node;
  typedef typename Alloc::template rebind<Node>::other allocator;

  //构造函数
  StackAlloc()
  {
    head_ = 0;
  }

  //析构
  ~StackAlloc()
  {
    clear();
  }

  //当栈中元素为空时，返回true
  bool empty()
  {
    return head_ == 0;
  }

  //释放栈中元素
  void clear()
  {
    Node* cur = head_;
    while(cur!=0){
        Node* tmp = cur->prev;
        allocator_.destroy(cur);
        allocator_.deallocate(cur,1);
        cur = tmp;
    }
    head_ = 0;
  }

  //压入
  void push(T element)
  {
    //为一个节点分配内存
    Node* newNode = allocator_.allocate(1);
    //调用节点构造函数
    allocator_.construct(newNode,Node());

    //入栈
    newNode->data = element;
    newNode->prev = head_;
    head_ = newNode;
  }

  //出栈
  T pop()
  {
    T result = head_->data;
    Node* tmp = head_->prev;
    allocator_.destroy(head_);
    allocator_.deallocate(head_,1);
    head_ = tmp;
    return result;
  }

  //栈顶
  T top()
  {
    return head_->data;
  }

private:
  allocator allocator_;
  Node* head_;
};
