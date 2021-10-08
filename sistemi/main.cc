#include <iostream>
#include <iomanip>
using namespace std;

typedef struct MATRICE{
  float **celle;
  int nRighe;
  int nColonne;
} Matrice;

void creaMatrice(Matrice *matrice, int nRighe, int nColonne);
void stampaMatrice(Matrice matrice);
void E(Matrice *matrice, int riga1, int riga2, float bias);
void S(Matrice *matrice, int riga1, int riga2);
void D(Matrice *matrice, int riga, float bias);
int getPivot(Matrice matrice, int riga);
void ordina(Matrice *matrice);

int main(){
  int nEquazioni, nVariabili;
  cout << "Numero equazioni: ";
  cin >> nEquazioni;
  
  cout << "Numero variabili: ";
  cin >> nVariabili;

  Matrice matrice;
  creaMatrice(&matrice, nEquazioni, nVariabili+1); //+1 perché ci sono anche le costanti

  //prende in input le equazioni
  for (int i=0; i<nEquazioni; i++){
    cout << "Inserire coeficienti e termine noto eq."<<(i+1)<<":"<<endl;
    for (int j=0; j<nVariabili+1; j++){
      cout << "> ";
      cin >> *(*(matrice.celle+i)+j);
    }
  }

  cout << "Matrice originale:"<<endl;
  stampaMatrice(matrice);

  //trasforma in una matrice a scaletta
  for (int i=0; i<matrice.nRighe-1; i++){
    for (int j=i+1; j<matrice.nRighe; j++){
      float div = (**(matrice.celle+i));
      E(&matrice, j,i,-( (**(matrice.celle+j)) / ((div!=0)?div:1) ));
    }
    ordina(&matrice);
  }

  cout << "Matrice a scaletta:"<<endl;
  stampaMatrice(matrice);

  //metto i pivot a 1
  for (int i=0; i<matrice.nRighe; ++i){
    D(&matrice, i, 1/(*((*(matrice.celle+i))+getPivot(matrice,i))));
  }

  cout << "Matrice con uni:"<<endl;
  stampaMatrice(matrice);

  //finisco di ridurre
  for (int i=matrice.nRighe-1; i>=1; --i){
    int posPivot = getPivot(matrice,i);
    for (int j=i-1; j>=0; --j){
      E(&matrice, j,i, -(*((*(matrice.celle+j))+posPivot)));
    }
  }

  cout << "Matrice ridotta:"<<endl;
  stampaMatrice(matrice);
}


void ordina(Matrice *m){
  Matrice &mat = *m;
  for (int i=mat.nRighe-2; i>=0; i--){
    for (int j=0; j<=i; j++){
      if (getPivot(*m, j) > getPivot(*m, j+1)){
	float *holder = *(mat.celle+j);
	*(mat.celle+j) = *(mat.celle+j+1);
	*(mat.celle+j+1) = holder;
      }
    }
  }
  return;
}

void stampaMatrice(Matrice matrice){
  for (int i=0; i<matrice.nRighe; i++){
    cout << "| ";
    for (int j=0; j<matrice.nColonne; j++){
      cout << setw(4) << *(*(matrice.celle+i)+j);
    }
    cout << " |" << endl;
  }
  return;
}

int getPivot(Matrice mat, int riga){
  int ret=0;
  while (ret<mat.nColonne && *((*(mat.celle+riga))+ret)==0){
    ++ret;
  }

  return ret;
}

void E(Matrice *m, int riga1, int riga2, float bias){
  Matrice &mat = *m;
  
  for (int i=0; i<mat.nColonne; i++){
    *(*(mat.celle+riga1)+i) += (*(*(mat.celle+riga2)+i))*bias; //mat[riga1][i] += mat[riga2][i]*bias
  }
}

void S(Matrice *m, int riga1, int riga2){
  Matrice &mat = *m;

  float* holder = *(mat.celle+riga1);
  *(mat.celle+riga1) = *(mat.celle+riga2);
  *(mat.celle+riga2) = holder;
}

void D(Matrice *m, int riga, float bias){
  Matrice &mat = *m;
  for (int i=0; i<mat.nColonne; i++){
    *((*(mat.celle+riga))+i) *= bias;
  }
}

void creaMatrice(Matrice *m, int nRighe, int nColonne){
  Matrice &mat=*m;
  mat.nRighe = nRighe;
  mat.nColonne = nColonne;
  mat.celle = (float**)malloc(sizeof(float*)*nRighe);
  for (int i=0; i<nRighe; i++){
    *(mat.celle+i) = (float*)malloc(sizeof(float)*nColonne);
  }
  return;
}
