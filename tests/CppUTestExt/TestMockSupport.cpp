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
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"
#include "CppUTestExt/MockFailure.h"
#include "TestMockFailure.h"

TEST_GROUP(MockSupportTest)
{
	MockSupport mock;

	void setup()
	{
		mock.setMockFailureReporter(MockFailureReporterForTest::getReporter());
	}

	void teardown()
	{
		CHECK_MOCK_NO_FAILURE_LEFT();
		mock.clearExpectations();
	}
};

TEST(MockSupportTest, clearExpectations)
{
	mock.expectOneCall("func");
	mock.clearExpectations();
	CHECK(! mock.expectedCallsLeft());
}

TEST(MockSupportTest, checkExpectationsDoesntFail)
{
	mock.checkExpectations();
}

TEST(MockSupportTest, checkExpectationsClearsTheExpectations)
{
	mock.expectOneCall("foobar");
	mock.checkExpectations();
	CHECK(! mock.expectedCallsLeft());
	CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN("foobar");
}

TEST(MockSupportTest, exceptACallThatHappens)
{
	mock.expectOneCall("func");
	mock.actualCall("func");
	CHECK(! mock.expectedCallsLeft());
}

TEST(MockSupportTest, exceptACallInceasesExpectedCallsLeft)
{
	mock.expectOneCall("func");
	CHECK(mock.expectedCallsLeft());
}

TEST(MockSupportTest, unexpectedCallHappened)
{
	mock.actualCall("func");
	CHECK_MOCK_FAILURE_EXPECTED_CALL_HAPPENED("func");
}

TEST(MockSupportTest, ignoreOtherCallsExceptForTheExpectedOne)
{
	mock.expectOneCall("foo");
	mock.ignoreOtherCalls();
	mock.actualCall("bar").withParameter("foo", 1);;
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, ignoreOtherStillFailsIfExpectedOneDidntHappen)
{
	mock.expectOneCall("foo");
	mock.ignoreOtherCalls();
	mock.checkExpectations();
	CHECK_MOCK_FAILURE_EXPECTED_CALL_DID_NOT_HAPPEN("foo");
}

TEST(MockSupportTest, expectMultipleCallsThatHappen)
{
	mock.expectOneCall("foo");
	mock.expectOneCall("foo");
	mock.actualCall("foo");
	mock.actualCall("foo");
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, expectOneCallHoweverMultipleHappened)
{
	mock.expectOneCall("foo");
	mock.expectOneCall("foo");
	mock.actualCall("foo");
	mock.actualCall("foo");
	mock.actualCall("foo");

	MockExpectedFunctionsList list;
	MockExpectedFunctionCall call1;
	MockExpectedFunctionCall call2;
	call1.withName("foo");
	call2.withName("foo");
	call1.callWasMade();
	call2.callWasMade();
	list.addExpectedCall(&call1);
	list.addExpectedCall(&call2);
	MockUnexpectedCallHappenedFailure expectedFailure(mockFailureTest(), "foo", list);
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndValue)
{
	mock.expectOneCall("foo").withParameter("parameter", 10);
	mock.actualCall("foo").withParameter("parameter", 10);
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, expectOneDoubleParameterAndValue)
{
	mock.expectOneCall("foo").withParameter("parameter", 1.0);
	mock.actualCall("foo").withParameter("parameter", 1.0);
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, expectOneStringParameterAndValue)
{
	mock.expectOneCall("foo").withParameter("parameter", "string");
	mock.actualCall("foo").withParameter("parameter", "string");
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, expectOnePointerParameterAndValue)
{
	mock.expectOneCall("foo").withParameter("parameter", (void*) 0x01);
	mock.actualCall("foo").withParameter("parameter", (void*) 0x01);
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, expectOneStringParameterAndValueFails)
{
	mock.expectOneCall("foo").withParameter("parameter", "string");
	mock.actualCall("foo").withParameter("parameter", "different");

	MockFunctionParameter parameter("parameter", "char*");
	parameter.value_.stringValue_ = "different";

	MockExpectedFunctionCall call;
	call.withName("foo").withParameter("parameter", "string");
	MockExpectedFunctionsList list;
	list.addExpectedCall(&call);

	MockUnexpectedParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, list);
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndFailsDueToParameterName)
{
	mock.expectOneCall("foo").withParameter("parameter", 10);
	mock.actualCall("foo").withParameter("different", 10);

	MockFunctionParameter parameter("different", "int");
	parameter.value_.intValue_ = 10;

	MockExpectedFunctionCall call;
	call.withName("foo").withParameter("parameter", 10);
	MockExpectedFunctionsList list;
	list.addExpectedCall(&call);

	MockUnexpectedParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, list);
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndFailsDueToValue)
{
	mock.expectOneCall("foo").withParameter("parameter", 10);
	mock.actualCall("foo").withParameter("parameter", 8);

	MockFunctionParameter parameter("parameter", "int");
	parameter.value_.intValue_ = 8;

	MockExpectedFunctionCall call;
	call.withName("foo").withParameter("parameter", 10);
	MockExpectedFunctionsList list;
	list.addExpectedCall(&call);

	MockUnexpectedParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, list);
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectOneIntegerParameterAndFailsDueToTypes)
{
	mock.expectOneCall("foo").withParameter("parameter", 10);
	mock.actualCall("foo").withParameter("parameter", "heh");

	MockFunctionParameter parameter("parameter", "char*");
	parameter.value_.stringValue_ = "heh";

	MockExpectedFunctionCall call;
	call.withName("foo").withParameter("parameter", 10);
	MockExpectedFunctionsList list;
	list.addExpectedCall(&call);

	MockUnexpectedParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, list);
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, expectMultipleCallsWithDifferentParametersThatHappenOutOfOrder)
{
	mock.expectOneCall("foo").withParameter("p1", 1);
	mock.expectOneCall("foo").withParameter("p1", 2);
	mock.actualCall("foo").withParameter("p1", 2);
	mock.actualCall("foo").withParameter("p1", 1);
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, expectMultipleCallsWithMultipleDifferentParametersThatHappenOutOfOrder)
{
	mock.expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 20);

	mock.actualCall("foo").withParameter("p1", 1).withParameter("p2", 20);
	mock.actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, twiceCalledWithSameParameters)
{
	mock.expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.actualCall("foo").withParameter("p1", 1).withParameter("p2", 2);
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

TEST(MockSupportTest, calledWithoutParameters)
{
	mock.expectOneCall("foo").withParameter("p1", 1);
	mock.actualCall("foo");
	mock.checkExpectations();

	MockExpectedFunctionsList list;
	MockExpectedFunctionCall call;
	call.withName("foo");
	call.withParameter("p1", 1);
	list.addExpectedCall(&call);
	MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", list);
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

}

TEST(MockSupportTest, newCallStartsWhileNotAllParametersWerePassed)
{
	mock.expectOneCall("foo").withParameter("p1", 1);
	mock.actualCall("foo");
	mock.actualCall("foo");

	MockExpectedFunctionsList list;
	MockExpectedFunctionCall call;
	call.withName("foo");
	call.withParameter("p1", 1);
	list.addExpectedCall(&call);
	MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", list);
	CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockSupportTest, threeExpectedAndActual)
{
	mock.expectOneCall("function1");
	mock.expectOneCall("function2");
	mock.expectOneCall("function3");
	mock.actualCall("function1");
	mock.actualCall("function2");
	mock.actualCall("function3");
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
}

class MyTypeForTesting
{
public:
	MyTypeForTesting(int val) : value(val){};
	int value;
};

class MyTypeForTestingComparator : public MockParameterComparator
{
public:
	virtual bool isEqual(void* object1, void* object2)
	{
		return ((MyTypeForTesting*)object1)->value == ((MyTypeForTesting*)object2)->value;
	}
	virtual SimpleString valueToString(void* object)
	{
		return StringFrom(((MyTypeForTesting*)object)->value);
	}

};


TEST(MockSupportTest, customObjectParameterFailsWhenNotHavingAComparisonRepository)
{
	MyTypeForTesting object(1);
	mock.expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
	mock.actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
	CHECK_MOCK_FAILURE_NO_WAY_TO_COMPARE_TYPE("MyTypeForTesting");
}

TEST(MockSupportTest, customObjectParameterSucceeds)
{
	MyTypeForTesting object(1);
	MyTypeForTestingComparator comparator;
	mock.installComparator("MyTypeForTesting", comparator);
	mock.expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
	mock.actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
	mock.checkExpectations();
	CHECK_MOCK_NO_FAILURE();
	mock.removeAllComparators();
}

//TEST(MockSupportTest, calledWithoutMultipleParameters)
//{
//	mock.expectOneCall("foo").withParameter("p1", 1).withParameter("p2", 2);
//	mock.actualCall("foo");
//	mock.checkExpectations();
//	CHECK_MOCK_FAILURE_EXPECTED_PARAMETER_BUT_DID_NOT_HAPPEN("foo", "p1", "1", "p2", "2");
//}
