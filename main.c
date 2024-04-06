#include<gtk/gtk.h>
#include <stdlib.h>

int opened=0; 
GFile* file;
static void open_file(GtkWidget * widget,GtkWidget* tv){
  GtkTextBuffer *tb;
  GtkTextIter start, end;
  GError *err = NULL;
  gchar *text;
  gsize length;
 if(opened){
    
    tb=gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
    gtk_text_buffer_get_bounds(tb, &start, &end);
    text = gtk_text_buffer_get_text(tb, &start, &end, FALSE);
    char* pfilename=g_file_get_basename(file);
    if(g_file_replace_contents (file, text, strlen(text), NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL, &err)){
        GtkAlertDialog* dialog= gtk_alert_dialog_new("File Saved.\n");
        gtk_alert_dialog_show(dialog,NULL);      
        gtk_text_buffer_set_text (tb, pfilename, strlen(pfilename));
        gtk_text_view_set_buffer(GTK_TEXT_VIEW(tv),tb);
        gtk_button_set_label(GTK_BUTTON(widget),"Open File");
        opened=0;
    }else{
      GtkAlertDialog* dialog= gtk_alert_dialog_new("%s.\n",err->message);
      gtk_alert_dialog_show(dialog,NULL);      
    }
    
    
 }else{
    tb=gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
    char *contents;
    
    gtk_text_buffer_get_bounds(tb, &start, &end);
    text = gtk_text_buffer_get_text(tb, &start, &end, FALSE);
    file=g_file_new_for_path(text);
   
    if (g_file_load_contents(file, NULL, &contents, &length, NULL, &err)) {
        gtk_text_buffer_set_text (tb, contents, length);
        opened=1;
        gtk_button_set_label(GTK_BUTTON(widget),"Close");
    }else{
      FILE* f;
      if(f=fopen(text,"w")){
            fclose(f);
            file=g_file_new_for_path(text);
            gtk_text_buffer_set_text (tb, "", 0);
            opened=1;
            gtk_button_set_label(GTK_BUTTON(widget),"Close");
            
      }else{
        GtkAlertDialog* dialog = gtk_alert_dialog_new("%s.\n",err->message);
        gtk_alert_dialog_show(dialog,NULL);
      }      
    }
      
    
 }
 g_free(text);


}
static void activate(GtkApplication * app){
  GtkWidget* window;
  GtkWidget* button;
  GtkWidget* box;
  GtkTextBuffer *tb;
  GtkWidget *tv;
  GtkWidget *scw;
  
  window=gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window),"New App");
  gtk_window_set_default_size(GTK_WINDOW(window) ,800,600);
  box=gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
  gtk_window_set_child(GTK_WINDOW(window),box);
  scw=gtk_scrolled_window_new();
  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scw),300);
  button=gtk_button_new_with_label("Open File");
  

  tv=gtk_text_view_new();
  tb=gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
  g_signal_connect(button, "clicked", G_CALLBACK (open_file), tv);

  gtk_text_buffer_set_text(tb,"Enter Filename",14);
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(tv),tb);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(tv),true);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scw),tv);
  gtk_box_append(GTK_BOX(box),scw);
  gtk_box_append(GTK_BOX(box),button);
  gtk_window_present(GTK_WINDOW(window));

}

int main(int argc, char  **argv)
{
  GtkApplication * app=gtk_application_new("github.addy897.texteditor",G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
  int res=g_application_run(G_APPLICATION(app),argc,argv);
  g_object_unref(app);
  return res;
}