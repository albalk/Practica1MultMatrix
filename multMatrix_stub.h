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

            std::cout<<"\tConstructor paso1\n";

            pack(rpcOut, op);
            std::cout<<"\tConstructor paso2\n";
            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId, rpcIn);
            std::cout<<"\tConstructor paso6\n";
            int ok = unpack<unsigned char>(rpcIn);
            //si no es ok error
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }
            std::cout<<"\tConstructor paso7\n";

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
            //enviar la informacion al servidor (nombre del archivo)
                //crear paquete
                multMatrixOp op=readMatrixOp;
                std::vector<unsigned char> rpcOut;
                std::vector<unsigned char> rpcIn;
                std::cout<<"\tRead paso1\n";
                
                //empaquetar nombre de funcion
                pack(rpcOut, op);
                std::cout<<"\tRead paso2\n";
                //empaquetar el tamaño del fichero
                int tam = strlen(fileName)+1;
                pack(rpcOut, tam);
                std::cout<<"\tRead paso3\n";
                //empaquetar nombre del fichero
                packv(rpcOut, fileName, tam);
                std::cout<<"\tRead paso4\n";
                //enviar paquete
                sendMSG(serverConection.serverId, rpcOut);
            
            //recibir numero de filas y columnas en la estructura
                //recibir paquete
                recvMSG(serverConection.serverId,  rpcIn);
                std::cout<<"\tRead paso11\n";
                //crear la estructura resultado
                matrix_t* m=new matrix_t[1];
                
                m->rows=unpack<int>(rpcIn);
                m->cols=unpack<int>(rpcIn);
                m->data=new int [m->rows * m->cols];
                std::cout<<"\tRead paso12\n";
                unpackv(rpcIn, m->data, m->rows * m->cols);
                std::cout<<"\tRead paso13\n";

            //rellenarla desempaquetando datos
                //desempaquetar ok
                int ok = unpack<unsigned char>(rpcIn);
                //si no es ok error
                if(ok!=MSG_OK){
                    std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
                }
                    
                return m; 
            
        };

        matrix_t *multMatrices(matrix_t* m1, matrix_t *m2){
            //enviar la informacion al servidor (nombre de la funcion, m1, m2)
                //crear paquete
                multMatrixOp op=multMatricesOp;
                std::vector<unsigned char> rpcOut;
                std::vector<unsigned char> rpcIn;
                std::cout<<"\tMult paso1\n";
                //empaquetar nombre de funcion
                pack(rpcOut, op);
                std::cout<<"\tMult paso2\n";
                //empaquetar filas y columnas de m1
                pack(rpcOut, m1->rows);
                pack(rpcOut, m1->cols);
                std::cout<<"\tMult paso3\n";
                //empaquetar filas y columnas de m2
                pack(rpcOut, m2->rows);
                pack(rpcOut, m2->cols);
                std::cout<<"\tMult paso4\n";
                //enviar paquete
                sendMSG(serverConection.serverId, rpcOut);

            //recibir numero de filas y columnas en la estructura
                //recibir paquete
                recvMSG(serverConection.serverId,  rpcIn);
                std::cout<<"\tMult paso13\n";
                //crear la estructura resultado
                matrix_t* m=new matrix_t[1];
                //al multiplicar las dimensiones de la matriz resultado son el numfilas de m1 y numColumnas de m2
                m->rows=m1->rows;
                m->cols=m2->cols;
                m->data=nullptr;
                std::cout<<"\tMult paso14\n";
                

            //rellenarla desempaquetando datos
                //desempaquetar ok
                int ok = unpack<unsigned char>(rpcIn);
                std::cout<<"\tMult paso15\n";

                //coger el tamaño
                    //redimensionar la variable
                    std::cout<<"\tMult paso17\n";
                    m->data=new int[m1->rows*m2->cols];
                    std::cout<<"\tMult paso18\n";
                    //desempaquetar
                    std::cout<<"\tMult paso19\n";
                    unpackv(rpcIn, m->data, m->rows * m->cols);
                    std::cout<<"\tMult paso20\n";
                //si no es ok error
                if(ok!=MSG_OK){
                    std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
                }
                    
                return m; 
        };

        void writeMatrix(matrix_t* m, const char *fileName){
            //enviar la informacion al servidor (nombre del archivo)
                //crear paquete
                multMatrixOp op=writeMatrixOp;
                std::vector<unsigned char> rpcOut;
                std::vector<unsigned char> rpcIn;
                std::cout<<"\tWrite paso1\n";
                //empaquetar nombre de funcion
                pack(rpcOut, op);
                std::cout<<"\tWrite paso2\n";
                //empaquetar filas y columnas
                pack(rpcOut, m->rows);
                pack(rpcOut, m->cols);
                std::cout<<"\tWrite paso3\n";
                //empaquetar el tamaño del fichero
                int tam = strlen(fileName)+1;
                pack(rpcOut, tam);
                std::cout<<"\tWrite paso4\n";
                //empaquetar nombre del fichero
                packv(rpcOut, fileName, tam);
                std::cout<<"\tWrite paso5\n";
                //enviar paquete
                sendMSG(serverConection.serverId, rpcOut);
            
            //recibir numero de filas y columnas en la estructura
                //recibir paquete
                recvMSG(serverConection.serverId,  rpcIn);
                std::cout<<"\tWrite paso11\n";
                m->rows=unpack<int>(rpcIn);
                m->cols=unpack<int>(rpcIn);
                std::cout<<"\tWrite paso12\n";
                unpackv(rpcIn, m->data, m->rows * m->cols);
                std::cout<<"\tWrite paso13\n";
                //desempaquetar ok
                int ok = unpack<unsigned char>(rpcIn);
                //si no es ok error
                if(ok!=MSG_OK){
                    std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
                }      
        };
        
        matrix_t *createIdentity(int rows, int cols){
            //enviar la informacion al servidor (nombre de la funcion, filas, columnas)
                //crear paquete
                multMatrixOp op=createIdentityOp;
                std::vector<unsigned char> rpcOut;
                std::vector<unsigned char> rpcIn;

                //empaquetar nombre de funcion
                pack(rpcOut, op);
                //empaquetar filas y columnas
                pack(rpcOut, rows);
                pack(rpcOut, cols);
                //enviar paquete
                sendMSG(serverConection.serverId, rpcOut);

            //recibir numero de filas y columnas en la estructura
                //recibir paquete
                recvMSG(serverConection.serverId,  rpcIn);
                //crear la estructura resultado
                matrix_t* m=new matrix_t[1];
                m->rows=rows;
                m->cols=cols;
                m->data=nullptr;
                

            //rellenarla desempaquetando datos
                //desempaquetar ok
                int ok = unpack<unsigned char>(rpcIn);
                //si no es ok error
                if(ok!=MSG_OK){
                    std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
                }else{
                    //coger el tamaño
                    //redimensionar la variable
                    m->data=new int[rows*cols];
                    //desempaquetar
                    unpackv(rpcIn, m->data, m->rows * m->cols);
                }
                    
                return m; 
        };

        matrix_t *createRandMatrix(int rows, int cols){
            //enviar la informacion al servidor (nombre de la funcion, filas, columnas)
                //crear paquete
                multMatrixOp op=createRandMatrixOp;
                std::vector<unsigned char> rpcOut;
                std::vector<unsigned char> rpcIn;
                std::cout<<"\tRand paso1\n";
                //empaquetar nombre de funcion
                pack(rpcOut, op);
                std::cout<<"\tRand paso2\n";
                //empaquetar filas y columnas
                pack(rpcOut, rows);
                pack(rpcOut, cols);
                std::cout<<"\tRand paso3\n";
                //enviar paquete
                sendMSG(serverConection.serverId, rpcOut);

            //recibir numero de filas y columnas en la estructura
                //recibir paquete
                recvMSG(serverConection.serverId,  rpcIn);
                std::cout<<"\tRand paso7\n";
                //crear la estructura resultado
                matrix_t* m=new matrix_t[1];
                m->rows=rows;
                m->cols=cols;
                m->data=nullptr;

            //rellenarla desempaquetando datos
                //desempaquetar ok
                std::cout<<"\tRand paso8\n";
                int ok = unpack<unsigned char>(rpcIn);
                
                //coger el tamaño
                    //redimensionar la variable
                    m->data=new int[rows*cols];
                    //desempaquetar
                    std::cout<<"\tRand paso11\n";
                    unpackv(rpcIn, m->data, m->rows * m->cols);
                    std::cout<<"\tRand paso12\n";
                
                //si no es ok error
                if(ok!=MSG_OK){
                    std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
                }
                    
                return m; 
                    

        };

    //añadir free de los files
};