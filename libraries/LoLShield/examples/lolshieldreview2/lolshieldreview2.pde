/* 



This file was dynamically created by the Lol Shield Theatre: http://falldeaf.com/lolshield/
Feel free to drop by and create your own cinema masterpiece :)

-falldeaf



Animation information - 
///////////////////////// 
//title: demo animation 
//author: tronixstuff 
//description: Vegas  eat your heart out 
///////////////////////// 
//current score: 0 (as of Wednesday 7th of September 2011 03:49:44 AM ) 
//animation page at: http://falldeaf.com/lolshield/show.php?anim=433 
///////////////////////// 


 This program is a modification of the Basic LoL Shield Test

 

 Modified by falldeaf on 2/27/2011.

 

 Writen for the LoL Shield, designed by Jimmie Rodgers:

 http://jimmieprodgers.com/kits/lolshield/

 

 This needs the Charliplexing library, which you can get at the

 LoL Shield project page: http://code.google.com/p/lolshield/

 

 Created by Jimmie Rodgers on 12/30/2009.

 Adapted from: http://www.arduino.cc/playground/Code/BitMath

 

 History:

  	December 30, 2009 - V1.0 first version written at 26C3/Berlin



  This is free software; you can redistribute it and/or

  modify it under the terms of the GNU Version 3 General Public

  License as published by the Free Software Foundation; 

  or (at your option) any later version.



  This library is distributed in the hope that it will be useful,

  but WITHOUT ANY WARRANTY; without even the implied warranty of

  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU

  Lesser General Public License for more details.



  You should have received a copy of the GNU Lesser General Public

  License along with this library; if not, write to the Free Software

  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA



*/ 


#include <Charliplexing.h> //Imports the library, which needs to be



byte line = 0;       //Row counter

char buffer[10];

int value;



void setup() 

{

  LedSign::Init();  //Initializes the screen

}



void loop()

{



	delay(100);
	DisplayBitMap(16383);
	DisplayBitMap(8193);
	DisplayBitMap(8193);
	DisplayBitMap(8193);
	DisplayBitMap(8193);
	DisplayBitMap(8193);
	DisplayBitMap(8193);
	DisplayBitMap(8193);
	DisplayBitMap(16383);
	delay(100);
	DisplayBitMap(0);
	DisplayBitMap(8190);
	DisplayBitMap(4098);
	DisplayBitMap(4098);
	DisplayBitMap(4098);
	DisplayBitMap(4098);
	DisplayBitMap(4098);
	DisplayBitMap(8190);
	DisplayBitMap(0);
	delay(100);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(4092);
	DisplayBitMap(2052);
	DisplayBitMap(2052);
	DisplayBitMap(2052);
	DisplayBitMap(4092);
	DisplayBitMap(0);
	DisplayBitMap(0);
	delay(100);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(4088);
	DisplayBitMap(2056);
	DisplayBitMap(4088);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	delay(100);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(2032);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	delay(100);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(992);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	delay(100);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(448);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	delay(100);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(128);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);
	DisplayBitMap(0);


}



void DisplayBitMap(int lineint)

{

  //int data[9] = {95, 247, 123, 511, 255, 1, 5, 31, 15};

  

  //for(line = 0; line < 9; line++) {

  for (byte led=0; led<14; ++led) {

    if (lineint & (1<<led)) {

      LedSign::Set(led, line, 1);

    } else {

      LedSign::Set(led, line, 0);

    }

  }

    

  line++;

  if(line >= 9) line = 0;

}
