#Set this to @ to keep the makefile quiet
SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = CppUTest
TARGET_LIB = \
	lib/lib$(COMPONENT_NAME).a
	
TEST_TARGET = \
	$(COMPONENT_NAME)_tests

#--- Inputs ----#
CPPUTEST_HOME = .
CPP_PLATFORM = Gcc

#CFLAGS are set to override malloc and free to get memory leak detection in C programs
#CFLAGS = -Dmalloc=cpputest_malloc -Dfree=cpputest_free
CPPFLAGS = 
#GCOVFLAGS = -fprofile-arcs -ftest-coverage

#SRC_DIRS is a list of source directories that make up the target library
#If test files are in these directories, their IMPORT_TEST_GROUPs need
#to be included in main to force them to be linked in.  By convention
#put them into an AllTests.h file in each directory
SRC_DIRS = \
	src/CppUTest \
	src/Platforms/$(CPP_PLATFORM)\
	src/CppUTest/Extensions

#TEST_SRC_DIRS is a list of directories including 
# - A test main (AllTests.cpp by conventin)
# - OBJ files in these directories are included in the TEST_TARGET
# - Consequently - AllTests.h containing the IMPORT_TEST_GROUPS is not needed
# - 
TEST_SRC_DIRS = \
	tests \
	tests/Extensions

#includes for all compiles	
INCLUDES =\
  -I.\
  -I$(CPPUTEST_HOME)/include\
  -I$(CPPUTEST_HOME)/include/Platforms/$(CPP_PLATFORM)\

#Flags to pass to ld
LDFLAGS +=
	
include $(CPPUTEST_HOME)/build/ComponentMakefile
