/**
 * @file cy3240_types.h
 *
 * @brief CY3240 Type definitions
 *
 * 
 * @ingroup CY3240
 *
 * @owner  Kevin Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin Kirkup (kevin.kirkup@gmail.com)
 */
#ifndef INCLUSION_GUARD_CY3240_TYPES_H
#define INCLUSION_GUARD_CY3240_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif


//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include "hid.h"

//@} End of Includes


//////////////////////////////////////////////////////////////////////
/// @name Types
//@{

typedef unsigned char uint8;
typedef unsigned short uint16;

/**
 * Function pointer for the HID write function
 */
typedef hid_return
(*hid_write_fpt)(
          HIDInterface* const,
          unsigned int const,
          const char*,
          unsigned int const,
          unsigned int const
          );

/**
 * Function pointer for the HID read function
 */
typedef hid_return
(*hid_read_fpt)(
          HIDInterface* const,
          unsigned int const,
          char* const,
          unsigned int const,
          unsigned int const
          );

/**
 * Function pointer for the HID init function
 */
typedef hid_return
(*hid_init_fpt)(
          void
          );

/**
 * Common Error Codes
 */
typedef enum {
     CY3240_ERROR_OK,                 ///< Everything went OK
     CY3240_ERROR_HID,                ///< HID Error occurred
     CY3240_ERROR_TX,                 ///< Transmit Error occurred
     CY3240_ERROR_RX,                 ///< Receive Error occurred
     CY3240_ERROR_RECONFIG,           ///< Error during reconfigure
     CY3240_ERROR_INVALID_PARAMETERS, ///< Invalid parameters provided
     CY3240_ERROR_UNKNOWN             ///< Unknown Error
} Cy3240_Error_t;


/**
 * CY3240 I2C Clock Speeds
 */
typedef enum {
     CY3240_100kHz   = 0x00,                    ///< 100 kHz Clock
     CY3240_400kHz   = 0x04,                    ///< 400 kHz Clock
     CY3240_50kHz    = 0x08,                    ///< 50 kHz Clock
     CY3240_Reserved = 0x0C                     ///< Reserved
} Cy3240_I2C_ClockSpeed_t;

/**
 * CY3240 Bus configurations
 */
typedef enum {
     CY3240_BUS_I2C  = 0x00,                    ///< I2C Bus configuration
     CY3240_BUS_SPI  = 0x01,                    ///< SPI Bus configuration
     CY3240_BUS_UART = 0x02,                    ///< UART Bus configuration
     CY3240_BUS_LIN  = 0x03                     ///< LIN Bus configuration
} Cy3240_Bus_t;

/**
 * CY3240 Power configurations
 */
typedef enum {
     CY3240_POWER_EXTERNAL = 0x00,              ///< External Power
     CY3240_POWER_5V       = 0x01,              ///< 5V Power
     CY3240_POWER_3_3V     = 0x02               ///< 3.3V Power
} Cy3240_Power_t;

/**
 * CY3240 device state structure
 */
typedef struct {
     uint16 vendor_id;                          ///< Vendor ID
     uint16 product_id;                         ///< Product ID
     int iface_number;                          ///< The interface number
     int timeout;                               ///< USB Transfer timeout
     Cy3240_I2C_ClockSpeed_t clock;             ///< The clock speed
     Cy3240_Bus_t bus;                          ///< The bus configuration
     Cy3240_Power_t power;                      ///< The power configuration
     HIDInterface *pHid;                        ///< HID Interface
     hid_init_fpt init;                         ///< Pointer to the hid init function
     hid_write_fpt write;                       ///< Pointer to the hid write function
     hid_read_fpt read;                         ///< Pointer to the hid read function

} Cy3240_t;

//@} End of Types

#ifdef __cplusplus
}
#endif

#endif // INCLUSION_GUARD_CY3240_TYPES_H

