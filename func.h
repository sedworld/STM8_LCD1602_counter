#include "stm8s.h"


/*========================================================PERIPH_FUNCTOINS====*/
void beep(uint16_t time_ms);

/*============================================================================*/
bool goBurn (uint16_t value, uint16_t delay);                                   //DO BURN WITH USER VALUE AND DELAY. no back
bool goBack (uint16_t value, uint16_t delay);                                   //BACK AFTER BURN. not parking
bool goParking(uint16_t delay);                                                 //PARKING DRILL



/*=============================================MOTOR_FUNCTIONS================*/
void stepUp(uint8_t howStep);                                                   //MOTOR STEP UP        
void stepDown(uint8_t howStep);                                                //MOTOR STEP DOWN
void stepPhase(int8_t ph);                                                      //MOTOR DRIVER STEP PHASE SETUP 



/*======================================================DISPLAY_FUNCTOINS=====*/
void LcdBacklightBlink(uint8_t n);                                              //DISPLAY BACKLIGHT BLINK
void setMicroStep();                                                            //SETUP STEP PHASE MENU
