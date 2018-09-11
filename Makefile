all: nrg2iso

nrg2iso: nrg2iso.c
	gcc nrg2iso.c -o nrg2iso

clean:
	rm -f nrg2iso
	rm -f *~

install:
	cp nrg2iso /usr/local/bin

.PHONY: all clean install
