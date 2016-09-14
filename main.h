#include "stm8s.h"
#include "delay.h"
#include "HD44780.h"

#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)

/* Private defines -----------------------------------------------------------*/
#define LED GPIOD, GPIO_PIN_3
#define TIM4_PERIOD       124




/* Private function prototypes -----------------------------------------------*/
void clockInit();
void gpioInit();
void UART1_Config();
void TIM4_Config();


void clockInit(void)
{
  CLK_DeInit();
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

void gpioInit(void)
{
  //GPIO_DeInit(GPIOD);

  GPIO_DeInit(LCDPort);                                                         //reset port c
  /* GPIO reset */
  GPIO_DeInit(LCDPwrPort);                                                       //reset port d
  /* GPIO reset */
  GPIO_DeInit(LCDControlPort);                                                   //reset port a

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
 
//  /* Configure PC4 (push button) as input floating */
//  GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_PU_IT);
  
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);                      /*!< Output push-pull, low level, 2MHz */


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
   UART1_Init((uint32_t)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);


}
