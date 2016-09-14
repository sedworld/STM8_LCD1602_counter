#include "stm8s.h"
#include "delay.h"
#include "HD44780.h"


/* Private defines -----------------------------------------------------------*/
#define LED GPIOD, GPIO_PIN_3
#define BUTTON1 GPIOB, GPIO_PIN_5
#define BUTTON2 GPIOB, GPIO_PIN_4

#define MOT_MS1 GPIOA, GPIO_PIN_2
#define MOT_MS2 GPIOA, GPIO_PIN_1
#define MOT_STEP GPIOD, GPIO_PIN_6
#define MOT_DIR GPIOD, GPIO_PIN_5
#define MOT_EN GPIOA, GPIO_PIN_3

#define BEEP_OUT GPIOD, GPIO_PIN_4


#define TIM4_PERIOD       124
#define 



/* Private function prototypes -----------------------------------------------*/
void clockInit();
void gpioInit();
void UART1_Config();
void TIM4_Config();
void beepInit();


bool button_state();

void clockInit(void)
{
  CLK_DeInit();
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

void gpioInit(void)
{
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);


  /* Configure LCDPort D7 output push-pull low LCD Bus*/
  GPIO_Init(LCDPort, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);
  /* Configure LCDPort D6  output push-pull low LCD Bus*/
  GPIO_Init(LCDPort, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
  /* Configure LCDPort D5  output push-pull low LCD Bus*/
  GPIO_Init(LCDPort, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
  /* Configure LCDPort D4  output push-pull low LCD Bus*/
  GPIO_Init(LCDPort, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
  
  /* Configure LCDControlPort E output push-pull low LCD Enable Pin*/
  GPIO_Init(LCDControlPort, LCD_Enable, GPIO_MODE_OUT_PP_LOW_FAST);
  /* Configure LCDPort RS output push-pull low LCD RS Pin*/
  GPIO_Init(LCDControlPort, LCD_RS, GPIO_MODE_OUT_PP_LOW_FAST);
  
  /* Configure LCDPwrOnPort VDD output push-pull low LCD Power Supply*/
  GPIO_Init(LCDPwrPort, LCDPwrPin, GPIO_MODE_OUT_PP_LOW_FAST);
 
  /* Configure PB5, PB4 (push button) as input floating */
  GPIO_Init(BUTTON1, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(BUTTON2, GPIO_MODE_IN_PU_NO_IT);
  
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);                      /*!< Output push-pull, low level, 2MHz */

    /* Configure BEEP*/
  GPIO_Init(BEEP_OUT, GPIO_MODE_OUT_PP_LOW_SLOW);

  GPIO_Init(MOT_STEP, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(MOT_DIR, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(MOT_MS1, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(MOT_MS2, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(MOT_EN, GPIO_MODE_OUT_PP_LOW_SLOW);
}

void TIM4_Config(void)
{
   /* TIM4 configuration:
    - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
    clock used is 16 MHz / 128 = 125 000 Hz
   - With 125 000 Hz we can generate time base:
       max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
       min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
   - In this example we need to generate a time base equal to 1 ms
    so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);

}

void UART1_Config()
{
UART1_DeInit();
  UART1_Init(9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
               UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE); 
  UART1_ITConfig (UART1_IT_RXNE_OR,DISABLE);
  UART1_Cmd(ENABLE);

}

void beepInit()
{
  BEEP_Cmd(DISABLE);
  BEEP_Init(BEEP_FREQUENCY_1KHZ);
}

void stepPhase(int8_t ph)
{
  if (ph!=1 | ph!=2 | ph!=4 | ph!=8)
  {
    switch (ph)
  {
  case 1: 
    GPIO_WriteLow(MOT_MS1);
    GPIO_WriteLow(MOT_MS1);
    break;
  case 2:
    GPIO_WriteHigh(MOT_MS1);
    GPIO_WriteLow(MOT_MS2);
    break;
  case 4:
    GPIO_WriteLow(MOT_MS1);
    GPIO_WriteHigh(MOT_MS2);
    break;
  case 8: 
    GPIO_WriteHigh(MOT_MS1);
    GPIO_WriteHigh(MOT_MS2);
    break;
  }
  }else return;
  
}






