

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "delay.h"
#include "HD44780.h"


char STCustom[48] =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Blank
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, // 1column  |
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, // 2columns ||
    0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, // 3columns |||
    0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, // 4columns ||||
    0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, // 5columns |||||
  };

  char disp[] = "\0\0\0\0\0\0\0\0\n";
  char message[5][7] = {"STM8S", "Value", "Line", "8-Bit", "Micro"};



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
    /* Set @CGRAM address start */
  LCD_CMD(CGRAM_address_start);
  /* Loading 6 characters @CGRAM address from STCustom tab */
  LCD_LOAD_CGRAM(STCustom, 6);

  //LCD_printstring("Hello world\n");
  uint8_t cpufreq = CLK_GetClockFreq()/1000000;
   uint8_t cnt = 0;
  /* Infinite loop */  
  while (1)
  {
    GPIO_WriteReverse(LED);
    Delay(500);
    LCD_CLEAR_DISPLAY();
    LCD_printf("FREQ %d\n", cnt++);
    
    //LCD_printstring("Привет\n");

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


