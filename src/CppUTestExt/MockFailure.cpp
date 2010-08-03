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

void MockFailureReporter::failTest(const MockFailure& failure)
{
	getTestToFail()->getTestResult()->addFailure(failure);
	if (crashOnFailure_)
		((MockFailureReporter*) 0x1)->failTest(failure);

	getTestToFail()->exitCurrentTest();
}

Utest* MockFailureReporter::getTestToFail()
{
	return Utest::getCurrent();
}

MockFailure::MockFailure(Utest* test) : TestFailure(test, "Test failed with MockFailure without an error! Something went seriously wrong.")
{
}

void MockFailure::addExpectationsAndCallHistory(const MockExpectedFunctionsList& expectations)
{
	message_ += "\tEXPECTED calls that did NOT happen:\n";
	message_ += expectations.unfulfilledFunctionsToString();
	message_ += "\n\tACTUAL calls that did happen:\n";
	message_ += expectations.fulfilledFunctionsToString();
}


MockExpectedCallsDidntHappenFailure::MockExpectedCallsDidntHappenFailure(Utest* test, const MockExpectedFunctionsList& expectations) : MockFailure(test)
{
	message_ = "Mock Failure: Expected call did not happen.\n";
	addExpectationsAndCallHistory(expectations);
}

MockUnexpectedCallHappenedFailure::MockUnexpectedCallHappenedFailure(Utest* test, const SimpleString& name, const MockExpectedFunctionsList& expectations)  : MockFailure(test)
{
	int amountOfExpectations = expectations.amountOfExpectationsFor(name);
	if (amountOfExpectations)
		message_ = StringFromFormat("Mock Failure: Unexpected additional (%dth) call to function: ", amountOfExpectations+1);
	else
		message_ = "Mock Failure: Unexpected call to function: ";
	message_ += name;
	message_ += "\n";
	addExpectationsAndCallHistory(expectations);
}

MockUnexpectedParameterFailure::MockUnexpectedParameterFailure(Utest* test, const SimpleString& functionName, const MockFunctionParameter& parameter, const MockExpectedFunctionsList& expectations)  : MockFailure(test)
{
	MockExpectedFunctionsList expectationsForFunction;
	expectations.addExpectationsToList(&expectationsForFunction);
	expectationsForFunction.onlyKeepExpectationsRelatedTo(functionName);

	MockExpectedFunctionsList expectationsForFunctionWithParameterName;
	expectationsForFunction.addExpectationsToList(&expectationsForFunctionWithParameterName);
	expectationsForFunctionWithParameterName.onlyKeepExpectationsWithParameterName(parameter.name_);

	if (expectationsForFunctionWithParameterName.isEmpty()) {
		message_ = "Mock Failure: Unexpected parameter name to function \"";
		message_ += functionName;
		message_ += "\": ";
		message_ += parameter.name_;
	}
	else {
		message_ = "Mock Failure: Unexpected parameter value to parameter \"";
		message_ += parameter.name_;
		message_ += "\" to function \"";
		message_ += functionName;
		message_ += "\": <";
		message_ += StringFrom(parameter.type_, parameter.value_);
		message_ += ">";
	}

	message_ += "\n\tEXPECTED calls that DID NOT happen related to function: ";
	message_ += functionName;
	message_ += "\n";

	message_ += expectationsForFunction.unfulfilledFunctionsToString();

	message_ += "\n\tACTUAL calls that DID happen related to function: ";
	message_ += functionName;
	message_ += "\n";

	message_ += expectationsForFunction.fulfilledFunctionsToString();

	message_ += "\n\tACTUAL unexpected parameter passed to function: ";
	message_ += functionName;
	message_ += "\n";

	message_ += "\t\t       ";
	message_ += parameter.type_;
	message_ += " ";
	message_ += parameter.name_;
	message_ += ": <";
	message_ += StringFrom(parameter.type_, parameter.value_);
	message_ += ">";
}

MockExpectedParameterDidntHappenFailure::MockExpectedParameterDidntHappenFailure(Utest* test, const SimpleString& functionName, const MockExpectedFunctionsList& expectations) : MockFailure(test)
{
	MockExpectedFunctionsList expectationsForFunction;
	expectations.addExpectationsToList(&expectationsForFunction);
	expectationsForFunction.onlyKeepExpectationsRelatedTo(functionName);

	message_ = "Mock Failure: Expected parameter for function \"";
	message_ += functionName;
	message_ += "\" did not happen.\n";

	message_ += "\tEXPECTED calls that DID NOT happen related to function: ";
	message_ += functionName;
	message_ += "\n";

	message_ += expectationsForFunction.unfulfilledFunctionsToString();

	message_ += "\n\tACTUAL calls that DID happen related to function: ";
	message_ += functionName;
	message_ += "\n";

	message_ += expectationsForFunction.fulfilledFunctionsToString();

	message_ += "\n\tMISSING parameters that didn't happen:\n";
	message_ += "\t\t";
	message_ += expectationsForFunction.missingParametersToString();
#if 0
	message_ += "\t\t       ";
	message_ += parameter.type_;
	message_ += " ";
	message_ += parameter.name_;
	message_ += ": <";
	message_ += StringFrom(parameter.type_, parameter.value_);
	message_ += ">";
#endif
}

MockNoWayToCompareCustomTypeFailure::MockNoWayToCompareCustomTypeFailure(Utest* test, const SimpleString& typeName) : MockFailure(test)
{
	message_ = StringFromFormat("MockFailure: No way to compare type <%s>. Please install a ParameterTypeComparator.", typeName.asCharString());
}


