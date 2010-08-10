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
#include "CppUTestExt/MockFailure.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"
#include "CppUTestExt/MockExpectedFunctionsList.h"


TEST_GROUP(MockFailureTest)
{
	MockFailureReporter reporter;

	MockExpectedFunctionsList *list;
	MockExpectedFunctionCall* call1;
	MockExpectedFunctionCall* call2;
	MockExpectedFunctionCall* call3;

	void setup ()
	{
		list = new MockExpectedFunctionsList;
		call1 = new MockExpectedFunctionCall;
		call2 = new MockExpectedFunctionCall;
		call3 = new MockExpectedFunctionCall;
	}
	void teardown ()
	{
		delete list;
		delete call1;
		delete call2;
		delete call3;
	}
	void addAllToList()
	{
		list->addExpectedCall(call1);
		list->addExpectedCall(call2);
		list->addExpectedCall(call3);
	}
};

TEST(MockFailureTest, noErrorFailureSomethingGoneWrong)
{
	MockFailure failure(this);
	STRCMP_EQUAL("Test failed with MockFailure without an error! Something went seriously wrong.", failure.getMessage().asCharString());
}

TEST(MockFailureTest, unexpectedCallHappened)
{
	MockUnexpectedCallHappenedFailure failure(this, "foobar", *list);
	STRCMP_EQUAL("Mock Failure: Unexpected call to function: foobar\n"
				 "\tEXPECTED calls that did NOT happen:\n"
				 "\t\t<none>\n"
				 "\tACTUAL calls that did happen:\n"
				 "\t\t<none>", failure.getMessage().asCharString());
}

TEST(MockFailureTest, expectedCallDidNotHappen)
{
	call1->withName("foobar");
	call2->withName("world").withParameter("boo", 2).withParameter("hello", "world");
	call3->withName("haphaphap");
	call3->callWasMade();
	addAllToList();

	MockExpectedCallsDidntHappenFailure failure(this, *list);
	STRCMP_EQUAL("Mock Failure: Expected call did not happen.\n"
				 "\tEXPECTED calls that did NOT happen:\n"
				 "\t\tfoobar -> no parameters\n"
				 "\t\tworld -> int boo: <2>, char* hello: <world>\n"
				 "\tACTUAL calls that did happen:\n"
				 "\t\thaphaphap -> no parameters", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedAdditionalCallFailure)
{
	call1->withName("bar");
	call1->callWasMade();
	list->addExpectedCall(call1);

	MockUnexpectedCallHappenedFailure failure(this, "bar", *list);
	STRCMP_CONTAINS("Mock Failure: Unexpected additional (2th) call to function: bar\n\tEXPECTED", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedParameterFailure)
{
	call1->withName("foo").withParameter("boo", 2);
	call2->withName("foo").withParameter("boo", 10);
	call3->withName("unrelated");
	addAllToList();

	MockNamedValue actualParameter("bar");
	actualParameter.setValue(2);

	MockUnexpectedParameterFailure failure(this, "foo", actualParameter, *list);
	STRCMP_EQUAL("Mock Failure: Unexpected parameter name to function \"foo\": bar\n"
			     "\tEXPECTED calls that DID NOT happen related to function: foo\n"
				 "\t\tfoo -> int boo: <2>\n"
				 "\t\tfoo -> int boo: <10>\n"
				 "\tACTUAL calls that DID happen related to function: foo\n"
				 "\t\t<none>\n"
			     "\tACTUAL unexpected parameter passed to function: foo\n"
			     "\t\tint bar: <2>", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockUnexpectedParameterValueFailure)
{
	call1->withName("foo").withParameter("boo", 2);
	call2->withName("foo").withParameter("boo", 10);
	call3->withName("unrelated");
	addAllToList();

	MockNamedValue actualParameter("boo");
	actualParameter.setValue(20);

	MockUnexpectedParameterFailure failure(this, "foo", actualParameter, *list);
	STRCMP_EQUAL("Mock Failure: Unexpected parameter value to parameter \"boo\" to function \"foo\": <20>\n"
			     "\tEXPECTED calls that DID NOT happen related to function: foo\n"
				 "\t\tfoo -> int boo: <2>\n"
				 "\t\tfoo -> int boo: <10>\n"
				 "\tACTUAL calls that DID happen related to function: foo\n"
				 "\t\t<none>\n"
			     "\tACTUAL unexpected parameter passed to function: foo\n"
			     "\t\tint boo: <20>", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockExpectedParameterDidntHappenFailure)
{
	call1->withName("foo").withParameter("bar", 2).withParameter("boo", "str");
	call2->withName("foo").withParameter("bar", 10).withParameter("boo", "bleh");
	call2->callWasMade();
	call2->parameterWasPassed("bar");
	call2->parameterWasPassed("boo");
	call3->withName("unrelated");
	addAllToList();

	MockExpectedParameterDidntHappenFailure failure(this, "foo", *list);
	STRCMP_EQUAL("Mock Failure: Expected parameter for function \"foo\" did not happen.\n"
			     "\tEXPECTED calls that DID NOT happen related to function: foo\n"
				 "\t\tfoo -> int bar: <2>, char* boo: <str>\n"
				 "\tACTUAL calls that DID happen related to function: foo\n"
				 "\t\tfoo -> int bar: <10>, char* boo: <bleh>\n"
				 "\tMISSING parameters that didn't happen:\n"
			     "\t\tint bar, char* boo", failure.getMessage().asCharString());
}

TEST(MockFailureTest, MockNoWayToCompareCustomTypeFailure)
{
	MockNoWayToCompareCustomTypeFailure failure(this, "myType");
	STRCMP_EQUAL("MockFailure: No way to compare type <myType>. Please install a ParameterTypeComparator.", failure.getMessage().asCharString());
}
