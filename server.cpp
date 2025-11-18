#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_NUMBER_OF_REQUESTS 10
#define PORT 8008

int setup_server();
void *handle_connection(int client_fd);

int main(int argc,char *argv[]){
  int clients_fd[MAX_NUMBER_OF_REQUESTS];
  int number_of_clients = 0;
  int server_socket_fd;

  server_socket_fd = setup_server();

  while(true){
    int client_fd = accept(server_socket_fd,NULL,NULL);
    // handle_connection(client_fd);
    
    
    clients_fd[number_of_clients++] = client_fd;
    //send receive how and so on
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

  bind(server_socket_fd,(struct sockaddr *)&server_adress,sizeof(server_adress));
  listen(server_socket_fd,MAX_NUMBER_OF_REQUESTS);

  return server_socket_fd;
}


//void *handle_connection(int client_fd){


//}
//