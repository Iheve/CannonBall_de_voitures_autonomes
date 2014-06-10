package fr.ensimag.cannonball;

import java.io.IOException;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialProber;

import android.content.Context;
import android.hardware.usb.UsbManager;
import android.util.Log;

/**
 * Class to send data over serial communication
 * @author Thibaut Coutelou, Benjamin Mugnier, Guillaume Perrin
 *
 */
public class Serial {
    
    private static final String TAG = "Serial";
    
    /* The serial instance */
    private static Serial Instance = null;
    
    private static final int Baudrate = 115200;
    private UsbSerialDriver mUsbSerialDriver = null;
    
    private Serial(Context context) {
        UsbManager usbManager = (UsbManager) context.getSystemService(Context.USB_SERVICE);
        mUsbSerialDriver = UsbSerialProber.acquire(usbManager);
    }

    public static Serial getInstance() {
        return Instance;
    }
    
    /**
     * Create an instance of Serial if no one already exist
     * @param context the application context
     */
    public static void createInstance(Context context) {
        if (Instance == null) {
            Log.i(TAG, "Create new instance of Serial");
            Instance = new Serial(context);
        }
    }
    
    /**
     * Open the serial communication
     * @throws IOException
     */
    public void openSerial() throws IOException {
        mUsbSerialDriver.open();
        mUsbSerialDriver.setBaudRate(Baudrate);
        Log.d(TAG, "Serial connection opened");
    }
    
    /**
     * Close the serial communication
     * @throws IOException
     */
    public void closeSerial() throws IOException {
        mUsbSerialDriver.close();
        Log.d(TAG, "Serial connection closed");
    }
    
    /**
     * Write data over the serial
     * The communication must be opened
     * @param data data to send
     * @throws IOException
     */
    public void writeData(byte[] data) throws IOException {
        mUsbSerialDriver.write(data, 1000);
        Log.d(TAG, "Write data to serial : " + data);
    }
    
}
