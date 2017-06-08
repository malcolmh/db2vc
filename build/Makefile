all: db2vc

clean:
	rm -vf *.o db2vc

db2vc: db2vc.o dbcan.o
	cc -o db2vc db2vc.o dbcan.o -lmnd \
`pkg-config --libs dbus-glib-1` \
`pkg-config --libs dbus-1` \
`pkg-config --libs glib-2.0` \
-L/usr/local/lib/ -lmnd

dbcan.o: ../src/dbcan.c ../src/dbcan.h
	cc -c -std=gnu11 \
`pkg-config --cflags dbus-glib-1` \
`pkg-config --cflags dbus-1` \
`pkg-config --cflags glib-2.0` \
../src/dbcan.c

db2vc.o: ../src/db2vc.c ../src/dbcan.h
	cc -c -std=gnu11 \
`pkg-config --cflags dbus-glib-1` \
`pkg-config --cflags dbus-1` \
`pkg-config --cflags glib-2.0` \
../src/db2vc.c
