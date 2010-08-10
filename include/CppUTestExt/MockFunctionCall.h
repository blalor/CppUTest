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

#ifndef D_MockFunctionCall_h
#define D_MockFunctionCall_h

/*
 * MockParameterComparator is an interface that needs to be used when creating Comparators.
 * This is needed when comparing values of non-native type.
 */

class MockNamedValueComparator
{
public:
	MockNamedValueComparator() {};
	virtual ~MockNamedValueComparator() {};

	virtual bool isEqual(void* object1, void* object2)=0;
	virtual SimpleString valueToString(void* object)=0;
};


/*
 * MockNamedValue is the generic value class used. It encapsulates basic types and can use them "as if one"
 * Also it enables other types by putting object pointers. They can be compared with comparators.
 *
 * Basically this class ties together a Name, a Value, a Type, and a Comparator
 */

class MockNamedValue
{
public:
	MockNamedValue(const SimpleString& name);
	virtual ~MockNamedValue();

	virtual void setValue(int value);
	virtual void setValue(double value);
	virtual void setValue(void* value);
	virtual void setValue(const char* value);
	virtual void setObjectPointer(const SimpleString& type, void* objectPtr);

	virtual void setComparator(MockNamedValueComparator* comparator);

	virtual bool equals(const MockNamedValue& p) const;

	virtual SimpleString toString() const;

	virtual SimpleString getName() const;
	virtual SimpleString getType() const;

	virtual MockParameterValue getValue();
private:
	SimpleString name_;
	SimpleString type_;
	union {
		int intValue_;
		double doubleValue_;
		const char* stringValue_;
		void* pointerValue_;
		void* objectPointerValue_;
	} value_;
	MockNamedValueComparator* comparator_;
};

/*
 * MockFunctionCall is the main interface for recording and actualizing mock calls.
 *
 */

class MockNamedValueComparatorRepository;
class MockFunctionCall
{
public:
	MockFunctionCall();
	virtual ~MockFunctionCall();

	virtual MockFunctionCall& withName(const SimpleString& name)=0;
	virtual MockFunctionCall& withParameter(const SimpleString& name, int value)=0;
	virtual MockFunctionCall& withParameter(const SimpleString& name, double value)=0;
	virtual MockFunctionCall& withParameter(const SimpleString& name, const char* value)=0;
	virtual MockFunctionCall& withParameter(const SimpleString& name, void* value)=0;
	virtual MockFunctionCall& withParameterOfType(const SimpleString& typeName, const SimpleString& name, void* value)=0;

	virtual void setComparatorRepository(MockNamedValueComparatorRepository* repository);

protected:
	void setName(const SimpleString& name);
	SimpleString getName() const;
	MockNamedValueComparator* getComparatorForType(const SimpleString& type) const;
private:
	SimpleString functionName_;
	MockNamedValueComparatorRepository* comparatorRepository_;
};

/*
 * MockParameterComparatorRepository is a class which stores comparators which can be used for comparing non-native types
 *
 */

struct MockNamedValueComparatorRepositoryNode;
class MockNamedValueComparatorRepository
{
	MockNamedValueComparatorRepositoryNode* head_;
public:
	MockNamedValueComparatorRepository();
	virtual ~MockNamedValueComparatorRepository();

	virtual void installComparator(const SimpleString& name, MockNamedValueComparator& comparator);
	virtual MockNamedValueComparator* getComparatorForType(const SimpleString& name);

	void clear();
};


class MockIgnoredCall : public MockFunctionCall
{
public:
	virtual MockFunctionCall& withName(const SimpleString&) { return *this;};
	virtual MockFunctionCall& withParameter(const SimpleString&, int) { return *this; };
	virtual MockFunctionCall& withParameter(const SimpleString&, double) { return *this; };
	virtual MockFunctionCall& withParameter(const SimpleString&, const char*) { return *this; };
	virtual MockFunctionCall& withParameter(const SimpleString& , void*) { return *this; };
	virtual MockFunctionCall& withParameterOfType(const SimpleString&, const SimpleString&, void*) { return *this; };

	static MockFunctionCall& instance() { static MockIgnoredCall call; return call; };
};

#endif
