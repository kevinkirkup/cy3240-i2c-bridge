/**
 * @file
 * @author Kevin Kirkup (kevin.kirkup@sonyericsson.com)
 */

package com.cypress.cy3240;

/**
 * JNI interface to the CY3240 Bridge controller
 */
public class Cy3240BridgeController {

    /**
     * The amount of time to wait between commands to the bridge controller in
     * milliseconds.
     */
    private static final int SLEEP_BETWEEN_CMD = 250;

    static {

        // Load the static library for the bridge controller
        System.loadLibrary("cy3240");
    }

    // The device handle to the bridge controller
    private int handle = 0;

    /**
     * Method to create the interface to the CY3240 device
     * 
     * @param iface [in] the interface number
     * @param timeout [in] the transmit and received timeout in milliseconds
     * @param power [in] the power mode
     * @param bus [in] the bus mode
     * @param clock [in] the clock speed
     * @return Cy3240ErrorCode
     */
    public native int create(int iface, int timeout, byte power, byte bus, byte clock);

    /**
     * Method to open the device
     * 
     * @param handle [in] the device handle
     * @return Cy3240ErrorCode
     */
    private native int open(int handle);

    /**
     * Method to open the device
     * 
     * @return Cy3240ErrorCode
     */
    public int open() {

        return open(handle);
    }

    /**
     * Method to close the device
     * 
     * @param handle [in] the device handle
     * @return Cy3240ErrorCode
     */
    private native int close(int handle);

    /**
     * Method to close the device
     * 
     * @return Cy3240ErrorCode
     */
    public int close() {
        return close(handle);
    }

    /**
     * Method to write to an I2C device
     * 
     * @param handle [in] the device handle
     * @param address [in] the device address
     * @param data [in] the data to write
     * @return Cy3240ErrorCode
     */
    private native int write(int handle, byte address, byte[] data);

    /**
     * Method to write to an I2C device
     * 
     * @param address [in] the device address
     * @param data [in] the data to write
     * @return Cy3240ErrorCode
     */
    public int write(byte address, byte[] data) {
        return write(handle, address, data);
    }

    /**
     * Method to read from an I2C device
     * 
     * @param handle [in] the device handle
     * @param address [in] the device address
     * @param data [out] the data read from the device
     * @return Cy3240ErrorCode
     */
    private native int read(int handle, byte address, byte[] data);

    /**
     * Method to read for an I2C device
     * 
     * @param address [in] the device address
     * @param data [out] the data read from the device
     * @return Cy3240ErrorCode
     */
    public int read(byte address, byte[] data) {
        return read(handle, address, data);
    }

    /**
     * Method to restart the bridge controller
     * 
     * @param handle [in] the device handle
     * @return Cy3240ErrorCode
     */
    private native int restart(int handle);

    /**
     * Method to restart the bridge controller
     * 
     * @return Cy3240ErrorCode
     */
    public int restart() {
        return restart(handle);
    }

    /**
     * Method to reconfigure the bridge controller settings
     * 
     * @param handle [in] the device handle
     * @param power [in] the power mode to use
     * @param bus [in] the bus mode to use
     * @param clock [in] the clock speed to use
     * @return Cy3240ErrorCode
     */
    private native int reconfigure(int handle, byte power, byte bus, byte clock);

    /**
     * Method to reconfigure the bridge controller settings
     * 
     * @param power [in] the power mode to use
     * @param bus [in] the bus mode to use
     * @param clock [in] the clock speed to use
     * @return Cy3240ErrorCode
     */
    public int reconfigure(byte power, byte bus, byte clock) {
        return reconfigure(handle, power, bus, clock);
    }

    /**
     * Method to reinitialize the bridge controller
     * 
     * @param handle [in] the device handle
     * @return Cy3240ErrorCode
     */
    private native int reinitialize(int handle);

    /**
     * Method to reinitialize the bridge controller
     * 
     * @return Cy3240ErrorCode
     */
    public int reinitialize() {
        return reinitialize(handle);
    }

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
        c.open(c.handle);

        // Create the data array
        byte[] data = new byte[128];

        // Configure the Bridge controller
        c.reconfigure(c.handle, Cy3240PowerMode.CY3240_POWER_5V, Cy3240BusMode.CY3240_BUS_I2C,
                Cy3240ClockSpeed.CY3240_CLOCK_100kHz);

        try {

            Thread.sleep(SLEEP_BETWEEN_CMD);

            // Blink the first led
            for (int retry = 0; retry < 10; retry++) {

                data[5] = 0x02;
                data[6] = (byte)~data[6];
                data[7] = 0x01;

                c.write(c.handle, (byte)0x00, data);

                Thread.sleep(BLINK_DELAY);

            }

        } catch (InterruptedException e) {

            // Print the stack
            e.printStackTrace();
        }

        // Configure the Bridge controller
        c.reconfigure(c.handle, Cy3240PowerMode.CY3240_POWER_EXTERNAL,
                Cy3240BusMode.CY3240_BUS_I2C, Cy3240ClockSpeed.CY3240_CLOCK_100kHz);
    }
}
