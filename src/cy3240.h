/**
 * @file cs3240.h
 *
 * @brief Cypress CY3240 I2C to USB Bridge
 *
 * Define constant packets, addresses, lengths, etc. for writing to theDefine constant packets, addresses, lengths, etc. for writing to the
 * CY3240 USB HID to I2C converter
 * 
 * @ingroup CY3240
 *
 * @owner  Kevin Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin Kirkup (kevin.kirkup@gmail.com)
 */
#ifndef INCLUSION_GUARD_CS3240_H
#define INCLUSION_GUARD_CS3240_H

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include <hid.h>
#include "cy3240_types.h"

//@} End of Includes

//////////////////////////////////////////////////////////////////////
/// @name Defines
//@{

// The product and vendor id for the Cypress 3240 USB-I2C bridge
#define CY3240_VID (0x04B4)
#define CY3240_PID (0xF232)

/* Debug configuration */
#define DBG(X)  X

#define TRUE   (1)
#define FALSE  (0)

/* Result Macros */
#define CY3240_SUCCESS(s)  ((s == CY3240_ERROR_OK) ? TRUE : FALSE)
#define CY3240_FAILURE(s)  ((s != CY3240_ERROR_OK) ? TRUE : FALSE)

//@} End of Defines

//////////////////////////////////////////////////////////////////////
/// @name Data
//@{

#define SEND_PACKET_LEN (64)
#define RECV_PACKET_LEN (64)

// TODO: TEMPORARY Remove later
extern const int INPUT_ENDPOINT;
extern const int OUTPUT_ENDPOINT;

extern char SEND_PACKET[SEND_PACKET_LEN];
extern char RECV_PACKET[SEND_PACKET_LEN];
//@} End of Data
//@} End of Defines

//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

//-----------------------------------------------------------------------------
/**
 *  Method to write data to the CY3240
 *
 *  @param[in,out]
 *  @returns CY3240_Error_t
 */
//-----------------------------------------------------------------------------
CY3240_Error_t
cy3240_write(
          Cy3240_t* const pCy3240,
          uint8 address,
          const uint8* const pData,
          uint16* const pLength
          );

//-----------------------------------------------------------------------------
/**
 *  Method to read data from the CY3240
 *
 *  @param[in,out]
 *  @returns CY3240_Error_t
 */
//-----------------------------------------------------------------------------
CY3240_Error_t
cy3240_read(
          const Cy3240_t* const pCy3240,
          uint8 address,
          uint8* const pData,
          uint16* const pLength
          );

//-----------------------------------------------------------------------------
/**
 *  Method to open the CY3240
 *
 *  @param iface_num [in] the usb interface number
 *  @returns CY3240_Error_t
 */
//-----------------------------------------------------------------------------
CY3240_Error_t
cy3240_open(
          int iface_num,
          Cy3240_t* const pCy3240
          );

//-----------------------------------------------------------------------------
/**
 *  Method to close the CY3240 usb device
 *
 *  @param[in,out]
 *  @returns CY3240_Error_t
 */
//-----------------------------------------------------------------------------
CY3240_Error_t
cy3240_close(
          Cy3240_t* const pCy3240
          );

//@} End of Methods

#ifdef __cplusplus
}
#endif

#endif // INCLUSION_GUARD_CS3240_H
