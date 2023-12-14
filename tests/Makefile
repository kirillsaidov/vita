IFLAGS := -I../inc
LFALGS := -L../lib -lvita -lm
CFLAGS := -g

ifeq ($(OS), Windows_NT)
	ifeq ($(FILE), $(filter $(FILE), test_sockets_server test_sockets_client))
		LFALGS += -lws2_32
	endif
else
	CFLAGS += -fno-omit-frame-pointer -fsanitize=address
endif

all:
	mkdir -p bin && gcc -o bin/$(FILE) src/$(FILE).c $(IFLAGS) $(LFALGS) $(CFLAGS) 
run:
	./bin/$(FILE)
clean:
	rm -rf bin
