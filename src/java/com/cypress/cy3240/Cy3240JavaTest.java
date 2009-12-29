/**
 * @file
 * @author Kevin Kirkup (kevin.kirkup@sonyericsson.com)
 */

package com.cypress.cy3240;

/**
 * 
 */
public class Cy3240JavaTest {

    // The log tag
    private static final String TAG = "Cy3240JavaTest";

    /**
     * Main entry point
     * 
     * @param args
     */
    public static void main(String[] args) {

        final int BLINK_DELAY = 500;

        Cy3240BridgeController c = new Cy3240BridgeController();

        // Create the device
        c.create(0, 1000, Cy3240PowerMode.CY3240_POWER_5V, Cy3240BusMode.CY3240_BUS_I2C,
                Cy3240ClockSpeed.CY3240_CLOCK_100kHz);

        // Open usb
        c.open();

        // Create the data array
        byte[] data = new byte[8];

        // Configure the Bridge controller
        c.reconfigure(Cy3240PowerMode.CY3240_POWER_5V, Cy3240BusMode.CY3240_BUS_I2C,
                Cy3240ClockSpeed.CY3240_CLOCK_100kHz);

        try {

            Thread.sleep(Cy3240BridgeController.SLEEP_BETWEEN_CMD);

            // Blink the first led
            for (int retry = 0; retry < 10; retry++) {

                data[5] = 0x01;
                data[6] = (byte)~data[6];
                data[7] = 0x01;

                c.write((byte)0x00, data);

                Thread.sleep(BLINK_DELAY);

            }
            
            // Blink the first led
            for (int retry = 0; retry < 10; retry++) {

                data[5] = 0x02;
                data[6] = (byte)~data[6];
                data[7] = 0x01;

                c.write((byte)0x00, data);

                Thread.sleep(BLINK_DELAY);

            }

        } catch (InterruptedException e) {

            // Print the stack
            e.printStackTrace();
        }

        // Configure the Bridge controller
        c.reconfigure(Cy3240PowerMode.CY3240_POWER_EXTERNAL, Cy3240BusMode.CY3240_BUS_I2C,
                Cy3240ClockSpeed.CY3240_CLOCK_100kHz);
    }
}
