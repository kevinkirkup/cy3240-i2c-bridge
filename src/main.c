/**
 * @file main.c
 *
 * @brief Main Entry point for the CY3240 i2c test
 *
 * Main Entry point for the CY3240 i2c test
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
#include <string.h>
#include <unistd.h> /* for getopt() */
#include <time.h>
#include <hid.h>
#include "cy3240.h"
#include "cy3240_util.h"
#include "i2c_demo.h"

//@} End of Includes

//////////////////////////////////////////////////////////////////////
/// @name Defines
//@{

#define SLEEP_BETWEEN_CMD   (250000)
#define BLINK_DELAY         (500000)

//@} End of Defines


//////////////////////////////////////////////////////////////////////
/// @name Private Methods
//@{

//-----------------------------------------------------------------------------
/**
 *  Method to parse the command line arguments
 *  Currently, we only accept the "-d" flag, which works like "lsusb", and the
 *  "-i" flag to select the interface (default 0). The syntax is one of the
 *  following:
 *
 *  $ test_libhid -d 1234:
 *  $ test_libhid -d :5678
 *  $ test_libhid -d 1234:5678
 *
 *  Product and vendor IDs are assumed to be in hexadecimal.
 *
 *  TODO: error checking and reporting.
 *
 *  @param argc       [in]  The number of arguments
 *  @param argv       [in]  The command line arguments
 *  @param vendor_id  [out] The vendor id
 *  @param product_id [out] The product id
 */
//-----------------------------------------------------------------------------
static void
parse_arguments (
          int argc,
          char *argv[],
          uint16 *vendor_id,
          uint16 *product_id,
          int *iface_num
          )
{
     char *vendor;
     char *product;
     int flag;

     // Iterate through all of the command line arguments
     while((flag = getopt(argc, argv, "d:i:")) != -1) {

          // Check the current argument
          switch (flag) {

               // The usb device id
               case 'd':
                    product = optarg;
                    vendor = strsep(&product, ":");

                    if(vendor && *vendor) {
                         vendor_id = strtol(vendor, NULL, 16);
                    }
                    if(product && *product) {
                         product_id = strtol(product, NULL, 16);
                    }
                    break;

               // The usb interface
               case 'i':
                    iface_num = atoi(optarg);
                    break;
          }
     }

}   /* -----  end of static function parse_arguments  ----- */


//-----------------------------------------------------------------------------
/**
 *  Method print out the specified buffer
 *
 *  @param buffer [in] the data buffer to print
 *  @praam length [in] the length of the buffer
 */
//-----------------------------------------------------------------------------
void
print_buffer(
          const uint8* const buffer,
          uint16 length
          )
{
     int count;

     printf("\nBuffer: (Length=%i)", length);

     for (count = 0; count < length; count ++) {

          if(count % 16 == 0) {
               printf("\n0%d: ", (int)(count / 16));
          }

          if(count % 4 == 0) {
               printf(" %02x",buffer[count]);

          } else {
               printf("%02x",buffer[count]);
          }
     }

     printf("\n");
}


//@} End of Private Methods


//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

//-----------------------------------------------------------------------------
/**
 *  Main entry point
 *
 *  @param argc [in] The number of arguments
 *  @param argv [in] The command line arguments
 *  @returns The result
 */
//-----------------------------------------------------------------------------
int
main(
    int argc,
    char *argv[]
    )
{
     int iface_num = 0;
     int retry;

     hid_return ret;
     Cy3240_t cy3240 = {0};

     uint8 data[8] = {0};
     uint16 length = 0;

     uint16 vendor_id = CY3240_VID;
     uint16 product_id = CY3240_PID;

     // Parse the command line arguments
     parse_arguments(argc, argv, vendor_id, product_id, &iface_num);

     fprintf(stderr, "VendorID: 0x%04x, ProductID: 0x%04x, Interface: %i\n", vendor_id, product_id, iface_num);

     // Initialize the device
     cy3240.vendor_id = vendor_id;
     cy3240.product_id = product_id;
     cy3240.timeout = 1000;
     cy3240.power = CY3240_POWER_5V;
     cy3240.bus = CY3240_BUS_I2C;
     cy3240.clock = CY3240_100kHz;

     // Open the device
     cy3240_open(iface_num, &cy3240);

     /* Read the configuration from an undefined address
      * I think this is M8C_SetBank1
      */
     length = sizeof(data);
     cy3240_read(
               &cy3240,
               CONTROL_1,
               data,
               &length);

     print_buffer(&data, length);

     usleep(SLEEP_BETWEEN_CMD);

     // Configure the bridge controller
     cy3240_reconfigure(&cy3240, false, false);

     usleep(SLEEP_BETWEEN_CMD);

     /* blink LED 1 */
     for (retry = 0; retry < 10; retry++) {

          data[LED_NUMBER] = 0x01;
          data[BRIGHTNESS] = ~data[BRIGHTNESS];
          data[UNK_7] = 0x01;

          length = sizeof(data);

          cy3240_write(&cy3240, PSOC, data, &length);


          usleep(BLINK_DELAY);
     }

     /* blink LED 2 */
     for (retry = 0; retry < 10; retry++) {

          data[LED_NUMBER] = 0x02;
          data[BRIGHTNESS] = ~data[BRIGHTNESS];
          data[UNK_7] = 0x01;

          length = sizeof(data);

          cy3240_write(&cy3240, PSOC, data, &length);

          usleep(BLINK_DELAY);

     }

     /* Turn off debug messages */
     hid_set_debug(HID_DEBUG_NONE);

     cy3240.power = CY3240_POWER_EXTERNAL;

     // Reconfigure the bridge controller
     cy3240_reconfigure(&cy3240, true, false);

     usleep(SLEEP_BETWEEN_CMD);

     // Close the device
     cy3240_close(&cy3240);

     return 0;
}

//@} End of Methods
