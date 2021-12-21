
#include"header.h"    
//verifie si un des client dans la table myArrayofclients possede deja le port l'ip

bool verifClientinArray(int *pos,struct myArrayofclients *clients,struct client cl){

  for(int i=0;i<clients->taille;i++){
  if(clients->myclient[i].my_addr.sin_addr.s_addr==cl.my_addr.sin_addr.s_addr &&
         clients->myclient[i].my_addr.sin_port==cl.my_addr.sin_port){
         
      return (*pos=i);
  }
}
*pos=-1;
return true;
}    
    
    
//ajout d'un client dans la table myArrayofclients
bool ajoutClientinArray(struct myArrayofclients *clients,struct client cl){
int pos=0;
verifClientinArray(&pos,clients,cl);    
if (clients->taille<MaxClient && pos==-1){
clients->myclient[clients->taille].my_addr=cl.my_addr;
clients->taille=(clients->taille)+1;
return false;    
}    
return true;
}


//suppression des  clients dans la table myArrayofclients en mettant la talle du tableau a 0 et les values des clients a 0
bool removeAll(struct myArrayofclients *clients){
return (clients->taille=0);    
}
//remise a zéro des values de chaque clients
bool ClearValueofClientsinArray(struct myArrayofclients *clients){
    for(int i=0;i<MaxClient;i++){
        clients->myclient[i].value=0;
    }
return false;    
}



//permet de mettre a jour les values transmis par les client qui sont present dans la table myArrayofclients 

bool updateArrayofclient(struct myArrayofclients *clients,struct GameInteractionServeurClient Interaction){
int pos=0;
verifClientinArray(&pos,clients,Interaction.myclient);

if(pos>=0){
    return (clients->myclient[pos].value=Interaction.myclient.value);
}

return true;
    
}

bool clearbuffer(struct GameInteractionServeurClient *Interaction){
     bzero(Interaction->Msgbuffer,sizeof(Interaction->Msgbuffer)); 
     return false;
    
}

//message transmis au joueur selon si il est plus ou moins proche de la valeur

char msg1[buffsize] ="\nIndication: chill,you are far from the value\n";
char msg2[buffsize] ="\nIndication: you approach slowly\n";
char msg3[buffsize] ="\nIndication: continue you are in the good way\n";
char msg4[buffsize] ="\nIndication: you are very close becarful\n";
char msg5[buffsize] ="\nIndication: Move carefully\n";

bool cluemsg(char *msg[],int value,int val){
    if(value>val*5){
        return (*msg=msg1);
    }else if(value>val*4){
         return (*msg=msg2);
    }else if(value>val*3){
         return (*msg=msg3);
    }else if(value>val*2){
         return (*msg=msg4);
    }else{
         return (*msg=msg5);
    }
}

bool randomValue(int *random){
    srand((unsigned int)time(NULL));
    return (*random=rand()%Maxvalue+Minvalue);//random between min and max
}

bool clearGametools(struct Gametool *gametool){
      gametool->myValue=0;
      gametool->PlayersValue=0;
    return false;
    
}

bool deleteclient(struct myArrayofclients *clients,struct GameInteractionServeurClient *Interaction,struct Gametool *gametool){
    
int pos=0;

verifClientinArray(&pos,clients,Interaction->myclient);

if(pos>=0){
    for(int i=pos;i<clients->taille-1;i++){
    
        clients->myclient[i]=clients->myclient[i+1];
    }
    gametool->nbjoueur--;
    clients->taille--;
    
    
 return false;   
}
    
    
    

return true;    
}
