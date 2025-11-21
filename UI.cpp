#include <gtk/gtk.h>

void on_send_clicked(GtkButton *button,gpointer user_data){
  GtkEditable *entry = GTK_EDITABLE(user_data);
  const char *text = gtk_editable_get_text(entry);
  if(text && *text){
    g_print("Message: %s\n",text);
    gtk_editable_set_text(entry,"");
  }
}

void on_entry_activate(GtkEntry *entry,gpointer user_data){
  GtkButton *send_button = GTK_BUTTON(user_data);
  g_signal_emit_by_name(send_button,"clicked");

}

void activate(GtkApplication* app,gpointer data){

  GtkWidget* window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window),"vladchatapp");
  gtk_window_set_default_size(GTK_WINDOW(window),1200,800);
  
  //
  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), main_box);

  GtkWidget *chat_area = gtk_text_view_new(); // very soon you'll use this
  gtk_text_view_set_editable(GTK_TEXT_VIEW(chat_area), FALSE);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(chat_area), GTK_WRAP_WORD_CHAR);
  gtk_widget_set_vexpand(chat_area, TRUE);
  //
  GtkWidget *bottom_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);

  GtkWidget* send_button = gtk_button_new_with_label("Send");
  GtkWidget* entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Type a message...");
  gtk_widget_set_hexpand(entry, TRUE);//asta tot
 
  gtk_box_append(GTK_BOX(bottom_box),entry);
  gtk_box_append(GTK_BOX(bottom_box),send_button);

  //
  gtk_box_append(GTK_BOX(main_box), chat_area);
  gtk_box_append(GTK_BOX(main_box), bottom_box);
  //

  g_signal_connect(send_button,"clicked",G_CALLBACK(on_send_clicked),entry);
  g_signal_connect(entry,"activate",G_CALLBACK(on_entry_activate),send_button);
  
  gtk_window_set_child(GTK_WINDOW(window),
  bottom_box);

  gtk_window_present(GTK_WINDOW(window));

}


int main(int argc, char** argv) {
  GtkApplication* chatapp = gtk_application_new("com.vlad.chatapp",G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(G_APPLICATION(chatapp),"activate",G_CALLBACK(activate),NULL);

 
  int status = g_application_run(G_APPLICATION(chatapp),argc,argv);
  g_object_unref(chatapp);
  return status;
}
