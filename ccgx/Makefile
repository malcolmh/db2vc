all: db2vc

clean:
	rm -vf *.o db2vc

db2vc: db2vc.o dbcan.o
	$$CC -o db2vc db2vc.o dbcan.o \
`pkg-config --libs dbus-glib-1` \
`pkg-config --libs dbus-1` \
`pkg-config --libs glib-2.0` \
-L../../mndlib/ccgx/ -lmnd -lm

dbcan.o: ../src/dbcan.c ../src/dbcan.h ../src/dbcan.t
	$$CC -c -std=gnu11 \
`pkg-config --cflags dbus-glib-1` \
`pkg-config --cflags dbus-1` \
`pkg-config --cflags glib-2.0` \
../src/dbcan.c

db2vc.o: ../src/db2vc.c ../src/dbcan.h
	$$CC -c -std=gnu11 \
`pkg-config --cflags dbus-glib-1` \
`pkg-config --cflags dbus-1` \
`pkg-config --cflags glib-2.0` \
../src/db2vc.c
