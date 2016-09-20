//#include "func.h"
#include "stm8s.h"

#include "delay.h"




/*========================================================PERIPH_FUNCTOINS====*/




//DO BURN WITH USER VALUE AND DELAY. no back
bool goBurn (uint16_t value, uint16_t delay)
{
  uint16_t steps = value * LENGTH_STEP_FACTOR;
  //uint16_t prc = steps * 100 / value;
  
  while(steps>0)
  {
    stepDown(step);                                                             //create var step. 1 or 2 step motor for 1 step sofware
    steps--;
    Delay(delay);
  }    
  return TRUE;
    
}

//BACK AFTER BURN. not parking
bool goBack (uint16_t value, uint16_t delay)
{
  uint16_t steps = value * LENGTH_STEP_FACTOR ;
  //uint16_t prc = steps * 100 / value;
  
  while(steps>0)
  {
    stepUP(step);
    steps--;
    Delay(delay);
  }
  return TRUE;
}

//PARKING DRILL
bool goParking(uint16_t delay)
{
  while(HOME_POSITION)
  {
    stepUP(step);
    Delay(delay);
  }
  return TRUE;
}
/*=============================================MOTOR_FUNCTIONS================*/
//MOTOR STEP UP
void stepUp(uint8_t howStep)
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

//MOTOR STEP DOWN
void stepDown(uint8_t howStep)
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

//MOTOR DRIVER STEP PHASE SETUP 
void stepPhase(int8_t ph)
{
  switch (ph)
  {
  case 1: 
    GPIO_WriteLow(MOT_MS1);
    GPIO_WriteLow(MOT_MS2);
    phMlt = 1;
    break;
  case 2:
    GPIO_WriteHigh(MOT_MS1);
    GPIO_WriteLow(MOT_MS2);
    phMlt = 1;
    break;
  case 4:
    GPIO_WriteLow(MOT_MS1);
    GPIO_WriteHigh(MOT_MS2);
    phMlt = 1;
    break;
  case 8: 
    GPIO_WriteHigh(MOT_MS1);
    GPIO_WriteHigh(MOT_MS2);
    phMlt = 1;
    break;
  case 16: 
    GPIO_WriteLow(MOT_MS1);
    GPIO_WriteLow(MOT_MS2);    
//    phMlt = 2;
    break;
  }
//  LCD_printf("Set phase: %u/%u", phMlt, ph%15);
  
}

/*======================================================DISPLAY_FUNCTOINS=====*/
//DISPLAY BACKLIGHT BLINK
void LcdBacklightBlink(uint8_t n)
{
  while(n>0)
  {
    LCD_PWROFF();
    BEEP_Cmd(ENABLE);
    Delay(500);
    LCD_PWRON();
    BEEP_Cmd(DISABLE);
    Delay(500);
    n--;
  }
  
}

//SETUP STEP PHASE MENU
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

/*=============================================BUTTON_FUNCTIONS===============*/


