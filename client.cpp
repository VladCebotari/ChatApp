#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <string.h>
#include <arpa/inet.h>


#define MAX_NUMBER_OF_REQUESTS 10
#define PORT 8008

int setup_client();


int main(){
  int client_socket_fd;
  client_socket_fd = setup_client();
  fd_set readfds;

 
  while(true){
      FD_ZERO(&readfds);
      FD_SET(0,&readfds);
      FD_SET(client_socket_fd,&readfds);
      int maxfd = client_socket_fd;
      int bytes;
    select(maxfd+1,&readfds,NULL,NULL,NULL);  
    
    if(FD_ISSET(client_socket_fd,&readfds)){
        char buffer[256];
        bytes = recv(client_socket_fd,buffer,sizeof(buffer)-1,0);
        buffer[bytes] = '\0';
        printf("%s",buffer);

      }
    if(FD_ISSET(0,&readfds)){
        char buffer[256];
        fgets(buffer,sizeof(buffer),stdin);
        send(client_socket_fd,buffer,strlen(buffer),0);
      }

  }

  return 0;
}

int setup_client(){
  struct sockaddr_in server_adress;
  server_adress.sin_family = AF_INET;
  server_adress.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_adress.sin_port = htons(8008);

  int client_socket_fd = socket(AF_INET,SOCK_STREAM,0);
 
  int connection_status = connect(client_socket_fd,(struct sockaddr *)&server_adress,sizeof(server_adress));
  
  if (connection_status == -1){
    perror("connect issue");
    exit(1);
  }

  return client_socket_fd;
}
