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
 *  Currently, we only accept the "-i" flag to select the interface (default 0).
 *  The syntax is one of the following:
 *
 *  $ test_libhid
 *  $ test_libhid -i 1
 *
 *  @param argc       [in]  The number of arguments
 *  @param argv       [in]  The command line arguments
 */
//-----------------------------------------------------------------------------
static void
parse_arguments (
          int argc,
          char *argv[],
          int *iface_num
          )
{
     char *vendor;
     char *product;
     int flag;

     // Iterate through all of the command line arguments
     while((flag = getopt(argc, argv, "i:")) != -1) {

          // Check the current argument
          switch (flag) {

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

     // Parse the command line arguments
     parse_arguments(argc, argv, &iface_num);

     fprintf(stderr, "Interface: %i\n", iface_num);

     // Initialize the device
     cy3240_util_factory(
               &cy3240,
               iface_num,
               1000,
               CY3240_POWER_5V,
               CY3240_BUS_I2C,
               CY3240_100kHz
               );

     // Open the device
     cy3240_open(&cy3240);

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
