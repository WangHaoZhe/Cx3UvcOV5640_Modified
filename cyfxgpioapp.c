#include <cyu3system.h>
#include <cyu3os.h>
#include <cyu3error.h>
#include <cyu3gpio.h>
#include <cyu3uart.h>
#include "cyfxgpioapp.h"

CyU3PThread gpioOutputThread;   /* GPIO thread structure */
CyU3PEvent glFxGpioAppEvent;    /* GPIO input event group. */

/* GPIO interrupt callback handler. This is received from
 * the interrupt context. So DebugPrint API is not available
 * from here. Set an event in the event group so that the
 * GPIO thread can print the event information. */
void CyFxGpioIntrCb (
        uint8_t gpioId /* Indicates the pin that triggered the interrupt */
        )
{
    CyBool_t gpioValue = CyFalse;
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;

    /* Get the status of the pin */
    apiRetStatus = CyU3PGpioGetValue (gpioId, &gpioValue);
    if (apiRetStatus == CY_U3P_SUCCESS)
    {
        /* Check status of the pin */
        if (gpioValue == CyTrue)
        {
            /* Set GPIO high event */
            CyU3PEventSet(&glFxGpioAppEvent, CY_FX_GPIOAPP_GPIO_HIGH_EVENT,
                    CYU3P_EVENT_OR);
            //CyU3PGpioSetValue (24, CyFalse);
        }
        else
        {
            /* Set GPIO low Event */
            CyU3PEventSet(&glFxGpioAppEvent, CY_FX_GPIOAPP_GPIO_LOW_EVENT,
                    CYU3P_EVENT_OR);
            //CyU3PGpioSetValue (24, CyTrue);
        }
    }
}

void
CyFxGpioInit (void)
{
    CyU3PGpioClock_t gpioClock;
    CyU3PGpioSimpleConfig_t gpioConfig;
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;

    /* Init the GPIO module */
    gpioClock.fastClkDiv = 2;
    gpioClock.slowClkDiv = 0;
    gpioClock.simpleDiv = CY_U3P_GPIO_SIMPLE_DIV_BY_2;
    gpioClock.clkSrc = CY_U3P_SYS_CLK;
    gpioClock.halfDiv = 0;

    apiRetStatus = CyU3PGpioInit(&gpioClock, CyFxGpioIntrCb);
    if (apiRetStatus != 0)
    {
        /* Error Handling */
        CyU3PDebugPrint (4, "CyU3PGpioInit failed, error code = %d\n", apiRetStatus);
    }

    /* Configure GPIO 24 as output */
    apiRetStatus = CyU3PDeviceGpioOverride (24, CyTrue);
    if (apiRetStatus != 0)
    {
        /* Error Handling */
        CyU3PDebugPrint (4, "CyU3PDeviceGpioOverride failed, error code = %d\n",
                apiRetStatus);
    }
    gpioConfig.outValue = CyFalse;
    gpioConfig.driveLowEn = CyTrue;
    gpioConfig.driveHighEn = CyTrue;
    gpioConfig.inputEn = CyFalse;
    gpioConfig.intrMode = CY_U3P_GPIO_NO_INTR;
    apiRetStatus = CyU3PGpioSetSimpleConfig(24, &gpioConfig);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        /* Error handling */
        CyU3PDebugPrint (4, "CyU3PGpioSetSimpleConfig failed, error code = %d\n",
                apiRetStatus);
    }
    CyU3PGpioSetIoMode(24, CY_U3P_GPIO_IO_MODE_WPD);
}

/* Entry function for the gpioOutputThread */
void
GpioOutputThread_Entry (
        uint32_t input)
{
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;

    for (;;)
    {
        /* Set the GPIO 24 to high */
    	if (CyU3PGetTime() % 1000 == 0)
    	{
            apiRetStatus = CyU3PGpioSetValue (24, CyTrue);
            if (apiRetStatus != CY_U3P_SUCCESS)
            {
                /* Error handling */
                CyU3PDebugPrint (4, "CyU3PGpioSetValue failed, error code = %d\n",
                        apiRetStatus);
            }
    	}

        /* Set the GPIO 24 to low */
    	if (CyU3PGetTime() % 1000 == 200)
    	{
            apiRetStatus = CyU3PGpioSetValue (24, CyFalse);
            if (apiRetStatus != CY_U3P_SUCCESS)
            {
                /* Error handling */
                CyU3PDebugPrint (4, "CyU3PGpioSetValue failed, error code = %d\n",
                        apiRetStatus);
            }
    	}
    }
}

/* [ ] */

