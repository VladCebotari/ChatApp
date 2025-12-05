#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

extern int global_socket_fd;
extern GtkWidget *chat_area;


void activate(GtkApplication* app,gpointer data);
void on_send_clicked(GtkButton *button,gpointer user_data);
void on_entry_activate(GtkEntry *entry,gpointer user_data);
gboolean ui_display_message(gpointer user_data);


void* recv_thread_func(void* arg);


#endif