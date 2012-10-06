/*

  RotEnc.pde
  
  LiquidCrystal 16x4 example (Incremental Rotary Encoder)

  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2011  olikraus@gmail.com

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

#include <LiquidCrystal.h>
#include "M2tk.h"
#include "m2ghlc.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_ALIGN(top_menu);

//=================================================
// Simple dialog: Input two values n1 and n2

uint8_t n1 = 0;
uint8_t n2 = 0;

M2_LABEL(el_l1, NULL, "value 1:");
M2_U8NUM(el_u1, "c2", 0, 99, &n1);
M2_LABEL(el_l2, NULL, "value 2:");
M2_U8NUM(el_u2, "c2", 0, 99, &n2);

M2_LIST(list) = { &el_l1, &el_u1, &el_l2, &el_u2 };
M2_GRIDLIST(el_gridlist, "c2", list);
M2_ALIGN(top_menu, "-1|1W64H64", &el_gridlist);

// m2 object and constructor
// Note: Use the "m2_eh_4bd" handler, which fits better to the "m2_es_arduino_rotary_encoder" 
M2tk m2(&top_menu, m2_es_arduino_rotary_encoder, m2_eh_4bd, m2_gh_lc);

//=================================================
// Arduino Setup & Loop

void setup() {
  m2_SetLiquidCrystal(&lcd, 16, 4);
  
  // define button for the select message
  m2.setPin(M2_KEY_SELECT, 0); 
  
  // The incremental rotary encoder is conected to these two pins
  m2.setPin(M2_KEY_ROT_ENC_A, 10);
  m2.setPin(M2_KEY_ROT_ENC_B, 9);
}

void loop() {
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}
