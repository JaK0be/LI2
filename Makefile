CFLAGS=-ansi -Wall -Wextra -pedantic -O2
OBJS =$(patsubst %.c, %.o, $(wildcard *.c))
EXEC=dc
instalar: $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)
	sudo cp $(EXEC) /usr/lib/cgi-bin
	sudo cp -R images /var/www/html
	sudo mkdir -p /var/www/html/saves
	sudo chmod a+rw /var/www/html/saves
limpar:
	rm $(EXEC) *.o
doc:
	doxygen -g
	doxygen 


