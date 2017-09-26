all: nrg2iso.c
	gcc nrg2iso.c -o nrg2iso

clean: 
	rm -f nrg2iso
	rm -f *~
