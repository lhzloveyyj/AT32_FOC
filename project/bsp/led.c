#include "led.h"          

#define LED_NUM                          1

#define LED0_PIN                         GPIO_PINS_8
#define LED0_GPIO                        GPIOA
#define LED0_GPIO_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK

gpio_type *led_gpio_port[LED_NUM]        = {LED0_GPIO};
uint16_t led_gpio_pin[LED_NUM]           = {LED0_PIN};

void at32_led_toggle(led_type led)
{
  if(led > (LED_NUM - 1))
    return;
  if(led_gpio_pin[led])
    led_gpio_port[led]->odt ^= led_gpio_pin[led];
}
