all:build

build:
	gcc -o UnixLs UnixLs.c

run:
	./UnixLs
git:
	git add *
	git commit
	git push