/*
||
|| @file Menu.h
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making menus
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#ifndef MENU_H
#define MENU_H

#include "Arduino.h"
#include "interfaces/MenuInterface.h"
#include "interfaces/MenuItemInterface.h"
#include "MenuItem.h"

#define ItemChangeEvent MenuItemInterface*
#define ItemUseEvent MenuItemInterface*

#define MAXIMUM_MENU_ITEMS 10 

class Menu : public MenuInterface { 
    public:
        Menu( void (*onMenuUse)(MenuItemInterface*) );
        Menu( void (*onMenuUse)(MenuItemInterface*) , void (*onMenuChange)(MenuItemInterface*));
        
        virtual void up();
        virtual void down();
        virtual void previous();
        virtual bool select(byte select);
        virtual void use();
        virtual MenuItemInterface* getCurrentItem();
        
        virtual bool addMenuItem( MenuItem& menuItem );
        
        bool isCurrentSubMenu();
    private:
        virtual void setCurrentIndex( byte select );
        void (*menuUse)(MenuItemInterface*);
        void (*menuChange)(MenuItemInterface*);
        byte previousIndex;
        byte currentIndex;
        byte numberOfMenuItems;
        MenuItem* item[MAXIMUM_MENU_ITEMS];
};

#endif

/*
|| @changelog
|| | 1.0 2009-04-22 - Alexander Brevig : Initial Release
|| #
*/