#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int p;//persons
struct client{
    char name[100];
    char group[100];
    int socket;
    int friend;
    int gchat;
    //struct sockaddr_in metadata;
};

struct client carray[10];
void socketerror(int ok)
{   
     if (ok < 0){
        perror("Chat Server failed ");
        exit(0);
    }
}


void registerUser(int conn){

    char *ret;
    char message[1024];
    char buffer[1024];//message buffers

   A:
        strcpy(message,"Chat01");
        send(conn, message, sizeof(message), 0);
        printf("[~]Registration Request\n");
		recv(conn, buffer, 1024, 0);
        ret=strstr(buffer,":200 OK Chat01");
        if (ret==NULL){
            goto A;
        }else{
            char * token = strtok(buffer, ":");
            strcpy(carray[p].name,token);
        }
        B:
        strcpy(message,"Chat02");
        send(conn, message, sizeof(message), 0);
        printf("[~]GROUP PLACEMENT\n");
		recv(conn, buffer, 1024, 0);
        ret=strstr(buffer,":200 OK Chat02");
        if (ret==NULL){
            goto B;
        }else{
            char * token = strtok(buffer, ":");
          
            if (strcmp(token,"1")){
                strcpy(carray[p].group,"Work");
            }else if(strcmp(token,"2")){
                strcpy(carray[p].group,"Fun");
            }else{
                goto B;
            }
            carray[p].socket=conn;
            carray[p].friend=-1;
            carray[p].gchat=0;
        }
        printf("[~]Registration Done New User: %s\n",carray[p].name);
        strcpy(message,"100 Ok");
        send(conn, message, sizeof(message), 0);
        p++;

}
void friendrequest(int a,int b){
   int uconn=carray[a].socket;
   int fconn=carray[b].socket;
   char message[1024];
   char buffer[1024];//message buffers

    strcpy(message,"400 Chat :");//code for no avaiable users
    strcat(message,carray[a].name);
    send(fconn, message, sizeof(message), 0);
    printf("[~] Friend Request Sent\n");
    bzero(buffer, sizeof(buffer));
    recv(fconn, buffer, 1024, 0);
    printf("[buffer] %s \n",buffer);
    if(strcmp(buffer,"400 Chat Yes")==0){
     printf("[~] Friend Request Accepted\n");

       carray[a].friend=b;
       carray[b].friend=a;
       if(carray[b].gchat!=0){
         printf("[~] %s has exit %s group chat \n",carray[b].name,carray[b].group);
       }
      if(carray[b].gchat!=0){
         printf("[~] %s has exit %s group chat \n",carray[b].name,carray[b].group);
       }
       carray[b].gchat=0;
       carray[a].gchat=0;

      strcpy(message,"500 Friend Chat :");
       strcat(message,carray[a].name);
       send(fconn, message, sizeof(message), 0);
       strcpy(message,"501 Friend Chat  :");
       strcat(message,carray[b].name);
       send(uconn, message, sizeof(message), 0);
       printf("[~] Friend Chat Between %s and %s\n",carray[a].name,carray[b].name);
       return;
    }
    printf("[~] Friend Request Denied\n");
    strcpy(message,"000 Ok");
    send(uconn, message, sizeof(message), 0);
    strcpy(message,"011 Go");
    send(fconn, message, sizeof(message), 0);
    return;
   // printf("%s\n",buffer);  
}



void  AvaiableUsers(int pos){
    int count=0;
    char buffer[1024];//message buffers
    char message[1024];
    int conn=carray[pos].socket;
    char str[3];
    for(int i=0; i<p ; i++){
        if (i!=pos && carray[i].friend==-1 && carray[i].socket!=0){
           strcpy(message,"202 Code :");//code for  avaiable users with number of user for loop on recvieving
           sprintf(str,"%d",i);
           strcat(str,":");
           strcat(message,str);
           strcat(message,carray[i].name);
           send(conn, message, sizeof(message), 0);
           recv(conn, buffer, 1024, 0);
           count++;
          
        }
    }
  if (count==0){
          strcpy(message,"101 Code");//code for no avaiable users
          send(conn, message, sizeof(message), 0);
          return;

    }else{
          int post;
          strcpy(message,"303 Code");//code for no avaiable users
          printf("[~] Friend Selection\n");
          send(conn, message, sizeof(message), 0);
          bzero(buffer,sizeof(buffer));
          recv(conn, buffer, 1024, 0);
          if(strcmp(buffer,"900 No")==0){
             strcpy(message,"000 OK");
             send(conn, message, sizeof(message), 0);
             printf("Skip Friend\n");
             return;
          }else{
               sscanf(buffer, "%d", &post);
               friendrequest(pos,post);    
          }
     }
    
    
}


void chat(char buffer[],int x){
      char *t;
      char temp[1024];
        strcpy(temp,buffer);
      int f=carray[x].friend;
      int conn2= carray[f].socket;
      printf("%s send' %s' to %s\n",carray[x].name, temp, carray[f].name);
      send(conn2, buffer, strlen(buffer), 0);
      t=strstr(buffer,"SOS");
      if (t!=NULL){
          carray[x].friend=-1;
          carray[f].friend=-1;
          printf("[~] Friend Chat Ended Between %s and %s\n",carray[x].name,carray[f].name);
    
      }
  
}
int chat1=-1;
void ServerWizard(char buffer[],int x){
      char *ptr;
      char temp[1024];
      ptr=strstr(buffer,"401 Go");
      if (ptr!=NULL){
                    bzero(buffer,strlen(buffer));
                    AvaiableUsers(x);
                    return;
       }
      ptr=strstr(buffer,"102 Waiting For Users");//Waiting call
       if (ptr!=NULL){
          strcpy(buffer,"000 OK");
          send(carray[x].socket, buffer, strlen(buffer), 0);
          return;
       }          
       ptr=strstr(buffer,"001 Go");//GROUP CHAT
       if (ptr!=NULL){
         //Send previous message//
         printf("[~]%s has enter %s group chat\n",carray[x].name,carray[x].group);
         if(strcmp(carray[x].group,"WORK")==0){
            carray[x].gchat=1;
         }else{
              carray[x].gchat=2;
         }
          printf("\n%d Chat", carray[x].gchat);
         strcpy(buffer,"010 Ok");
          write(carray[x].socket, buffer,strlen(buffer));
         
       } 
       ptr=strstr(buffer,"011 Go");// accept input CHAT
        if (ptr!=NULL){
             strcpy(buffer,"5555 GCHAT");//get message chat
             write(carray[x].socket, buffer,strlen(buffer));
            return;
          }
         ptr=strstr(buffer,"4444 GCHAT");//send message
        if (ptr!=NULL){
            strcpy(temp,"");
             strcat(temp,"[");
             strcat(temp,carray[x].name);
             strcat(temp,"]");
             char * token = strtok(buffer, ":");
             strcat(temp,token);
             printf("Message Sent to group :%s ",temp);
             strcat(temp,"3333 GCHAT");
            if (p>1){
             for(int h=0;h<p;h++){
             if ( h!=x && carray[h].gchat==carray[x].gchat && carray[h].socket!=0){
                   printf("%s\n",temp);
                   write(carray[h].socket, temp,strlen(temp));
               }}}
             strcpy(buffer,"5555 GCHAT");//get message chat
             write(carray[x].socket, buffer,strlen(buffer));
          
        
       
        }
  
     if(carray[x].friend!=-1){
        chat(buffer,x);
         return;
                       
      }
  
  
}
int main(){
     int sock,conn;
    struct sockaddr_in address,newaddress;
    unsigned short port=60000;
    char buffer[1024];//message buffers
    char message[1024];
    fd_set readysockets,currentsockets;
    int max_sd,loop,sd;
    int client_socket[10] ;
    socklen_t address_size;
    int count,bytesread;
    char *users[10];

    sock=socket(AF_INET , SOCK_STREAM , 0);
    socketerror(sock);
    memset(&address,0,sizeof(address));// set socket attributes
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons( port); 
    conn=bind(sock,(struct sockaddr *) &address,sizeof(address));//bind socket to port
    socketerror(conn);
    conn=listen(sock, 5);//listen for connection
    socketerror(conn);
    printf ("[*]Server Created Listening on Port %d  \n",port);
    FD_ZERO(&currentsockets);
    FD_SET(sock, &currentsockets); 
    count=0;
    int i;
    while(1){
        FD_ZERO(&readysockets);   
        FD_SET(sock, &readysockets);   
        max_sd = sock;  
        for (i=0;i<p;i++){
            sd=carray[i].socket;
            if (sd>0)
            {
                FD_SET(sd,&readysockets);
            }
            if(sd>max_sd){
                max_sd=sd;
            }

        }
        conn=select(max_sd+1,&readysockets,NULL,NULL,NULL);
        socketerror(conn);
        if(FD_ISSET(sock,&readysockets)){
             conn = accept(sock, (struct sockaddr*)&newaddress,&address_size);
             socketerror(conn);
             printf("[~]Connection accepted from %s:%d\n", inet_ntoa(newaddress.sin_addr), ntohs(newaddress.sin_port));
             registerUser(conn);

        }
        for (int x = 0; x < p; x++)//all users saved
        {  conn=carray[x].socket;
           if(FD_ISSET(conn,&readysockets)){
               bytesread=read(conn,buffer,1024);
               if(bytesread<=0){
                   printf("[#]%s has exited the Chat\n",carray[x].name);
                   close(conn);
                   if(carray[x].friend!=-1){
                      int y=carray[x].friend;
                     carray[x].friend=-1;
                     carray[y].friend=-1;
                      if(carray[x].gchat!=0){
                         printf("[~] %s has exit %s group chat \n",carray[x].name,carray[x].group);
                       }
                     carray[x].gchat=0;
                     
                   }
                   carray[x].socket=0;

               }
               else{
                  
                    buffer[bytesread]='\0';
                    ServerWizard(buffer,x);

                   
               }

               
           }
        }
        
        
        }
    
    






}