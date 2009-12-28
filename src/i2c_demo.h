/**
 * @file i2c_demo.h
 *
 * @brief The Cypress CY3240 Demo board
 *
 * The Cypress CY3240 Demo board
 *
 * @ingroup Demo board
 *
 * @owner  Kevin Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin Kirkup (kevin.kirkup@gmail.com)
 */
#ifndef INCLUSION_GUARD_I2C_DEMO_H
#define INCLUSION_GUARD_I2C_DEMO_H

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////
/// @name Defines
//@{

/* Define the data byte locations for the example device */
#define UNK_0          (0)
#define UNK_1          (1)
#define UNK_2          (2)
#define UNK_3          (3)
#define UNK_4          (4)
#define LED_NUMBER     (5)
#define BRIGHTNESS     (6)
#define UNK_7          (7)

/* Define the device addresses used */
#define CONTROL_0      (0x80)
#define CONTROL_1      (0x81)
#define PSOC           (0x00) /* example device shipped in dev kit */

//@} End of Defines

#ifdef __cplusplus
}
#endif

#endif // INCLUSION_GUARD_I2C_DEMO_H
