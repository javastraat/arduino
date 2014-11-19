/*
|| 
|| @description
|| | This is a simple menu system, that uses input from the serial to change and use menuitems.
|| #
|| 
*/

#include <SubMenuItem.h>
#include <SubMenu.h>
#include <MenuItem.h>
#include <Menu.h>
//initialize menu
Menu menu = Menu(menuUsed,menuChanged); 
//initialize menuitems
  MenuItem menuItem1 = MenuItem();
  MenuItem menuItemWithSubmenu = MenuItem();
    SubMenu subMenu = SubMenu(menuChanged);
      SubMenuItem subMenuItem1 = SubMenuItem();
      SubMenuItem subMenuItem2 = SubMenuItem(); 
      SubMenuItem subMenuItem3 = SubMenuItem();
  MenuItem menuItem3 = MenuItem();

void setup(){
  Serial.begin(9600);
  
  //configure menu
  menu.addMenuItem(menuItem1);
  menu.addMenuItem(menuItemWithSubmenu);
    menuItemWithSubmenu.addSubMenu(subMenu);
      subMenu.addSubMenuItem(subMenuItem1);
      subMenu.addSubMenuItem(subMenuItem2);
      subMenu.addSubMenuItem(subMenuItem3);
  menu.addMenuItem(menuItem3);
  
  menu.select(0);
  subMenu.select(0);
}
void loop(){
  if (Serial.available()){
    switch (Serial.read()){  
      case 'w':
        if (menu.isCurrentSubMenu()){ 
          subMenu.down(); //set index closer to 0
        }
        break;
      case 's':
        if (menu.isCurrentSubMenu()){
          subMenu.up(); //set index closer to maximum
        }
        break;
      case 'a':
        menu.down();
        break;
      case 'd':
        menu.up();
        break;
      case ' ':
        menu.use();
        break;
      case '0':
        menu.select(0);
        break;
      case '1':
        menu.select(1);
        break;
      case '2':
        menu.select(2);
        break;
    }
  }
}

void menuChanged(ItemChangeEvent event){
  Serial.println("menuChanged"); 
  if( event == &menuItem1 ){
    Serial.println("\tmenuItem1 selected"); //user feedback
  }else if( event == &menuItemWithSubmenu ){
    Serial.println("\tmenuItemWithSubmenu selected"); //user feedback
  }else if( event == &subMenuItem1 ){
    Serial.println("\tsubMenuItem1 selected"); //user feedback
  }else if( event == &subMenuItem2 ){
    Serial.println("\tsubMenuItem2 selected"); //user feedback
  }else if( event == &subMenuItem3 ){
    Serial.println("\tsubMenuItem3 selected"); //user feedback
  }else if( event == &menuItem3 ){
    Serial.println("\tmenuItem3 selected"); //user feedback
  }
}

void menuUsed(ItemUseEvent event){
  Serial.println("menuUsed"); 
  if( event == &menuItem1 ){
    Serial.println("\tmenuItem1 used"); //user feedback
  }else if( event == &subMenuItem1 ){
    Serial.println("\tsubMenuItem1 used"); //user feedback
  }else if( event == &subMenuItem2 ){
    Serial.println("\tsubMenuItem2 used"); //user feedback
  }else if( event == &subMenuItem3 ){
    Serial.println("\tsubMenuItem3 used"); //user feedback
  }else if( event == &menuItem3 ){
    Serial.println("\tmenuItem3 used"); //user feedback
  }
}
