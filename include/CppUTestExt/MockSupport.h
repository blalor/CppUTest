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

#ifndef D_MockSupport_h
#define D_MockSupport_h

#include "CppUTestExt/MockFailure.h"
#include "CppUTestExt/MockFunctionCall.h"
#include "CppUTestExt/MockExpectedFunctionCall.h"
#include "CppUTestExt/MockExpectedFunctionsList.h"

class Utest;

class MockSupport
{
public:
	MockSupport();
	virtual ~MockSupport();

	virtual MockFunctionCall& expectOneCall(const SimpleString& functionName);
	virtual MockFunctionCall& actualCall(const SimpleString& functionName);

	virtual void disable();
	virtual void enable();
	virtual void ignoreOtherCalls();

	virtual void clearExpectations();
	virtual bool expectedCallsLeft();

	virtual void checkExpectations();

	virtual void setMockFailureReporter(MockFailureReporter* reporter);
	virtual void installComparator(const SimpleString& typeName, MockNamedValueComparator& comparator);
	virtual void removeAllComparators();

	virtual void crashOnFailure();

	bool hasData(const SimpleString& name);
	void setData(const SimpleString& name, int value);
	MockNamedValue getData(const SimpleString& name);
protected:
	virtual MockActualFunctionCall* createActualFunctionCall();

private:
	MockFailureReporter* reporter_;
	MockFailureReporter defaultReporter_;

	MockExpectedFunctionsList expectations_;
	bool ignoreOtherCalls_;
	bool enabled_;
	MockActualFunctionCall* lastActualFunctionCall_;
	MockNamedValueComparatorRepository comparatorRepository_;

	MockNamedValue* data_;
};

#endif

