/**
 * @file unittest
 *
 * @brief Unit Test common data
 *
 * Unit Test common data
 *
 * @ingroup UnitTest
 *
 * @owner  Kevin S Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin S Kirkup (kevin.kirkup@gmail.com)
 */

//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include <string.h>
#include "unittest.h"
#include "cy3240_private_types.h"

//@} End of Includes

//////////////////////////////////////////////////////////////////////
/// @name Data
//@{

/**
 * The component that will be used for the unit tests.
 */
Cy3240_t* pMyData;

// The sending buffer
uint8_t SEND_BUFFER[SEND_BUFFER_SIZE] = {0};
uint8_t RECEIVE_BUFFER[RECEIVE_BUFFER_SIZE] = {0};

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
        )
{
    DBG(printf("Generic HID Init\n");)

    return HID_RET_SUCCESS;
}   /* -----  end of static function init_test  ----- */

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
        )
{
    DBG(printf("Generic HID Close\n");)

    return HID_RET_SUCCESS;
}   /* -----  end of static function init_test  ----- */

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
        )
{
    DBG(printf("Generic HID Write\n");)

    // Write the data to the send buffer
    memcpy(SEND_BUFFER, bytes, size);

    return HID_RET_SUCCESS;
}

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
        )
{
    DBG(printf("Generic HID Read\n");)

    // Copy the acknowledgments in the return buffer
    memcpy(bytes, RECEIVE_BUFFER, size);

    // Set the status byte to something unique
    bytes[0] = 0x07;

    return HID_RET_SUCCESS;
}

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
        )
{
    DBG(printf("Generic HID Cleanup\n");)

    return HID_RET_SUCCESS;
}

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
        )
{
    DBG(printf("Generic HID DeleteIf\n");)

    return;
}

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
        )
{
    DBG(printf("Generic HID Force Open\n");)

    return HID_RET_SUCCESS;
}

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
        )
{
    DBG(printf("Generic HID new Interface\n");)

    // Give a constant for the interface type to avoid memory leak
    return (HIDInterface*)0x01;
}

//@} End of Methods
