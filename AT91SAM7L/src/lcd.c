//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : dbgu.c
//* Object              : DBGU routines written in C
//* Creation            : JG   16/Aug/2004
//*----------------------------------------------------------------------------

// Include Standard files
#include "board.h"
#include "lcd.h"
#include <string.h>


#define COMMON_NUMBER   3<<0   //4 commons
#define SEGMENT_NUMBER  25<<8  //26 segments

// initialize the LCD driver (clock, common, segments...)
void Lcd_Init(void)
{
  if (AT91C_BASE_SLCDC -> SLCDC_SR == AT91C_SLCDC_ENA) return;
  AT91C_BASE_SLCDC -> SLCDC_CR = AT91C_SLCDC_SWRST;
  
  AT91C_BASE_SLCDC -> SLCDC_MR = COMMON_NUMBER | AT91C_SLCDC_BUFTIME_0_percent | AT91C_SLCDC_BIAS_1_3 | SEGMENT_NUMBER;
  AT91C_BASE_SLCDC -> SLCDC_FRR = AT91C_SLCDC_PRESC_SCLK_32 | AT91C_SLCDC_DIV_1;
  AT91C_BASE_SLCDC -> SLCDC_CR = AT91C_SLCDC_LCDEN;

  //while (pSLCDC -> SLCDC_SR != AT91C_SLCDC_ENA); //TODO: put back...
}

// disable the LCD drivers...
void Lcd_Disable()
{
  AT91C_BASE_SLCDC->SLCDC_CR = AT91C_SLCDC_LCDDIS; // BDs
  AT91C_BASE_SUPC->SUPC_MR = (0xA5UL << 24) | (AT91C_BASE_SUPC->SUPC_MR & ~AT91C_SUPC_LCDMODE); // BDs
  while ((AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_LCDS) == AT91C_SUPC_LCDS); // BDs
}

void Lcd_SetContrast(int contrast)
{
  AT91C_BASE_SUPC->SUPC_MR = (((AT91C_BASE_SUPC->SUPC_MR)|(0xA5UL << 24))&0xFFFFFFF0)|(contrast&0xf);
}

// sets default values for LCD...
// including power supply....
void LCDMODE_Select(int LCDMODE)
{
  AT91C_BASE_SUPC->SUPC_MR = ((AT91C_BASE_SUPC->SUPC_MR|(0xA5UL << 24))&0xFFFFFFCF)|LCDMODE;

  if (LCDMODE==AT91C_SUPC_LCDMODE_OFF || LCDMODE==AT91C_SUPC_LCDMODE_OFF_AFTER_EOF)
    while( (AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_LCDS) ); 
  else
      while( !(AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_LCDS) ); 
}

void Lcd_Enable(int contrast)
{
  LCDMODE_Select(AT91C_SUPC_LCDMODE_INTERNAL); 
  memset((unsigned int *) (AT91C_SLCDC_MEM), 10*2*4, 0); // set all pixels to blank
  Lcd_Init();
  Lcd_SetContrast(contrast);
}


