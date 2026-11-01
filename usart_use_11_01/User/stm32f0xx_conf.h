#ifndef __STM32F0XX_CONF_H
#define __STM32F0XX_CONF_H

#include "stm32f0xx_adc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_flash.h"
#include "stm32f0xx_pwr.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_syscfg.h"
#include "stm32f0xx_misc.h"

/* Assert configuration */
#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line);

#define assert_param(expr) \
    ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

#else

#define assert_param(expr) ((void)0)

#endif

#endif
