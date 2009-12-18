/**
 * @file cy3240_private_types
 *
 * @brief Private type definitions for the CY3240 bridge controller
 *
 * Private type definitions for the CY3240 bridge controller
 *
 * @ingroup CY3240
 *
 * @owner  Kevin S Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin S Kirkup (kevin.kirkup@gmail.com)
 */
#ifndef INCLUSION_GUARD_CY3240_PRIVATE_TYPES_H
#define INCLUSION_GUARD_CY3240_PRIVATE_TYPES_H

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
/// @name Types
//@{

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

#endif // INCLUSION_GUARD_CY3240_PRIVATE_TYPES_H
