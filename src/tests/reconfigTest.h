/** AceUnit test header file for fixture reconfigTest.
 *
 * You may wonder why this is a header file and yet generates program elements.
 * This allows you to declare test methods as static.
 *
 * @warning This is a generated file. Do not edit. Your changes will be lost.
 * @file reconfigTest.h
 */

#ifndef _RECONFIGTEST_H
/** Include shield to protect this header file from being included more than once. */
#define _RECONFIGTEST_H

/** The id of this fixture. */
#define A_FIXTURE_ID 7

#include "AceUnit.h"

/* The prototypes are here to be able to include this header file at the beginning of the test file instead of at the end. */
A_Test void testReconfigError(void);
A_Test void testReconfigPowerExternal(void);
A_Test void testReconfigPower5V(void);
A_Test void testReconfigPower3_3V(void);
A_Test void testReconfigClock100kHz(void);
A_Test void testReconfigClock400kHz(void);
A_Test void testReconfigClock50kHz(void);
A_Test void testReconfigClockReserved(void);
A_Test void testReconfigBusI2C(void);
A_Test void testReconfigBusSPI(void);
A_Test void testReconfigBusUART(void);
A_Test void testReconfigBusLIN(void);
A_Test void testReinitError(void);
A_Before void testReconfigSetup(void);
A_After void testReconfigCleanup(void);

/** The test case ids of this fixture. */
static const TestCaseId_t testIds[] = {
     8, /* testReconfigError */
     9, /* testReconfigPowerExternal */
    10, /* testReconfigPower5V */
    11, /* testReconfigPower3_3V */
    12, /* testReconfigClock100kHz */
    13, /* testReconfigClock400kHz */
    14, /* testReconfigClock50kHz */
    15, /* testReconfigClockReserved */
    16, /* testReconfigBusI2C */
    17, /* testReconfigBusSPI */
    18, /* testReconfigBusUART */
    19, /* testReconfigBusLIN */
    20, /* testReinitError */
};

#ifndef ACEUNIT_EMBEDDED
/** The test names of this fixture. */
static const char *const testNames[] = {
    "testReconfigError",
    "testReconfigPowerExternal",
    "testReconfigPower5V",
    "testReconfigPower3_3V",
    "testReconfigClock100kHz",
    "testReconfigClock400kHz",
    "testReconfigClock50kHz",
    "testReconfigClockReserved",
    "testReconfigBusI2C",
    "testReconfigBusSPI",
    "testReconfigBusUART",
    "testReconfigBusLIN",
    "testReinitError",
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
    1,
    1,
    1,
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
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
#endif

/** The test cases of this fixture. */
static const testMethod_t testCases[] = {
    testReconfigError,
    testReconfigPowerExternal,
    testReconfigPower5V,
    testReconfigPower3_3V,
    testReconfigClock100kHz,
    testReconfigClock400kHz,
    testReconfigClock50kHz,
    testReconfigClockReserved,
    testReconfigBusI2C,
    testReconfigBusSPI,
    testReconfigBusUART,
    testReconfigBusLIN,
    testReinitError,
    NULL
};

/** The before methods of this fixture. */
static const testMethod_t before[] = {
    testReconfigSetup,
    NULL
};

/** The after methods of this fixture. */
static const testMethod_t after[] = {
    testReconfigCleanup,
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
const TestFixture_t reconfigTestFixture = {
    7,
#ifndef ACEUNIT_EMBEDDED
    "reconfigTest",
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

#endif /* _RECONFIGTEST_H */
