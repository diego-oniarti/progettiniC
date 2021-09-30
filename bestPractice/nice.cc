#include <iostream>
using namespace std;

int main(){

  int a = 0;

 loop:
  cout << a << endl;
  if (++a < 10) goto loop;


  for (cout<<a<<endl;false;){}
  
  return 0;
}
