//Diego Oniarti
#include <iostream>
#include <stdint.h>
using namespace std;
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

//Stampa a video un immagine
int main(int argc, char **argv) {
  //Se non è specificato il nome file, esci
  if (argc<2){
    cout << "Missing filename"<endl;
    return -1;
  }
  char* fileName = argv[1];

  //dimensioni dell'immagine
  int width, height, bpp;
  uint8_t* rgb_image = stbi_load(fileName, &width, &height, &bpp, 3);
  //Se l'immagine non viene caricata correttamente, esci
  if (rgb_image==NULL){
    cout << "Errore di caricamento immagine"<<endl;
    return -1;
  }

  //Ottiene le dimensioni del terminale (colonne e righe)
  struct winsize size;
  ioctl( 0, TIOCGWINSZ, (char *) &size );
  int PWidth = size.ws_col, PHeight = size.ws_row;

  // Decide come scalare l'immagine (il fattore limitante potrebbe essere l'altezza oppure la largezza in base alle proporzioni dell'immagine e del terminale)
  float scalaW = (float)width/(PWidth*0.8F);
  float scalaH = (float)height/(PHeight)/2;

  //Gli step indicano quanti pixel vanno saltati per campionare l'immagine correttamente
  //Gli step verticali sono il doppio di quelli orizzontali per compensare il fatto che i carattere nel terminale sono più alti che larghi
  int stepY,stepX;
  if (height/scalaW < PHeight*2){
    stepY = scalaW*2;
    stepX = scalaW;
  }else{
    stepY = scalaH*2;
    stepX = scalaH;
  }
  
  
  int r,g,b, media;
  for (int y=0; y<height; y+=stepY){
    for  (int x=0; x<width; x+=stepX){
      r = (int) *(rgb_image + (y*width+x)*3    );
      g = (int) *(rgb_image + (y*width+x)*3 + 1);
      b = (int) *(rgb_image + (y*width+x)*3 + 2);
      media = (r+g+b)/3;
      if (media<51){
	cout<<' ';
      }else if (media<102){
	cout<<"░";
      }else if (media < 153){
	cout<<"▒";
      }else if (media < 204){
	cout<<"▓";
      }else{
	cout<<"█";
      }
    }
    cout << endl;
  }
  
  stbi_image_free(rgb_image);
  
  return 0;
}
