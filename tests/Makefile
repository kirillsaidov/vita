all:
	mkdir -p bin && gcc -o bin/$(FILE) src/$(FILE).c -L../lib -lvita -g -lm
run:
	./bin/$(FILE)
clean:
	rm -rf bin
