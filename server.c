#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>

#define MAX_NUMBER_OF_REQUESTS 10
#define PORT 8008

int setup_server();


int main(int argc,char *argv[]){
  int clients_fd[MAX_NUMBER_OF_REQUESTS];
  int server_socket_fd;
  int new_client;
  fd_set readfds;
  fd_set master;
  
  struct sockaddr_in client_addr;
  socklen_t addrlen = sizeof(client_addr);

  server_socket_fd = setup_server();

  for(int i=0;i<MAX_NUMBER_OF_REQUESTS;i++){
      clients_fd[i] = -1;
  }
      
  FD_ZERO(&master);
  FD_ZERO(&readfds);
  FD_SET(server_socket_fd,&master);
  int maxfd = server_socket_fd;

  while(1){
    readfds = master;

    select(maxfd+1,&readfds,NULL,NULL,NULL);
    
    if(FD_ISSET(server_socket_fd,&readfds)){
      new_client = accept(server_socket_fd,(struct sockaddr*)&client_addr,&addrlen);
      if(new_client < 0){
        perror("accept");
      }else{ 
      printf("New client connected (fd: %d)\n", new_client);
      int placed = 0;
        for(int i=0;i<MAX_NUMBER_OF_REQUESTS;i++){
            if(clients_fd[i] == -1){
              clients_fd[i] = new_client;
              FD_SET(new_client,&master);
              if (new_client > maxfd) maxfd = new_client;
              placed = 1;
              break;
            }
          }
        if (!placed){
          fprintf(stderr,"No free slot for new clinet %d,closing",new_client);
          close(new_client);
        }
      }
    }

    for(int i = 0;i < MAX_NUMBER_OF_REQUESTS;i++){
      int fd = clients_fd[i];
      if(clients_fd[i] == -1) continue;

      if(FD_ISSET(clients_fd[i],&readfds)){
      char buffer[512];
      int bytes = recv(clients_fd[i],buffer,sizeof(buffer)-1,0);
        if(bytes < 0){
          perror("recv failed");
          close(fd);
          clients_fd[i] = -1;
          continue;
            }
        if(bytes == 0){
          printf("Client %d disconnected\n", clients_fd[i]); 
          close(fd);
          clients_fd[i] = -1;
          continue;
        }
        buffer[bytes] ='\0';
        printf("Client %d sent: %s", clients_fd[i], buffer);
      
        for(int j=0;j<MAX_NUMBER_OF_REQUESTS;j++){
          int outfd = clients_fd[j];
          if(outfd != -1 && outfd != fd ){
            ssize_t s = send(outfd,buffer,strlen(buffer),0);
            if(s < 0){
              perror("send to client failed");
              close(outfd);
              clients_fd[j] = -1;
            }
          }
        }
      } 
    }
  }

  for (int i =0;i < MAX_NUMBER_OF_REQUESTS;i++){
    if(clients_fd[i] != -1)close(clients_fd[i]);

  }

  close(server_socket_fd);
  return 0;
}


int setup_server(){

  struct sockaddr_in server_adress;
  server_adress.sin_family = AF_INET;
  server_adress.sin_port = htons(PORT) ;
  server_adress.sin_addr.s_addr = INADDR_ANY;

  int server_socket_fd = socket(AF_INET,SOCK_STREAM,0);

  if(server_socket_fd < 0){
        perror("socket failed");
        exit(1);
    }

  int opt = 1;
  setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  bind(server_socket_fd,(struct sockaddr *)&server_adress,sizeof(server_adress));
  listen(server_socket_fd,MAX_NUMBER_OF_REQUESTS);

  printf("Server is running on port %d\n",PORT);
  return server_socket_fd;
}


