#include "serveur.h"
#define PORT 5555

    
    
    

     int main(int argc, char *argv[]){
         

         
//initialise des paramètre de jeu et taille de  myArrayofclients a  0
         struct Gametool gametool;
         gametool.nbjoueur=0;
         gametool.myValue=0;
         gametool.PlayersValue=0;
         
         struct myArrayofclients clients;
         clients.taille=0;
//declaration de la struct GameInteractionServeurClient
         
         struct GameInteractionServeurClient Interaction;
         Interaction.needAnswer=false;
         Interaction.YesnoQuestion=false;
         Interaction.Answer=false;
         
         
         char buffertosend[buffsize];// utiliser pour transmettre msg lors de la connexion de client
         
//initialisation de l'addresse serveur et declaration addresse client         
         struct sockaddr_in my_addr, peer_addr;
         socklen_t my_addr_size;
         socklen_t peer_addr_size;
         struct client actualClient;
         
         my_addr.sin_family=AF_INET;
         my_addr.sin_addr.s_addr=INADDR_ANY;
         my_addr.sin_port=htons(PORT);
         
         
         my_addr_size=sizeof(my_addr);
         peer_addr_size=sizeof(peer_addr);
        
       
//creation de la socket serveur    
        int  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
        socketerror(udp_socket);
            
//bind de la socket serveur            
       int bind1=bind(udp_socket, (struct sockaddr*)&my_addr,my_addr_size);
       binderror(bind1);
            
        ssize_t data;
        ssize_t send;
        char CharofplayerNumber[buffsize];
         char valofopponent[buffsize];

 while(1){ 
printf("Waiting for players...\n"); 
fflush(0);
       

    for(int i=0;i<clients.taille;i++){
            Interaction.needAnswer=false;
               
            peer_addr=clients.myclient[i].my_addr;
            Interaction.myclient=clients.myclient[i];
               
            strcpy(Interaction.Msgbuffer,"Some players have left !\n");
            strcat(Interaction.Msgbuffer,"You will be the Player ");
            sprintf(CharofplayerNumber,"%d",i+1);
            strcat(Interaction.Msgbuffer,CharofplayerNumber);
            strcat(Interaction.Msgbuffer,"\n");
            if(gametool.nbjoueur!=MaxClient){
            strcat(Interaction.Msgbuffer,"Let's wait for more players...\n");
            }
               
               send=sendto(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
               sendtoerror(send);
               clearbuffer(&Interaction);
                  
              }
              


     
  while(1){
             if(gametool.nbjoueur==MaxClient){
              printf("All players are here,lets start the game\n");
              fflush(0);
              strcpy(Interaction.Msgbuffer,"All players are here,lets start the game\n");
              
              for(int i=0;i<clients.taille;i++){
               peer_addr=clients.myclient[i].my_addr;
               Interaction.myclient=clients.myclient[i];
               
               send=sendto(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
               sendtoerror(send);
                  
              }
              

             break; 
             }
         
          data=recvfrom(udp_socket,(struct client*)&actualClient,sizeof(actualClient),0,(struct sockaddr*)NULL ,0);
          recvfromerror(data);
        
         if(ajoutClientinArray(&clients, actualClient)==false){
          
           peer_addr=actualClient.my_addr;
           gametool.nbjoueur++;

            strcpy(buffertosend,"You have been connected succesfully\n");
            strcat(buffertosend,"You will be the Player ");
            sprintf(CharofplayerNumber,"%d",gametool.nbjoueur);
            strcat(buffertosend,CharofplayerNumber);
            strcat(buffertosend,"\n");
            if(gametool.nbjoueur!=MaxClient){
            strcat(buffertosend,"Lets wait for more players...\n");
            }
    
             send=sendto(udp_socket,buffertosend,buffsize,0,(struct sockaddr*)&peer_addr,peer_addr_size);
             sendtoerror(send);
            
             bzero(buffertosend,sizeof(buffertosend));
             printf("actual Nbjoueur: %d\n",gametool.nbjoueur);
             fflush(0);
             for(int j=0;j<clients.taille;j++){
              Interaction.needAnswer=false;   
              peer_addr=clients.myclient[j].my_addr;
              Interaction.myclient=clients.myclient[j];   
              
              strcpy(Interaction.Msgbuffer,"Remaining player before starting: ");
              sprintf(CharofplayerNumber,"%d",MaxClient-gametool.nbjoueur);
              strcat(Interaction.Msgbuffer,CharofplayerNumber);
              strcat(Interaction.Msgbuffer,"\n");
              
              send=sendto(udp_socket,(struct Interaction*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
               sendtoerror(send);
                 
             }
            
                 

         }
       
 }
 

        
 while(1){
     
        if(  gametool.myValue==0){
         int random=0;
         randomValue(&random);
         gametool.myValue=random;//initialisation de la valeur a ne pas par les joueurs 
         printf("The chosen value is  %d\n",gametool.myValue);
         fflush(0);
         
         }
         
        
        for(int i=0;i<clients.taille;i++){
        Interaction.needAnswer=true;
        Interaction.YesnoQuestion=false;
        
        
        Interaction.myclient=clients.myclient[i];
        peer_addr=clients.myclient[i].my_addr;
        
        printf("send msg to PLAYER  %d\n",i+1);
        fflush(0);
        
        clearbuffer(&Interaction);
        
        char* msg;
        cluemsg(&msg,gametool.myValue,gametool.PlayersValue);
        strcpy(Interaction.Msgbuffer,msg);
         
        int lastclient=0;
          if(i==0){
           lastclient=clients.taille-1;
          }else{
          lastclient=i-1;
        }
        
           
         if(clients.myclient[lastclient].value!=0){
    
            strcat(Interaction.Msgbuffer,"Your last opponent sent the value ");
            sprintf(valofopponent,"%d",clients.myclient[lastclient].value);
            strcat(Interaction.Msgbuffer,valofopponent);
            strcat(Interaction.Msgbuffer,"\n");
             
            strcat(Interaction.Msgbuffer,"the sum of the current values: "); 
            char sumofval[buffsize];
            sprintf(sumofval,"%d",gametool.PlayersValue);
            strcat(Interaction.Msgbuffer,sumofval);
            strcat(Interaction.Msgbuffer,"\n");
             
            
         }else{
             strcat(Interaction.Msgbuffer,"You got the first move \n");
         }
        
         strcat(Interaction.Msgbuffer,"\nEnter your value then press the enter key  :\n");
                 
        
         send=sendto(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
         
         sendtoerror(send);
         clearbuffer(&Interaction);
         
         
         for(int j=0;j<clients.taille;j++){ 
          
         if(j!=i){    
         Interaction.needAnswer=false;
         strcpy(Interaction.Msgbuffer,"Wait it 's the player ");
              sprintf(CharofplayerNumber,"%d",i+1);
              strcat(Interaction.Msgbuffer,CharofplayerNumber);
              strcat(Interaction.Msgbuffer," turn...\n");
         Interaction.myclient=clients.myclient[j];
         peer_addr=clients.myclient[j].my_addr;
         
         send=sendto(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
         
         sendtoerror(send);
         clearbuffer(&Interaction);
         }
         
         }
         Interaction.needAnswer=true;
         
         Interaction.myclient=clients.myclient[i];
         peer_addr=clients.myclient[i].my_addr;
         
       
         data=recvfrom(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,&peer_addr_size); 
         
          
          recvfromerror(data);
          updateArrayofclient(&clients,Interaction);
          
         
         printf("The player send the value %d\n",  clients.myclient[i].value);
         fflush(0);
         
         gametool.PlayersValue=gametool.PlayersValue+Interaction.myclient.value;
         
         
         if(gametool.myValue<gametool.PlayersValue){
              Interaction.YesnoQuestion=true;  
             printf("all players have win except the player %d who lost \n",i+1);
             fflush(0);
          
         //envoi de msg de defaite au perdant
        
           clearbuffer(&Interaction);
          strcpy(Interaction.Msgbuffer,"You lose !\n the value was ");
          char myvalue[buffsize];
          sprintf(myvalue,"%d",gametool.myValue);
          strcat(Interaction.Msgbuffer,myvalue);
          strcat(Interaction.Msgbuffer,"\n");
          
          strcat(Interaction.Msgbuffer,"the sum of the current values: "); 
          char sumofval[buffsize];
          sprintf(sumofval,"%d",gametool.PlayersValue);
          strcat(Interaction.Msgbuffer,sumofval);
          strcat(Interaction.Msgbuffer,"\n");
          
          strcat(Interaction.Msgbuffer,"Do you wanna replay?(answer yes=1/no=0) ) \n"); 
          
          send=sendto(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
          sendtoerror(send);
           
          clearbuffer(&Interaction);
         
          data=recvfrom(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr ,&peer_addr_size); 
         
          
         recvfromerror(data);
         
          sprintf(valofopponent,"%d",clients.myclient[i].value);
          int clientlength=clients.taille;
          
         if(Interaction.Answer==false){
             
         printf("the player %d has left\n",i+1);
         fflush(0);
         deleteclient(&clients,&Interaction,&gametool);
        }else{
               Interaction.needAnswer=false;
               strcpy(Interaction.Msgbuffer,"Let's ask to the remaining players...\n");
               
               send=sendto(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
               sendtoerror(send);
               clearbuffer(&Interaction);
        }
         
         
         
          
          //envoi de msg de victoire au vainqueur
          for(int j=0;j<clientlength;j++){
           Interaction.needAnswer=true;    
          if(j!=i){
          strcpy(Interaction.Msgbuffer,"You win !\nthe value was ");
          strcat(Interaction.Msgbuffer,myvalue);
          strcat(Interaction.Msgbuffer,"\n");
          
          strcat(Interaction.Msgbuffer,"the Player ");
          sprintf(CharofplayerNumber,"%d",i+1);
          strcat(Interaction.Msgbuffer,CharofplayerNumber);
          strcat(Interaction.Msgbuffer," lost , he sent the value ");
          strcat(Interaction.Msgbuffer,valofopponent);
          strcat(Interaction.Msgbuffer,"\n");
             
          strcat(Interaction.Msgbuffer,"the sum of the current values: "); 
          strcat(Interaction.Msgbuffer,sumofval);
          strcat(Interaction.Msgbuffer,"\n");
          
          strcat(Interaction.Msgbuffer,"Do you wanna replay?(answer yes=1/no=0) )\n "); 
      
          peer_addr=clients.myclient[j].my_addr;  
          Interaction.myclient=clients.myclient[j];
          
          
          
          send=sendto(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
          sendtoerror(send);
          clearbuffer(&Interaction);
          
          data=recvfrom(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr ,&peer_addr_size); 
         
          recvfromerror(data);
         
         if(Interaction.Answer==false){
           printf("the player %d has left\n",j+1);   
           fflush(0);
          deleteclient(&clients,&Interaction,&gametool);
         }else{
               Interaction.needAnswer=false;
               strcpy(Interaction.Msgbuffer,"Let's ask to the remaining players...\n");
               
               send=sendto(udp_socket,(struct GameInteractionServeurClient*)&Interaction,sizeof(Interaction),0,(struct sockaddr*)&peer_addr,peer_addr_size);
               sendtoerror(send);
               clearbuffer(&Interaction);
             
        }
          }
         }
          
          clearGametools(&gametool);
          ClearValueofClientsinArray(&clients);
          
          break;   
         }
         
        }
    if( clients.taille!=MaxClient){
       if(clients.taille==0){
        int closeVal=close(udp_socket);
 
        closerror(closeVal);
        
        return 0;
       
           
    }
        
        
        break;
    }
 }
 
 

 }
 
 int closeVal=close(udp_socket);
 
 closerror(closeVal);
  
 
 return 0;
       }
