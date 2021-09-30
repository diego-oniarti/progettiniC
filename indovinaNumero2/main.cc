#include <iostream>
using namespace std;

int main(){
  int min=1, max=100;
  char in;
  int guess;
  do{
    guess = min+(max-min)/2;
    cout << "Il tuo numero è "<<guess<<"?"<<endl<<"(c=corretto, u=up, d=down)"<<endl<<">";
    cin >> in;
    if (in=='u' || in=='U'){
      min=guess+1;
    }else if (in=='d' || in=='D'){
      max=guess-1;
    }
  }while(in != 'c');
  cout << ":D"<<endl;
  return 0;
}
