all:build

build:
	gcc -o UnixLs UnixLs.c list.o

run:
	./UnixLs
git:
	git add *
	git commit
	git push