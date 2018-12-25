#include <gtk/gtk.h>
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include "client.h"
#include "GUI.h"
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;
extern Person_chat user;

/*获取客户端文件存取路径*/
char* get_path(char* userID,char *friendID)//写出获得Path的方法
{ 
    static char path[100];
    strcpy(path,"/home/new/Desktop/neutalk/");
    strcat(path,userID);
    strcat(path,"/history/");
    strcat(path,friendID);
    strcat(path,".txt"); 
    return path;
}

/*创建聊天记录页面*/
void create_chat_message_window(Person_chat *friend)
{
    GtkWidget *window, *scwin;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *view;
    GtkWidget *button;
    GtkTextBuffer *chat_message_buffer;
    GtkTextIter start, end;
    FILE *fp;
    char* path;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window),"聊天记录");
    gtk_window_set_default_size(GTK_WINDOW(window),620,550);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);

    vbox = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),vbox);
    gtk_widget_set_size_request(vbox,600,500);
    scwin = gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scwin),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    view = gtk_text_view_new();
    chat_message_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view),GTK_WRAP_WORD);//处理多行显示
    gtk_text_view_set_justification(GTK_TEXT_VIEW(view),GTK_JUSTIFY_LEFT);//对齐方式
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);//不允许内容修改
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(view),5);
    gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(view),5);
    gtk_text_view_set_pixels_inside_wrap(GTK_TEXT_VIEW(view),5);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view),10);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view),10);

    gtk_widget_set_size_request(view,540,470);
    gtk_box_pack_start(GTK_BOX(vbox),scwin,FALSE,FALSE,5);
    gtk_container_add(GTK_CONTAINER(scwin),view);

    //处理文件
    path = get_path(user.ID,friend->ID);
    char readline[1024];             //每行最大读取的字符数
    fp = fopen(path,"r");

    while (fgets(readline,1024,fp)!=NULL) 
    { 
        gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(chat_message_buffer), &start, &end);
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(chat_message_buffer), &end,readline,strlen(readline));
    } 
    fclose(fp);                 
    



    hbox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
    button = gtk_button_new_with_label("   关闭    ");
    gtk_box_pack_end(GTK_BOX(hbox),button,FALSE,FALSE,0);
    g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),window); 

    gtk_widget_show_all(window);  
}


