/**
 * @file
 * @author Kevin Kirkup (kevin.kirkup@sonyericsson.com)
 */

package com.cypress.cy3240;

/**
 * The available bus modes for the CY3240 Bridge Controller
 */
public class Cy3240BusMode {

    public static final byte CY3240_BUS_I2C = 0x00;

    public static final byte CY3240_BUS_SPI = 0x01;

    public static final byte CY3240_BUS_UART = 0x02;

    public static final byte CY3240_BUS_LIN = 0x03;
}
