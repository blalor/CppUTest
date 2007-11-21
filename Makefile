export CPP_U_TEST = $(shell pwd)
export CPP_PLATFORM = Gcc
export APP_CXXFLAGS = -Wall

default: testharness test

all: testharness example test

testharness:
	$(MAKE) -C src/Platforms/Gcc all
	$(MAKE) -C src/CppUTest all
	$(MAKE) -C lib all
 
test:
	$(MAKE) -s -C tests test

example: 
	$(MAKE) -C examples all
	$(MAKE) -s -C examples/AllTests test
 
clean:
	$(MAKE) -C src/Platforms/Gcc clean
	$(MAKE) -C src/CppUTest clean 
	$(MAKE) -C tests clean 
	$(MAKE) -C lib clean
	$(MAKE) -C examples clean
	
depend:
	$(MAKE) -C src/Platforms/Gcc depend
	$(MAKE) -C src/CppUTest depend 
	$(MAKE) -C tests depend 
	$(MAKE) -C examples depend
	