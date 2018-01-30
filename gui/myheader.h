#ifndef MYHEADER_H
#define MYHEADER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

#include "mainwindow.h"
#include "gameclass.h"
#include "mythread.h"

#include<QApplication>

void startGame(int sd,char name[100],gameclass *gc){
        //gc->update(signal);
        mythread *mthread = new mythread(gc,sd);
        mthread->start();

        while(1){
            qApp->processEvents();
            if(mthread->alive==-1) break;
        }

        mthread->terminate();
        mthread->wait();
}

void login(MainWindow *w,char param[100]){
    {
        int sd;
        struct sockaddr_in server;
        int nr=0;
        char buf[10];
        char* addr = "127.0.0.1";
        int port=2908;

        if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
            w->setText("socket error");

        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(addr);
        server.sin_port = htons (port);

        if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
               w->setText("connect error");
        // send server main request;
        int x=2,sgn = 0;
        char buffer[100];

        write(sd,param,100);
        write(sd,w->getName(),100);
        write(sd,w->getPW(),100);

        // X lines of answer from server go here!, they will be appended to textBox window1

        read(sd,&x,sizeof(int));

        if(x==1 && !strcmp(param,"login")){
            read(sd,&sgn,sizeof(int));
            if(sgn==1){
                /*
                while(1){
                    int state;
                    read(sd,&state,sizeof(int));
                    if(state == 1) break;
                    else
                        w->SetProgressBar(state - 10);
                  qApp->processEvents();
                }
                */

                gameclass *gc = w->signalStage();
                w->setText("Login complete!");
                startGame(sd,w->getName(),gc);
            }
            else
                w->setText("User doesn't exist, or incorrect password!");
            }
        else
            while(x>0){
                read(sd,buffer,100);
                w->setText(buffer);
                x--;
            }
            //

        close(sd);
    }
}



#endif // MYHEADER_H
