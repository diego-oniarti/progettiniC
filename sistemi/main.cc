#include <iostream>
#include <iomanip>
using namespace std;


//Calcola i valori che soddisfano un sistema di n equazioni lineari in m variabili
//delle variabili devono essere indicati solo i coefficienti
//Es.
//equazioni: 5x + 3y -9z = 4
//           5x -4z = 13
//input:     5 3 -9 4
//           5 0 -4 13


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
bool isEmptyLine(Matrice matrice, int riga);
int getPivot(Matrice matrice, int riga);
void ordina(Matrice *matrice);
bool isImpossible(Matrice mat);
void eliminaRiga(Matrice *matrice, int riga);
bool risolviMatrice(Matrice *matrice);
void distruggiMatrice(Matrice *mat);

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

  bool risolta = risolviMatrice(&matrice);

  if (risolta){
    cout << "risolta: "<<endl;
    stampaMatrice(matrice);
    cout << "Soluzioni:"<<endl;
    for (int i=0; i<matrice.nRighe; i++){
      cout << (*((*(matrice.celle+i))+matrice.nColonne-1)) << " ";
    }
    cout << endl;
  }else{
    cout << "inrisolvibile"<<endl;
  }
  distruggiMatrice(&matrice);
  return 0;
}


bool risolviMatrice(Matrice *m){
  Matrice &matrice = *m;
  bool ret = true;
  //trasforma in una matrice a scaletta
  for (int i=0; i<matrice.nRighe-1; i++){
    for (int j=i+1; j<matrice.nRighe; j++){
      float div = *((*(matrice.celle+i))+i);
      E(&matrice, j,i,-( (*((*(matrice.celle+j))+i)) / ((div!=0)?div:1) ));
    }
    ordina(&matrice);
  }

  //cout << "Matrice a scaletta:"<<endl;
  //stampaMatrice(matrice);

  if (isImpossible(matrice)){
    //cout << "Impossibile!" << endl;
    ret = false;
  }else{

    for (int i=matrice.nRighe-1; i>=0; --i){
      if (isEmptyLine(matrice,i)){
	eliminaRiga(&matrice,i);
      }
    }
    
    //metto i pivot a 1
    for (int i=0; i<matrice.nRighe; ++i){
      D(&matrice, i, 1/(*((*(matrice.celle+i))+getPivot(matrice,i))));
    }

    //cout << "Matrice con uni:"<<endl;
    //stampaMatrice(matrice);

    //finisco di ridurre
    for (int i=matrice.nRighe-1; i>=1; --i){
      int posPivot = getPivot(matrice,i);
      for (int j=i-1; j>=0; --j){
	E(&matrice, j,i, -(*((*(matrice.celle+j))+posPivot)));
      }
    }

    //cout << "Matrice ridotta:"<<endl;
    //questo ciclo potrebbe essere utile
    for (int i=0; i<matrice.nRighe; ++i){
      D(&matrice, i, 1/(*((*(matrice.celle+i))+getPivot(matrice,i))));
    }
    //stampaMatrice(matrice);
  }
  return ret;
}

bool isEmptyLine(Matrice mat, int riga){
  return getPivot(mat,riga)==mat.nColonne;
}
bool isImpossible(Matrice mat){
  bool impossibile = false;
  int i=0;
  while (!impossibile && i<mat.nRighe){
    impossibile = (getPivot(mat,i)==mat.nColonne-1);
    ++i;
  }
  return impossibile;
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
      cout << setw(4) << *(*(matrice.celle+i)+j) << " ";
    }
    cout << " |" << endl;
  }
  return;
}

void eliminaRiga(Matrice *m, int riga){
  Matrice &mat = *m;

  for (int i=riga; i<mat.nRighe-1; i++){
    *(mat.celle+i)=*(mat.celle+i+1);
  }
  --mat.nRighe;
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

void distruggiMatrice(Matrice *m){
  Matrice &mat=*m;
  for (int i=0; i<mat.nRighe; ++i){
    free(*(mat.celle+i));
  }
  free(mat.celle);
  return;
}
