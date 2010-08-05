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

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"

namespace
{
const int failLineNumber = 2;
const char* failFileName = "fail.cpp";
}

TEST_GROUP(TestFailure)
{
	Utest* test;
	StringBufferTestOutput* printer;

	void setup()
	{
		test = new NullTest();
		printer = new StringBufferTestOutput();
	}
	void teardown()
	{
		delete test;
		delete printer;
	}
	;
};
#define FAILURE_EQUAL(a, b) STRCMP_EQUAL_LOCATION(a, b.getMessage().asCharString(), __FILE__, __LINE__)

TEST(TestFailure, CreateFailure)
{
	TestFailure f1(test, failFileName, failLineNumber, "the failure message");
	TestFailure f2(test, "the failure message");
	TestFailure f3(test, failFileName, failLineNumber);
}

TEST(TestFailure, CreatePassingEqualsFailure)
{
	EqualsFailure f(test, failFileName, failLineNumber, "expected", "actual");
	FAILURE_EQUAL("expected <expected>\n\tbut was  <actual>", f);
}

TEST(TestFailure, CheckFailure)
{
	CheckFailure f(test, failFileName, failLineNumber, "chk");
	FAILURE_EQUAL("CHECK(chk) failed", f);
}

TEST(TestFailure, FailFailure)
{
	FailFailure f(test, failFileName, failLineNumber, "chk");
	FAILURE_EQUAL("chk", f);
}

TEST(TestFailure, LongsEqualFailure)
{
	LongsEqualFailure f(test, failFileName, failLineNumber, 1, 2);
	FAILURE_EQUAL("expected <1 0x1>\n\tbut was  <2 0x2>", f);
}

TEST(TestFailure, StringsEqualFailure)
{
	StringEqualFailure f(test, failFileName, failLineNumber, "abc", "abd");
	FAILURE_EQUAL("expected <abc>\n\tbut was  <ab<!>d>", f);
}

TEST(TestFailure, StringsEqualNoCaseFailure)
{
	StringEqualNoCaseFailure f(test, failFileName, failLineNumber, "ABC", "abd");
	FAILURE_EQUAL("expected <ABC>\n\tbut was  <ab<!>d>", f);
}

