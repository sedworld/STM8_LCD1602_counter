

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "delay.h"
#include "HD44780.h"





  char str11[] = "Set steps: \n";
  char str18[] = "Speed: \n"; //1, 1/2, 1/4, 1/8 
  uint8_t set_steps = 0;
  uint8_t now_steps = 0;


void main(void)
{
  clockInit();
  gpioInit();
  TIM4_Config();

    enableInterrupts();

    
  LCD_PWRON();
  Delay(100);
  LCD_INIT();
  LCD_CLEAR_DISPLAY(); 


  uint8_t cpufreq = CLK_GetClockFreq()/1000000;
   uint8_t cnt = 0;
   
      BEEP_Cmd(DISABLE);
      BEEP_Init(BEEP_FREQUENCY_1KHZ);
      
  /* Infinite loop */  
  while (1)
  {
    GPIO_WriteReverse(LED);
    Delay(500);
    LCD_CLEAR_DISPLAY();
    LCD_printf("Set steps %d\n", cnt);
    
    if (!GPIO_ReadInputPin(BUTTON1))
    {
      cnt++;
      BEEP_Cmd(ENABLE);
      Delay(100);
      BEEP_Cmd(DISABLE);
    }
    
  


  }
  
}

/* Private functions ---------------------------------------------------------*/



#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


