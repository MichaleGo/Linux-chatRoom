#include <gtk/gtk.h> 
#include <stdlib.h>
#include "client.h"
#include <stdio.h>
#include <string.h>
#include "GUI.h"
#include<malloc.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "thread.h"
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;

/*获取本机IP*/
char* Get_ip(void) {
    char hname[128];
    struct hostent *hent;
    int i;

    gethostname(hname, sizeof(hname));

    //hent = gethostent();
    hent = gethostbyname(hname);

    //printf("hostname: %s/naddress list: ", hent->h_name);
    for(i = 0; hent->h_addr_list[i]; i++) {
       /*   */
    }
    return "192.168.0.105";
}

/*点击注册后*/
void on_finish_register_clicked (GtkWidget* button,regEntry* reg_entry) 
{    
    if(strlen(gtk_entry_get_text(GTK_ENTRY(reg_entry->entry1))) == 0){
          if(strlen(gtk_entry_get_text(GTK_ENTRY(reg_entry->entry2))) == 0)
                create_message_dialog ("用户名和密码不能为空！");
          else
                create_message_dialog ("用户名不能为空！");
          create_register_window(reg_entry->ID);
          return;
    }
    if(strlen(gtk_entry_get_text(GTK_ENTRY(reg_entry->entry2))) == 0){
          create_message_dialog ("密码不能为空！");
          create_register_window(reg_entry->ID);
          return;
    }
    Person_reg newuser;
    strcpy(newuser.ID,reg_entry->ID);
    strcpy(newuser.username,gtk_entry_get_text(GTK_ENTRY(reg_entry->entry1)));
    strcpy(newuser.password,gtk_entry_get_text(GTK_ENTRY(reg_entry->entry2)));

    create_user_file(newuser);//newuser在本地创建文件夹
    char* ip;
    int res; pthread_t b_thread; void *thread_result;
    ip=(char*)Get_ip();

    char *buf=(char*)malloc(sizeof(newuser.username)+sizeof(newuser.password)+sizeof(newuser.ID)+sizeof(ip));
    sprintf(buf,"00%s00%s%%%s%%%s%%",newuser.ID,newuser.username,newuser.password,ip);
    strcpy(smsg,buf);  //消息更改
    event=1;  //触发事件
    
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
    
    printf("waitting for instructions:...\n");
    while(Link<0) ;
    if(Link==0)
    {
     printf("账号成功创建\n");Link=-1;
    }
    else Link=-1;
    
} 


/*创建注册窗口*/
void create_register_window(char ID[20]) 
{  
    GtkWidget* window;  
    GtkWidget* box;  
    GtkWidget* box1;  
    GtkWidget* box2;  
    GtkWidget* box3;
    GtkWidget* box4;
    GtkWidget* box5;
    GtkWidget* sep;
    GtkWidget* image;
    GtkWidget* label1;  
    GtkWidget* label2; 
    GtkWidget* label3;
    GtkWidget* label4;
    GtkWidget* ID_label;
    GtkWidget* button;
    regEntry *reg_entry;
    char* title;  
    reg_entry = malloc(sizeof(regEntry));
    strcpy(reg_entry->ID,ID);

//设置窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window),"注册窗口");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),0); 

//创建纵向盒子，元素不均匀排布，间隔为０
    box = gtk_vbox_new(FALSE,0);  
    gtk_container_add(GTK_CONTAINER(window),box);  
    box1 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box1,FALSE,FALSE,0);  
    box2 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box2,FALSE,FALSE,20);  
    box3 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box3,FALSE,FALSE,0);
    box4 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box4,FALSE,FALSE,20);
    sep = gtk_hseparator_new();  
    gtk_box_pack_start(GTK_BOX(box),sep,FALSE,FALSE,5); 
    box5 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box5,FALSE,FALSE,5);   

//在box1中放入图片
    image = gtk_image_new_from_file("1.JPG");
    gtk_box_pack_start(GTK_BOX(box1),image,FALSE,FALSE,0);

//在box2和3和4中分别放入ID号和用户名和密码输入框
    label1 = gtk_label_new("                           ＩＤ 号：");  
    ID_label =   gtk_label_new(ID);
    gtk_box_pack_start(GTK_BOX(box2),label1,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box2),ID_label,FALSE,FALSE,5);    
    label2 =  gtk_label_new("                            用户名："); 
    reg_entry->entry1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box3),label2,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box3),reg_entry->entry1,FALSE,FALSE,5); 
    label3 = gtk_label_new("                            密     码：");   
    reg_entry->entry2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(reg_entry->entry2),FALSE);
    gtk_box_pack_start(GTK_BOX(box4),label3,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box4),reg_entry->entry2,FALSE,FALSE,5); 

//创建注册用户的个人信息
   
//将按钮放入box5中
    button = gtk_button_new_with_label("     注册     ");
    label4 = gtk_label_new(NULL);
    title = "<span foreground=\"blue\"><i>欢迎使用NeuTalk</i></span>";  
    gtk_label_set_markup(GTK_LABEL(label4),title);

    //点击注册，调用注册函数
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_finish_register_clicked),reg_entry);
    //点击注册，关闭注册窗口
    g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),window); 

    gtk_box_pack_end(GTK_BOX(box5),label4,FALSE,FALSE,5);
    gtk_box_set_spacing(GTK_BOX(box5),50);
    gtk_box_pack_end(GTK_BOX(box5),button,FALSE,FALSE,5); 
       
    gtk_widget_show_all(window); 

    gtk_main();
    
} 

