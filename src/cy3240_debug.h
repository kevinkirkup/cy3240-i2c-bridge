/**
 * @file cy3240_debug.h
 *
 * @brief CY3240 debug functions and macros
 *
 * 
 * @ingroup CY3240
 *
 * @owner  Kevin Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin Kirkup (kevin.kirkup@gmail.com)
 */
#ifndef INCLUSION_GUARD_CY3240_DEBUG_H
#define INCLUSION_GUARD_CY3240_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include <stdint.h>

//@} End of Includes

//////////////////////////////////////////////////////////////////////
/// @name Defines
//@{

#if DEBUG
#define CY3240_DEBUG_PRINT_PACKET     cy3240_debug_print_packet
#define CY3240_DEBUG_PRINT_TX_PACKET  cy3240_debug_print_send_packet
#define CY3240_DEBUG_PRINT_RX_PACKET  cy3240_debug_print_receive_control_packet

#else

#define CY3240_DEBUG_PRINT_PACKET
#define CY3240_DEBUG_PRINT_TX_PACKET
#define CY3240_DEBUG_PRINT_RX_PACKET

#endif // DEBUG

//@} End of Defines


//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

//-----------------------------------------------------------------------------
/**
 *  Method to print the USB packet
 *
 *  @param buffer[in] the receive buffer to print
 *  @returns 0
 */
//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_debug_print_packet(
        const uint8_t* const buffer,
        uint16_t length
        );

//-----------------------------------------------------------------------------
/**
 *  Method to print out the sending packet to the USB device
 *
 *  @param buffer [in] The send buffer
 *  @returns 0
 */
//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_debug_print_send_packet(
        const uint8_t* const buffer,
        uint16_t length
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
Cy3240_Error_t
cy3240_debug_print_receive_control_packet(
        const uint8_t* const packet,
        uint16_t length
        );

//@} End of Methods


#ifdef __cplusplus
}
#endif

#endif // INCLUSION_GUARD_CY3240_DEBUG_H

