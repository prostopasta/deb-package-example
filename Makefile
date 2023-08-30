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
	@mkdir -p package/usr/bin/
	@gcc -o package/usr/bin/program src/program.c

.PHONY: dpkg-deb
dpkg-deb:
	@dpkg-deb --build package
