DIR = ./src ./example
all:
	make all -C ./src
	make all -C ./example

clean:
	make clean -C ./src
	make clean -C ./example
	
tar:
	tar -cf event_plus.tar ./*
