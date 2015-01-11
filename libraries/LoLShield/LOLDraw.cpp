/*
	LOLDraw - a library for the Olimexino STM32 + LoLShield
			  for drawing basic shapes
			- Depends on the Charliplexing and math libraries.
	
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

#include "LOLDraw.h"
#include "math.h"
#include "Charliplexing.h"

void LOLDraw::Point(unsigned char x, unsigned char y)
{
	if(x>=0 && x<=13 && y>=0 && y<=8)
	    LedSign::Set(x,y,1);
}

void LOLDraw::Line( int x1, int y1, int x2, int y2)
{
	int dx, dy, sx, sy, err, e2;
	dx = fabs(x2-x1);
	dy = fabs(y2-y1);
	if (x1<x2)
		sx = 1;
	else
		sx = -1;
	
	
	if (y1<y2)
		sy = 1;
	else
		sy = -1;
	
	err = dx-dy;
	
	do
	{
		LOLDraw::Point(x1, y1);
		if ((x1 == x2) && (y1 == y2))
			break;
		e2 = 2*err;
		if (e2 > -dy)
		{
			err = err - dy;
			x1 = x1+sx;
		}
		if (e2 < dx)
		{
			err = err + dx;
			y1 = y1 + sy;
		}
	}
	while (1);
	
}


void LOLDraw::Triangle (unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3)
{
    LOLDraw::Line (x1, y1, x2, y2);
	LOLDraw::Line (x2, y2, x3, y3);
	LOLDraw::Line (x3, y3, x1, x1);

}


void LOLDraw::Rectangle( unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    LOLDraw::Line(x1, y1, x1, y2);
	LOLDraw::Line(x1, y1, x2, y1);
	LOLDraw::Line(x2, y1, x2, y2);
	LOLDraw::Line(x1, y2, x2, y2);

}

void LOLDraw::Draw_4_Ellipse_Points(unsigned char CX, unsigned char CY, unsigned char X, unsigned char Y)
{
	LOLDraw::Point(CX+X, CY+Y);
	LOLDraw::Point(CX-X, CY+Y);
	LOLDraw::Point(CX-X, CY-Y);
	LOLDraw::Point(CX+X, CY-Y);

}

void LOLDraw::Ellipse(unsigned char CX, unsigned char CY, unsigned char XRadius, unsigned char YRadius)
{
	
	int X, Y, XChange, YChange, EllipseError, TwoASquare, TwoBSquare, StoppingX, StoppingY;
	TwoASquare = 2 * XRadius*XRadius;
	TwoBSquare = 2 * YRadius*YRadius;
	X = XRadius;
	Y = 0;
	XChange = YRadius*YRadius * (1-2*XRadius);
	YChange = XRadius*XRadius;
	EllipseError = 0;
	StoppingX = TwoBSquare*XRadius;
	StoppingY = 0;

	while (StoppingX >= StoppingY)	// 1st set of points, y'> -1
	{
		LOLDraw::Draw_4_Ellipse_Points (CX, CY, X, Y);
		Y++;
		StoppingY = StoppingY + TwoASquare;
		EllipseError = EllipseError + YChange;
		YChange = YChange + TwoASquare;
		if ((2*EllipseError + XChange) > 0)
		{
			X--;
			StoppingX = StoppingX - TwoBSquare;
			EllipseError = EllipseError + XChange;
			XChange = XChange + TwoBSquare;
		}
	}

	// 1st point set is done; start the 2nd set of points

	X = 0;
	Y = YRadius;
	XChange = YRadius*YRadius;
	YChange = XRadius*XRadius * (1-2*YRadius);
	EllipseError = 0;
	StoppingX = 0;
	StoppingY = TwoASquare * YRadius;

	while (StoppingX <= StoppingY)	// 2nd set of points, y'< -1
	{
		LOLDraw::Draw_4_Ellipse_Points (CX, CY, X, Y);
		X++;
		StoppingX = StoppingX + TwoBSquare;
		EllipseError = EllipseError + XChange;
		XChange = XChange + TwoBSquare;
		if ((2*EllipseError + YChange) > 0)
		{
			Y--;
			StoppingY = StoppingY - TwoASquare;
			EllipseError = EllipseError + YChange;
			YChange = YChange + TwoASquare;
		}	
	}
}

void LOLDraw::Circle(unsigned char x, unsigned char y, unsigned char r)
{
	LOLDraw::Ellipse(x, y, r, r);

}