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

#include "unittest.h"
#include "cy3240_private_types.h"

//@} End of Includes

//////////////////////////////////////////////////////////////////////
/// @name Data
//@{

/**
 * The component that will be used for the unit tests.
 */
Cy3240_t myData;

// The sending buffer
uint8 SEND_BUFFER[SEND_BUFFER_SIZE] = {0};
uint8 RECEIVE_BUFFER[SEND_BUFFER_SIZE] = {0};

//@} End of Data
