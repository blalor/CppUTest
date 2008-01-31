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

/******************************************************************************
 *
 * TESTHARNESS_c.H
 *
 * Provides an interface for when working with pure C
 *
 * Remember to use extern "C" when including in a cpp file!
 *
*******************************************************************************/
 
#ifndef D_TestHarness_c_h
#define D_TestHarness_c_h

#define CHECK_EQUAL_C_INT(expected,actual) \
  CHECK_EQUAL_C_INT_LOCATION(expected,actual,__FILE__,__LINE__)

#define CHECK_EQUAL_C_REAL(expected,actual,threshold) \
  CHECK_EQUAL_C_REAL_LOCATION(expected,actual,threshold,__FILE__,__LINE__)

#define CHECK_EQUAL_C_CHAR(expected,actual) \
  CHECK_EQUAL_C_CHAR_LOCATION(expected,actual,__FILE__,__LINE__)

#define CHECK_EQUAL_C_STRING(expected,actual) \
  CHECK_EQUAL_C_STRING_LOCATION(expected,actual,__FILE__,__LINE__)

#define FAIL_TEXT_C(text) \
  FAIL_TEXT_C_LOCATION(text,__FILE__,__LINE__)

#define FAIL_C() \
  FAIL_C_LOCATION(__FILE__,__LINE__)

/* CHECKS that can be used from C code */
extern void  CHECK_EQUAL_C_INT_LOCATION(int expected, int actual, const char* fileName, int lineNumber);
extern void  CHECK_EQUAL_C_REAL_LOCATION(double expected, double actual, double threshold, const char* fileName, int lineNumber);
extern void  CHECK_EQUAL_C_CHAR_LOCATION(char expected, char actual, const char* fileName, int lineNumber);
extern void  CHECK_EQUAL_C_STRING_LOCATION(char* expected, char* actual, const char* fileName, int lineNumber);
extern void  FAIL_TEXT_C_LOCATION(char* text, const char* fileName, int lineNumber);
extern void  FAIL_C_LOCATION(const char* fileName, int lineNumber);

/* Memory allocation routines that use new, so the memory leak checker works */

extern char* cpputest_malloc(unsigned int size);
extern void cpputest_free(char* buffer);
void cpputest_malloc_set_out_of_memory();
void cpputest_malloc_set_not_out_of_memory();

#endif
