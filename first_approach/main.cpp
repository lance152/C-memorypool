#include <iostream>

#include "foo.h"

using namespace std;

int main()
{
  int arr_size = 10;
  foo *f[arr_size];
  cout<<"sizeof(foo) = "<<sizeof(foo)<<endl;

  for(int i=0;i<arr_size;i++){
    f[i]=new foo();
    cout<<f[i]<<endl;
  }

  for(int i=0;i<arr_size;i++){
    delete f[i];
  }
  return 0;
}
