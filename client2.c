

#include "header.h"
#define ServeurPORT 5555
#define PORT 8888



     
     int main(int argc, char *argv[])
       {
        int quit=-1;   
        struct sockaddr_in my_addr, peer_addr;
        socklen_t my_addr_size,peer_addr_size;
    
        
        ssize_t readVal;
        int nb;      

        
        fd_set read_set;
        fd_set exceptfds;
        char buffrecu[buffsize];//use during the connexion
          
        my_addr.sin_family=AF_INET;
        my_addr.sin_addr.s_addr=INADDR_ANY;
        my_addr.sin_port=htons(PORT);
      
        my_addr_size=sizeof(my_addr);
        
        peer_addr.sin_family=AF_INET;
        peer_addr.sin_addr.s_addr=INADDR_ANY;
        peer_addr.sin_port=htons(ServeurPORT);
      
        peer_addr_size=sizeof(peer_addr);
    
       int  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
         
       socketerror(udp_socket);
             
            
       int bind1=bind(udp_socket, (struct sockaddr*)&my_addr,my_addr_size);
       
       binderror(bind1);
        
       ssize_t send;
       ssize_t data;
       
 fcntl(udp_socket,F_SETFL,O_NONBLOCK);
// initialisation des donnés client et envoi numero de port
        struct client cl1;
        cl1.my_addr=my_addr;
        cl1.value=0;
        
        struct GameInteractionServeurClient Interaction;//use to interact with the server during the game (not use for the connexion to serveur)        
        
         send=sendto(udp_socket,(struct client*)&cl1,sizeof(cl1),0,(struct sockaddr*)&peer_addr,peer_addr_size);
        sendtoerror(send);
        
        FD_SET(udp_socket,&read_set);
        FD_SET(fileno(stdin),&read_set);
        nb=select(udp_socket+1,&read_set,NULL,&exceptfds,NULL);

         
        selecterror(nb);
        
         if(FD_ISSET(fileno(stdin),&read_set)){
            readVal=read(0,buffrecu,sizeof(buffrecu));
            readerror(readVal);
            printf("Please wait for new players\n" );
           
        }else if(FD_ISSET(udp_socket,&read_set)){
        
            
        data=  recvfrom(udp_socket,buffrecu,buffsize,0,(struct sockaddr*)&peer_addr,&peer_addr_size);
        
        recvfromerror(data);
    
          
          
          if(strcmp(buffrecu,"exit")==0){
              printf("The game has already started, please try later\n");
              fflush(0);
          
              return 0;
        }else{
          printf("%s\n",buffrecu);
          fflush(0);
        
            
        }
          

        }
        
       
 while(true){        
//envoi des information clients au serveur        
        bzero(buffrecu,sizeof(buffrecu));
        FD_ZERO(&read_set);
        FD_SET(fileno(stdin),&read_set);
        FD_SET(udp_socket,&read_set);
        nb=select(udp_socket+1,&read_set,NULL,&exceptfds,NULL);
        
        selecterror(nb);
        
        if(FD_ISSET(fileno(stdin),&read_set)){
        readVal=read(0,buffrecu,sizeof(buffrecu));
        readerror(readVal);
        printf("Please wait for opponent answer or for the game to start \n" );
           
        }else if(FD_ISSET(udp_socket,&read_set)){
        data=recvfrom(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,&peer_addr_size);
        
        recvfromerror(data);
         
          printf("%s",Interaction.Msgbuffer);
          fflush(0);
          
          if(Interaction.needAnswer==true){
          if(Interaction.YesnoQuestion==true){
              while(quit!=0 && quit!=1){
               char temp[buffsize];    
               printf("The value should be 0 to quit or 1 to replay\n");
               fflush(0);
                 scanf("%s",temp);
                 quit=atoi(temp);
                 if(strcmp(temp,"0")!=0 && quit==0 ){
                 quit=-1;
                }
                 
              }
              
            if(quit==0){
             Interaction.Answer=false;    
            }else if(quit==1){
             Interaction.Answer=true;   
              quit=-1;
            }
              
          }else{
           
             cl1.value=0; 
             while(cl1.value<Minvalue || cl1.value>Maxvalue){ 
             char temp[buffsize];    
             printf("The value should be between %d and %d \n",Minvalue,Maxvalue);
             fflush(0);
             scanf("%s",temp);
             cl1.value=atoi(temp);
             }
             Interaction.myclient=cl1;
          }
          send=sendto(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
           sendtoerror(send);
          }    
            
          if(quit==0){
              
              int closeVal=close(udp_socket);
 
              closerror(closeVal);
          
              return 0;
        }
        
            
       
       
        }
        
 }
 
 
  
 
 
 return 0;
           
       }
