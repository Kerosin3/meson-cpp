all: compile
 
.PHONY: setup build clean
 
setup:
	mkdir builddir
	meson setup builddir --buildtype=debug
 
build:
	meson compile -C builddir
 
clean::
	rm -rf ./builddir
