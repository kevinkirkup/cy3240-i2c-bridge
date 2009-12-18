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
extern Cy3240_t myData;

// The sending buffer
extern uint8 SEND_BUFFER[SEND_BUFFER_SIZE];
extern uint8 RECEIVE_BUFFER[SEND_BUFFER_SIZE];

//@} End of Data

#ifdef __cplusplus
}
#endif

#endif // INCLUSION_GUARD_UNITTEST_H
