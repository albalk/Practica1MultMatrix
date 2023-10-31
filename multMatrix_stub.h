#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operaciones.h"

typedef struct matrix_t{
    int rows;
    int cols;
    int* data;
}matrix_t;

//solo se encarga de comunicaciones
class multMatrix {
    private:
        std::string ip="172.31.50.30";
        int port=60000;
        connection_t serverConection;
    public:
        multMatrix(){ //constructor
            //conectar con servidor
            serverConection=initClient(ip, port);
            multMatrixOp op=constructorOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId, rpcIn);

            if(rpcIn[0]!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        };

        ~multMatrix(){ //destructor
            //cerrar conexion
            multMatrixOp op=destructorOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId,  rpcIn);

            if(rpcIn[0]!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

            closeConnection(serverConection.serverId);

        };

        matrix_t* readMatrix(const char* fileName){

        };

        matrix_t *multMatrices(matrix_t* m1, matrix_t *m2){

        };

        void writeMatrix(matrix_t* m, const char *fileName){

        };
        
        matrix_t *createIdentity(int rows, int cols){

        };

        matrix_t *createRandMatrix(int rows, int cols){

        };


};