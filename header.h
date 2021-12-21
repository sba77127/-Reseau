#define buffsize 1024
#define MaxClient 2
#define Minvalue 1
#define Maxvalue 10000
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include<stdbool.h>
#include<fcntl.h>
#include<time.h>
#include <unistd.h>


struct client{
    struct sockaddr_in my_addr;
    int32_t value;
}__attribute__((__packed__)) ;



struct myArrayofclients{
   struct client myclient[MaxClient];
   int32_t taille;   
}__attribute__((__packed__)) ;    



struct GameInteractionServeurClient{
     struct client myclient;
     char Msgbuffer[buffsize];
     bool needAnswer;
     bool YesnoQuestion;//true -> on s'attend a ce que Answer contient la reponse au msg du buffer, , false-> on s'attend a ce que value dans myclient contient la reponse au msg du buffer
     bool Answer;
}__attribute__((__packed__)) ;

 
struct Gametool{
    int32_t nbjoueur;
    int32_t myValue;//la valeur que les joueur ne doivent pas depasser
    int32_t PlayersValue;//la somme des valeurs cumulées des joueurs,si elle supérieur à myValue , le joueur qui envoyer la dernière valeur perd et l'autre est vainqueur    
}__attribute__((__packed__)) ;


bool socketerror(int socket){
    if(socket<0){
         perror("Error socket"); 
         exit(EXIT_FAILURE);
        return true;
    }
    return false;
}

bool binderror(int bind){
    if(bind<0){
        perror("error bind");
        exit(EXIT_FAILURE);
        return true;
    }
    return false;
}

bool recvfromerror(ssize_t data){
    if(data<=0){
          perror("error recvfrom");
          exit(EXIT_FAILURE);
        return true;
    }
    return false;
}


bool sendtoerror(ssize_t send){
    if(send<0){
          perror(" error sendto");
          exit(EXIT_FAILURE);
        return true;
    }
    return false;
}

bool closerror(int closeVal){
 if(closeVal<0){
     perror("Socket close error");
     exit(EXIT_FAILURE);
 
     return true;
}
return false;
}

bool selecterror(int nb){
 if(nb<0){
       perror("error select");
       exit(EXIT_FAILURE);
 return true;
     
}
return false;
    
}

bool readerror(ssize_t readval){
 if(readval<=0){
       perror("error read");
       exit(EXIT_FAILURE);
 return true;
     
}
return false;
    
}
