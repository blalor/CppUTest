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
#include "CppUTest/TestFailure.h"
#include "CppUTest/TestOutput.h"

TestFailure::TestFailure(Utest* test, const char* fileName, int lineNumber, const SimpleString& theMessage) :
	testName_(test->getFormattedName()), fileName_(fileName), lineNumber_(lineNumber), message_(theMessage)
{
}

TestFailure::TestFailure(Utest* test, const SimpleString& theMessage) :
	testName_(test->getFormattedName()), fileName_(test->getFile()), lineNumber_(test->getLineNumber()), message_(theMessage)
{
}

TestFailure::TestFailure(Utest* test, const char* fileName, int lineNum) :
	testName_(test->getFormattedName()), fileName_(fileName), lineNumber_(lineNum), message_("no message")
{
}

TestFailure::TestFailure(const TestFailure& f) :
	testName_(f.testName_), fileName_(f.fileName_), lineNumber_(f.lineNumber_), message_(f.message_)
{
}

TestFailure::~TestFailure()
{
}

SimpleString TestFailure::getFileName() const
{
	return fileName_;
}

SimpleString TestFailure::getTestName() const
{
	return testName_;
}

int TestFailure::getLineNumber() const
{
	return lineNumber_;
}

SimpleString TestFailure::getMessage() const
{
	return message_;
}

EqualsFailure::EqualsFailure(Utest* test, const char* fileName, int lineNumber, const SimpleString& expected, const SimpleString& actual) :
	TestFailure(test, fileName, lineNumber)
{

	const char* format = "expected <%s>\n\tbut was  <%s>";
	message_ = StringFromFormat(format, expected.asCharString(), actual.asCharString());
}

ContainsFailure::ContainsFailure(Utest* test, const char* fileName, int lineNumber, const SimpleString& expected, const SimpleString& actual) :
	TestFailure(test, fileName, lineNumber)
{

	const char* format = "actual <%s>\n\tdid not contain  <%s>";
	message_ = StringFromFormat(format, actual.asCharString(), expected.asCharString());
}
