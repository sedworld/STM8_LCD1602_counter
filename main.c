

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "delay.h"
#include "HD44780.h"


uint16_t stepCount(uint16_t n, uint16_t t);


char str11[] = "Set steps: \n";
char str18[] = "Step div \n"; //1, 1/2, 1/4, 1/8
uint16_t set_steps= 0 ; //set spets by user
uint16_t now_steps = 0; // step in system
uint8_t now_Phase = 1; //use phase 
uint8_t handle = 0;
uint16_t stepDelay = 500;




void doBurn(uint16_t value, uint16_t delay);
void stepUp(uint16_t howStep);
void stepDown(uint16_t howStep);
void setMicroStep();
void beep(uint16_t time_ms);
void menu3s();
void setHandle();
void setStepDelay();




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
  
  //read values from EEPROM
  set_steps = (uint16_t)readFlashWord(0x4000); //WORD of steps
  handle = FLASH_ReadByte(0x4004);             //auto ot handle mode
  stepDelay = (uint16_t)readFlashWord(0x4005);           //stepDelay
  
  /* Infinite loop */  
  while (1)
  {
    
    //GPIO_WriteReverse(LED);
    //Delay(500);
    
    LCD_LOCATE(1, 1);
    LCD_printf("©c¿a½o³ºa %-6u", set_steps);
    
    if (!GPIO_ReadInputPin(BUTTON1))
    {
     
      ++set_steps;
      if(handle)
      {
        stepUp(1);
      }
      Delay(10);

        if (!GPIO_ReadInputPin(BUTTON1))
        {
                Delay(680);
          while(!GPIO_ReadInputPin(BUTTON1))
          {
            ++set_steps;
            if(handle)
            {
              stepUp(1);
            }
            Delay(150);
          }
          
        }
      
      
    }
    
    if (!GPIO_ReadInputPin(BUTTON3))
    {
      if(set_steps>0)
      {
        --set_steps;
      }
      if(handle)
      {
        stepDown(1);
      }
      
      Delay(stepDelay);
    }
    
    
    
    if(!GPIO_ReadInputPin(BUTTON2))
    {
      Delay(300);
      if(!GPIO_ReadInputPin(BUTTON2))
      {
        Delay(680);
        if (!GPIO_ReadInputPin(BUTTON2))                                         // CREATE MORE SUBMENU BY LONGPRESS. 
        {
          LCD_CLEAR_DISPLAY();
          LCD_printstring("C²poc?\n");
          Delay(3000);
          if (!GPIO_ReadInputPin(BUTTON2)) 
          {
            LCD_CLEAR_DISPLAY();
            LCD_printstring("Hac¿po¹º¸\n");
            Delay(1000);
            menu3s();
          }
          else //if <3000ms press
          {
            set_steps = 0;
          }
          
        }
        else    //if 680 ms press
        {
          if (set_steps == 0)
          {
            setMicroStep();
          }
        }
        
      }
      else
      {
        if(handle==0)
        {
          writeFlashWord(0x4000,set_steps);
          doBurn(set_steps,stepDelay);
          
        }
        
        
      }
      
      //break;
    }
    
    
  }
  
}

void doBurn(uint16_t value, uint16_t delay)
{     
  
  if (value>0)                         // chek for zero
  {
    now_steps=0;
    //clear 2 line LCD
    clear2row();
    LCD_printstring("C³ep»o³ºa...\n");//ñâåðëîâêà...
    
    /*---increment---*/      
    while (now_steps<value)
    {
      if(GPIO_ReadInputPin(BUTTON3))
      {
        now_steps++;
        stepUp(phMlt);
        Delay(delay);
      }
      else
        return;
    }
    //print End message
    clear2row();                 
    LCD_printf("¡o¿o³o: %d", now_steps);
    beep(2500);
    
    /*---decrement---*/      
    //clear 2ns line LCD
    clear2row();
    LCD_printstring("¥·³»eÀe½¸e...\n");
    
    
    //decrement value
    while(now_steps>0)
    {
      stepDown(phMlt);
      now_steps--;
      Delay(delay);
    }
    //Done Message and blink backlight
    LCD_CLEAR_DISPLAY(); 
    LCD_LOCATE(1, 6);
    LCD_printstring("¡OTOBO\n");
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
  LCD_printstring("©c¿. äa·Ã Áa´a\n");
  LCD_LOCATE(2, 1);
  LCD_printf("ªa·a: %u/%u", phMlt, now_Phase%15);
  
  
  
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

void menu3s()
{
  LCD_CLEAR_DISPLAY();
  LCD_printstring("Hac¿po¹º¸\n");
  LCD_LOCATE(2, 1);
  LCD_printstring("+ Pe¶¸¼; - ¤aãep\n");
  
  while(GPIO_ReadInputPin(BUTTON2))
  {    
    if (!GPIO_ReadInputPin(BUTTON1)) 
    {
      setHandle();
    }
    
    if (!GPIO_ReadInputPin(BUTTON3)) 
    {
      setStepDelay();
    }
  }
  LCD_CLEAR_DISPLAY();
        Delay(680);
}


void setHandle()
{
  if(handle>0)
  {
    LCD_CLEAR_DISPLAY();
    LCD_printstring("PyÀ½o¹ pe¶¸¼\n");
    Delay(1000);
  }
  else
  {
    LCD_CLEAR_DISPLAY();
    LCD_printstring("A³¿o pe¶¸¼\n");
    Delay(1000);
  }
  
  
  while(GPIO_ReadInputPin(BUTTON2))
  {
    if(!GPIO_ReadInputPin(BUTTON1))
    {
      handle=~handle;
      if(handle>0)
      {
        LCD_CLEAR_DISPLAY();
        LCD_printstring("PyÀ½o¹ pe¶¸¼\n");
        Delay(1000);
      }
      else
      {
        LCD_CLEAR_DISPLAY();
        LCD_printstring("A³¿o pe¶¸¼\n");
        Delay(1000);
      }
    }
  }
  FLASH_ProgramByte(0x4004, handle);
  LCD_CLEAR_DISPLAY();

}

void setStepDelay()
{
  LCD_CLEAR_DISPLAY();
  LCD_printstring("T ¼e¶ãy Áa´, ¼c\n");//T ìåæäó øàã, ìñ
  LCD_LOCATE(2, 1);
  LCD_printf("Bpe¼Ç T, ¼c %-4u", stepDelay);//Âðåìÿ Ò, ìñ 
  while(GPIO_ReadInputPin(BUTTON2))
  {
    if(!GPIO_ReadInputPin(BUTTON1))
    {
      stepDelay++;
      LCD_LOCATE(2, 1);
      LCD_printf("Bpe¼Ç T, ¼c %-4u", stepDelay);
    }
    
    if(!GPIO_ReadInputPin(BUTTON3))
    {
      stepDelay--;
      LCD_LOCATE(2, 1);
      LCD_printf("Bpe¼Ç T, ¼c %-4u", stepDelay);
    }
  }
  
  writeFlashWord(0x4005, stepDelay);
  LCD_CLEAR_DISPLAY();

  
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


