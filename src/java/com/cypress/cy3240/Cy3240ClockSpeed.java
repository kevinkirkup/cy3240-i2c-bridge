/**
 * @file
 * @author Kevin Kirkup (kevin.kirkup@sonyericsson.com)
 */

package com.cypress.cy3240;

/**
 * The available clock rates for the CY3240 Bridge controller
 */
public class Cy3240ClockSpeed {

    public static final byte CY3240_CLOCK_100kHz = 0x00;

    public static final byte CY3240_CLOCK_400kHz = 0x04;

    public static final byte CY3240_CLOCK_50kHz = 0x08;

    public static final byte CY3240_CLOCK_Reserved = 0x0C;
}
