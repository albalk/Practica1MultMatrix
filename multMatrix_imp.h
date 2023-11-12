#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"

//intermediario entre cliente (stub) la persona

class multMatrix_imp{
    private:
        int clientId=-1;
        multMatrix* m=nullptr;

        void freeMatrix(matrix_t* m){
            delete[] m->data;
            delete[] m;
        }


    public:
    //constructor
    multMatrix_imp(int clientId): clientId(clientId){}; //inicializa clientid con el clientid que nos pasan

    bool conectionClosed(){
        return m==nullptr;
    }

    void recibeOp(){
        std::vector<unsigned char>  rpcIn;
        std::vector<unsigned char>  rpcOut;
        //recibe operaciones
        recvMSG(clientId, rpcIn);
        multMatrixOp operacion = unpack<multMatrixOp>(rpcIn); //desempaqueta la operación

        //ejecuta
        switch(operacion){
            case constructorOp:{
                m=new multMatrix(); //llamada al constructor
                pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK
            }break;
            case destructorOp:{
                delete m; //elimina la matriz
                m=nullptr;
                pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK
            }break;
            case readMatrixOp:{
                //crea la matriz resultado
                matrix_t* mres = new matrix_t();
                //desempaqueta el tamaño del fichero
                int tam = unpack<int>(rpcIn);
                char fileName[tam]; //reserva memoria para el nombre del fichero

                //desempaqueta el nombre del fichero
                unpackv(rpcIn, (char*)fileName, tam);
                mres = m->readMatrix(fileName); //llamada a la función readMatrix

                //empaqueta el numero de filas y columnas
                pack(rpcOut, mres->rows);
                pack(rpcOut, mres->cols);
                //empaqueta el contenido de la matriz
                packv(rpcOut, mres->data, mres->rows * mres->cols);
                pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK

                //liberar memoria
                freeMatrix(mres);

            }break;
            case multMatricesOp:{
                //crear matrices resultado, 1 y 2
                matrix_t* mres = new matrix_t();
                matrix_t* m1 = new matrix_t();
                matrix_t* m2 = new matrix_t();

                //desempaquetar el num rows y cols de m1
                m1->rows = unpack<int>(rpcIn);
                m1->cols =  unpack<int>(rpcIn);
                m1->data=new int[m1->rows*m1->cols]; //reserva de memoria
                //desempaqueta el contenido de m1
                unpackv(rpcIn, m1->data, m1->rows * m1->cols);

                //desempaquetar el num rows y cols  de m2
                m2->rows = unpack<int>(rpcIn);
                m2->cols = unpack<int>(rpcIn);
                m2->data=new int[m2->rows*m2->cols]; //reserva de memoria
                //desempaqueta el contenido de m1
                unpackv(rpcIn, m2->data, m2->rows * m2->cols);

                //matriz resultado
                mres = m->multMatrices(m1, m2); //llamada a la funcion multMatrices
                
                pack(rpcOut, (unsigned char)MSG_OK);  //empaqueta MSG_OK
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                packv(rpcOut, mres->data, mres->rows * mres->cols);
                
                //liberar memoria
                freeMatrix(mres);
                freeMatrix(m1);
                freeMatrix(m2);
            }break;
            case writeMatrixOp:{
                //declarar la matriz resultado
                matrix_t* mres = new matrix_t();
                //desempaquetar las dimensiones
                mres->rows=unpack<int>(rpcIn);
                mres->cols=unpack<int>(rpcIn);
                mres->data=new int[mres->rows * mres->cols]; //reserva de memoria

                //desempaquetar el tamaño del fichero
                int tam = unpack<int>(rpcIn);
                char fileName[tam]; //reserva de memoria
                //desempaquetar el nombre del fichero
                unpackv(rpcIn, (char*)fileName, tam);
                m->writeMatrix(mres, fileName); //llamada a la función writeMatrix
                
                pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK

                //liberar memoria
                freeMatrix(mres);
            }break;
            case createIdentityOp:{
                //declarar matriz resultado
                matrix_t* mres = new matrix_t();
                //desempaquetar las dimensiones
                int numFilas = unpack<int>(rpcIn);
                int numColumnas =  unpack<int>(rpcIn);
                mres = m->createIdentity(numFilas, numColumnas); //llamada a la función createIdentity
                
                pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                //empaquetar el contenido de la matriz resultado
                packv(rpcOut, mres->data, mres->rows * mres->cols);

                //liberar memoria
                freeMatrix(mres);
            }break;
            case createRandMatrixOp:{
                //declarar matriz resultado
                matrix_t* mres = new matrix_t();
                //desempaquetar las dimensiones
                int numFilas = unpack<int>(rpcIn);
                int numColumnas =  unpack<int>(rpcIn);
                mres = m->createRandMatrix(numFilas, numColumnas); //llamada a la función createRandMatrix
                    
                pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                //empaquetar el contenido de la matriz resultado
                packv(rpcOut, mres->data, mres->rows * mres->cols);
                    
                //liberar memoria
                freeMatrix(mres);
                    
            }break;
            default:{
                    
                pack(rpcOut, (unsigned char)MSG_NOK);
            }break;

        }
        //devuelve resultados
            
        sendMSG(clientId, rpcOut);
            
    }


};