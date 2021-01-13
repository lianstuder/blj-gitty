
build:
	crystal build src/main.cr -o gitty

clean:
	rm -f gitty

install:
	mv gitty /usr/local/bin

uninstall:
	rm -f /usr/local/bin/gitty