#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "client.h"
#include "GUI.h"
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;
extern Person_chat user;

Person_chat friend_list[15];

GtkWidget *fixed;

int count = 0;

    static GdkColor red = {0, 0xffff, 0, 0};
    static GdkColor green = {0, 0, 0xffff, 0};
    static GdkColor blue = {0, 0, 0, 0xffff};
    static GdkColor yellow = {0, 0xffff, 0xffff, 0};
    static GdkColor black = {0, 0, 0, 0};

	GtkWidget *Sysbutton;

/*点击添加好友后，向系统发出消息*/
void  on_ok_clicked  (GtkWidget *window, GtkWidget *entry_id)
{
        g_print("ID是%s\n",gtk_entry_get_text(GTK_ENTRY(entry_id)));
	g_print("已向该用户发出申请\n");

}

/*创建好友按钮*/
GtkWidget* create_frdbutton(Person_chat per)
{
	GtkWidget* box;
	GtkWidget* image;
	GtkWidget* label1;
	GtkWidget* label2;
	GtkWidget* button;
	image = gtk_image_new_from_file("qq.png");
	label1 = gtk_label_new(per.username);
	label2 = gtk_label_new(per.ID);
	box = gtk_hbox_new(FALSE,2);
	gtk_container_set_border_width(GTK_CONTAINER(box),6);
	gtk_box_pack_start(GTK_BOX(box),image,FALSE,FALSE,3);
	gtk_box_pack_start(GTK_BOX(box),label1,FALSE,FALSE,3);
	gtk_box_pack_start(GTK_BOX(box),label2,FALSE,FALSE,2);
	gtk_widget_show(image);
	gtk_widget_show(label1);
	gtk_widget_show(label2);
	button = gtk_button_new();
	gtk_container_add(GTK_CONTAINER(button),box);
	gtk_widget_show(box);
	return button ;
}

/*接受好友申请后,主页面添加新好友*/
void sys_ok_clicked (GtkWidget *window, gpointer data)
{
	strcpy(friend_list[2].ID,"02");
	strcpy(friend_list[2].username,"Jack");
	Person_chat *one_friend;
	one_friend = malloc(sizeof(Person_chat));
	strcpy(one_friend->IP,"");
	strcpy(one_friend->username,friend_list[2].username);
	strcpy(one_friend->ID,friend_list[2].ID);
	GtkWidget* new_friend;
        new_friend = create_frdbutton(friend_list[2]);
	count++;
	int pos=200+(count-1)*60;
	count++;
	gtk_fixed_put(GTK_FIXED(fixed),new_friend,30,pos);
	gtk_widget_set_size_request(new_friend,300,100);
	gtk_widget_show_all(fixed);
	add_new_friend(user.ID, friend_list[2].ID);
	gtk_widget_modify_fg(GTK_BIN(Sysbutton)->child, GTK_STATE_NORMAL, &black);
	g_signal_connect(G_OBJECT(new_friend),"clicked",G_CALLBACK(on_chat_clicked),one_friend);
}

/*创建添加好友界面*/
void AddFriends (void)
{
	GtkWidget *entry_id;
        GtkWidget *win;
	GtkWidget* vbox;
	GtkWidget* table;
	GtkWidget* bbox;
	GtkWidget* label;
	GtkWidget* button;
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win),"好友申请");
	gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(win),"delete_event",G_CALLBACK(gtk_widget_destroy),win);
	gtk_container_set_border_width(GTK_CONTAINER(win),10);
	vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(win),vbox);
	table = gtk_table_new(2,2,FALSE);
	gtk_box_pack_start(GTK_BOX(vbox),table,FALSE,FALSE,5);
	label = gtk_label_new("ID");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,0,1);
	entry_id = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),entry_id,1,2,0,1);
	bbox = gtk_hbutton_box_new();
	gtk_box_pack_start(GTK_BOX(vbox),bbox,FALSE,FALSE,5);
	gtk_box_set_spacing(GTK_BOX(bbox),5);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_END);
	button = gtk_button_new_from_stock(GTK_STOCK_OK);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_ok_clicked),entry_id);
        g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),win);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,5);
	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),win);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,5);
	gtk_widget_show_all(win);
}

/*创建系统消息界面*/
void SysInf(void)
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *bbox;
	GtkWidget *button;
	GtkWidget *label;
	GtkWidget *image;
	GtkWidget *sep;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"系统消息");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	hbox = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
	image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING,GTK_ICON_SIZE_DIALOG);
	gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
	label = gtk_label_new("Jack ID：02请求添加您为好友\n请问您是否同意？");
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,5);
	sep = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(vbox),sep,FALSE,FALSE,5);
	bbox = gtk_hbutton_box_new();
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_SPREAD);
	gtk_box_pack_start(GTK_BOX(vbox),bbox,FALSE,FALSE,5);
	button = gtk_button_new_from_stock(GTK_STOCK_OK);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(sys_ok_clicked),NULL);
	g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),window);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,0);
	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),window);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,0);
	gtk_widget_show_all(window);
	gtk_main();
}


/*传递聊天好友信息至聊天页面*/
void on_chat_clicked(GtkWidget *window, Person_chat *friend)//这里应该直接传进来friend指针
{
        create_chat_window(friend);
}

/*创建主页面*/
void create_mainpage_window(void)
{
	char *id="12";
        GtkWidget *window;
	GtkWidget *Addbutton;
	GtkWidget *PerImage;
	GtkWidget *PerInlabel1;
	GtkWidget *PerInlabel2;
	GtkWidget *PerInlabel3;
	GtkWidget* friend;

//主窗口：
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);/*生成主窗口*/
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);/*连接信号，关闭窗口*/
	gtk_window_set_title(GTK_WINDOW(window),"NeuTalk");/*设置主窗口标题*/
	gtk_container_set_border_width(GTK_CONTAINER(window),10);/*设置主窗口边框*/
	gtk_widget_set_size_request(window,400,800);/*设置主窗口初始化大小*/
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);/*设置主窗口初始位置*/


//固定布局容器:
	fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), fixed);


//用户个人头像：
        PerImage = gtk_image_new_from_file("qq.png");
        gtk_fixed_put(GTK_FIXED(fixed),PerImage,30,30);


//添加好友按钮：
	Addbutton = gtk_button_new_with_label ("+");
	g_signal_connect (G_OBJECT (Addbutton), "clicked", G_CALLBACK (AddFriends), (gpointer) window);
	gtk_fixed_put(GTK_FIXED(fixed),Addbutton, 320,120);


//系统消息按钮：
        Sysbutton = gtk_button_new_with_label ("!");
        gtk_widget_modify_fg(GTK_BIN(Sysbutton)->child, GTK_STATE_NORMAL, &red);
        g_signal_connect (G_OBJECT (Sysbutton), "clicked", G_CALLBACK (SysInf), (gpointer) window);
        gtk_fixed_put(GTK_FIXED(fixed),Sysbutton,350,120);


//用户信息   
        Person_chat users={"IP: ","ID: ","我的昵称: "};//便于显示
        strcat(users.IP,user.IP);
	strcat(users.username,user.username);
	strcat(users.ID,user.ID);
	PerInlabel1 = gtk_label_new(users.username);
	PerInlabel2 = gtk_label_new(users.ID);
	PerInlabel3 = gtk_label_new(users.IP);
	gtk_fixed_put(GTK_FIXED(fixed), PerInlabel1, 150,20);
	gtk_fixed_put(GTK_FIXED(fixed), PerInlabel2, 150,60);
	gtk_fixed_put(GTK_FIXED(fixed), PerInlabel3, 150,100);

	
//好友信息
	char path[100] = "/home/new/Desktop/neutalk/";
        strcat(path,user.ID);
        FILE *fp=fopen(strcat(path,"/friend.txt"),"r");
        char readline[1024];
	while(fscanf(fp,"%s",readline)!=-1)
	{
		if (count%2 == 0)
			strcpy(friend_list[count/2].ID, readline);
		else {
			strcpy(friend_list[count/2].username,readline);
			strcpy(friend_list[count/2].IP,"");
			friend = create_frdbutton(friend_list[count/2]); 
			int pos=200+(count-1)*60;
			gtk_fixed_put(GTK_FIXED(fixed), friend, 30,pos);
			gtk_widget_set_size_request(friend, 300, 100);
			Person_chat *one_friend;
			one_friend = malloc(sizeof(Person_chat));
			strcpy(one_friend->IP,friend_list[count/2].IP);
			strcpy(one_friend->username,friend_list[count/2].username);
			strcpy(one_friend->ID,friend_list[count/2].ID);
			g_signal_connect(G_OBJECT(friend),"clicked",G_CALLBACK(on_chat_clicked),one_friend);			
		}
                count++;
         }	
	fclose(fp);
		
	
        gtk_widget_show_all(window);
	gtk_main();
}
