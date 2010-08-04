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
#include "CppUTestExt/MockActualFunctionCall.h"
#include "CppUTestExt/MockExpectedFunctionsList.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"
#include "CppUTestExt/MockFailure.h"

MockActualFunctionCall::MockActualFunctionCall(MockFailureReporter* reporter, const MockExpectedFunctionsList& allExpectations)
	: reporter_(reporter), state_(CALL_SUCCEED), allExpectations_(allExpectations)
{
	unfulfilledExpectations_.addUnfilfilledExpectations(allExpectations);
}

MockActualFunctionCall::~MockActualFunctionCall()
{
}

Utest* MockActualFunctionCall::getTest() const
{
	return reporter_->getTestToFail();
}

void MockActualFunctionCall::failTest(const MockFailure& failure)
{
	setState(CALL_FAILED);
	reporter_->failTest(failure);
}

void MockActualFunctionCall::callHasSucceeded()
{
	setState(CALL_SUCCEED);
	unfulfilledExpectations_.removeOneFulfilledExpectation();
	unfulfilledExpectations_.resetExpectations();
}

MockFunctionCall& MockActualFunctionCall::withName(const SimpleString& name)
{
	setName(name);
	setState(CALL_IN_PROGESS);

	unfulfilledExpectations_.onlyKeepUnfulfilledExpectationsRelatedTo(name);
	if (unfulfilledExpectations_.isEmpty()) {
		MockUnexpectedCallHappenedFailure failure(getTest(), name, allExpectations_);
		failTest(failure);
		return *this;
	}

	unfulfilledExpectations_.callWasMade();

	if (unfulfilledExpectations_.hasFulfilledExpectations())
		callHasSucceeded();

	return *this;
}

void MockActualFunctionCall::checkActualParameter(const MockFunctionParameter& actualParameter)
{
	unfulfilledExpectations_.onlyKeepUnfulfilledExpectationsWithParameter(actualParameter);

	if (unfulfilledExpectations_.isEmpty()) {
		MockUnexpectedParameterFailure failure(getTest(), getName(), actualParameter, allExpectations_);
		failTest(failure);
		return;
	}

	unfulfilledExpectations_.parameterWasPassed(actualParameter.name_);
	if (unfulfilledExpectations_.hasFulfilledExpectations())
		callHasSucceeded();
}

MockFunctionCall& MockActualFunctionCall::withParameter(const SimpleString& name, int value)
{
	MockFunctionParameter actualParameter(name, "int");
	actualParameter.value_.intValue_ = value;
	checkActualParameter(actualParameter);
	return *this;
}

MockFunctionCall& MockActualFunctionCall::withParameter(const SimpleString& name, double value)
{
	MockFunctionParameter actualParameter(name, "double");
	actualParameter.value_.doubleValue_ = value;
	checkActualParameter(actualParameter);
	return *this;
}

MockFunctionCall& MockActualFunctionCall::withParameter(const SimpleString& name, const char* value)
{
	MockFunctionParameter actualParameter(name, "char*");
	actualParameter.value_.stringValue_ = value;
	checkActualParameter(actualParameter);
	return *this;
}

MockFunctionCall& MockActualFunctionCall::withParameter(const SimpleString& name, void* value)
{
	MockFunctionParameter actualParameter(name, "void*");
	actualParameter.value_.pointerValue_ = value;
	checkActualParameter(actualParameter);
	return *this;
}

MockFunctionCall& MockActualFunctionCall::withParameterOfType(const SimpleString& type, const SimpleString& name, void* value)
{
	if (getComparatorForType(type) == NULL) {
		MockNoWayToCompareCustomTypeFailure failure(getTest(), type);
		failTest(failure);
		return *this;
	}
	MockFunctionParameter actualParameter(name, type);
	actualParameter.value_.objectPointerValue_ = value;
	actualParameter.comparator_ = getComparatorForType(type);
	checkActualParameter(actualParameter);
	return *this;
}

bool MockActualFunctionCall::isFulfilled() const
{
	return state_ == CALL_SUCCEED;
}

bool MockActualFunctionCall::hasFailed() const
{
	return state_ == CALL_FAILED;
}

void MockActualFunctionCall::checkExpectations()
{
	if (state_ != CALL_IN_PROGESS) return;

	if (! unfulfilledExpectations_.hasUnfullfilledExpectations())
		FAIL("Actual call is in progress. Checking expectations. But no unfulfilled expectations. Cannot happen.")

	MockExpectedParameterDidntHappenFailure failure(getTest(), getName(), allExpectations_);
	failTest(failure);
}

void MockActualFunctionCall::checkStateConsistency(ActualCallState oldState, ActualCallState newState)
{
	if (oldState == newState)
		FAIL(StringFromFormat("State change to the same state: %d.", newState).asCharString());
	if (oldState == CALL_FAILED)
		FAIL("State was already failed. Cannot change state again.");
}

void MockActualFunctionCall::setState(ActualCallState state)
{
	checkStateConsistency(state_, state);
	state_ = state;
}
