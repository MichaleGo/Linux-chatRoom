client:client.o login.o register.o mainpage.o chat.o chat_message.o thread_listen.o thread_send.o  GUI.o userfile.o
	gcc $^ -o $@ `pkg-config --cflags --libs gtk+-2.0`
%.o:%.c
	gcc -c $< -I../include -I`pkg-config --cflags --libs gtk+-2.0`
all:client
clean:all
	rm -f *.o

