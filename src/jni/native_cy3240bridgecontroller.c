/**
 * @file native_cy3240bridgecontroller
 *
 * @brief JNI implementation for the CY3240 bridge project
 *
 * JNI implementation for the C3240 bridge project
 *
 * @ingroup JNI
 *
 * @owner  Kevin S Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin S Kirkup (kevin.kirkup@gmail.com)
 */

//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include <jni.h>
#include <stdio.h>
#include "native_cy3240bridgecontroller.h"
#include "cy3240_types.h"
#include "cy3240.h"

//@} End of Includes

//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

/*
 * Class:     com_cypress_cy3240_Cy3240BridgeController
 * Method:    create
 * Signature: (IIBBB)I
 */
JNIEXPORT jint JNICALL
Java_com_cypress_cy3240_Cy3240BridgeController_create(
        JNIEnv *jenv,
        jobject jobj,
        jint iface,
        jint timeout,
        jbyte power,
        jbyte bus,
        jbyte clock
        )
{
    Cy3240_Error_t result = CY3240_ERROR_OK;

    // The device handle
    jint handle = 0;
    jfieldID handleId = NULL;

    // Get the class for the object
    jclass objClass = (*jenv)->GetObjectClass(
            jenv,
            jobj
            );

    if (objClass == NULL) {
        result = CY3240_ERROR_JNI;
        printf("Failed to find get Object class\n");
    }

    // Create the device
    if CY3240_SUCCESS(result) {

        // Get the field
        handleId = (*jenv)->GetFieldID(
                jenv,
                objClass,
                "handle",
                "I"
                );

        // Check the handle
        if (handleId == NULL) {
            result = CY3240_ERROR_JNI;
            printf("Failed to find \"handle\" field\n");
        }
    }

    // Create the device
    if CY3240_SUCCESS(result) {

        result = cy3240_factory(
                (int*)&handle,
                (uint8_t)iface,
                (int)timeout,
                (uint8_t)power,
                (uint8_t)bus,
                (uint8_t)clock
                );

        if CY3240_FAILURE(result)
            printf("Failed to create device!\n");
    }

    // Update the field of the device
    if CY3240_SUCCESS(result)
        (*jenv)->SetIntField(
                jenv,
                jobj,
                handleId,
                handle
                );

    return result;
}

/*
 * Class:     com_cypress_cy3240_Cy3240BridgeController
 * Method:    open
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL
Java_com_cypress_cy3240_Cy3240BridgeController_open(
        JNIEnv *jenv,
        jobject jobj,
        jint handle
        )
{
    Cy3240_Error_t result = CY3240_ERROR_OK;

    // Open the device
    result = cy3240_open((int)handle);

    if (CY3240_FAILURE(result))
        printf("Failed to open CY3240 Bridge Controller!\n");

    return result;
}

/*
 * Class:     com_cypress_cy3240_Cy3240BridgeController
 * Method:    close
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL
Java_com_cypress_cy3240_Cy3240BridgeController_close(
        JNIEnv *jenv,
        jobject jobj,
        jint handle
        )
{
    Cy3240_Error_t result = CY3240_ERROR_OK;

    result = cy3240_close((int)handle);

    if (CY3240_FAILURE(result))
        printf("Failed to close CY3240 Bridge Controller!\n");

    return result;
}

/*
 * Class:     com_cypress_cy3240_Cy3240BridgeController
 * Method:    write
 * Signature: (IB[B)I
 */
JNIEXPORT jint JNICALL
Java_com_cypress_cy3240_Cy3240BridgeController_write(
        JNIEnv *jenv,
        jobject jobj,
        jint handle,
        jbyte address,
        jbyteArray dataout
        )
{
    Cy3240_Error_t result = CY3240_ERROR_OK;

    jsize length = 0;
    jbyte* pBytes;

    // Get the number of elements of the array
    length = (*jenv)->GetArrayLength(
        jenv,
        dataout
        );

    // Get a pointer to the data
    pBytes = (*jenv)->GetByteArrayElements(
        jenv,
        dataout,
        NULL
        );

    // Write the data to the device
    result = cy3240_write(
            (int)handle,
            (uint8_t)address,
            (uint8_t*)pBytes,
            (uint16_t*)&length
            );


    // Release the pointer to the data
    (*jenv)->ReleaseByteArrayElements(
            jenv,
            dataout,
            pBytes,
            0
            );

    if (CY3240_FAILURE(result))
        printf("Failed to write data to CY3240 Bridge Controller!\n");

    return 0x00;
}

/*
 * Class:     com_cypress_cy3240_Cy3240BridgeController
 * Method:    read
 * Signature: (IB[B)I
 */
JNIEXPORT jint JNICALL
Java_com_cypress_cy3240_Cy3240BridgeController_read(
        JNIEnv *jenv,
        jobject jobj,
        jint handle,
        jbyte address,
        jbyteArray datain
        )
{
    Cy3240_Error_t result = CY3240_ERROR_OK;

    jsize length = 0;
    jbyte* pBytes;

    // Get the number of elements of the array
    length = (*jenv)->GetArrayLength(
        jenv,
        datain
        );

    // Get a pointer to the data
    pBytes = (*jenv)->GetByteArrayElements(
        jenv,
        datain,
        NULL
        );

    // Read the data from the device
    result = cy3240_read(
            (int)handle,
            (uint8_t)address,
            (uint8_t*)pBytes,
            (uint16_t*)&length
            );

    // Release the pointer to the data
    (*jenv)->ReleaseByteArrayElements(
            jenv,
            datain,
            pBytes,
            0
            );

    if (CY3240_FAILURE(result))
        printf("Failed to read data from CY3240 Bridge Controller!\n");

    return result;
}

/*
 * Class:     com_cypress_cy3240_Cy3240BridgeController
 * Method:    restart
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL
Java_com_cypress_cy3240_Cy3240BridgeController_restart(
        JNIEnv *jenv,
        jobject jobj,
        jint handle
        )
{
    Cy3240_Error_t result = CY3240_ERROR_OK;

    result = cy3240_restart((int)handle);

    if (CY3240_FAILURE(result))
        printf("Failed to restart CY3240 Bridge Controller!\n");

    return result;
}

/*
 * Class:     com_cypress_cy3240_Cy3240BridgeController
 * Method:    reconfigure
 * Signature: (IBBB)I
 */
JNIEXPORT jint JNICALL
Java_com_cypress_cy3240_Cy3240BridgeController_reconfigure(
        JNIEnv *jenv,
        jobject jobj,
        jint handle,
        jbyte power,
        jbyte bus,
        jbyte clock
        )
{
    Cy3240_Error_t result = CY3240_ERROR_OK;

    printf("Starting reconfigure request\n");

    result = cy3240_reconfigure(
            (int)handle,
            (uint8_t)power,
            (uint8_t)bus,
            (uint8_t)clock
            );

    printf("Finished reconfigure request\n");

    if (CY3240_FAILURE(result))
        printf("Failed to read data from CY3240 Bridge Controller!\n");

    return result;

}

/*
 * Class:     com_cypress_cy3240_Cy3240BridgeController
 * Method:    reinitialize
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL
Java_com_cypress_cy3240_Cy3240BridgeController_reinitialize(
        JNIEnv *jenv,
        jobject jobj,
        jint handle
        )
{
    Cy3240_Error_t result = CY3240_ERROR_OK;

    result = cy3240_reinit((int)handle);

    if (CY3240_FAILURE(result))
        printf("Failed to read data from CY3240 Bridge Controller!\n");

    return result;

}

//@} End of Methods
