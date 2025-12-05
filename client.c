#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <string.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <errno.h>

#include "UI.h"

#define MAX_NUMBER_OF_REQUESTS 10
#define PORT 8008

int global_socket_fd = -1;

int setup_client();
void *recv_thread_func(void *arg);

int main(int argc, char **argv){
 
  global_socket_fd = setup_client();
  
  GtkApplication* chatapp = gtk_application_new("com.vlad.chatapp",G_APPLICATION_NON_UNIQUE);
  g_signal_connect(G_APPLICATION(chatapp),"activate",G_CALLBACK(activate),NULL);

 
  int status = g_application_run(G_APPLICATION(chatapp),argc,argv);
  g_object_unref(chatapp);

  if (global_socket_fd != -1) {
    close(global_socket_fd);
    global_socket_fd = -1;
  }
  return status;
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


void *recv_thread_func(void *arg){

  char buffer[512];

  while(global_socket_fd != -1){
    int current_fd = global_socket_fd;
    if (current_fd == -1) break;
 
    int bytes = recv(global_socket_fd,buffer,sizeof(buffer)-1,0); 

    if(bytes < 0){
      perror("recv failed");
    } 

    if(bytes == 0){
      fprintf(stderr,"Server closed connection");
      if (global_socket_fd != -1) {
          close(global_socket_fd);
          global_socket_fd = -1;
      }
      g_idle_add(ui_display_message,g_strdup("*** Disconnected from server ***\n"));
      break;
    }
    buffer[bytes] = '\0';

    g_idle_add(ui_display_message,g_strdup(buffer));
  }
  return NULL;
}