#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "multMatrix_stub.h"

void freeMatrix(matrix_t* m){
    delete[] m->data;
    delete[] m;

}
int main()
{ 
    std::cout<<"\tllamada Constructor\n";
    multMatrix* mmatrix=new multMatrix();

    //std::cout<<"\tllamada RandMatrix\n"; //FUNCIONA
    //matrix_t* m1= mmatrix->createRandMatrix(5,5);

    //std::cout<<"\tllamada Identity\n"; //FUNCIONA
    //matrix_t* m2= mmatrix->createIdentity(5,5);

    //std::cout<<"\tllamada multMatrix\n";
    //matrix_t* mres=mmatrix->multMatrices(m1,m2); //ERROR UNPACKV FINAL

    //std::cout<<"\tllamada writeMatrix\n";
    //mmatrix->writeMatrix(mres,"resultado.txt");

    std::cout<<"\tllamada readMatrix\n";
    matrix_t* m3=mmatrix->readMatrix("resultado.txt");
    
    std::cout<<"\tllamada writeMatrix\n";
    mmatrix->writeMatrix(m3,"resultado2.txt");
    
    //matrix_t* mres2=mmatrix->multMatrices(m1,m3);
    //mmatrix->writeMatrix(mres2,"resultado2.txt");



    //freeMatrix(m1);
    //freeMatrix(m2);
    //freeMatrix(mres);
    //freeMatrix(m3);
    //freeMatrix(mres2);
    delete mmatrix;

}