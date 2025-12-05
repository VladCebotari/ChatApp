#include <gtk/gtk.h>
#include <pthread.h>
#include <sys/socket.h>  
#include <arpa/inet.h>   
#include <unistd.h>
#include <string.h>
#include "UI.h"


GtkWidget *chat_area = NULL;
extern int global_socket_fd;
extern void *recv_thread_func(void *arg);

gboolean ui_display_message(gpointer user_data){
  char *msg = (char*)user_data;

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_area));
  GtkTextIter end;
  gtk_text_buffer_get_end_iter(buffer,&end);
  gtk_text_buffer_insert(buffer,&end,msg,-1);

  if (msg[0] != '\0' && msg[strlen(msg) - 1] != '\n') {
            gtk_text_buffer_insert(buffer, &end, "\n", -1);
        }
    
  g_free(msg);
  return FALSE;

}


void on_send_clicked(GtkButton *button,gpointer user_data){
  GtkEditable *entry = GTK_EDITABLE(user_data);
  const char *text = gtk_editable_get_text(entry);
  if(text && *text){
    if(global_socket_fd != -1){
      char full_msg[512]; 
      snprintf(full_msg, sizeof(full_msg), "%s\n", text);

      ssize_t sent = send(global_socket_fd,text,strlen(text),0);
        if(sent == -1){
          perror("send failed");
        }
    }

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_area));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, text, -1);
    gtk_text_buffer_insert(buffer, &end, "\n", -1);

    gtk_editable_set_text(entry, "");
  }
}

void on_entry_activate(GtkEntry *entry,gpointer user_data){
  GtkButton *send_button = GTK_BUTTON(user_data);
  g_signal_emit_by_name(send_button,"clicked");

}

void activate(GtkApplication* app,gpointer data){

  GtkWidget* window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window),"vladchatapp");
  gtk_window_set_default_size(GTK_WINDOW(window),600,400);
  
  
  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), main_box);

  chat_area = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(chat_area), FALSE);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(chat_area), GTK_WRAP_WORD_CHAR);
  gtk_widget_set_vexpand(chat_area, TRUE);
  
  GtkWidget *bottom_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,2);
  GtkWidget* send_button = gtk_button_new_with_label("Send");
  GtkWidget* entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Type a message...");
  gtk_widget_set_hexpand(entry, TRUE);
 
  gtk_box_append(GTK_BOX(bottom_box),entry);
  gtk_box_append(GTK_BOX(bottom_box),send_button);

  
  gtk_box_append(GTK_BOX(main_box), chat_area);
  gtk_box_append(GTK_BOX(main_box), bottom_box);
  

  g_signal_connect(send_button,"clicked",G_CALLBACK(on_send_clicked),entry);
  g_signal_connect(entry,"activate",G_CALLBACK(on_entry_activate),send_button);
  
  gtk_window_present(GTK_WINDOW(window));

  pthread_t recv_thread;
  pthread_create(&recv_thread,NULL,recv_thread_func,NULL);

}
