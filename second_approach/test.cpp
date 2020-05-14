#include <iostream>

using namespace std;


int main(){
  int b = 10;
  int a =100;
  int *p;
  p = &b;
  cout<<"&b: "<<&b<<endl;
  cout<<"&p: "<<&p<<endl;
  cout<<"p: "<<p<<endl;
  cout<<"int p: "<<(int) p<<endl;
  cout<<"*p: "<<*p<<endl;
  cout<<"int *p: "<<(int) *p <<endl;
  unsigned char** q = (unsigned char**)p;
  cout<<"&q: "<<&q<<endl;
  cout<<"q: "<<q<<endl;
  cout<<"*q: "<<*q<<endl;
  *q = (unsigned char*)p;
  cout<<(int)&a<<endl;
  cout<<"*q: "<<*q<<endl;
  cout<<"*p: "<<*p<<endl;
  cout<<"p: "<<p<<endl;
  cout<<b<<endl;
}
