/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
	Intoarce corect identificatorul din program al thread-ului.
  
   
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include <random>
#include <sqlite3.h>
/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;

#define QuestionCap 5

int CurrentQuestion;
int CurrentRound;
int CorrectAnswer;
int ServerStarted;
int UsedResource;
int CleanDB;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;


char QuestionString[200],A1[200],A2[200],A3[200],A4[200],Winner[200];

void gamestart(thData tdL,char name[100]){
	
	int question=0;
	char buffer[200];

	int ResponseToWaitInLine = 0;
	int PlayerPoints = 0;
	int PlayerAnswer = 0;
	int CorrectAnswer_LagCompensation = CorrectAnswer;

	if(ServerStarted==0){
		 ServerStarted=1;
		// write(tdL.cl,&ServerStarted,sizeof(int));
	}
	else
		while(1){

			ResponseToWaitInLine = 10 + CurrentQuestion;
		//	write(tdL.cl,&ResponseToWaitInLine,sizeof(int));

			if(CurrentQuestion==1){
		//		write(tdL.cl,&ServerStarted,sizeof(int));
				break;	
			}
			usleep(20000);
		}

	while(1)
		{
		// UNDERSTAND WHAT THE PLAYER WANTS
		read(tdL.cl,&question,sizeof(int));

		if(question==10){
			printf("\n user disconnected");
			break;
		}
		if(question==1){
			int secondsLeft;

			if(CurrentQuestion==1)
				secondsLeft=41-CurrentRound;
			else
				secondsLeft=11-CurrentRound;

			write(tdL.cl,&secondsLeft,sizeof(int));
		}

		if(question==2)
			CurrentRound=-1;
		if(question==3)
			write(tdL.cl,name,100);
		if(question==4)
			write(tdL.cl,&CurrentQuestion,sizeof(int));

		//GET QUESTION ASKED
		if(question==5){
			bzero(buffer,200);

			if(CorrectAnswer_LagCompensation == PlayerAnswer){
					PlayerPoints++;
				//	printf("\n Player \" %s \" Answered correct!",name);
					strcpy(buffer,"OK!");
				//	printf("\n Answered %d but correct was %d",PlayerAnswer,CorrectAnswer_LagCompensation);
			}else 	{
			//	printf("\n Player \" %s \" was wrong",name);
				strcpy(buffer,"NOT OK!");
			//	printf("\n Answered %d but correct was %d",PlayerAnswer,CorrectAnswer_LagCompensation);
			
			}
			CorrectAnswer_LagCompensation = CorrectAnswer;
			PlayerAnswer = 5;

			write(tdL.cl,QuestionString,200);
			write(tdL.cl,buffer,200);
					// update players points
			if(CurrentQuestion==0){ 

			if(UsedResource==0) UsedResource = 1;
			else while(1){
				usleep(15000);
				if(UsedResource==0){
					UsedResource = 1;
					break;
				}
			}
			char pts[20];
			sprintf(pts,"%d",PlayerPoints);
			char sql[100] = "INSERT INTO results VALUES ('";
			strcat(sql,name);
			strcat(sql,"','");
			strcat(sql,pts);
			strcat(sql,"');");

			char* err=0;
			sqlite3 *db;
			int rc;
			rc = sqlite3_open("TB",&db);
			rc = sqlite3_exec(db,sql,0,0,&err);

			sqlite3_close(db);
			
			PlayerPoints=0;
			UsedResource = 0;
			}
			
		}

		//GET ANSWER 1
		if(question==6){
			//bzero(buffer,200);strcpy(buffer,"A1");
			//write(tdL.cl,buffer,200);
			write(tdL.cl,A1,200);
		}
		//GET ANSWER 2
		if(question==7){
			//bzero(buffer,200);strcpy(buffer,"A2");
			//write(tdL.cl,buffer,200);
			write(tdL.cl,A2,200);
		}
		//GET ANSWER 3
		if(question==8){
			//bzero(buffer,200);strcpy(buffer,"A3");
			//write(tdL.cl,buffer,200);
			write(tdL.cl,A3,200);
		}
		//GET ANSWER 4
		if(question==9){
			//bzero(buffer,200);strcpy(buffer,"A4");
			//write(tdL.cl,buffer,200);
			write(tdL.cl,A4,200);
		}
		//check player answer
		if(question==11){
			int _PAnswer;
			read(tdL.cl,&_PAnswer,sizeof(int));
			if(_PAnswer==0) PlayerAnswer = 5;
			else 			PlayerAnswer = _PAnswer;
			//check if _PAnswer is correct, sum up points
		}


		//push ranking to client
		if(question==12){

			if(UsedResource==0) UsedResource = 1;
			else while(1){
				usleep(15000);
				if(UsedResource==0){
					UsedResource = 1;
					break;
				}
			}
			sqlite3 *db;
			sqlite3_stmt *selectstmt;
			const char* err = 0;

			char sql[100];
			strcpy(sql,"SELECT * FROM results order by score desc;");

			int rc;
			rc=sqlite3_open("TB",&db);
			sqlite3_prepare_v2(db,sql,-1,&selectstmt,&err);

			while(sqlite3_step(selectstmt)==SQLITE_ROW){

				char *name = (char*)sqlite3_column_text(selectstmt,0);
				char *points = (char*)sqlite3_column_text(selectstmt,1);

				char *rankingLine = (char*)malloc(strlen(name)+strlen(points)+3);

				strcpy(rankingLine,points);
				strcat(rankingLine,". ");
				strcat(rankingLine,name);

				printf("\n Retrieved rankingLine : %s",rankingLine);

				bzero(buffer,200);
				strcpy(buffer,rankingLine);
			 	write(tdL.cl,buffer,200);

			}

			sqlite3_finalize(selectstmt);
			sqlite3_close(db);

			strcpy(buffer,"END");
		 	write(tdL.cl,buffer,200);

		 	UsedResource = 0;
		}

		if(question==13)
			write(tdL.cl,&PlayerPoints,sizeof(int));

		//printf("\n Server Question %d,Second %d,request %d",CurrentQuestion,CurrentRound,question);
		printf("\n Server Question %d, player answer is %d and C is %d his pts %d",CurrentQuestion,PlayerAnswer,CorrectAnswer_LagCompensation,PlayerPoints);
		}	
}

void ResetGlobalQuestion(){
		// reset global question

			sqlite3 *db;
			sqlite3_stmt *selectstmt;
			const char* err = 0;

			char sql[100];
			char sql2[100];
			strcpy(sql2,"delete from results;");
			strcpy(sql,"SELECT * FROM questions order by random() limit 1;");

			int rc;
			rc=sqlite3_open("TB",&db);
			sqlite3_prepare_v2(db,sql,-1,&selectstmt,&err);

			if(sqlite3_step(selectstmt)==SQLITE_ROW){

				//strcpy(QuestionString,sqlite3_column_text(selectstmt,1));
				//strcpy(A1,sqlite3_column_text(selectstmt,2));
				//strcpy(A2,sqlite3_column_text(selectstmt,3));
				//strcpy(A3,sqlite3_column_text(selectstmt,4));
				//strcpy(A4,sqlite3_column_text(selectstmt,5));
				char *Q1 = (char*)sqlite3_column_text(selectstmt,1);strcpy(QuestionString,Q1);
				char *A11 = (char*)sqlite3_column_text(selectstmt,2);strcpy(A1,A11);
				char *A22 = (char*)sqlite3_column_text(selectstmt,3);strcpy(A2,A22);
				char *A33 = (char*)sqlite3_column_text(selectstmt,4);strcpy(A3,A33);
				char *A44 = (char*)sqlite3_column_text(selectstmt,5);strcpy(A4,A44);
				char *R = (char*)sqlite3_column_text(selectstmt,6);

				if(!strcmp(R,"1")) CorrectAnswer = 1; else
				if(!strcmp(R,"2")) CorrectAnswer = 2; else
				if(!strcmp(R,"3")) CorrectAnswer = 3; else
				if(!strcmp(R,"4")) CorrectAnswer = 4; else CorrectAnswer=-1;

				printf("\n Retrieved question : %s %s %s %s %s %d",QuestionString,A1,A2,A3,A4,CorrectAnswer);
			}
			else{
				printf("\n something happend retrieving ");
			}

			if(CleanDB==0){
				char* er;
				sqlite3_exec(db,sql2,0,0,&er);
				CleanDB=1;			
			}

			sqlite3_finalize(selectstmt);
			sqlite3_close(db);
			
			//
}

static void *gameTreat(void *arg){
	CurrentQuestion=1; // question
	CurrentRound = 13;//seconds
	ResetGlobalQuestion();

	while(ServerStarted==0){
		usleep(200000);
		printf("\n Waiting for clients.");
	}

	while(1){
		if(CurrentRound==11){
			CurrentRound=0;
			CurrentQuestion++;
			ResetGlobalQuestion();
			
		}

		if(CurrentRound==40){
			CurrentQuestion++;
			CurrentRound=0;
			ResetGlobalQuestion();
		}

		if(CurrentQuestion==6){
			CurrentQuestion=0;
			sleep(20);
			CurrentRound=13;
			CurrentQuestion=1;
		}

		CurrentRound++;
		sleep(1);
	}
}

static int callback(void *NotUsed, int argc, char **argv, char**azColName){
	int i;
	for(i=0;i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	return 0;
}



static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */

void raspunde(void *);



int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int nr;		//mesajul primit de trimis la client 
  int sd;		//descriptorul de socket 
  int pid;
  //pthread_t th;    //Identificatorii thread-urilor care se vor crea
  pthread_t th[1000];
  int i=0;
  pthread_create(&th[i++],NULL,&gameTreat, NULL);
  //pthread_create(&th,NULL,&gameTreat, NULL);

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
    {
      int client;
      thData * td; //parametru functia executata de thread     
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

	//client= malloc(sizeof(int));
      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, (socklen_t*)&length)) < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}
	
        /* s-a realizat conexiunea, se astepta mesajul */
    
	int idThread; //id-ul threadului
	int cl; //descriptorul intors de accept

	td=(struct thData*)malloc(sizeof(struct thData));	
	td->idThread=i++;
	td->cl=client;
	pthread_create(&th[i], NULL, &treat, td);	
	//pthread_create(&th, NULL, &treat, td);      
				
	}//while    
};

static void *treat(void * arg)
{		
		struct thData tdL; 
		tdL= *((struct thData*)arg);

		printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());	



		raspunde((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);

		return(NULL);	
  		
};


void raspunde(void *arg)
{
    int nr, i=0;
    char msg[100],name[100],pw[100];

    bzero(msg,100);
    bzero(name,100);
    bzero(pw,100);

	struct thData tdL; 
	tdL= *((struct thData*)arg);

	if (read (tdL.cl, msg, 100) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
			}
	
	read(tdL.cl, name, 100);
	read(tdL.cl, pw, 100);

	printf ("[Thread %d]Mesajul a fost receptionat...%s\n",tdL.idThread, msg);
			      
			      /*pregatim mesajul de raspuns */	
	if(!strcmp(msg,"login")){
			int nrOfWrites = 1,ok=0;
			write(tdL.cl, &nrOfWrites,sizeof(int));
			
			
			char sql[200];
			strcpy(sql,"select * from users where name='");
			strcat(sql,name);
			strcat(sql,"' and pw='");
			strcat(sql,pw);
			strcat(sql,"';");
			


			printf("\n %s",sql);
			// connect to DB here to check if user and pw exists
			const char* err=0;
			sqlite3 *db;
			sqlite3_stmt *selectstmt;
			int rc;
			rc=sqlite3_open("TB",&db);
			sqlite3_prepare_v2(db,sql,-1,&selectstmt,&err);

			if(sqlite3_step(selectstmt)==SQLITE_ROW){
				ok = 1; //printf("sql then");
			}
			else{
				ok = 0; //printf("\n sql else");
			}

			sqlite3_finalize(selectstmt);
			sqlite3_close(db);
			// 
			write(tdL.cl, &ok,sizeof(int));
			if(ok==1)
				gamestart(tdL,name);
			sleep(100);

	}
	else{
			int nrOfWrites = 1;

			write(tdL.cl, &nrOfWrites,sizeof(int));
			//write(tdL.cl, name, 100);
			//write(tdL.cl, pw, 100);

		

			//char sql2[100] = " delete from results";
			char sql[100] = "INSERT INTO USERS VALUES ('";
			strcat(sql,name);
			strcat(sql,"','");
			strcat(sql,pw);
			strcat(sql,"',0);");

			char* err=0;
			sqlite3 *db;
			int rc;
			rc = sqlite3_open("TB",&db);
			rc = sqlite3_exec(db,sql,callback,0,&err);
			if( rc != SQLITE_OK ) {
				bzero(msg,100);
				strcpy(msg,"Name already exists!");
				write(tdL.cl, msg, 100);
				} else {
				  	bzero(msg,100);
					strcpy(msg,"register complete!");
					write(tdL.cl, msg, 100);
				}
			sqlite3_close(db);
	}

}
