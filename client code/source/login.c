#include <gtk/gtk.h> 
#include <stdlib.h>
#include "client.h"
#include <stdio.h>
#include <string.h>
#include "GUI.h"
#include<pthread.h>
#include<malloc.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include"thread.h"
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;
static loginEntry* login_entry; 

Person_chat user;

/*获取本机IP*/
char* get_ip(void) {
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
   // return inet_ntoa(*(struct in_addr*)(hent->h_addr_list[1]));
   return "192.168.0.105";
}

/*显示错误提示框*/
void create_message_dialog (char* message) 
{  
    GtkWidget* dialog; 
    dialog = gtk_message_dialog_new(NULL, 
    GTK_DIALOG_MODAL, 
    GTK_MESSAGE_ERROR, 
    GTK_BUTTONS_OK,
    message);
    gtk_window_set_title(GTK_WINDOW(dialog),"错误提示");
    gtk_dialog_run(GTK_DIALOG(dialog));  
    gtk_widget_destroy(dialog); 
}
/*点击登录后*/
void on_login_clicked (GtkWidget* button,GtkWidget* window) 
{
    const gchar *ID = gtk_entry_get_text(GTK_ENTRY(login_entry->entry1));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(login_entry->entry2));
    char *buf=(char*)malloc(sizeof(ID)+sizeof(password)); 
    char* ip;
    char fuid[2];
    int res; pthread_t b_thread;  void *thread_result;
    ip=(char*)get_ip();
    Friend* finfo;
    sprintf(buf,"01%s00%%%s%%",ID,password);
    strcpy(smsg,buf);
    event=2;
//send message...
    res = pthread_create(&b_thread, NULL, thread_send,NULL);
    if (res !=0)
    {
       perror("Send Thread creation failed");
       exit(EXIT_FAILURE);
    }
    res = pthread_join(b_thread, &thread_result);   //发送消息结束
    if (res !=0)
    {
       perror("Thread join failed");
       exit(EXIT_FAILURE);
    }

printf("waitting for instructions:....\n");
    while(Link<0) ;  //等待消息传递
    while(Link!=9)   //消息处理
{
    if (Link==1)
    {
         
         fuid[0]=rmsg[2]; fuid[1]=rmsg[3]; 
         //finfo=(Friend*)malloc(sizeof(Friend));
         /*finfo=user[count].head;
	 while(finfo->next!=NULL)
	{
	  finfo=finfo->next;
	  if(memcmp(finfo->UID,fuid,2)==0) //UID匹配
	{
		FILE* fp;
		fp=finfo->fid;
		if((fp=fopen(finfo->filename,"a"))==NULL)  
    		{
        		printf("cannot open file!/n");
        		return ;
    		}
		for(int i=4;i<strlen(rmsg);i++)
		{
			fputc(rmsg[i],fp);
		}//写入日志文件
		//能否发送回车符？
		finfo->counter++;  //需要预先加载，刷新消息计数器
		break;
	}
	}*/
	//暂且先不处理找不到对应的好友的情况
	Link=-1; //消息处理完毕标志
    }
    else if (Link==7)
    {
      /*
      fuid[0]=rmsg[2]; fuid[1]=rmsg[3];
      finfo=(Friend*)malloc(sizeof(Friend));
      finfo=user[count].head;
	while(finfo->next!=NULL)
	{
	  finfo=finfo->next;
          if(memcmp(finfo->UID,fuid,2)==0)
	{
	  finfo->onoff=1;
	  break;
	}
	}
   */
      Link=-1;
    }
    else if(Link==99)
    {
        fuid[0]=rmsg[2]; fuid[1]=rmsg[3];
        int error=(fuid[0]-'0')*10+fuid[1]-'0';
        switch(error)
	{
	case 2: create_message_dialog ("输入的ＩＤ不存在"); 
		gtk_entry_set_text(GTK_ENTRY(login_entry->entry1),"");
		gtk_entry_set_text(GTK_ENTRY(login_entry->entry2),""); break;
	case 3: create_message_dialog ("输入的密码错误"); 
		gtk_entry_set_text(GTK_ENTRY(login_entry->entry1),"");
		gtk_entry_set_text(GTK_ENTRY(login_entry->entry2),""); break;
	default: break;
	}
	Link=-1;
	return ;  //结束程序，回到登录页面
    }
    else {  Link=9;}
}
    Link=-1;
    strcpy(user.ID,ID);
    strcpy(user.IP,ip);//待修改 
    strcpy(user.username,"Jane");//待修改 ．．用户名我不知道
    gtk_widget_destroy(window);
    create_mainpage_window(); 
} 

/*点击注册后*/
void on_start_register_clicked (GtkWidget* button,gpointer data) 
{  
    //收到服务器分配的ＩＤ
           char ID[3];
	   char *buf,*ip;
           int res; pthread_t b_thread;  void *thread_result;
           ip=get_ip();
	   buf=(char*)malloc(sizeof(ip));
     	   sprintf(buf,"000000%%%s%%",ip);
	   strcpy(smsg,buf);
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
           while(Link<0) ;
	   if(Link==0)
	   {
	     ID[0]=rmsg[2]; ID[1]=rmsg[3];ID[2]='\0';
             create_register_window(ID);
	     Link=-1;
           }
	if(Link==99)
	{
	int tmp=(rmsg[2]-'0')*10+rmsg[3]-'0';
	switch(tmp)
	{
	case 1: create_message_dialog ("系统未分配ＩＤ"); break;
	default: break;
	}
	   Link=-1;
	   return;
	}
           Link=-1;

} 

/*创建登录窗口*/
GtkWidget* create_login_window(void)
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
    GtkWidget* button1;  
    GtkWidget* button2; 

    login_entry = malloc(sizeof(loginEntry)); 

//设置窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    gtk_window_set_title(GTK_WINDOW(window),"登录窗口");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),0); 

//创建纵向盒子，元素不均匀排布，间隔为０
    box = gtk_vbox_new(FALSE,0);  
    gtk_container_add(GTK_CONTAINER(window),box);  
    box1 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box1,FALSE,FALSE,0);  
    box2 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box2,FALSE,FALSE,30);  
    box3 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box3,FALSE,FALSE,0);
    box4 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box4,FALSE,FALSE,10);
    sep = gtk_hseparator_new();  
    gtk_box_pack_start(GTK_BOX(box),sep,FALSE,FALSE,7); 
    box5 = gtk_hbox_new(FALSE,0);  
    gtk_box_pack_start(GTK_BOX(box),box5,FALSE,FALSE,5);   
  
//在box1中放入图片
    image = gtk_image_new_from_file("1.JPG");
    gtk_box_pack_start(GTK_BOX(box1),image,FALSE,FALSE,0);

//在box2和3中分别放入用户名和密码输入框
    label1 = gtk_label_new("                           ＩＤ号：");  
    login_entry->entry1 = gtk_entry_new();  
    gtk_box_pack_start(GTK_BOX(box2),label1,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box2),login_entry->entry1,FALSE,FALSE,5);    
    label2 = gtk_label_new("                            密    码：");  
    login_entry->entry2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(login_entry->entry2),FALSE);
    gtk_box_pack_start(GTK_BOX(box3),label2,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box3),login_entry->entry2,FALSE,FALSE,5); 
   
//将按钮放入box5中
    button1 = gtk_button_new_with_label("   登录   ");
    button2 = gtk_button_new_with_label("   注册   "); 

    gtk_box_pack_end(GTK_BOX(box5),button2,FALSE,FALSE,5); 
    //设置两个按钮间距
    gtk_box_set_spacing(GTK_BOX(box5),120);
    gtk_box_pack_end(GTK_BOX(box5),button1,FALSE,FALSE,5);

    //点击登录，关闭登录窗口，调用登录函数 
    g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(on_login_clicked),window);
    //g_signal_connect_swapped(G_OBJECT(button1),"clicked",G_CALLBACK(gtk_widget_destroy),window);
    
    //点击注册，调用开始注册函数（进入注册页面）
    g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(on_start_register_clicked),NULL);
     
    return window;
} 

