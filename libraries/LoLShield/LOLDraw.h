/*
	LOLDraw - a library for the Olimexino STM32 + LoLShield
			  for drawing basic shapes
			- Depends on the Charliplexing library.
	
	If you have any questions, email
	support@olimex.com
		
	OLIMEX, July 2012
	http://www.olimex.com/dev

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.

*/
#ifndef	LOLDRAW_H
#define	LOLDRAW_H

#include "Charliplexing.h"

namespace LOLDraw
{
	/* x and y are the coordinates of a sigle diode to be lit 
	   where x is the column and y the row */
	void Point( unsigned char x, unsigned char y);
	
	/* Draws a line between diodes (x1;y1) and (x2,y2) */
	void Line( int x1, int y1, int x2, int y2);
	
	/* Draws a triangle between (x1;y1), (x2,y2) and (x3,y3) */
	void Triangle (unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3);
	
	/* Draws a rectangle using (x1;y1) as the top left corner and 
	   (x2;y2) as the bottom right corner */
	void Rectangle( unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
	
	/* Draws an Ellipse using (CX,CY) as a center and XRadius and YRadius
       as the two radiuses */	
	void Ellipse(unsigned char CX, unsigned char CY, unsigned char XRadius, unsigned char YRadius);
	
	/* A helper function for the Ellipse function; not to be called directly */
	void Draw_4_Ellipse_Points(unsigned char CX, unsigned char CY, unsigned char X, unsigned char Y);
	
	/* Draws a Circle using (x;y) as a center and r as the radius */
	void Circle(unsigned char x, unsigned char y, unsigned char r);	
}
#endif