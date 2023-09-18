.ONESHELL:
SHELL = /bin/bash

default: build

.PHONY: deps
deps:
	@sudo apt install $(cat bindep.txt) -y

.PHONY: build
build: compile dpkg-deb

.PHONY: compile
compile:
	@mkdir -p myserver/usr/bin/
	@gcc -o myserver/usr/bin/myserver src/myserver.c

.PHONY: dpkg-deb
dpkg-deb:
	@dpkg-deb --build myserver
