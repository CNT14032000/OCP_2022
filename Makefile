CFLAGS=-Wall -Wextra -Werror -O2
TARGETS= lab1cntN3250 lib1cntN3250.so lib_add.so

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -rf *.o $(TARGETS)

lab1cntN3250: lab1cntN3250.c plugin_api.h
	gcc $(CFLAGS) -o lab1cntN3250 lab1cntN3250.c -ldl

lib1cntN3250.so: lib1cntN3250.c plugin_api.h
	gcc $(CFLAGS) -shared -fPIC -o lib1cntN3250.so lib1cntN3250.c -ldl -lm

lib_add.so: lib_add.c plugin_api.h
	gcc $(CFLAGS) -shared -fPIC -o lib_add.so lib_add.c -ldl -lm

