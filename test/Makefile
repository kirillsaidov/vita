all:
	mkdir -p bin && gcc -o bin/$(FILE) src/$(FILE).c -L../lib -lvita -O0 -g
run:
	./bin/$(BIN)
clean:
	rm -rf bin
