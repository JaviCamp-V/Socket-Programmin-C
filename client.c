#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>




char name[100];
char fname[100];
int n;
int j;
char temp[1024];
int bytesread;
void socketerror(int ok)
{
     if (ok < 0){
        printf("Server failed\n");
        exit(0);
    }
}
void processMessage(char buffer[],int sock){
    char *p;
    char message[1024];
    int bytesread;
  
    p=strstr(buffer,"Chat01");
    if(p!=NULL){
        printf("[*] Username:\t");
        scanf("%s",message);
        strcpy(name,message);
        strcat(message,":200 OK Chat01");
        send(sock, message, strlen(message), 0);
        return;
    }
    p=strstr(buffer,"Chat02");
    if(p!=NULL){
        int entry;
        printf("[*] Group Selection: \n \t1--->Work Group\n\t2--->Fun Group\n[*]--\t");
        scanf("%d",&entry);
        sprintf(message,"%d",entry);
        strcat(message,":200 OK Chat02");
        send(sock, message, strlen(message), 0);
        return;
    }
    
   bzero(message,sizeof(message));
    p=strstr(buffer,"100 Ok");
    if(p!=NULL){
        strcpy(message,"401 Go");
        send(sock, message, strlen(message), 0);
        return;
    }
    p=strstr(buffer,"000 OK");
    if(p!=NULL){
        printf("jah");

        strcpy(message,"001 Go");
        send(sock, message, strlen(message), 0);
        return;
    }
      bzero(message,sizeof(message));

    p=strstr(buffer,"010 Ok");
    if(p!=NULL){
        strcpy(message,"011 Go");
        send(sock, message, strlen(message), 0);
        printf("[Server]\tStart Chat \n");
        printf("[~] Group Chat rules Enter EXIT to close chat and close socket \n");
        printf("[~] to see friend chat request  enter read   \n");
        printf("[~]  read to recieve \n");
        return;
    }
     p=strstr(buffer,"5555 GCHAT");
     if(p!=NULL){
             strcpy(message," ");
             char temp[1024];
             printf("[%s]\t",name);
             //fgets(message, 1024, stdin);
              scanf("%s",message);
             //strcpy(message,temp);
             if(strcmp(message,"read")==0){
                return;
             }else if(strcmp(message,"EXIT")==0){
                 close(sock);
             }
            else{
               strcat(message," : 4444 GCHAT");
             }
            // printf("%s",message);
             write(sock, message, sizeof(message));
             return;
     }
     p=strstr(buffer,"3333 GCHAT");
     if(p!=NULL){
             char *token= strtok(buffer,":");
              printf("%s",token);
              strcpy(message,"011 Go");
              write(sock, message, sizeof(message));
             return;
     }
     bzero(message,sizeof(message));

    p=strstr(buffer,"101 Code");
    if(p!=NULL){
    printf("[~] No available  Friends\n");
    strcpy(message,"102 Waiting For Users");//send signal
    send(sock, message, strlen(message), 0);
    return;
    }
      bzero(message,sizeof(message));

    p=strstr(buffer,"202 Code :");
    if(p!=NULL){  
        char g[1024];
        int h=-1;
        printf("[Avaliable User]: %s \n",buffer);
        strcpy(message,"203 Waiting For Users");//send signal say waiting
        char *token= strtok(buffer,":");
        token = strtok(NULL, ":");
        send(sock, message, strlen(message), 0);
        sscanf(token, "%d", &h);
        n++;
        return;
    }
      bzero(message,sizeof(message));

    p=strstr(buffer,"303 Code");
    if(p!=NULL){
      int pick=0;
      int location;
      char str[1024];
      K:
      printf("1)-->Pick Friend or 2)-->Skip: \t");
      scanf("%d",&pick);
      if (pick==1){
      printf("Select Friend Position: ");
      scanf("%d",&location);
      sprintf(str,"%d",location);
      strcpy(message,str);
      send(sock, message, strlen(message), 0);
       return;
      }
      if(pick==2){
           strcpy(message,"900 No");
           send(sock, message, strlen(message), 0);
           return;
      }
      goto K;
      }
    bzero(message,sizeof(message));
    p=strstr(buffer,"400 Chat");
     if(p!=NULL){
        int option=0;
       char *token= strtok(buffer,":");
        token = strtok(NULL, ":");
       printf("%s has sent you a friend request\n",token);
       C:
       printf("1--->Accept\n");
       printf("2--->Deny\n");
       scanf("%d",&option);
       if(option==1){
         strcpy(message,"400 Chat Yes");
         send(sock, message, strlen(message), 0);
         return;
       }else if(option==1){
         strcpy(message,"400 Chat No");
         send(sock, message, strlen(message), 0);
         return;

       }
       goto C;
     }
    bzero(message,sizeof(message));
    p=strstr(buffer,"500 Friend Chat");//reciever then reply
    if(p!=NULL){
      puts(" Enter SOS to exit chat");
      char * token = strtok(buffer, ":");
      token = strtok(NULL,":");
      strcpy(fname,token);
       while(1){
          int n=0;
          bytesread=read(sock,buffer,1024);
          buffer[bytesread]='\0';
          //recv(sock, buffer, 1024, 0);
          printf("[%s]\t%s\n",fname,buffer);
          p=strstr(buffer,"SOS");
           if (p!=NULL){
             break;
            }
          bzero(buffer,sizeof(bytesread));
          printf("[%s]\t",name);
          scanf("%s",message);
           //fflush(stdin);
          //fgets(message,1024,stdin);
          write(sock, message, sizeof(message));
          p=strstr(message,"SOS");
           if (p!=NULL){
             break;
            }
           bzero(message,sizeof(message));
        }
       strcpy(message,"102 Waiting For Users");
       send(sock,message,strlen(message),0);
      return;
    }
    bzero(message,sizeof(message));
    p=strstr(buffer,"501 Friend Chat");//sned then reply
    if(p!=NULL){
       puts(" Enter SOS to exit chat 2");
      char * token = strtok(buffer, ":");
      token = strtok(NULL,":");
      strcpy(fname,token);
      int n;
       while(1){
          //fgets(message,1024,stdin);
          printf("[%s]\t",name);
          scanf("%s",message);
         write(sock, message, sizeof(message));
          p=strstr(message,"SOS");
           if (p!=NULL){
             break;
            }       
         bzero(message,sizeof(message));
         bytesread=read(sock,buffer,1024);
         buffer[bytesread]='\0';
         printf("[%s]\t%s\n",fname,buffer);
          p=strstr(buffer,"SOS");
           if (p!=NULL){
             break;
            }
          //rec
       }
       strcpy(message,"102 Waiting For Users");
       send(sock,message,strlen(message),0);
      return;
    }

    bzero(message,sizeof(message));
    p=strstr(buffer,"200 OK");
    if(p!=NULL){
       printf("[~]Wait for connections\n");
       sleep(10); 
       return;
    }
  
}

int main(){
    int sock,conn,e;
    struct sockaddr_in address,newaddress;
     unsigned short port=60000;
    char buffer[1024];//message buffers
    char message[1024];

    sock=socket(AF_INET, SOCK_STREAM,0);//create socket
    socketerror(sock);
    memset(&address,0,sizeof(address));// set socket attributes
    address.sin_family = AF_INET; 
    address.sin_port = htons( port); 
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    conn=connect(sock,(struct sockaddr *) &address,sizeof(address));
    socketerror(conn);
    printf("[+]Connected to Server.\n");
    while (1)
    {
            e=recv(sock, buffer, 1024, 0);
            socketerror(e);
            processMessage(buffer,sock);
            bzero(buffer, sizeof(buffer));
      
    }
    printf("[+] Close Conection to Server.\n");

     close(sock)
}
