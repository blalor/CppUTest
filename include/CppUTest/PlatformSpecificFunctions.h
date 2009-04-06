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

#ifndef PLATFORMSPECIFICFUNCTIONS_H_
#define PLATFORMSPECIFICFUNCTIONS_H_

#include <stdarg.h>


/* Platform specific interface we use in order to minimize dependencies with LibC.
 * This enables porting to different embedded platforms.
 *
 * Lib C dependencies that are currently still left:
 *
 * stdarg.h -> We use formatting functions and va_list requires to include stdarg.h
 * stdlib.h -> The TestHarness_c.h includes this to try to avoid conflicts in its malloc #define. This dependency can
 * easily be removed by not enabling the MALLOC overrides.
 */


/* For test execution control (long_jmp set_jmp) */

/*  bool Utest::executePlatformSpecificSetup()
 *  void Utest::executePlatformSpecificTestBody()
 *  void Utest::executePlatformSpecificTeardown()
 *  void Utest::executePlatformSpecificRunOneTest(TestPlugin* plugin, TestResult& result)
 *  void Utest::executePlatformSpecificExitCurrentTest()
 */

/* Time operations */
long GetPlatformSpecificTimeInMillis();
void SetPlatformSpecificTimeInMillisMethod(long (*platformSpecific) ());

SimpleString GetPlatformSpecificTimeString();
void SetPlatformSpecificTimeStringMethod(SimpleString (*platformMethod) ());

/* String operations */
int PlatformSpecificAtoI(const char*str);
int PlatformSpecificStrLen(const char* str);
char* PlatformSpecificStrCat(char* s1, const char* s2);
char* PlatformSpecificStrCpy(char* s1, const char* s2);
char* PlatformSpecificStrNCpy(char* s1, const char* s2, unsigned int size);
int PlatformSpecificStrCmp(const char* s1, const char* s2);
int PlatformSpecificStrNCmp(const char* s1, const char* s2, unsigned int size);
char* PlatformSpecificStrStr(const char* s1, const char* s2);
int PlatformSpecificVSNprintf(char *str, unsigned int size, const char* format, va_list va_args_list);
int PlatformSpecificSprintf(char *str, unsigned int size, const char *format, ...);

/* Misc */
double PlatformSpecificFabs(double d);
int PlatformSpecificAtExit(void (*func) ());

/* IO operations */
typedef void* PlatformSpecificFile;

PlatformSpecificFile PlatformSpecificFOpen(const char* filename, const char* flag);
void PlatformSpecificFPuts(const char* str, PlatformSpecificFile file);
void PlatformSpecificFClose(PlatformSpecificFile file);

int PlatformSpecificPutchar(int c);
void PlatformSpecificFlush();

/* Dynamic Memory operations */
void* PlatformSpecificMalloc(unsigned int size);
void* PlatformSpecificRealloc (void* memory, unsigned int size);
void PlatformSpecificFree(void* memory);
void* PlatformSpecificMemCpy(void* s1, const void* s2, unsigned int size);

#endif
