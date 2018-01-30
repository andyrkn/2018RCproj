#include "mythread.h"
#include <unistd.h>
#include <string.h>

mythread::mythread(gameclass *gc,int filedesc)
{
    this->gc = gc;
    this->filedesc=filedesc;
    this->alive=0;
}


void mythread::run()
{
    int signal = 3;
    int CurrentQuestion = -1;
    char name[100];
    char buffer[200];

    // get user name and set,
    signal = 3;
    write(filedesc,&signal,sizeof(int));
    read(filedesc,name,100);
    gc->setUserName(name);

    while(1){
        //check if game ended
        if(gc->alive==-1){
            signal = 10;
            write(filedesc,&signal,sizeof(int));

            this->alive=-1;
            this->terminate();
        }

        //get my points
        signal = 13;
        write(filedesc,&signal,sizeof(int));
        read(filedesc,&signal,sizeof(int));
        gc->updateMyPoints(signal);

        //check player answer
        signal = 11;
        write(filedesc,&signal,sizeof(int));
        int answer = gc->getOption();
        write(filedesc,&answer,sizeof(int));

        // update seconds left here
        signal = 1;
        write(filedesc,&signal,sizeof(int));
        read(filedesc,&signal,sizeof(int));
        gc->updateSecondsLeft(signal);

        // update current question
        signal = 4;
        write(filedesc,&signal,sizeof(int));
        read(filedesc,&signal,sizeof(int));
        gc->setQuestionNumber(signal);

        //update question / answers
        if(signal != CurrentQuestion){
            CurrentQuestion = signal;
            gc->resetOption();

            signal = 5;  //question
            write(filedesc,&signal,sizeof(int));
            read(filedesc,buffer,200);
            if(CurrentQuestion>0)gc->setQuestion(buffer);
            else gc->setQuestion("-");
            read(filedesc,buffer,200);
            if(CurrentQuestion>0)gc->appendText(buffer); //append if correct or not

            if(CurrentQuestion==0) {  //ranking
                gc->clearRanking();
                gc->updateMyPoints(0);
                signal = 12;
                write(filedesc,&signal,sizeof(int));
                while(1){
                    read(filedesc,buffer,200);
                    if(!strcmp(buffer,"END"))
                        break;
                    gc->appendRankingLine(buffer);
                }

                while(1){
                    this->msleep(100);
                    signal=4;
                    write(filedesc,&signal,sizeof(int));
                    read(filedesc,&signal,sizeof(int));
                    if(signal!=0) break;

                    if(gc->alive==-1){
                        signal = 10;
                        write(filedesc,&signal,sizeof(int));

                        this->alive=-1;
                        this->terminate();
                    }
                }
            }



            if(CurrentQuestion>0){

            signal = 6;  //A1
            write(filedesc,&signal,sizeof(int));
            read(filedesc,buffer,200);
            gc->updateA1(buffer);

            signal = 7;  //A2
            write(filedesc,&signal,sizeof(int));
            read(filedesc,buffer,200);
            gc->updateA2(buffer);

            signal = 8;  //A3
            write(filedesc,&signal,sizeof(int));
            read(filedesc,buffer,200);
            gc->updateA3(buffer);

            signal = 9;  //A4
            write(filedesc,&signal,sizeof(int));
            read(filedesc,buffer,200);
            gc->updateA4(buffer);
            }
            else{
                gc->updateA1("-");
                gc->updateA2("-");
                gc->updateA3("-");
                gc->updateA4("-");
            }
        }
        this->msleep(100);
    }
}
