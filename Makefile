export CPP_U_TEST = $(shell pwd)
export CPP_PLATFORM = Gcc
export APP_CXXFLAGS = -Wall

all: compile test

compile:
	$(MAKE) -C src/Platforms/Gcc all
	$(MAKE) -C src/CppUTest all
	$(MAKE) -C lib all
 
test: all
	$(MAKE) -C tests all test

example: all
	$(MAKE) -C examples all
 
clean:
	$(MAKE) -C src/Platforms/Gcc clean
	$(MAKE) -C src/CppUTest clean 
	$(MAKE) -C tests clean 
	$(MAKE) -C lib clean
	$(MAKE) -C examples clean
	
depend:
	$(MAKE) -C src/Platforms/Gcc depend
	$(MAKE) -C src depend
	