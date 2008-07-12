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

#include "Platform.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/JUnitTestOutput.h"
#include "CppUTest/TestResult.h"
#include <stdio.h>

static long millisTime;

static char* theTime = "1978-10-03T00:00:00";

static long MockGetPlatformSpecificTimeInMillis()
{
		return millisTime;
}

static SimpleString MockGetPlatformSpecificTimeString()
{
		return theTime;
}


TEST_GROUP(JUnitOutputTest)
{
	class MockJUnitTestOutput : public JUnitTestOutput
	{
	public:
		enum {testGroupSize = 10};
		enum {defaultSize = 7};

		int filesOpened;
		int fileBalance;

		SimpleString fileName;
		SimpleString buffer;

		TestResult* res;
		struct TestData
		{
			TestData() : tst_(0), testName_(0), failure_(0){};
			Utest* tst_;
			SimpleString* testName_;
			Failure* failure_;
		};

		struct TestGroupData
		{
			TestGroupData() : numberTests_(0), totalFailures_(0), name_(""), testData_(0){};
			
			int numberTests_;
			int totalFailures_;
			SimpleString name_;

			TestData* testData_;
		};

		TestGroupData testGroupData_[testGroupSize];

		TestGroupData& currentGroup()
		{
			return testGroupData_[filesOpened-1];
		}
		
		void resetXmlFile()
		{
			buffer = "";
		}

		MockJUnitTestOutput() : filesOpened(0), fileBalance(0), res(0)
		{
			for (int i = 0; i < testGroupSize; i++) {
				testGroupData_[i].numberTests_ = 0;
				testGroupData_[i].totalFailures_ = 0;
			}
		};
		
		void setResult(TestResult* testRes)
		{
			res = testRes;
		}
		
		virtual ~MockJUnitTestOutput()
		{
			for (int i = 0; i < testGroupSize; i++) {
				for (int j = 0; j < testGroupData_[i].numberTests_; j++) {
					delete testGroupData_[i].testData_[j].tst_;
					delete testGroupData_[i].testData_[j].testName_;
					if (testGroupData_[i].testData_[j].failure_) delete testGroupData_[i].testData_[j].failure_;
				}
				if (testGroupData_[i].testData_) delete [] testGroupData_[i].testData_;
			}
			
			LONGS_EQUAL(0, fileBalance);
		}
		
		void writeToFile(const SimpleString& buf)
		{
			buffer += buf;
		}
		
		void openFileForWrite(const SimpleString& in_FileName)
		{
			filesOpened++;
			fileBalance++;
			fileName = in_FileName;
		}
		
		void closeFile()
		{
			CHECK_XML_FILE();
			resetXmlFile();
			fileBalance--;
		}

		void createTestsInGroup(int index, int amount, const char* group, const char* basename)
		{
			testGroupData_[index].name_ = group;
			testGroupData_[index].numberTests_ = amount;

			testGroupData_[index].testData_ = new TestData[amount];
			for (int i = 0; i < amount; i++) {
				TestData& testData = testGroupData_[index].testData_[i]; 
				testData.testName_ = new SimpleString(basename);
				*testData.testName_ += StringFrom((long)i);
				testData.tst_ = new Utest(group, testData.testName_->asCharString(), "file", 1);
			}
		}
		void runTests()
		{
			res->testsStarted();
			for (int i = 0; i < testGroupSize; i++) {
				TestGroupData& data = testGroupData_[i];
				if (data.numberTests_ == 0) continue; 
				
				millisTime = 0;
				res->currentGroupStarted(data.testData_[0].tst_);
				for (int i = 0; i < data.numberTests_; i++) {
					TestData& testData = data.testData_[i]; 
					
					millisTime = 0;
					res->currentTestStarted(testData.tst_);
					if (testData.failure_) 
						print(*testData.failure_);
					millisTime = 10;
					res->currentTestEnded(testData.tst_);
				}
				millisTime = 50;
				res->currentGroupEnded(data.testData_[0].tst_);
			}
			res->testsEnded();
		}
		
		void setFailure(int groupIndex, int testIndex, const char* fileName, int lineNumber, const char* message) 
		{
			TestData& data = testGroupData_[groupIndex].testData_[testIndex];
			data.failure_ = new Failure(data.tst_, fileName, lineNumber, message);
			testGroupData_[groupIndex].totalFailures_++;
		}
		
		void CHECK_HAS_XML_HEADER(SimpleString string)
		{
			STRCMP_EQUAL("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n", string.asCharString());
		}
	
		void CHECK_TEST_SUITE_START(SimpleString output)
		{
			TestGroupData& group = currentGroup();
			char buf[1024];
			sprintf(buf, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" tests=\"%d\" time=\"50.0\" timestamp=\"%s\">\n",
				group.totalFailures_, group.name_.asCharString(), group.numberTests_, theTime);
			STRCMP_EQUAL(buf, output.asCharString());
		}
	
		void CHECK_XML_FILE()
		{
			int totalSize =  currentGroup().numberTests_+ defaultSize + (currentGroup().totalFailures_ * 2);
			SimpleString* arr = 0;
			int size = buffer.split("\n", arr);
			CHECK(size >= totalSize);
			CHECK(arr != 0);
			CHECK_HAS_XML_HEADER(arr[0]);
			CHECK_TEST_SUITE_START(arr[1]);
			CHECK_PROPERTIES_START(arr[2]);
			CHECK_PROPERTIES_END(arr[3]);
			CHECK_TESTS(&arr[4]);
			CHECK_SYSTEM_OUT(arr[size-3]);
			CHECK_SYSTEM_ERR(arr[size-2]);
			CHECK_TEST_SUITE_END(arr[size-1]);
			delete [] arr;
		}
	
		void CHECK_PROPERTIES_START(const SimpleString& output)
		{
			STRCMP_EQUAL("<properties>\n", output.asCharString());
		}
		
		void CHECK_PROPERTIES_END(const SimpleString& output)
		{
			STRCMP_EQUAL("</properties>\n", output.asCharString());
		}
		
		void CHECK_SYSTEM_OUT(const SimpleString& output)
		{
			STRCMP_EQUAL("<system-out></system-out>\n", output.asCharString());
		}
		
		void CHECK_SYSTEM_ERR(const SimpleString& output)
		{
			STRCMP_EQUAL("<system-err></system-err>\n", output.asCharString());
		}
		
		void CHECK_TEST_SUITE_END(const SimpleString& output)
		{
			STRCMP_EQUAL("</testsuite>", output.asCharString());
		}
		void CHECK_TESTS(SimpleString* arr)
		{
			static char buf[1024];
			
			for (int index = 0, curTest = 0; curTest < currentGroup().numberTests_; curTest++, index++) {
				snprintf(buf, 1024, "<testcase classname=\"%s\" name=\"%s\" time=\"10.0\">\n", 
					currentGroup().name_.asCharString(), currentGroup().testData_[curTest].tst_->getName().asCharString()); 				
				STRCMP_EQUAL(buf, arr[index].asCharString());
				if (currentGroup().testData_[curTest].failure_) {
					CHECK_FAILURE(arr, index, curTest);
				}
				snprintf(buf, 1024, "</testcase>\n");
				STRCMP_EQUAL(buf, arr[++index].asCharString());
				
			}				
		}
		void CHECK_FAILURE(SimpleString* arr, int& i, int curTest)
		{
			Failure& f = *currentGroup().testData_[curTest].failure_;
			i++;
			static char buf[1024];
			SimpleString message = f.getMessage().asCharString();
			message.replace('"','\'');
			message.replace('<','[');
			message.replace('>',']');
			message.replace("\n","{newline}");
			snprintf(buf, 1024, "<failure message=\"%s:%d: %s\" type=\"AssertionFailedError\">\n", f.getFileName().asCharString(), f.getLineNumber(), message.asCharString()); 
			STRCMP_EQUAL(buf, arr[i].asCharString());
			i++;
			STRCMP_EQUAL("</failure>\n", arr[i].asCharString());
		}
	};
	
	MockJUnitTestOutput * output;
	TestResult *res;

	void setup()
	{
		output = new MockJUnitTestOutput();
		res = new TestResult(*output);
		output->setResult(res);
		SetPlatformSpecificTimeInMillisMethod(MockGetPlatformSpecificTimeInMillis);
		SetPlatformSpecificTimeStringMethod(MockGetPlatformSpecificTimeString);
	}
	void teardown()
	{
		delete output;
		delete res;
		SetPlatformSpecificTimeInMillisMethod(0);
		SetPlatformSpecificTimeStringMethod(0);
	}
	void runTests() 
	{
		output->printTestsStarted();
		output->runTests();
		output->printTestsEnded(*res);
	}		
};

TEST(JUnitOutputTest, oneTestInOneGroupAllPass)
{
	output->createTestsInGroup(0, 1, "group", "name"); 
	runTests();
	STRCMP_EQUAL("cpputest_group.xml", output->fileName.asCharString());
	LONGS_EQUAL(1, output->filesOpened);
}

TEST(JUnitOutputTest, fiveTestsInOneGroupAllPass)
{
	output->createTestsInGroup(0, 5, "group", "name"); 
	runTests();
}

TEST(JUnitOutputTest, multipleTestsInTwoGroupAllPass)
{
	output->createTestsInGroup(0, 3, "group", "name"); 
	output->createTestsInGroup(1, 8, "secondGroup", "secondName"); 
	runTests();
	LONGS_EQUAL(2, output->filesOpened);
}

TEST(JUnitOutputTest, oneTestInOneGroupFailed)
{
	output->createTestsInGroup(0, 1, "failedGroup", "failedName"); 
	output->setFailure(0, 0, "file", 1, "Test <\"just\"> failed");
	runTests();
}

TEST(JUnitOutputTest, fiveTestsInOneGroupAndThreeFail)
{
	output->printTestsStarted();
	output->createTestsInGroup(0, 5, "failedGroup", "failedName"); 
	output->setFailure(0, 0, "file", 1, "Test just failed");
	output->setFailure(0, 1, "file", 5, "Also failed");
	output->setFailure(0, 4, "file", 8, "And failed again");
	runTests();
}

TEST(JUnitOutputTest, fourGroupsAndSomePassAndSomeFail)
{
	output->printTestsStarted();
	output->createTestsInGroup(0, 5, "group1", "firstName"); 
	output->createTestsInGroup(1, 50, "group2", "secondName");
	output->createTestsInGroup(2, 3, "group3", "thirdName");
	output->createTestsInGroup(3, 5, "group4", "fourthName");
	
	output->setFailure(0, 0, "file", 1, "Test just failed");
	output->printTestsEnded(*res);
	runTests();
}

TEST(JUnitOutputTest, messageWithNewLine)
{
	output->createTestsInGroup(0, 1, "failedGroup", "failedName"); 
	output->setFailure(0, 0, "file", 1, "Test \n failed");
	runTests();
}
