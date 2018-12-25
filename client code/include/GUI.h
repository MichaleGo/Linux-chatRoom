#ifndef GUI_H
#define GUI_H
/********************结构体部分********************/
/*登录结构体*/
typedef struct login_node{
    GtkWidget* entry1;
    GtkWidget* entry2;
}entry, loginEntry;

/*注册结构体*/
typedef struct register_node{
    char ID[20];
    GtkWidget* entry1;
    GtkWidget* entry2;
}entrys, regEntry;

/*聊天个人信息结构体*/
struct node_chat{
	char IP[20];
	char ID[20];
	char username[20];
};

typedef struct node_chat Person_chat;

/*注册个人信息结构体*/
struct node_reg{
	char password[20];
	char ID[20];
	char username[20];
};

typedef struct node_reg Person_reg;


/********************登录部分********************/

/*信息提示框*/
void create_message_dialog (char* message);

/*点击登录后功能待修改*/
void on_login_clicked (GtkWidget* button,GtkWidget* window);

/*点击注册后功能待修改*/
void on_start_register_clicked (GtkWidget* button,gpointer data);

/*创建登录窗口*/
GtkWidget* create_login_window(void); 

/********************注册部分********************/

/*点击注册后功能待修改*/
void on_finish_register_clicked (GtkWidget* button,regEntry* reg_entry);

/*创建注册窗口*/
void create_register_window(char ID[]);

/********************主页面部分********************/

/*输入ID号后，发送请求添加好友消息，功能待修改*/
void on_ok_clicked(GtkWidget *window, GtkWidget *entry_id);

/*添加好友界面*/
void AddFriends(void);

/*系统消息界面*/
void SysInf(void);

/*接收好友请求后*/
void sys_ok_clicked (GtkWidget *window, gpointer data);

/*创建好友按钮*/
GtkWidget* create_frdbutton(Person_chat per);

/*创建主页面*/
void create_mainpage_window(void);

/*进入聊天页面*/
void on_chat_clicked(GtkWidget *window, Person_chat *friends);

/********************聊天页面部分********************/

/*发送文件功能待修改*/
void on_send_file(GtkWidget *w,GtkFileSelection *fs);

/*点击文件选择按钮，创建文件选择窗口*/
void on_create_file_selection_window(GtkButton* button, gpointer data);

/*点击发送消息按钮*/
void on_send_message(GtkButton* button, Person_chat *friends);

/*接收消息待修改*/
void on_receive_message(void);

/*点击聊天记录按钮*/
void on_find_chat_log(GtkButton* button, Person_chat *friends);


/*创建聊天页面*/
void create_chat_window(Person_chat *friends);

/********************聊天记录部分********************/

/*创建聊天记录页面*/
void create_chat_message_window(Person_chat *friends);

/*获得聊天记录存储位置*/
char* get_path(char *ID1, char *ID2);

/********************文件操作部分********************/

/*用户注册时创建文件夹*/
void create_user_file(Person_reg newuser);

/*写入用户聊天记录*/
void write_chat_message(char *userID,char *friendID, char *chattime, char *chat_content, int type);

/*添加新好友*/
void add_new_friend(char *userID, char *friendID);

#endif/*GUI_H*/

