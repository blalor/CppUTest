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

#ifndef D_MockExpectedFunctionCall_h
#define D_MockExpectedFunctionCall_h

#include "CppUTestExt/MockFunctionCall.h"
#include "CppUTestExt/MockNamedValue.h"

extern SimpleString StringFrom(const MockNamedValue& parameter);

class MockExpectedFunctionCall : public MockFunctionCall
{

public:
	MockExpectedFunctionCall();
	virtual ~MockExpectedFunctionCall();

	virtual MockFunctionCall& withName(const SimpleString& name);
	virtual MockFunctionCall& withParameter(const SimpleString& name, int value);
	virtual MockFunctionCall& withParameter(const SimpleString& name, double value);
	virtual MockFunctionCall& withParameter(const SimpleString& name, const char* value);
	virtual MockFunctionCall& withParameter(const SimpleString& name, void* value);
	virtual MockFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, void* value);

	virtual MockNamedValue getParameter(const SimpleString& name);
	virtual SimpleString getParameterType(const SimpleString& name);
	virtual SimpleString getParameterValueString(const SimpleString& name);

	virtual bool hasParameterWithName(const SimpleString& name);
	virtual bool hasParameter(const MockNamedValue& parameter);
	virtual bool relatesTo(const SimpleString& functionName);

	virtual bool isFulfilled();
	virtual bool areParametersFulfilled();

	virtual void callWasMade();
	virtual void parameterWasPassed(const SimpleString& name);
	virtual void resetExpectation();

	virtual SimpleString callToString();
	virtual SimpleString missingParametersToString();

private:

	class MockExpectedFunctionParameter : public MockNamedValue
	{
	public:
		MockExpectedFunctionParameter(const SimpleString& name)
			: MockNamedValue(name), fulfilled_(false) {}

		void setFulfilled(bool b) { fulfilled_ = b; };
		bool isFulfilled() const { return fulfilled_;};
		bool fulfilled_;
	};

	struct MockFunctionParameterNode
	{
		MockFunctionParameterNode(MockExpectedFunctionParameter* data) : data_(data), next_(NULL) {}
		MockExpectedFunctionParameter* data_;
		MockFunctionParameterNode* next_;
	};

	MockExpectedFunctionParameter* toParameter(MockNamedValueListNode* node)
	{
		return (MockExpectedFunctionParameter*) node->itemToBeRenamed();
	}
	bool wasCallMade_;
//	MockFunctionParameterNode* parameters_;
	MockNamedValueList* parameters_;

	bool parametersEqual(const MockNamedValue& p1, const MockNamedValue& p2);
};

#endif
