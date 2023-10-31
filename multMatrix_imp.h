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
        std::cout<<"Después de recibir mensaje\n";

        //ejecuta
        switch(operacion){
            case constructorOp:{
                std::cout<<"Constructor inicio\n";
                m=new multMatrix();
                std::cout<<"Constructor despues\n";
                
                //pack(rpcOut, (unsigned char)MSG_OK);
            }break;
            case destructorOp:{
                std::cout<<"Destructor inicio\n";
                delete m;
                m=nullptr;
                std::cout<<"Destructor despues\n";
                //pack(rpcOut, (unsigned char)MSG_OK);
            }break;
            case readMatrixOp:{
                
            }break;
            case multMatricesOp:{
                
                
            }break;
            case writeMatrixOp:{
                
            }break;
            case createIdentityOp:{
                
            }break;
            case createRandMatrixOp:{
                
            }break;
            default:{
                std::cout<<"Error: funcion no definida\n";
                pack(rpcOut, (unsigned char)MSG_NOK);
            }break;

        }
        //devuelve resultados
        sendMSG(clientId, rpcOut);
    }

};