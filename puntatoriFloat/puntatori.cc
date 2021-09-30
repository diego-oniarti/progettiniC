#include <iostream>
using namespace std;

int main(){
  float a,b;

  cout << "Inserire A e B:" << endl;
  cin >> a >> b;

  float dif = a-b;

  cout << "A-B: "<<dif<<endl;

  //sets the first bit of the float number to 0
  long tmp = *(long*)&dif;
  tmp &= 0b01111111111111111111111111111111;
  dif = *(float*)&tmp;
  
  cout << "|A-B|: "<<dif<<endl;
  
  return 0;
}
