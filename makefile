GTK_IFLAGS=$(shell pkg-config --cflags gtk4)
GTK_LFLAGS=$(shell pkg-config --libs gtk4)

main=main

all:$(main)
$(main): $(main).c
	gcc $(GTK_IFLAGS) -o $(main).exe $(main).c $(GTK_LFLAGS)

clean:
	del *.exe
