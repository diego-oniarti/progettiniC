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
bool isEmptyLine(Matrice matrice, int riga);
int getPivot(Matrice matrice, int riga);
void ordina(Matrice *matrice);
bool isImpossible(Matrice mat);
void eliminaRiga(Matrice *matrice, int riga);
bool risolviMatrice(Matrice *matrice);

int main(){
  float x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4, d1,d2,d3,d4;

  cout <<"x1,y1,z1,d1: ";
  cin >> x1>>y1>>z1>>d1;
  
  cout <<"x2,y2,z2,d2: ";
  cin >> x2>>y2>>z2>>d2;
  
  cout <<"x3,y3,z3,d3: ";
  cin >> x3>>y3>>z3>>d3;
  
  cout <<"x4,y4,z4,d4: ";
  cin >> x4>>y4>>z4>>d4;

  
  Matrice matrice;
  creaMatrice(&matrice, 4, 7); //+1 perché ci sono anche le costanti

  //prende in input le equazioni
  for (int i=0; i<matrice.nRighe; i++){
    for (int j=0; j<3; j++){
      *((*(matrice.celle+i))+j) = 1;
    }
  }
  matrice.celle[0][3]=-2*x1;
  matrice.celle[0][4]=-2*y1;
  matrice.celle[0][5]=-2*z1;
  matrice.celle[0][6]=(d1*d1)-(x1*x1)-(y1*y1)-(z1*z1);

  matrice.celle[1][3]=-2*x2;
  matrice.celle[1][4]=-2*y2;
  matrice.celle[1][5]=-2*z2;
  matrice.celle[1][6]=(d2*d2)-(x2*x2)-(y2*y2)-(z2*z2);

  matrice.celle[2][3]=-2*x3;
  matrice.celle[2][4]=-2*y3;
  matrice.celle[2][5]=-2*z3;
  matrice.celle[2][6]=(d3*d3)-(x3*x3)-(y3*y3)-(z3*z3);

  matrice.celle[3][3]=-2*x4;
  matrice.celle[3][4]=-2*y4;
  matrice.celle[3][5]=-2*z4;
  matrice.celle[3][6]=(d4*d4)-(x4*x4)-(y4*y4)-(z4*z4);

  bool risolta = risolviMatrice(&matrice);

  if (risolta){
    cout << "risolta: "<<endl;
    stampaMatrice(matrice);
    
    Matrice m2;
    creaMatrice(&m2, 3,4);
    //mette nella matrice m2 il contenuto dell'angolo in basso a destra della matrice appena "risolta"
    for (int i=0,j=1; i<m2.nRighe && j<matrice.nRighe; ++i,++j){
      for (int k=0,n=3; k<m2.nColonne && n<matrice.nColonne; ++n,++k){
	//m2.celle[i][k] = matrice.celle[j][n];
	*((*(m2.celle+i))+k) = *((*(matrice.celle+j))+n);
      }
    }

    risolta = risolviMatrice(&m2);
    if (risolta){
      cout << "Soluzione:"<<endl;
      stampaMatrice(m2);
    }else{
      cout << "Inrisolubile"<<endl;
    }
    
  }else{
    cout << "inrisolvibile"<<endl;
  }
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
