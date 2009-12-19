/**
 * @file unittest
 *
 * @brief Unit test common defines
 *
 * Unit test common defines
 *
 * @ingroup 
 *
 * @owner  Kevin S Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin S Kirkup (kevin.kirkup@gmail.com)
 */
#ifndef INCLUSION_GUARD_UNITTEST_H
#define INCLUSION_GUARD_UNITTEST_H

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include "config.h"
#include "cy3240.h"
#include "cy3240_types.h"
#include "cy3240_private_types.h"
#include "cy3240_util.h"
#include "cy3240_packet.h"
#include "AceUnitData.h"

//@} End of Includes

//////////////////////////////////////////////////////////////////////
/// @name Defines
//@{

#define MY_ADDRESS             (0)
#define SEND_BUFFER_SIZE       (128)
#define RECEIVE_BUFFER_SIZE    (128)

#if DEBUG
#define DBG(x) x

#else
#define DBG(x)

#endif // DEBUG

//@} End of Defines

//////////////////////////////////////////////////////////////////////
/// @name Data
//@{

/**
 * The component that will be used for the unit tests.
 */
extern Cy3240_t* pMyData;

// The sending buffer
extern uint8 SEND_BUFFER[SEND_BUFFER_SIZE];
extern uint8 RECEIVE_BUFFER[SEND_BUFFER_SIZE];

//@} End of Data

//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID init
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
hid_return
testGenericInit(
          void
          );

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID close
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
hid_return
testGenericClose(
          HIDInterface *const hidif
          );

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID read
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
hid_return
testGenericRead(
          HIDInterface* const hidif,
          unsigned int const ep,
          char* const bytes,
          unsigned int const size,
          unsigned int const timeout
          );

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID write
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
hid_return
testGenericWrite(
          HIDInterface* const hidif,
          unsigned int const ep,
          const char* bytes,
          unsigned int const size,
          unsigned int const timeout
          );

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID clean
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
hid_return
testGenericCleanup(
          void
          );

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID delete interface
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
void
testGenericDeleteIf(
          HIDInterface **const hidif
          );

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID Force Open
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
hid_return
testGenericForceOpen(
          HIDInterface *const hidif,
          int const interface,
          HIDInterfaceMatcher const *const matcher,
          unsigned short retries
          );

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the new HID Interface
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
HIDInterface *
testGenericNewHidInterface(
          void
          );

//@} End of Methods

#ifdef __cplusplus
}
#endif

#endif // INCLUSION_GUARD_UNITTEST_H
