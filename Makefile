nikita:	nikita-gtk.c
	$(CC) `pkg-config --cflags gtk+-3.0 libxml-2.0 libsoup-2.4` -o nikita nikita-gtk.c `pkg-config --libs gtk+-3.0 libxml-2.0 libsoup-2.4`
