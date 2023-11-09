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
        multMatrixOp operacion = unpack<multMatrixOp>(rpcIn);
        //std::cout<<"Después de recibir mensaje\n";

        //ejecuta
        switch(operacion){
            case constructorOp:{
                std::cout<<"\tConstructor paso3\n";
                m=new multMatrix();
                std::cout<<"\tConstructor paso4\n";
                pack(rpcOut, (unsigned char)MSG_OK);
                std::cout<<"\tConstructor paso5\n";
            }break;
            case destructorOp:{
                delete m;
                m=nullptr;
                pack(rpcOut, (unsigned char)MSG_OK);
            }break;
            case readMatrixOp:{ //Cambiar para recibir un string
                matrix_t* mres = new matrix_t();
                std::cout<<"\tRead paso5\n";
                int tam = unpack<int>(rpcIn);
                char fileName[tam];
                std::cout<<"\tRead paso6\n";
                unpackv(rpcIn, (char*)fileName, tam);
                std::cout<<"\tRead paso7\n";
                mres = m->readMatrix(fileName);
                std::cout<<"\tRead paso8\n";

                pack(rpcOut, mres->rows);
                pack(rpcOut, mres->cols);
                std::cout<<"\tRead paso9\n";
                packv(rpcOut, mres->data, mres->rows * mres->cols);
                std::cout<<"\tRead paso10\n";
                pack(rpcOut, (unsigned char)MSG_OK);

                //liberar memoria
                freeMatrix(mres);
            }break;
            case multMatricesOp:{
                //crear matrices resultado, 1 y 2
                std::cout<<"\tMult paso5\n";
                matrix_t* mres = new matrix_t();
                matrix_t* m1 = new matrix_t();
                matrix_t* m2 = new matrix_t();
                std::cout<<"\tMult paso6\n";
                //desempaquetar el num rows y cols de m1
                std::cout<<"\tMult paso7\n";
                int numFilas1 = unpack<int>(rpcIn);
                int numColumnas1 =  unpack<int>(rpcIn);
                std::cout<<"\tMult paso8\n";
                //desempaquetar el num rows y cols de m2
                std::cout<<"\tMult paso9\n";
                int numFilas2 = unpack<int>(rpcIn);
                int numColumnas2 =  unpack<int>(rpcIn);
                std::cout<<"\tMult paso10\n";
                //matriz resultado
                mres = m->multMatrices(m1, m2);
                std::cout<<"\tMult paso11\n";
                
                pack(rpcOut, (unsigned char)MSG_OK);
                std::cout<<"\tMult paso12\n";
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                std::cout<<"\tMult paso16\n";
                packv(rpcOut, mres->data, mres->rows * mres->cols);
                
                //liberar memoria
                freeMatrix(mres);
                freeMatrix(m1);
                freeMatrix(m2);
            }break;
            case writeMatrixOp:{
                matrix_t* mres = new matrix_t();
                mres->rows=unpack<int>(rpcIn);
                mres->cols=unpack<int>(rpcIn);
                int tam = unpack<int>(rpcIn);
                std::cout<<"\tWrite paso6\n";
                char fileName[tam];
                unpackv(rpcIn, (char*)fileName, tam);
                std::cout<<"\tWrite paso7\n";
                m->writeMatrix(mres, fileName);
                std::cout<<"\tWrite paso8\n";

                pack(rpcOut, mres->rows);
                pack(rpcOut, mres->cols);
                std::cout<<"\tWrite paso9\n";
                
                packv(rpcOut, mres->data, mres->rows * mres->cols);
                std::cout<<"\tWrite paso10\n";
                pack(rpcOut, (unsigned char)MSG_OK);

                //liberar memoria
                freeMatrix(mres);
            }break;
            case createIdentityOp:{
                matrix_t* mres = new matrix_t();;
                int numFilas = unpack<int>(rpcIn);
                int numColumnas =  unpack<int>(rpcIn);
                mres = m->createIdentity(numFilas, numColumnas);
                
                pack(rpcOut, (unsigned char)MSG_OK);
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                packv(rpcOut, mres->data, mres->rows * mres->cols);

                //liberar memoria
                freeMatrix(mres);
            }break;
            case createRandMatrixOp:{
                
                matrix_t* mres = new matrix_t();
                std::cout<<"\tRand paso4\n";
                int numFilas = unpack<int>(rpcIn);
                int numColumnas =  unpack<int>(rpcIn);
                std::cout<<"\tRand paso5\n";
                mres = m->createRandMatrix(numFilas, numColumnas);
                std::cout<<"\tRand paso6\n";
                pack(rpcOut, (unsigned char)MSG_OK);
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                packv(rpcOut, mres->data, mres->rows * mres->cols);
                std::cout<<"\tRand paso9\n";
                //liberar memoria
                freeMatrix(mres);
                std::cout<<"\tRand paso10\n";
            }break;
            default:{
                std::cout<<"Error: funcion no definida\n";
                pack(rpcOut, (unsigned char)MSG_NOK);
            }break;

        }
        //devuelve resultados
        std::cout<<"\tantes de enviar resultados\n";
        sendMSG(clientId, rpcOut);
        std::cout<<"\tdespues de enviar resultados\n";
    }

    //añadir free de los files

};