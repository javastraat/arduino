/*
||
|| @file SubMenu.cpp
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making submenus
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

#include "SubMenu.h"


SubMenu::SubMenu(){
    previousIndex = currentIndex = numberOfSubMenuItems = 0;
    item[currentIndex] = 0;
    menuChange = 0;
}

SubMenu::SubMenu( void (*onMenuChange)(MenuItemInterface*)){
    previousIndex = currentIndex = numberOfSubMenuItems = 0;
    item[currentIndex] = 0;
    menuChange = onMenuChange;
}

void SubMenu::up(){
    select(currentIndex+1);
}

void SubMenu::down(){
    select(currentIndex-1);
}

void SubMenu::previous(){
    select(previousIndex);
}

bool SubMenu::select(byte select){
    if (select>=0 && select<numberOfSubMenuItems){
        setCurrentIndex(select);
        if (menuChange!=0){
            menuChange( item[currentIndex]->use() );
        }
        return true;
    }
    return false;
}

MenuItemInterface* SubMenu::use(){
    return item[currentIndex];
}

bool SubMenu::addSubMenuItem( SubMenuItem& menuItem ){
    if (numberOfSubMenuItems+1<MAXIMUM_SUBMENU_ITEMS){
        item[numberOfSubMenuItems++] = &menuItem;
        return true;
    }
    return false;
}

//private
void SubMenu::setCurrentIndex( byte select ){
    if (select>=0 && select<numberOfSubMenuItems){
        previousIndex = currentIndex;
        currentIndex = select;
    }
}

/*
|| @changelog
|| | 1.0 2009-04-22 - Alexander Brevig : Initial Release
|| #
*/