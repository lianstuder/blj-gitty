make:
	sudo apt install libncurses-dev libncursesw5-dev
	gcc src/main.c -o gitty -lncurses -O3