/**
 * @file cy3240_util.h
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
#ifndef INCLUSION_GUARD_CY3240_UTIL_H
#define INCLUSION_GUARD_CY3240_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include "cy3240.h"
#include "cy3240_types.h"
#include "cy3240_util.h"

//@} End of Includes


//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

//-----------------------------------------------------------------------------
/**
 *  Method to check the serial number of the specified device
 *
 *  @param usbdev [in] The handle to the usb device
 *  @param custom [in] The serial number to check
 *  @param len    [in] The length of the custom serial number
 *  @returns true if the serial number matches the current device, otherwise, false
 */
//-----------------------------------------------------------------------------
bool
cy3240_util_match_serial_number(
    struct usb_dev_handle* usbdev,
    void* custom,
    unsigned int len
    );

//-----------------------------------------------------------------------------
/**
 *  Method to print the USB packet
 *
 *  @param buffer[in] the receive buffer to print
 *  @returns 0
 */
//-----------------------------------------------------------------------------
CY3240_Error_t
cy3240_util_print_packet(
    const uint8* const buffer,
    uint16 length
    );

//-----------------------------------------------------------------------------
/**
 *  Method to print out the sending packet to the USB device
 *
 *  @param buffer [in] The send buffer
 *  @returns 0
 */
//-----------------------------------------------------------------------------
CY3240_Error_t
cy3240_util_print_send_packet(
          const uint8* const buffer,
          uint16 length
          );

//-----------------------------------------------------------------------------
/**
 *  Method to print to information in the received control packet from the
 *  CY3240
 *
 *  @param packet [in] the control packet
 *
 *  @returns 0
 */
//-----------------------------------------------------------------------------
CY3240_Error_t
cy3240_util_print_receive_control_packet(
          const uint8* const packet,
          uint16 length
          );

//@} End of Methods

#ifdef __cplusplus
}
#endif

#endif // INCLUSION_GUARD_CY3240_UTIL_H

