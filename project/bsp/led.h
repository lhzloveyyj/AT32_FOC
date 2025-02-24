#ifndef __led_H
#define __led_H

#include "at32f403a_407_gpio.h" 
#include "at32f403a_407.h"              

typedef enum
{
  LED0                                   = 0,
} led_type;

void at32_led_toggle(led_type led);

#endif
