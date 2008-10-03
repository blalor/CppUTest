/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef D_UTestMacros_h
#define D_UTestMacros_h

  /*! \brief Define a goup of tests
   *
   * All tests in a TEST_GROUP share the same setup
   * and teardown.  setup is run before the opening
   * curly brace of the test group and teardown is
   * called after the closing curly brace of the test group.
   *
   */

#define TEST_GROUP_BASE(testGroup, baseclass) \
  int externTestGroup##testGroup = 0; \
  struct CppUTestGroup##testGroup : public baseclass

#define TEST_BASE(testBaseClass) \
  struct testBaseClass : public Utest

#define TEST_GROUP(testGroup) \
  TEST_GROUP_BASE(testGroup, Utest)


#define TEST_SETUP() \
  virtual void setup()

#define TEST_TEARDOWN() \
  virtual void teardown()

#define TEST(testGroup, testName) \
  class testGroup##_##testName##_Test : public CppUTestGroup##testGroup \
{ public: testGroup##_##testName##_Test () : CppUTestGroup##testGroup () {} \
       void testBody(); } \
    testGroup##_##testName##_Instance; \
  TestInstaller testGroup##_##testName##_Installer(&testGroup##_##testName##_Instance, #testGroup, #testName, __FILE__,__LINE__); \
	void testGroup##_##testName##_Test::testBody()

#define IGNORE_TEST(testGroup, testName)\
  class testGroup##testName##Test : public CppUTestGroup##testGroup \
{ public: testGroup##testName##Test () : CppUTestGroup##testGroup () {} \
    virtual void run (TestResult& result) { \
    	result.countIgnored(); } \
    virtual const char* getProgressIndicator() const {return "!";} \
  protected:  virtual SimpleString getMacroName() const \
      { return "IGNORE_TEST"; } \
  public: void testBodyThatNeverRuns (); } \
    testGroup##testName##Instance; \
  TestInstaller testGroup##testName##Installer(&testGroup##testName##Instance, #testGroup, #testName, __FILE__,__LINE__); \
	void testGroup##testName##Test::testBodyThatNeverRuns ()

#define IMPORT_TEST_GROUP(testGroup) \
  extern int externTestGroup##testGroup;\
  int* p##testGroup = &externTestGroup##testGroup

//Check any boolean condition

#define CHECK_LOCATION(condition, conditionString, file, line)\
  {if (!Utest::getCurrent()->assertTrue(condition, conditionString, file, line)) return;}

#define CHECK(condition)\
  CHECK_LOCATION(condition, #condition, __FILE__, __LINE__)



//This check needs the operator!=(), and a StringFrom(YourType) function
#define CHECK_EQUAL(expected,actual)\
  CHECK_EQUAL_LOCATION(expected, actual, __FILE__, __LINE__)

#define CHECK_EQUAL_LOCATION(expected,actual, file, line)\
  if ((expected) != (actual))\
  {\
	 Utest::getTestResult()->countCheck();\
  	 EqualsFailure _f(Utest::getCurrent(), file, line, StringFrom(expected), StringFrom(actual)); \
     Utest::getTestResult()->addFailure(_f);\
     return;\
  }\
  else\
	 Utest::getTestResult()->countCheck();

//This check checks for char* string equality using strcmp.
//This makes up for the fact that CHECK_EQUAL only compares the pointers to char*'s
#define STRCMP_EQUAL(expected,actual)\
  STRCMP_EQUAL_LOCATION(expected, actual, __FILE__, __LINE__)

#define STRCMP_EQUAL_LOCATION(expected,actual, file, line)\
  {if (!Utest::getCurrent()->assertCstrEqual(expected, actual, file, line)) return;}

//Check two long integers for equality
#define LONGS_EQUAL(expected,actual)\
  LONGS_EQUAL_LOCATION(expected,actual,__FILE__, __LINE__)

#define LONGS_EQUAL_LOCATION(expected,actual,file,line)\
  { if (!Utest::getCurrent()->assertLongsEqual(expected, actual,  file, line)) return; }

#define BYTES_EQUAL(expected, actual)\
    LONGS_EQUAL((expected) & 0xff,(actual) & 0xff)

#define POINTERS_EQUAL(expected, actual)\
    POINTERS_EQUAL_LOCATION((expected),(actual), __FILE__, __LINE__)

#define POINTERS_EQUAL_LOCATION(expected,actual,file,line)\
  { if (!Utest::getCurrent()->assertPointersEqual(expected, actual,  file, line)) return; }



//Check two doubles for equality within a tolerance threshold
#define DOUBLES_EQUAL(expected,actual,threshold)\
  DOUBLES_EQUAL_LOCATION(expected,actual,threshold,__FILE__,__LINE__)

#define DOUBLES_EQUAL_LOCATION(expected,actual,threshold,file,line)\
  { if (!Utest::getCurrent()->assertDoublesEqual(expected, actual, threshold,  file, line)) return; }

//Fail if you get to this macro
//The macro FAIL may already be taken, so allow FAIL_TEST too
#ifndef FAIL
#define FAIL(text)\
  FAIL_LOCATION(text, __FILE__,__LINE__)

#define FAIL_LOCATION(text, file, line)\
  { Utest::getCurrent()->fail(text,  file, line); return; }
#endif

#define FAIL_TEST(text, file, line)\
  FAIL_TEST_LOCATION(text, __FILE__,__LINE__)

#define FAIL_TEST_LOCATION(text, file,line)\
  { Utest::getCurrent()->fail(text, file, line); return; }


#endif /*D_UTestMacros_h*/
