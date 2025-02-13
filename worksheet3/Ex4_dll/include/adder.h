// begin = adder.h ============

// The following include guard prevents `adder.h` from being included
// multiple times in the same source file. This avoids compilation issues
// in large projects where managing includes manually is difficult.
#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

// We need to prefix our function names with an additional keyword,
// which differs depending on the operating system and whether we are 
// using or creating the library.
//
// The variable `maths_EXPORTS` must be defined at build time if we are 
// building the library, but not if we are using it.

#if defined(WIN32) || defined(_WIN32)
    #if defined(maths_STATIC)
        // No additional keyword is needed for static libraries
        #define MATHSLIB_API
    #else
        #if defined(maths_EXPORTS)
            // Exporting symbols when building a DLL
            #define MATHSLIB_API __declspec(dllexport)
        #else
            // Importing symbols when using the DLL
            #define MATHSLIB_API __declspec(dllimport)
        #endif
    #endif
#else
    // No additional keyword is needed for non-Windows platforms
    #define MATHSLIB_API
#endif

// The above definitions will add one of:
// - `__declspec(dllexport)` when building the DLL
// - `__declspec(dllimport)` when using the DLL
// These are Microsoft-specific C/C++ extensions.

// Function prototype for `add`, including the additional keyword
MATHSLIB_API int add(int a, int b);

#endif // MATHSLIB_ADDER_H

// end = adder.h ============
