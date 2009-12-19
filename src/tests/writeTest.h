/** AceUnit test header file for fixture writeTest.
 *
 * You may wonder why this is a header file and yet generates program elements.
 * This allows you to declare test methods as static.
 *
 * @warning This is a generated file. Do not edit. Your changes will be lost.
 * @file writeTest.h
 */

#ifndef _WRITETEST_H
/** Include shield to protect this header file from being included more than once. */
#define _WRITETEST_H

/** The id of this fixture. */
#define A_FIXTURE_ID 21

#include "AceUnit.h"

/* The prototypes are here to be able to include this header file at the beginning of the test file instead of at the end. */
A_Test void testWriteError(void);
A_Test void testWriteSmall(void);
A_Test void testWriteMedium(void);
A_Test void testWriteLarge(void);
A_Test void testWriteNack(void);
A_Before void testWriteSetup(void);
A_After void testWriteCleanup(void);

/** The test case ids of this fixture. */
static const TestCaseId_t testIds[] = {
    22, /* testWriteError */
    23, /* testWriteSmall */
    24, /* testWriteMedium */
    25, /* testWriteLarge */
    26, /* testWriteNack */
};

#ifndef ACEUNIT_EMBEDDED
/** The test names of this fixture. */
static const char *const testNames[] = {
    "testWriteError",
    "testWriteSmall",
    "testWriteMedium",
    "testWriteLarge",
    "testWriteNack",
};
#endif

#ifdef ACEUNIT_LOOP
/** The loops of this fixture. */
static const aceunit_loop_t loops[] = {
    1,
    1,
    1,
    1,
    1,
};
#endif

#ifdef ACEUNIT_GROUP
/** The groups of this fixture. */
static const AceGroupId_t groups[] = {
    0,
    0,
    0,
    0,
    0,
};
#endif

/** The test cases of this fixture. */
static const testMethod_t testCases[] = {
    testWriteError,
    testWriteSmall,
    testWriteMedium,
    testWriteLarge,
    testWriteNack,
    NULL
};

/** The before methods of this fixture. */
static const testMethod_t before[] = {
    testWriteSetup,
    NULL
};

/** The after methods of this fixture. */
static const testMethod_t after[] = {
    testWriteCleanup,
    NULL
};

/** The beforeClass methods of this fixture. */
static const testMethod_t beforeClass[] = {
    NULL
};

/** The afterClass methods of this fixture. */
static const testMethod_t afterClass[] = {
    NULL
};

/** This fixture. */
#if defined __cplusplus
extern
#endif
const TestFixture_t writeTestFixture = {
    21,
#ifndef ACEUNIT_EMBEDDED
    "writeTest",
#endif
#ifdef ACEUNIT_SUITES
    NULL,
#endif
    testIds,
#ifndef ACEUNIT_EMBEDDED
    testNames,
#endif
#ifdef ACEUNIT_LOOP
    loops,
#endif
#ifdef ACEUNIT_GROUP
    groups,
#endif
    testCases,
    before,
    after,
    beforeClass,
    afterClass
};

#endif /* _WRITETEST_H */
