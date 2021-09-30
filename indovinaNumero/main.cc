#include <time.h>
#include <iostream>
using namespace std;

int main(){
  srand(time(NULL));

  int segreto = (rand()%100)+1;
  cout << "Indovina il numero tra 1 e 100"<<endl;

  int guess;
  do{
    cout << "> ";
    cin >> guess;
    if (guess > segreto){
      cout << "Abbassa"<<endl;
    }else if (guess < segreto){
      cout << "Alza"<<endl;
    }
  }while (guess != segreto);
  cout << "Corretto!"<<endl;
}
