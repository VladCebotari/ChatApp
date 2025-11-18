#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_NUMBER_OF_REQUESTS 10
#define PORT 8008

int setup_client();


int main(){
  int client_socket_fd;
  client_socket_fd = setup_client();
  

 
  while(true){
    //recv(client_socket_fd);
    // send 
  }

  return 0;
}

int setup_client(){
  struct sockaddr_in client_adress;
  client_adress.sin_family = AF_INET;
  client_adress.sin_addr.s_addr = INADDR_ANY;
  client_adress.sin_port = htons(8008);

  int client_socket_fd = socket(AF_INET,SOCK_STREAM,0);
 
  int connection_status = connect(client_socket_fd,(struct sockaddr *)&client_adress,sizeof(client_adress));
  
  if (connection_status = -1){
    printf("Client failed to connect\n");

  }

  return client_socket_fd;
}
//