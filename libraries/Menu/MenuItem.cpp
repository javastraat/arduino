/*
||
|| @file MenuItem.cpp
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making menuitems
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

#include "MenuItem.h"

MenuItem::MenuItem(){
    subMenu = 0;
}

MenuItemInterface* MenuItem::use(){
    if (isSubMenu()){
        return subMenu->use();
    }else{
        return this;
    }
}

bool MenuItem::isSubMenu(){
    if (subMenu!=0) {
        return true;
    } 
    return false;
}

void MenuItem::addSubMenu(SubMenu& userSubMenu){
    subMenu = &userSubMenu;
}

/*
|| @changelog
|| | 1.0 2009-04-22 - Alexander Brevig : Initial Release
|| #
*/