all: clean comp run
comp: 
	cd Client; make all
	cd Database; make all
	cd Utils; make all
	cd Server; make all

clean:
	cd Client; make clean
	cd Server; make clean
	cd Utils; make clean
	cd Database; make clean
	cd Test; make clean

run: 
	chmod 777 run.sh	
	./run.sh
test:
	cd Database; make all
	cd Utils; make all
	cd Test; make all
	
.PHONY: clean test all
