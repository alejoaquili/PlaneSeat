all: clean comp
comp: 
	cd Database; make all
	cd Utils; make all
	cd Server; make all
	cd Client; make all

clean:
	cd Client; make clean
	cd Server; make clean
	cd Utils; make clean
	cd Database; make clean
	cd Test; make clean
	rm -rf planeSeat

test:
	cd Database; make all
	cd Utils; make all
	cd Test; make all
	
.PHONY: clean test all
