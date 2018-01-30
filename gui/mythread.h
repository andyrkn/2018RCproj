#ifndef MYTHREAD_H
#define MYTHREAD_H

#include<QThread>
#include "gameclass.h"

class mythread : public QThread
{
public:
    mythread(gameclass *gc,int filedesc);
    int alive;    //state of thread
    void run();

private:
    gameclass *gc;  //gameclass to handle
    int filedesc;   // descriptor to server
};

#endif // MYTHREAD_H
