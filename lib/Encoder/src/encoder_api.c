#include "hal/encoder_api.h"

#include "mbed_assert.h"
#include "mbed_critical.h"
#include "mbed_error.h"
#include "mbed_power_mgmt.h"

extern const PinMap PinMap_Encoder_TIM[];

static void encoder_registers_init(encoder_t * obj)
{
  TIM_Encoder_InitTypeDef sConfig = {
    .EncoderMode = TIM_ENCODERMODE_TI12,
    .IC1Polarity = TIM_ICPOLARITY_RISING,
    .IC1Selection = TIM_ICSELECTION_DIRECTTI,
    .IC1Prescaler = TIM_ICPSC_DIV1,
    .IC1Filter = 0,
    .IC2Polarity = TIM_ICPOLARITY_RISING,
    .IC2Selection = TIM_ICSELECTION_DIRECTTI,
    .IC2Prescaler = TIM_ICPSC_DIV1,
    .IC2Filter = 0,
  };

  if (HAL_TIM_Encoder_Init(&obj->TimHandle, &sConfig) != HAL_OK) error("Cannot initialize Encoder");

  if (HAL_TIM_Encoder_Start(&obj->TimHandle, TIM_CHANNEL_ALL) != HAL_OK)
    error("Cannot start Encoder");

  sleep_manager_lock_deep_sleep();

  __HAL_TIM_SET_COUNTER(&obj->TimHandle, 0);
}

void encoder_init_direct(encoder_t * obj, const encoder_pinmap_t * pinmap)
{
  MBED_ASSERT((int)pinmap->peripheral != NC);

#if defined(TIM1_BASE)
  if (pinmap->peripheral == TIM1_BASE) {
    __HAL_RCC_TIM1_CLK_ENABLE();
  }
#endif
#if defined(TIM2_BASE)
  if (pinmap->peripheral == TIM2_BASE) {
    __HAL_RCC_TIM2_CLK_ENABLE();
  }
#endif
#if defined(TIM3_BASE)
  if (pinmap->peripheral == TIM3_BASE) {
    __HAL_RCC_TIM3_CLK_ENABLE();
  }
#endif
#if defined(TIM4_BASE)
  if (pinmap->peripheral == TIM4_BASE) {
    __HAL_RCC_TIM4_CLK_ENABLE();
  }
#endif
#if defined(TIM5_BASE)
  if (pinmap->peripheral == TIM5_BASE) {
    __HAL_RCC_TIM5_CLK_ENABLE();
  }
#endif
#if defined(TIM8_BASE)
  if (pinmap->peripheral == TIM8_BASE) {
    __HAL_RCC_TIM8_CLK_ENABLE();
  }
#endif

  pin_function(pinmap->ch1_pin, pinmap->ch1_function);
  pin_function(pinmap->ch2_pin, pinmap->ch2_function);

  pin_mode(pinmap->ch1_pin, PullNone);
  pin_mode(pinmap->ch2_pin, PullNone);

  obj->TimHandle.Instance = (TIM_TypeDef *)pinmap->peripheral;
  obj->TimHandle.Init.Period = 0xFFFF;
  obj->TimHandle.Init.Prescaler = 0;
  obj->TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  obj->TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
#if !defined(TARGET_STM32L0) && !defined(TARGET_STM32L1)
  obj->TimHandle.Init.RepetitionCounter = 0;
#endif
#ifdef TIM_AUTORELOAD_PRELOAD_DISABLE
  obj->TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
#endif

  encoder_registers_init(obj);
}

void encoder_init(encoder_t * obj, PinName ch1, PinName ch2)
{
  uint32_t tim_ch1 = pinmap_peripheral(ch1, PinMap_Encoder_TIM);
  uint32_t tim_ch2 = pinmap_peripheral(ch2, PinMap_Encoder_TIM);

  uint32_t peripheral = pinmap_merge(tim_ch1, tim_ch2);

  uint32_t function_ch1 = pinmap_find_function(ch1, PinMap_Encoder_TIM);
  uint32_t function_ch2 = pinmap_find_function(ch2, PinMap_Encoder_TIM);

  const encoder_pinmap_t static_pinmap = {
    .peripheral = peripheral,
    .ch1_pin = ch1,
    .ch1_function = function_ch1,
    .ch2_pin = ch2,
    .ch2_function = function_ch2,
  };

  encoder_init_direct(obj, &static_pinmap);
}

void encoder_free(encoder_t * obj)
{
  pin_function(obj->ch1, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
  pin_function(obj->ch2, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));

  sleep_manager_unlock_deep_sleep();
}

count_t encoder_read(encoder_t * obj)
{
  count_t ret;

  core_util_critical_section_enter();
  ret = __HAL_TIM_GET_COUNTER(&obj->TimHandle);
  core_util_critical_section_exit();

  return ret;
}
