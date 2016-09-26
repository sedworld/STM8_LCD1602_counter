

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
uint16_t mkmOnFullStep = 14;
bool updateDisplay = TRUE;








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
//   openFlash(); 
  enableInterrupts();
  
  
  LCD_PWROFF();                                                                //BACKLIGHT
  // Delay(100);
  LCD_INIT();
  LCD_CLEAR_DISPLAY(); 
  
  
//  FLASH_EraseByte(0x4002);
//  FLASH_EraseByte(0x4003);
  
  
  uint8_t cpufreq = CLK_GetClockFreq()/1000000;
  
  //read values from EEPROM
  set_steps = readFlash2Byte(0x4000);
  stepDelay = readFlash2Byte(0x4002);
  // set_steps = (uint16_t)readFlashWord(0x4000); //WORD of steps
  //add sets_value =;
  handle = FLASH_ReadByte(0x4004);             //auto ot handle mode
  // stepDelay = (uint16_t)readFlashWord(0x4010);           //stepDelay
  stepPhase(FLASH_ReadByte(0x4009)); //step phase
  //stepDelay = readFlashWord(0x4010);
  
  /* Infinite loop */  
  while (1)
  {
    
    //GPIO_WriteReverse(LED);
    //Delay(500);
    
    if(updateDisplay)
    {   
      LCD_LOCATE(1, 1);
      LCD_printf("�c�a�o��a %-6u", set_steps*(mkmOnFullStep>>phDiv));
      //     LCD_printf("�c�a�o��a %-6u", metricValue(mkmOnFullStep));
      updateDisplay = FALSE;
    }   
    
    if (!GPIO_ReadInputPin(BUTTON1))
    {
      updateDisplay = TRUE;
      ++set_steps;
      
      if(handle)
      {
        stepUp(phMlt);
        Delay(stepDelay);
      }
      
      
    }
    
    
    
    
    if (!GPIO_ReadInputPin(BUTTON3))
    {
      updateDisplay = TRUE;
      
      if(set_steps>0)
      {
        --set_steps;
      }
      if(handle)
      {
        stepDown(phMlt);
        Delay(stepDelay/5);
      }
      
      
      
    }
    
    
    
    if(!GPIO_ReadInputPin(BUTTON2))
    {
      updateDisplay = TRUE;
      
      Delay(300);      
      if(!GPIO_ReadInputPin(BUTTON2))
      {
        Delay(680);
        if (!GPIO_ReadInputPin(BUTTON2))                                         // uf 0.7s click
        {
          LCD_CLEAR_DISPLAY();
          LCD_printstring("C�poc?\n");
          Delay(2000);
          if (!GPIO_ReadInputPin(BUTTON2))                                      //if 3s click
          {                                                                     //SETTINGS MENU
            LCD_CLEAR_DISPLAY();
            LCD_printstring("Hac�po���\n");
            Delay(1000);
            menu3s();
            //FLASH_ProgramWord(0x4010, stepDelay);
            
          }
          else                                                                  //if 0.7...3s click
          {                                                                     //SET TO ZERO MENU
            set_steps = 0;                                                      
          }
          
        }
        else                                                                    // if 0.4...0.7s click    
        {                                                                       //if set step = 0 - call MICROSTEP MENU                 
          if (set_steps == 0)                   
          {
            setMicroStep();
            
          }
        }
        
      }
      else                                                                      //if 0.3s...0.6 click
      {                                                                         //DO BURN FUNCTION
        if(handle==0)
        {
          //writeFlashWord(0x4000,set_steps);
          //FLASH_ProgramWord(0x4000,set_steps);
          writeFlash2Byte(0x4000, set_steps);
          
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
    LCD_printstring("C�ep�o��a...\n");//���������...
    
    /*---increment---*/      
    while (GPIO_ReadInputPin(BUTTON2))
    {
      if(now_steps<value)
      {
        //        if(GPIO_ReadInputPin(BUTTON3))
        //        {
        now_steps++;
        stepUp(phMlt);
        Delay(delay);
      }
      else
      {
        break;
      }
       
      
      //      }
    
    }
    //print End message
    clear2row();                 
    LCD_printf("�o�o�o: %d", now_steps);
    beep(2000);
    
    /*---decrement---*/      
    //clear 2ns line LCD
    clear2row();
    LCD_printstring("����e�e��e...\n");
    
    
    //decrement value
    while(now_steps>0)
    {
      stepDown(phMlt);
      now_steps--;
      Delay(delay/3);
    }
    //Done Message and blink backlight
    LCD_CLEAR_DISPLAY(); 
    LCD_LOCATE(1, 6);
    LCD_printstring("�OTOBO\n");
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

void setMicroStep()                                                             //MICROSTEP
{
  while(!GPIO_ReadInputPin(BUTTON2))
  {
    beep(680);            
  }
  
  
  LCD_CLEAR_DISPLAY();
  LCD_printstring("�c�. �a�� �a�a\n");
  LCD_LOCATE(2, 1);
  LCD_printf("�a�a: %u/%u", phMlt, now_Phase%15);
  
  
  
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
        stepPhase(now_Phase);
        break;
        
      case 2:
        stepPhase(now_Phase);
        break;
        
      case 4:
        stepPhase(now_Phase);
        break;
        
      case 8:
        stepPhase(now_Phase);
        break;
        
      case 16:
        
        stepPhase(now_Phase);
        break;
      }
      LCD_LOCATE(2, 1);
      LCD_printf("�a�a: %u/%u", phMlt, now_Phase%15);
    }
    
  }
  //FLASH_ProgramByte(0x4009, now_Phase);
  writeFlashByte(0x4009, now_Phase);
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
  LCD_printstring("Hac�po���\n");
  LCD_LOCATE(2, 1);
  LCD_printstring("+ Pe���; - �a�ep\n");
  
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
    LCD_printstring("Py��o� pe���\n");
    Delay(1000);
  }
  else
  {
    LCD_CLEAR_DISPLAY();
    LCD_printstring("A��o pe���\n");
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
        LCD_printstring("Py��o� pe���\n");
        Delay(1000);
      }
      else
      {
        LCD_CLEAR_DISPLAY();
        LCD_printstring("A��o pe���\n");
        Delay(1000);
      }
    }
  }
  //FLASH_ProgramByte(0x4004, handle);
  writeFlashByte(0x4004, handle);
  LCD_CLEAR_DISPLAY();
  
}

void setStepDelay()                                                             //STEP DELAY
{
  LCD_CLEAR_DISPLAY();
  LCD_printstring("T �e��y �a�, �c\n");//T ����� ���, ��
  LCD_LOCATE(2, 1);
  LCD_printf("Bpe�� T, �c %-4u", stepDelay);//����� �, �� 
  while(GPIO_ReadInputPin(BUTTON2))
  {
    if(!GPIO_ReadInputPin(BUTTON1))
    {
      stepDelay++;
      LCD_LOCATE(2, 1);
      LCD_printf("Bpe�� T, �c %-4u", stepDelay);
    }
    
    if(!GPIO_ReadInputPin(BUTTON3))
    {
      stepDelay--;
      LCD_LOCATE(2, 1);
      LCD_printf("Bpe�� T, �c %-4u", stepDelay);
    }
  }
  writeFlash2Byte(0x4002, stepDelay);
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


