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
                m=new multMatrix();
                pack(rpcOut, (unsigned char)MSG_OK);
            }break;
            case destructorOp:{
                delete m;
                m=nullptr;
                pack(rpcOut, (unsigned char)MSG_OK);
            }break;
            case readMatrixOp:{ //Cambiar para recibir un string
                matrix_t* m;
                int tam = unpack<int>(rpcIn);
                std::string fileName;
                unpackv(rpcIn, (char*)fileName.data(), tam);
                //m=readMatrix(fileName);
                
                pack(rpcOut, (unsigned char)MSG_OK);
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                packv(rpcOut, m->data, m->rows * m->cols);

                //liberar memoria
                freeMatrix(m);
            }break;
            case multMatricesOp:{
                //crear matrices resultado, 1 y 2
                matrix_t* m;
                matrix_t* m1;
                matrix_t* m2;
                //desempaquetar el num rows y cols de m1
                int numFilas1 = unpack<int>(rpcIn);
                int numColumnas1 =  unpack<int>(rpcIn);
                //desempaquetar el num rows y cols de m2
                int numFilas2 = unpack<int>(rpcIn);
                int numColumnas2 =  unpack<int>(rpcIn);
                //matriz resultado
                //m=multMatrices(m1, m2);
                
                pack(rpcOut, (unsigned char)MSG_OK);
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                packv(rpcOut, m->data, m->rows * m->cols);
                
                //liberar memoria
                freeMatrix(m);
                freeMatrix(m1);
                freeMatrix(m2);
            }break;
            case writeMatrixOp:{
                /*matrix_t* m;
                int numFilas = unpack<int>(rpcIn);
                int numColumnas =  unpack<int>(rpcIn);*/
            }break;
            case createIdentityOp:{
                matrix_t* m;
                int numFilas = unpack<int>(rpcIn);
                int numColumnas =  unpack<int>(rpcIn);
                //m=createIdentity(numFilas, numColumnas);
                
                pack(rpcOut, (unsigned char)MSG_OK);
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                packv(rpcOut, m->data, m->rows * m->cols);

                //liberar memoria
                freeMatrix(m);
            }break;
            case createRandMatrixOp:{

                matrix_t* m;
                int numFilas = unpack<int>(rpcIn);
                int numColumnas =  unpack<int>(rpcIn);
                //m=createRandMatrix(numFilas, numColumnas);
                
                pack(rpcOut, (unsigned char)MSG_OK);
                //pack(rpcOut, m->rows); no es necesario porque ya las tenemos (se envian por parametros)
                //pack(rpcOut, m->cols);
                packv(rpcOut, m->data, m->rows * m->cols);
                
                //liberar memoria
                freeMatrix(m);
            }break;
            default:{
                std::cout<<"Error: funcion no definida\n";
                pack(rpcOut, (unsigned char)MSG_NOK);
            }break;

        }
        //devuelve resultados
        sendMSG(clientId, rpcOut);
    }

    //añadir free de los files

};