#include <gtk/gtk.h>
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <time.h>
#include "client.h"
#include "GUI.h"
#include<pthread.h>
#include "thread.h"
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;
extern Person_chat user;

GtkTextBuffer *input_buffer;
GtkTextBuffer *chat_buffer;

/*发送文件*/
void on_send_file(GtkWidget *w,GtkFileSelection *fs)
{
   g_print("%s\n",gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
}

/*点击文件选择按钮，创建文件选择窗口 */
void on_create_file_selection_window(GtkButton* button, gpointer data)
{
    GtkWidget *filew;
 
    //创建一个新的文件选择构件
    filew = gtk_file_selection_new("文件选择对话框");
    g_signal_connect (GTK_OBJECT(filew), "destroy",G_CALLBACK (gtk_main_quit), NULL);
 
    //为ok_button按钮设置回调函数
    g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filew)->ok_button),"clicked",G_CALLBACK(on_send_file),filew);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(filew)->ok_button),"clicked",G_CALLBACK(gtk_widget_destroy),filew);
    //为cancel_button设置回调函数
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(filew)->cancel_button),"clicked",G_CALLBACK(gtk_widget_destroy),filew);

    gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew),"");

    gtk_widget_show (filew);
    gtk_main();
}

/*发送消息*/
void on_send_message(GtkButton* button, Person_chat *friend)
{
    //清除发送框消息
    gdk_threads_init();
    gdk_threads_enter();
    gtk_init(NULL,NULL);
    GtkTextIter start,end;
    GtkTextIter start1, end1;//处理上方窗口
    char input_message[1024];//
    int res; pthread_t b_thread;  void *thread_result;
    strcpy(input_message,"02");
    strcat(input_message,user.ID);
    strcat(input_message,friend->ID);

    char chattime[50] = "";
    char chat_content[1024] = "";
    

    time_t timep;
    struct tm *p;
    time (&timep);
    p=gmtime(&timep);
    char string[30];
    sprintf(string,"%d",1900+p->tm_year);
    strcat(chattime,string);
    strcat(chattime,"/");
    if(1+p->tm_mon < 10){
          strcat(chattime,"0");
          sprintf(string,"%d",1+p->tm_mon);
    }
    else
          sprintf(string,"%d",1+p->tm_mon);
    strcat(chattime,string);
    strcat(chattime,"/");
    if(p->tm_mday < 10){
          strcat(chattime,"0");
          sprintf(string,"%d",p->tm_mday);
    }
    else
          sprintf(string,"%d",p->tm_mday);
    strcat(chattime,string);
    strcat(chattime,"_");
    if(8+p->tm_hour < 10){
          strcat(chattime,"0");
          sprintf(string,"%d",8+p->tm_hour);
    }
    else
          sprintf(string,"%d",8+p->tm_hour);
    strcat(chattime,string);
    strcat(chattime,":");
    if(p->tm_min < 10){
          strcat(chattime,"0");
          sprintf(string,"%d",p->tm_min);
    }
    else
          sprintf(string,"%d",p->tm_min);
    strcat(chattime,string);
    strcat(chattime,":");
    if(p->tm_sec < 10){
          strcat(chattime,"0");
          sprintf(string,"%d",p->tm_sec);
    }
    else
          sprintf(string,"%d",p->tm_sec);
    strcat(chattime,string);
   
    strcat(input_message,chattime);

    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(input_buffer), &start, &end);
    strcpy(chat_content, gtk_text_buffer_get_text(input_buffer, &start, &end, FALSE));
    gtk_text_buffer_delete(input_buffer,&start,&end);

    strcat(input_message,chat_content);
    write_chat_message(user.ID,friend->ID,chattime,chat_content,1);//1表示发送者为user

    strcat(chattime,"      ID: ");//buffer写入chattime
    strcat(chattime,user.ID);
    strcat(chattime,"\n");
    strcat(chat_content,"\n");
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(chat_buffer), &start1, &end1);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(chat_buffer), &end1,chattime,strlen(chattime));
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(chat_buffer), &start1, &end1);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(chat_buffer), &end1,chat_content,strlen(chat_content));
    
    strcpy(smsg,input_message);
    event=2;//send 
    
    res = pthread_create(&b_thread, NULL, thread_send,NULL);
    if (res !=0)
    {
       perror("Send Thread creation failed");
       exit(EXIT_FAILURE);
    }
    res = pthread_join(b_thread, &thread_result);   //等待线程的结束
    if (res !=0)
    {
       perror("Thread join failed");
       exit(EXIT_FAILURE);
    }
    gdk_threads_leave();
    //这里只需保证发送出去即可
}

/*接收消息功能*/
void on_receive_message(void)
{
    gdk_threads_init();
    gdk_threads_enter();
    gtk_init(NULL,NULL);
    GtkTextIter start,end;
    char get_message[1024];//接收02032018/09/09_34:34:34sdkfjshdkfjsd020304sdkfjshdkfjsd
    strcpy(get_message,rmsg); 
    
    char friendID[5] = "";
    char chattime[30] = "";
    char chat_content[1024] ;
memset(chat_content,0,sizeof(chat_content));
    friendID[0] = get_message[2];
    friendID[1] = get_message[3];
    friendID[2] = '\0';
    int i;
    for(i = 4; i < 23; i++)
          chattime[i-4] = get_message[i];
    chattime[19] = '\0';
    while(get_message[i]!='\0')
    {
          chat_content[i-23] = get_message[i];
	  i++;
    }
    chat_content[i-23] = '\0';
    write_chat_message(user.ID,friendID,chattime,chat_content,0);//2表示发送者为friend
    strcat(chattime,"      ID: ");//buffer写入chattime
    strcat(chattime,friendID);
    strcat(chattime,"\n");
    strcat(chat_content,"\n");
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(chat_buffer), &start, &end);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(chat_buffer), &end,chattime,strlen(chattime));
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(chat_buffer), &start, &end);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(chat_buffer), &end,chat_content,strlen(chat_content));
    Link=-1;
    gdk_threads_leave();
}


/*点击聊天记录按钮*/
void on_find_chat_log(GtkButton* button, Person_chat *friend)
{
    create_chat_message_window(friend);
}

/*创建聊天页面*/
void create_chat_window(Person_chat *friend)
{  
    GtkWidget *window, *scwin1, *scwin2;  
    GtkWidget *hbox;//主盒子
    GtkWidget *vbox1, *vbox2;//左右盒子
    GtkWidget *hbox1,*hbox2,*hbox3,*hbox4,*hbox5;//左侧四部分
    GtkWidget *hbox6;//右侧第一部分
    GtkWidget *vbox3, *vbox4;//右边第二三部分
    GtkWidget *view1;//主页面
    GtkWidget *view2;//编辑页面  
    GtkWidget *button1;//文件
    GtkWidget *button2;//聊天记录
    GtkWidget *button3;//发送
    GtkWidget *label1, *label2;//页面左侧右侧上端  
    GtkWidget *label3,*label4,*label5,*label6,*label7,*label8;//朋友与自己的信息
    GtkTextIter start1, end1, start2, end2;  
    FILE *fp;
    char* path;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window),"聊天界面");
    gtk_window_set_default_size(GTK_WINDOW(window),700,550);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),10); 

    //放盒子布局
    hbox = gtk_hbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),hbox);
    vbox1 = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbox),vbox1,FALSE,FALSE,0);
    vbox2 = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbox),vbox2,FALSE,FALSE,30);
    gtk_widget_set_size_request(hbox,700,550);

    //创建左侧标签
    hbox1 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox1),hbox1,FALSE,FALSE,0);
    label1 = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(hbox1),label1,FALSE,FALSE,15);

    //创建容纳左侧上方滚动窗口的盒子
    hbox2 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox1),hbox2,FALSE,FALSE,5);

    //创建左侧上方滚动窗口
    scwin1 = gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scwin1),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    view1 = gtk_text_view_new(); //新建一个文本框
    
    chat_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view1));
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view1),GTK_WRAP_WORD);//处理多行显示
    gtk_text_view_set_justification(GTK_TEXT_VIEW(view1),GTK_JUSTIFY_LEFT);//对齐方式
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view1),FALSE);//不允许内容修改
    gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(view1),5);
    gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(view1),0);
    gtk_text_view_set_pixels_inside_wrap(GTK_TEXT_VIEW(view1),5);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view1),10);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view1),10);
    
    gtk_widget_set_size_request(view1,500,300);
    gtk_box_pack_start(GTK_BOX(hbox2),scwin1,FALSE,FALSE,15);
    gtk_container_add(GTK_CONTAINER(scwin1),view1);
    path = get_path(user.ID,friend->ID);

    char readline[1024];             //每行最大读取的字符数
    fp = fopen(path,"r");
    int sum_line = 0;//计算文件总行数
    int count_line = 0;//计算读到的行数

    while (!feof(fp)) 
    { 
        fgets(readline,1024,fp);  //读取一行
        sum_line++;
    } 
    rewind(fp);
    while(fgets(readline,1024,fp)!=NULL)
    {
        count_line++;
        if(sum_line - count_line <=12)
        {
            gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(chat_buffer), &start1, &end1);
            gtk_text_buffer_insert(GTK_TEXT_BUFFER(chat_buffer), &end1,readline,strlen(readline));
        }
    }
    fclose(fp);

    //创建左侧两个窗口之间部分
    hbox3 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox1),hbox3,FALSE,FALSE,0);
    button1 = gtk_button_new_with_label("选择文件");
    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(on_create_file_selection_window),NULL);
    gtk_box_pack_start(GTK_BOX(hbox3),button1,FALSE,FALSE,15);

    //创建容纳左侧下方滚动窗口的盒子
    hbox4 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox1),hbox4,FALSE,FALSE,5);

    //创建左侧下方滚动窗口
    scwin2 = gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scwin2),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    view2 = gtk_text_view_new();
    input_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view2));
    gtk_widget_set_size_request(view2,500,135);
    gtk_box_pack_start(GTK_BOX(hbox4),scwin2,FALSE,FALSE,15);
    gtk_container_add(GTK_CONTAINER(scwin2),view2);

    //创建左侧最下方部分
    hbox5 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox1),hbox5,FALSE,FALSE,0);
    button3 = gtk_button_new_with_label("聊天记录");
    g_signal_connect(G_OBJECT(button3),"clicked",G_CALLBACK(on_find_chat_log),friend); 
    gtk_box_pack_start(GTK_BOX(hbox5),button3,FALSE,FALSE,15);
    button2 = gtk_button_new_with_label("   发送   ");
    g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(on_send_message),friend);
    gtk_box_pack_start(GTK_BOX(hbox5),button2,FALSE,FALSE,15);
    gtk_box_set_spacing(GTK_BOX(hbox5),340);

    //创建右侧标签
    hbox6 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox2),hbox6,FALSE,FALSE,0);
    label2 = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(hbox6),label2,FALSE,FALSE,15);

    //传递基本信息！如果修改数据结构可能需要修改这部分！下部分的引用也需要修改
    Person_chat users={"IP: ","ID: ","我的昵称: "};//便于显示
    strcat(users.IP,user.IP);
    strcat(users.username,user.username);
    strcat(users.ID,user.ID);
    
    Person_chat myfriend={"IP: ","ID: ","朋友昵称: "};//便于显示
    strcat(myfriend.IP,friend->IP);
    strcat(myfriend.username,friend->username);
    strcat(myfriend.ID,friend->ID);
 

    //创建右侧信息
    vbox3 = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox2),vbox3,FALSE,FALSE,20);
    vbox4 = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox2),vbox4,FALSE,FALSE,30);
    label3 = gtk_label_new(myfriend.IP);
    gtk_box_pack_start(GTK_BOX(vbox3),label3,FALSE,FALSE,10);
    label4 = gtk_label_new(myfriend.ID);
    gtk_box_pack_start(GTK_BOX(vbox3),label4,FALSE,FALSE,10);
    label5 = gtk_label_new(myfriend.username);
    gtk_box_pack_start(GTK_BOX(vbox3),label5,FALSE,FALSE,10);
    label6 = gtk_label_new(users.IP);
    gtk_box_pack_start(GTK_BOX(vbox4),label6,FALSE,FALSE,10);
    label7 = gtk_label_new(users.ID);
    gtk_box_pack_start(GTK_BOX(vbox4),label7,FALSE,FALSE,10);
    label8 = gtk_label_new(users.username);
    gtk_box_pack_start(GTK_BOX(vbox4),label8,FALSE,FALSE,10);


    gtk_widget_show_all(window);  
    gtk_main();  
} 
