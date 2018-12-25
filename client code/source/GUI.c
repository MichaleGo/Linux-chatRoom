#include <gtk/gtk.h>
#include <stdio.h>
#include "GUI.h"
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short int INT16U;
typedef signed short int INT16S;

/*GUI线程*/
void* thread_GUI(int argc,char **argv)
{
    GtkWidget *window;
    gtk_init(&argc,&argv);
    window = create_login_window();
    gtk_widget_show_all(window);
    gtk_main();

}
