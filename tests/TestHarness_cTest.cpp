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

extern "C" {
	#include <stdlib.h>
	#include "TestHarness_c.h"

}
#include "TestHarness.h"
#include "MockTestOutput.h"
#include "GenericTest.h"

TEST_GROUP(TestHarness_c)
{
	GenericTestFixture* fixture;
	TEST_SETUP()
	{
		fixture = new GenericTestFixture();
	}
	TEST_TEARDOWN()
	{
		delete fixture;
	}
};

void _failIntMethod()
{
	CHECK_EQUAL_C_INT(1, 2);
}

TEST(TestHarness_c, checkInt)
{
	CHECK_EQUAL_C_INT(2, 2);
	fixture->setTestFunction(_failIntMethod);
	fixture->runAllTests();
	fixture->assertPrintContains("expected <1>\n	but was  <2>");
	fixture->assertPrintContains("arness_c");
//	fixture->assertPrintContains("TestHarness_cTest.cpp");;  //becuase of MS lowercasing the files
}

void _failRealMethod()
{
	CHECK_EQUAL_C_REAL(1.0, 2.0, 0.5);
}

TEST(TestHarness_c, checkReal)
{
	CHECK_EQUAL_C_REAL(1.0, 1.1, 0.5);
	fixture->setTestFunction(_failRealMethod);
	fixture->runAllTests();
	fixture->assertPrintContains("expected <1.000000>\n	but was  <2.000000>");
	fixture->assertPrintContains("arness_c");
//	fixture->assertPrintContains("TestHarness_cTest.cpp");
}

void _failCharMethod()
{
	CHECK_EQUAL_C_CHAR('a', 'c');
}

TEST(TestHarness_c, checkChar)
{
	CHECK_EQUAL_C_CHAR('a', 'a');
	fixture->setTestFunction(_failCharMethod);
	fixture->runAllTests();
	fixture->assertPrintContains("expected <a>\n	but was  <c>");
	fixture->assertPrintContains("arness_c");
//	fixture->assertPrintContains("TestHarness_cTest.cpp");
}

void _failStringMethod()
{
	CHECK_EQUAL_C_STRING("Hello", "World");
}

TEST(TestHarness_c, checkString)
{
	CHECK_EQUAL_C_STRING("Hello", "Hello");
	fixture->setTestFunction(_failStringMethod);
	fixture->runAllTests();
	fixture->assertPrintContains("expected <Hello>\n	but was  <World>");
	fixture->assertPrintContains("arness_c");
//	fixture->assertPrintContains("TestHarness_cTest.cpp");
}

void _failTextMethod()
{
	FAIL_TEXT_C("Booo");
}

TEST(TestHarness_c, checkFailText)
{
	fixture->setTestFunction(_failTextMethod);
	fixture->runAllTests();
	fixture->assertPrintContains("Booo");
	fixture->assertPrintContains("arness_c");
//	fixture->assertPrintContains("TestHarness_cTest.cpp");
}

void _failMethod()
{
	FAIL_C();
}

TEST(TestHarness_c, checkFail)
{
	fixture->setTestFunction(_failMethod);
	fixture->runAllTests();
	LONGS_EQUAL(1, fixture->getFailureCount());
	fixture->assertPrintContains("arness_c");
//	fixture->assertPrintContains("TestHarness_cTest.cpp");
}

