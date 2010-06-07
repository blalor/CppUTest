
/*
 * This file can be used to get extra debugging information about memory leaks in your production code.
 * It defines a preprocessor macro for malloc. This will pass additional information to the
 * malloc and this will give the line/file information of the memory leaks in your code.
 *
 * You can use this by including this file to all your production code. When using gcc, you can use
 * the -include file to do this for you.
 *
 */

/* Warning for maintainers:
 * This macro code is duplicate from TestHarness.h. The reason for this is to make the two files
 * completely independent from each other. NewMacros file can be included in production code whereas
 * TestHarness.h is only included in test code.
 */

#ifndef CPPUTEST_USE_MEM_LEAK_DETECTION
#ifdef CPPUTEST_MEM_LEAK_DETECTION_DISABLED
#define CPPUTEST_USE_MEM_LEAK_DETECTION 0
#else
#define CPPUTEST_USE_MEM_LEAK_DETECTION 1
#endif
#endif

#if CPPUTEST_USE_MEM_LEAK_DETECTION
#define CPPUTEST_USE_MALLOC_MACROS 1

#include <stdlib.h>

extern void* cpputest_malloc_location(size_t size, const char* file, int line);
extern void cpputest_free_location(void* buffer, const char* file, int line);

#define malloc(a) cpputest_malloc_location(a, __FILE__, __LINE__)
#define calloc(a, b) cpputest_calloc_location(a, b, __FILE__, __LINE__)
#define realloc(a, b) cpputest_realloc_location(a, b, __FILE__, __LINE__)
#define free(a) cpputest_free_location(a, __FILE__, __LINE__)

#endif
