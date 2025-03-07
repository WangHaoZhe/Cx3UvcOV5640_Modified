#ifndef CYFXGPIOAPP_H_
#define CYFXGPIOAPP_H_

#include <cyu3types.h>

#define CY_FX_GPIOAPP_GPIO_HIGH_EVENT    (1 << 0)   /* GPIO high event */
#define CY_FX_GPIOAPP_GPIO_LOW_EVENT     (1 << 1)   /* GPIO low event */

extern CyU3PEvent glFxGpioAppEvent;    /* GPIO input event group. */

/* GPIO interrupt callback handler */
void CyFxGpioIntrCb (uint8_t gpioId);

/* Initialize GPIO Ports */
void CyFxGpioInit (void);

/* Entry function for the gpioOutputThread */
void GpioOutputThread_Entry (uint32_t input);

#endif /* CYFXGPIOAPP_H_ */
