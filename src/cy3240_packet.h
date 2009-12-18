/**
 * @file cy3240_packet.h
 *
 * @brief The packet structure for the CY3240
 *
 * The packet structure for the CY3240
 *
 * @ingroup CY3240
 *
 * @owner  Kevin Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin Kirkup (kevin.kirkup@gmail.com)
 */

#ifndef INCLUSION_GUARD_CY3240_PACKET_H
#define INCLUSION_GUARD_CY3240_PACKET_H

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////
/// @name Defines
//@{

/* Define all the control bits according to Cypress docs */
#define CONTROL_BYTE_I2C_WRITE   (0x00)
#define CONTROL_BYTE_I2C_READ    (0x01)
#define CONTROL_BYTE_START       (0x02)
#define CONTROL_BYTE_RESTART     (0x04)
#define CONTROL_BYTE_STOP        (0x08)
#define CONTROL_BYTE_REINIT      (0x10)
#define CONTROL_BYTE_RECONFIG    (0x20)

/* Define the "last packet" control bit for the length field */
#define LENGTH_BYTE_MORE_PACKETS (0x80)
#define LENGTH_BYTE_LAST_PACKET  (0x00)

/* Bus configuration */
#define CY3240_BUS_MASK          (0xC0)

/**
 * Acknowledgment byte
 */
#define TX_ACK                   (0x01)

/**
 * Common Packet attributes
 */
#define INPUT_PACKET_INDEX_CMD       (0)        ///< The index of the command byte of input packets
#define INPUT_PACKET_INDEX_LENGTH    (1)        ///< The index of the length byte of input packets
#define INPUT_PACKET_INDEX_ADDRESS   (2)        ///< The index of the address byte of input packets
#define OUTPUT_PACKET_INDEX_STATUS   (0)        ///< The index of the status code in output packets
#define OUTPUT_PACKET_INDEX_DATA     (1)        ///< The index of the data for input packets

#define CY3240_STATUS_CODE_SIZE      (1)        ///< The length of the status code
#define CY3240_MAX_SIZE_PACKET       (64)
#define CY3240_MAX_WRITE_BYTES       (61)
#define CY3240_MAX_READ_BYTES        (61)

/**
 * Write packet parameters
 */
#define WRITE_INPUT_PACKET_INDEX_DATA      (3)  ///< The index of the data for write output packets

/**
 * Read packet parameters
 */
#define READ_INPUT_PACKET_SIZE             (3)

/**
 * The I2C address of the CY3240 control register
 */
#define CONTROL_I2C_ADDRESS (0x80)

/* Packets for the CY3240 are always 64 bytes */

//@} End of Defines

#ifdef __cplusplus
}
#endif

#endif // INCLUSION_GUARD_CY3240_PACKET_H
