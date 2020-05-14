#include <iostream>
#include <memory>
#include <limits>
#include <vector>

template<typename T>
class TrackingAllocator
{
public:
  using value_type = T;

  //optional alias
  using pointer = T*;
  using const_pointer = const T*;

  // using void_pointer = void *;
  // using const_void_pointer = const void*;

  using size_type = size_t;

  // using different_type = std::ptrdiff_t;

  // template<typename U>
  // struct rebind
  // {
  //   using other = TrackingAllocator<U>;
  // };

  TrackingAllocator() = default;

  template<typename U>
  TrackingAllocator(const TrackingAllocator<U> &other){}

  ~TrackingAllocator() = default;

  pointer allocate(size_type numObjects)
  {
    mAllocations+=numObjects;
    return static_cast<pointer>(operator new(sizeof(T)*numObjects));
  }

  // pointer allocate(size_type numObjects,const_void_pointer hint)
  // {
  //   return allocate(numObjects);
  // }

  void deallocate(pointer p,size_type numObjects)
  {
    operator delete(p);
  }

  // size_type max_size() const
  // {
  //   return std::numeric_limits<size_type>::max();
  // }

  // template<typename U,typename... Args>
  // void construct(U* p,Args && ... args)
  // {
  //   new(p) U(std::forward<Args>(args)...);
  // }
  //
  // template<typename U>
  // void destroy(U* p)
  // {
  //   p->~U();
  // }

  size_type get_allocations() const
  {
    return mAllocations;
  }

private:
  static size_type mAllocations;
};

template<typename T>
typename TrackingAllocator<T>::size_type TrackingAllocator<T>::mAllocations=0;

int main()
{
  // using TAint = TrackingAllocator<int>;
  // //rebind
  // using TAdouble = TAint::rebind<double>::other;
  std::vector<int,TrackingAllocator<int>> v(5);
  std::cout<<v.get_allocator().get_allocations()<<std::endl;
  return 0;
}
