
build:
	crystal build src/main.cr -o gitty

install:
	mv gitty /usr/local/bin

uninstall:
	rm -f /usr/local/bin/gitty