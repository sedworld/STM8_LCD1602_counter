

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "delay.h"
#include "HD44780.h"


uint16_t stepCount(uint16_t n, uint16_t t);


  char str11[] = "Set steps: \n";
  char str18[] = "Step div \n"; //1, 1/2, 1/4, 1/8
  uint16_t set_steps= 0 ;
  uint16_t now_steps = 0;
  uint8_t now_Phase = 1;
  



 void doBurn(uint16_t value, uint16_t delay);
 void stepUp(uint16_t howStep);
 void stepDown(uint16_t howStep);
 void setMicroStep();
 void beep(uint16_t time_ms);
 


void main(void)
{
 
  clockInit();
  TIM4_Config();
  gpioInit();
  ExtiConfig();
  beepInit();
  openFlash(); 
  enableInterrupts();
 
    
  LCD_PWRON();
 // Delay(100);
  LCD_INIT();
  LCD_CLEAR_DISPLAY(); 


   uint8_t cpufreq = CLK_GetClockFreq()/1000000;
  set_steps = (uint16_t)readFlashWord(0x4000);
      
  /* Infinite loop */  
  while (1)
  {
    //GPIO_WriteReverse(LED);
    //Delay(500);
    
    LCD_LOCATE(1, 1);
    LCD_printf("Set steps %-4d", set_steps);
    
    if (!GPIO_ReadInputPin(BUTTON1))
    {
      ++set_steps;
      Delay(68);
    }
    
    
    while(!GPIO_ReadInputPin(BUTTON2))
    {
      Delay(680);
        if (!GPIO_ReadInputPin(BUTTON2))                                         // CREATE MORE SUBMENU BY LONGPRESS. 
        {
           if (set_steps == 0)
           {
             setMicroStep();
           }
           else
           {
             set_steps = 0;
                                                                                  //ADD PARKING FUNC
           }
       break;
       
        }
        else
        {
          doBurn(set_steps,10);
          writeFlashWord(0x4000,set_steps);                                     //=====write EEPROM
        }
      //break;
    }
    
    
  }
  
}

 void doBurn(uint16_t value, uint16_t delay)
{     

  if (value>0)                         // chek for zero
  {
      //clear 2 line LCD
      LCD_LOCATE(2, 1);                         
      LCD_printstring("                \n");
      
/*---increment---*/      
      while (now_steps<value)
      {
        now_steps++;
        stepUp(phMlt);
        LCD_LOCATE(2, 1);
        LCD_printf("Burn: %d", now_steps);
        Delay(delay);
        
      }
      //print End message
      LCD_LOCATE(2, 1);                 
      LCD_printf("Complete: %d", now_steps);
      beep(2500);
      
/*---decrement---*/      
      //clear 2ns line LCD
      LCD_LOCATE(2, 1);
      LCD_printstring("                \n");
      
      //decrement value
      while(now_steps>0)
      {
        LCD_LOCATE(2, 1);
        LCD_printf("Extract: %-4d", now_steps);
        
        stepDown(phMlt);
        now_steps--;
        Delay(delay);
      }
      //Done Message and blink backlight
      LCD_CLEAR_DISPLAY(); 
      LCD_LOCATE(1, 7);
      LCD_printstring("Done\n");
      LcdBacklightBlink(5);
      LCD_PWRON ();
  }

}

void stepUp(uint16_t howStep)
{
  GPIO_WriteLow(MOT_DIR);
  for(uint16_t i = howStep; i>0; i--)
  {
    GPIO_WriteHigh(MOT_STEP);
    Delay(1);
    GPIO_WriteLow(MOT_STEP);
    Delay(1);
  }
  
}

void stepDown(uint16_t howStep)
{
  GPIO_WriteHigh(MOT_DIR);
  for(uint16_t i = howStep; i>0; i--)
  {
    GPIO_WriteHigh(MOT_STEP);
    Delay(1);
    GPIO_WriteLow(MOT_STEP);
    Delay(1);
  }
  
}

 void setMicroStep()
 {
      while(!GPIO_ReadInputPin(BUTTON2))
   {
     beep(680);            
   }

     LCD_CLEAR_DISPLAY();
     LCD_printstring("STEP PHASE SETUP\n");
     LCD_LOCATE(2, 1);
     LCD_printf("Set phase: %u/%u", phMlt, now_Phase%15);
     


    while(GPIO_ReadInputPin(BUTTON2))
    {
       Delay(68);
      if(!GPIO_ReadInputPin(BUTTON1))
         {
           now_Phase+=now_Phase;
           if(now_Phase>16)
           {
             now_Phase=1;
           }
           switch(now_Phase)
           {
           case 1: 
             LCD_LOCATE(2, 1);
             stepPhase(now_Phase);
             break;
             
           case 2:
             LCD_LOCATE(2, 1);
             stepPhase(now_Phase);
             break;
             
           case 4:
             LCD_LOCATE(2, 1);
             stepPhase(now_Phase);
             break;
             
           case 8:
             LCD_LOCATE(2, 1);
             stepPhase(now_Phase);
             break;
             
           case 16:
             LCD_LOCATE(2, 1);
             stepPhase(now_Phase);
             break;
           }
       
        }
   }
      
    LCD_CLEAR_DISPLAY();
    LCD_LOCATE(1, 8);
    LCD_printstring("OK\n");
    beep(680);
    LCD_CLEAR_DISPLAY();
    return;   
 }

void beep(uint16_t time_ms)
{
     BEEP_Cmd(ENABLE);
     Delay(time_ms);
     BEEP_Cmd(DISABLE);
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


