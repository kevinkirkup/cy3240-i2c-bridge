/**
 * @file cy3240_util.c
 *
 * @brief Utility functions for the CY3240
 *
 * Utility functions for the CY3240
 *
 * @ingroup CY3240
 *
 * @owner  Kevin Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin Kirkup (kevin.kirkup@gmail.com)
 */

//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include <stdio.h>
#include "hid.h"
#include "cy3240_types.h"
#include "cy3240_util.h"

//@} End of Includes


//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

//-----------------------------------------------------------------------------
bool
cy3240_util_match_serial_number(
        struct usb_dev_handle* usbdev,
        void* custom,
        unsigned int len
        )
{
    bool ret;

    // Allocate a buffer to read the current usb device's serial number
    char* buffer = (char*)malloc (len);

    // Get the serial number of the specfied device
    usb_get_string_simple(
            usbdev,
            usb_device(usbdev)->descriptor.iSerialNumber,
            buffer,
            len);

    // Compare the current serial number with the one we are looking for
    ret = strncmp(buffer, (char*)custom, len) == 0;

    // Free the temporary buffer
    free(buffer);

    // Return the result of the compare
    return ret;
}

//@} End of Methods
