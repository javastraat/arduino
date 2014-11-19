/*
||
|| @file Menu.cpp
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

#include "Menu.h"

Menu::Menu(  void (*onMenuUse)(MenuItemInterface*) ){
    menuUse = onMenuUse;
    menuChange = 0;
    previousIndex = currentIndex = numberOfMenuItems = 0;
    item[currentIndex] = 0;
}

Menu::Menu(  void (*onMenuUse)(MenuItemInterface*) , void (*onMenuChange)(MenuItemInterface*) ){
    menuUse = onMenuUse;
    menuChange = onMenuChange;
    previousIndex = currentIndex = numberOfMenuItems = 0;
    item[currentIndex] = 0;
}

void Menu::up(){
    select(currentIndex+1);
}
void Menu::down(){
    select(currentIndex-1);
}

void Menu::previous(){
    select(previousIndex);
}

bool Menu::select(byte select){
    if (select>=0 && select<numberOfMenuItems){
        setCurrentIndex(select);
        if (menuChange!=0){
            menuChange( item[currentIndex]->use() );
        }
        return true;
    }
    return false;
}

void Menu::use(){
    menuUse(item[currentIndex]->use());
}

MenuItemInterface* Menu::getCurrentItem(){
    return item[currentIndex];
}

bool Menu::addMenuItem( MenuItem& menuItem ){
    if (numberOfMenuItems+1<MAXIMUM_MENU_ITEMS){
        item[numberOfMenuItems++] = &menuItem;
        return true;
    }
    return false;
}

bool Menu::isCurrentSubMenu(){ return item[currentIndex]->isSubMenu(); }

//private
void Menu::setCurrentIndex( byte select ){
    if (select>=0 && select<numberOfMenuItems){
        previousIndex = currentIndex;
        currentIndex = select;
    }
}

/*
|| @changelog
|| | 1.0 2009-04-22 - Alexander Brevig : Initial Release
|| #
*/