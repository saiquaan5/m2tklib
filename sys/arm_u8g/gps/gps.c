/*

  gps.c 
  
  Main file of the GPS tracker device.
  
  m2tklib - Mini Interative Interface Toolkit Library
  u8glib - Universal 8bit Graphics Library
  
  Copyright (C) 2013  olikraus@gmail.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
*/

#include "u8g.h"
#include "u8g_arm.h"
#include "m2.h"
#include "m2ghu8g.h"

/*========================================================================*/
/* SystemInit & SysTick Interrupt */

#define SYS_TICK_PERIOD_IN_MS 10

void SystemInit()
{    
  init_system_clock();		/* SystemCoreClock will be set here */
  
  /* SysTick is defined in core_cm0.h */
  SysTick->LOAD = (SystemCoreClock/1000UL*(unsigned long)SYS_TICK_PERIOD_IN_MS) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
}

void __attribute__ ((interrupt)) SysTick_Handler(void)
{
}

/*=========================================================================*/
/* menu definitions */

uint8_t select_color = 0;

void fn_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
}

void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
}

M2_LABEL(el_label1, NULL, "red");
M2_RADIO(el_radio1, "v0", &select_color);

M2_LABEL(el_label2, NULL, "green");
M2_RADIO(el_radio2, "v1", &select_color);

M2_LABEL(el_label3, NULL, "blue");
M2_RADIO(el_radio3, "v2", &select_color);

M2_BUTTON(el_cancel, NULL, "cancel", fn_cancel);
M2_BUTTON(el_ok, NULL, " ok ", fn_ok);

M2_LIST(list) = { 
    &el_label1, &el_radio1, 
    &el_label2, &el_radio2,  
    &el_label3, &el_radio3, 
    &el_cancel, &el_ok 
};
M2_GRIDLIST(el_top, "c2",list);

/*=== extern declarations ===*/
M2_EXTERN_ALIGN(el_home);

/*=== reuseable elements ===*/

M2_ROOT(el_goto_home, NULL, "Home", &el_home);

/*=== test gps ===*/

M2_ALIGN(el_test_gps, NULL, &el_goto_home);

/*=== test compass ===*/

M2_ALIGN(el_test_compass, NULL, &el_goto_home);

/*=== toplevel menu ===*/

M2_ROOT(el_home_test_gps, NULL, "GPS" , &el_test_gps);
M2_ROOT(el_home_test_compass, NULL, "Compass", &el_test_compass);
M2_LIST(list_home) = {
  &el_home_test_gps,
  &el_home_test_compass
};
M2_VLIST(el_home_vlist, NULL, list_home);
M2_ALIGN(el_home, NULL, &el_home_vlist);


/*=========================================================================*/
/* global variables and objects */

u8g_t u8g;

/*=========================================================================*/
/* controller, u8g and m2 setup */

void setup(void)
{  

  /* eval board */
  /*
  u8g_pin_a0 = PIN(1,0);
  u8g_pin_cs = PIN(0,8);
  u8g_pin_rst = PIN(0,6);
  */

  /* GPS2 Board */
  u8g_pin_a0 = PIN(1,0);
  u8g_pin_cs = PIN(0,8);
  u8g_pin_rst = PIN(0,6);
  
  /* 1. Setup and create u8g device */
  u8g_InitComFn(&u8g, &u8g_dev_uc1701_dogs102_hw_spi, u8g_com_hw_spi_fn);
  u8g_SetFontRefHeightAll(&u8g);

  /* 2. Setup m2 */
  m2_Init(&el_home, m2_es_arm_u8g, m2_eh_4bs, m2_gh_u8g_bfs);

  /* 3. Connect u8g display to m2  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. Set a font, use normal u8g_font's */
  //m2_SetFont(0, (const void *)u8g_font_6x10r);
  m2_SetFont(0, (const void *)u8g_font_helvB08r);
	
  /* 5. Define keys */
    
  /* ARM LPC1114 GPS2 Board */
  m2_SetPin(M2_KEY_PREV, PIN(0, 1));
  m2_SetPin(M2_KEY_SELECT, PIN(1, 2));
  m2_SetPin(M2_KEY_NEXT, PIN(0, 11));
  
  u8g_SetRot180(&u8g);  
}

/*=========================================================================*/
/* system setup */

void sys_init(void)
{
#if defined(__AVR__)
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
#endif
}

/*=========================================================================*/
/* u8g draw procedure (body of picture loop) */

/* draw procedure of the u8g picture loop */
void draw(void)
{	
  /* call the m2 draw procedure */
  m2_Draw();
}

/*=========================================================================*/
/* main procedure with u8g picture loop */

int main(void)
{
  /* setup controller */
  sys_init();
	
  /* setup u8g and m2 libraries */
  setup();

  /* application main loop */
  for(;;)
  {  
    m2_CheckKey();
    if ( m2_HandleKey() ) 
    {
      /* picture loop */
      u8g_FirstPage(&u8g);
      do
      {
	draw();
        m2_CheckKey();
      } while( u8g_NextPage(&u8g) );
    }
  }  
}
