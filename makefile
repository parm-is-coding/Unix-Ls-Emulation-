all:build

build:
	gcc -o UnixLs UnixLs.c list.o

run: build
	./UnixLs arg1 arg2
git:
	git add *
	git commit
	git push