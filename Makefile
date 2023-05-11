all: main
# Call the makefile in project direcotory by cd'ing into and running make
main:
	cd project && make

format:
	cd project && make format
