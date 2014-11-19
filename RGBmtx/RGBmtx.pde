/*
 * RGBmtx - an RGB Matrix Editor/Controller v1.0
 *
 * hacked from
 *   mtXcontrol - a LED Matrix Editor - version 1.1
 */

//scl
public static class Config {
  public static int xdim = 8;
  public static int ydim = 8;
  public static boolean rgbmode = true;
}
PFont fontA;
PFont fontLetter;

Matrix matrix;
Device device;

int border = 10;

int offY = 30;
int offX = 30;

int current_delay = 0;
int current_speed = 5;
int prev_speed = -1;

// values for current_mode
int mode_record = 0;
int mode_play   = 1;
int mode_plasma = 2;
int mode_snow   = 3;
int mode_meteor = 4;
int mode_music  = 5; // music sync
int mode_cnt = 6;
int current_mode = mode_record;
boolean wait_frame_queue_empty = false;

boolean scanForDevice = true;

boolean record  = true;
boolean color_mode  = false;
boolean rain = true;

boolean keyCtrl = false;
boolean keyMac  = false;
boolean keyAlt  = false;

//scl
int colorBtnStart;
int colorBtnEnd;

boolean update = true;
Button[] buttons;

int hide_button_index;

/* +++++++++++++++++++++++++++++ */

void setup() {
  frame.setIconImage( getToolkit().getImage("sketch.ico") );

//scl  matrix = new Matrix(8, 8);
  matrix = new Matrix(Config.xdim, Config.ydim);
  matrix.current_color.set_color(255,0,0);

  size(780,720);
  smooth();
  noStroke();
  fontA = loadFont("Courier-Bold-32.vlw");
  fontLetter = loadFont("ArialMT-20.vlw");
  frameRate(15);
}

void setup_buttons() {
  buttons = new Button[60]; // width + height + ???
  int offset = 10;
  int button_index = 0;
  int y_pos = 0;

color button_color = #333333;
color button_color_over = #999999;
  int button_size = 15;

  for(int i = 0; i < matrix.rows; i++ ) {
    int x = offX + matrix.width() + offset;
    int y = offY + i * matrix.rad + matrix.border / 2;
    buttons[button_index++] = new RectButton( x, y, button_size, matrix.rad - matrix.border, button_color, button_color_over);
  }
  for(int i = 0; i < matrix.cols; i++ ) {
    int x = offX + i * matrix.rad + matrix.border / 2;
    int y = offY + matrix.width() + offset;
    buttons[button_index++] = new RectButton( x, y, matrix.rad - matrix.border, button_size, button_color, button_color_over);
  }
  buttons[button_index++] = new SquareButton( offX + matrix.width() + offset, offY + matrix.width() + offset, button_size, button_color, button_color_over );

  int button_x = offX + matrix.width() + offset + 30;
//  buttons[button_index++] = new ActionToggleButton( "Mode: RECORD",  "Mode: PLAY",    "10",   button_x, y_pos += 30);
  buttons[button_index++] = new ModeButton( "Mode: ",    "10",   button_x, y_pos += 30);
  buttons[button_index++] = new ActionToggleButton( "Device: SLAVE",  "Device: FREE", "a+10", button_x, y_pos += 30);
  if(! (device instanceof StandaloneDevice && device.enabled()) ) buttons[button_index-1].disable();

  buttons[button_index++] = new FrameChooser(offX, offY + matrix.height() + 65, 59, 10);

  hide_button_index = button_index;
  buttons[button_index++] = new TextElement( "Load from:", button_x, y_pos += 30);
  buttons[button_index++] = new ActionButton( "File",    "m+L", button_x,      y_pos += 30, 65, 25);
 //scl buttons[button_index++] = new ActionButton( "Device",  "a+L", button_x + 67, y_pos,       65, 25);
  //  if(! (device instanceof StandaloneDevice && device.enabled()) ) buttons[button_index-1].disable();

  buttons[button_index++] = new TextElement( "Save to:", button_x, y_pos += 30);
  buttons[button_index++] = new ActionButton( "File",    "m+S", button_x,      y_pos += 30, 65, 25);
  buttons[button_index++] = new ActionButton( "Device",  "a+S", button_x + 67, y_pos,       65, 25);
  if(! (device instanceof StandaloneDevice && device.enabled()) ) buttons[button_index-1].disable();

  buttons[button_index++] = new TextElement( "Color:", button_x, y_pos += 40);
  y_pos += 30;
  // scl
  colorBtnStart = button_index;
  int off = 140/8;
  PixelColor pc = new PixelColor(0,0,0);
  int i =0;
  buttons[button_index++] = new MiniColorButton( button_x + i++ * off, y_pos, off, 20, pc );
  pc = new PixelColor(255,0,0);
  buttons[button_index++] = new MiniColorButton( button_x + i++ * off, y_pos, off, 20, pc );
  pc = new PixelColor(0,255,0);
  buttons[button_index++] = new MiniColorButton( button_x + i++ * off, y_pos, off, 20, pc );
  pc = new PixelColor(0,0,255);
  buttons[button_index++] = new MiniColorButton( button_x + i++ * off, y_pos, off, 20, pc );
  pc = new PixelColor(255,255,0);
  buttons[button_index++] = new MiniColorButton( button_x + i++ * off, y_pos, off, 20, pc );
  pc = new PixelColor(0,255,255);
  buttons[button_index++] = new MiniColorButton( button_x + i++ * off, y_pos, off, 20, pc );
  pc = new PixelColor(255,0,255);
  buttons[button_index++] = new MiniColorButton( button_x + i++ * off, y_pos, off, 20, pc );
  pc = new PixelColor(255,255,255);
  buttons[button_index++] = new MiniColorButton( button_x + i++ * off, y_pos, off, 20, pc );
  colorBtnEnd = button_index;


  buttons[button_index++] = new TextElement( "Frame:", button_x, y_pos += 20);  
  buttons[button_index++] = new ActionButton( "Add",    " ", button_x, y_pos += 30);
  buttons[button_index++] = new ActionButton( "Delete", "D", button_x, y_pos += 30);

  buttons[button_index++] = new ActionButton( "^", "c+38", button_x + 47,  y_pos += 50, 40, 25);
  buttons[button_index++] = new ActionButton( "<", "c+37", button_x,       y_pos += 20, 40, 25);
  buttons[button_index++] = new ActionButton( ">", "c+39", button_x + 94,  y_pos,       40, 25);
  buttons[button_index++] = new ActionButton( "v", "c+40", button_x + 47,  y_pos += 15, 40, 25);

  buttons[button_index++] = new ActionButton( "Paste",  "m+V", button_x, y_pos += 50);
  buttons[button_index++] = new ActionButton( "Copy",   "m+C", button_x, y_pos += 30);
  buttons[button_index++] = new ActionButton( "Fill",   "F",   button_x, y_pos += 30);
  buttons[button_index++] = new ActionButton( "Clear",  "X",   button_x, y_pos += 30);

  //buttons[button_index++] = new ActionButton( "Plasma",  "P",   button_x, y_pos += 40);
  //buttons[button_index++] = new ActionButton( "Music Sync",  "M",   button_x, y_pos += 30);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void draw()
{  

  if (scanForDevice) {
    // can't scan for device in setup() because delay() doesn't work there
    device = new ColorduinoDevice(this); //scl
    device.setColorScheme();
    if(device.enabled()) {
        ((ColorduinoDevice) device).setModePlayFrame();
    }

    setup_buttons();
    scanForDevice = false;
  }

  if(update) {
    background(45);
    fill(50);
    rect(offX - matrix.border / 2, offY - matrix.border / 2, matrix.width() + matrix.border, matrix.height() + matrix.border);
    image(matrix.current_frame_image(), offX, offY);
    
    for(int i = 0; i < buttons.length; i++ ) {
      if(buttons[i] == null) break;
      buttons[i].display();
    }

    fill(255); //white
    if(!record) {
      text("Speed: " + current_speed, offX + matrix.width() + 65, 110);
    }

    if(!device.enabled()) {
      text("No output device found, running in standalone mode", 120, 20);  
    }

    device.write_frame(matrix.current_frame());
    update = false;
  }

  if(!record && (current_mode != mode_plasma)) next_frame();
}


/* +++++++++++++++++++++++++++++ */

void next_frame() {
    if (wait_frame_queue_empty) {
	// Colorduino queue is full.  It will not return a status value until
	// the queue has space.
	if (((ColorduinoDevice) device).waitForResponse() != -1) { // not timeout
	    // got a response
	    wait_frame_queue_empty = false;
	}
    }
    else {
	if(current_delay < 1) {
	    current_delay = current_speed;
	    if (current_mode == mode_snow) {
		matrix.snow();
	    }
	    else if (current_mode == mode_meteor) {
		matrix.meteor();
	    }
	    else {
		matrix.next_frame();
	    }
	    mark_for_update();
	}
	current_delay--;
    }
}

/* +++++++++++++++ ACTIONS +++++++++++++++ */
void mouseDragged() {
  if(!record) return; // ! record
  if(matrix.click(mouseX - offX, mouseY - offY, true)) mark_for_update();
}

void mousePressed() {
  if(!record) return; //!record
  if(matrix.click(mouseX - offX, mouseY - offY, false)) mark_for_update();
}

void mouseMoved() {
  for(int i = 0; i < buttons.length; i++ ) {
    if(buttons[i] == null) break;
    if(buttons[i].over()) mark_for_update();
  }
}

void mouseClicked() {
  for(int i = 0; i < buttons.length; i++ ) {
    if(buttons[i] == null) break;
    if(buttons[i].clicked()) mark_for_update();
  }
}


void keyPressed() {
 // print(keyCode);
  if(keyCode == 17)  keyCtrl = true; //control
  if(keyCode == 18)  keyAlt  = true; //alt
  if(keyCode == 157) keyMac  = true; //mac
  if(keyCode == 67) color_mode = true; //C
  
  //println("pressed " + key + " " + keyCode + " " +keyMac+ " "+  keyCtrl + " "+ keyAlt );

  if(color_mode) {
 //notyet    if(keyCode == 37) matrix.current_color.previous_color(); //arrow left
 //notyet    if(keyCode == 39) matrix.current_color.next_color();  //arrow right   
     mark_for_update();
     return;
  }

  for(int i = 0; i < buttons.length; i++ ) {
    if(buttons[i] == null) break;
    if(buttons[i].key_pressed(keyCode, keyMac, keyCtrl, keyAlt)) {
      mark_for_update();  
      return;
    }
  }
   
  if(keyAlt) {
    if(device instanceof StandaloneDevice) {
      if(keyCtrl) {
        if(keyCode == 37) ((StandaloneDevice) device).brightnessDown();   //arrow left
        if(keyCode == 39) ((StandaloneDevice) device).brightnessUp(); //arrow right        
      }
      else {        
        if(keyCode == 37) ((StandaloneDevice) device).speedUp();   //arrow left
        if(keyCode == 39) ((StandaloneDevice) device).speedDown(); //arrow right        
      }
    }
  }
  else if(keyCtrl) {
    PixelColor pc = null;
    if(keyCode >= 48) pc = matrix.current_frame().set_letter(char(keyCode), fontLetter, matrix.current_color);
    if( pc != null )  {
      matrix.current_color = pc;
      mark_for_update(); 
    }
    return;
  }
  else {
    if(!record && (current_mode != mode_music)) {
      if( keyCode == 40) speed_up();     //arrow up
      if( keyCode == 38) speed_down();   //arrow down
    }
  }
}

void keyReleased() {
  if( keyCode == 17 )  keyCtrl = false;
  if( keyCode == 18 )  keyAlt  = false;
  if( keyCode == 157 ) keyMac  = false;
  if( keyCode == 67 ) color_mode = false;
}

void mark_for_update() {
  update = true;
}

/* +++++++++++++++ modes +++++++++++++++ */
void toggle_mode() {
  matrix.current_frame_nr = 0;
  record = !record;
  for(int i = hide_button_index; i < buttons.length; i++ ) {
    if(buttons[i] == null) break;
    if(record) buttons[i].toggle(); 
    else buttons[i].hide();
  }
  if(record) buttons[hide_button_index-1].enable(); 
  else buttons[hide_button_index-1].disable();
}

void set_mode(int mode) {
  wait_frame_queue_empty = false;

  if (((current_mode == mode_snow) && (mode != mode_snow)) ||
      ((current_mode == mode_meteor) && (mode != mode_meteor))) {
      matrix.delete_first_frame();
  }

  current_mode = mode;
  if (mode == mode_record) {
    record = true;
  }
  else {
    record = false;
  }

  if ((mode == mode_snow) || (mode == mode_meteor)) {
      matrix.insert_first_frame();
  }

  matrix.current_frame_nr = 0;

  for(int i = hide_button_index; i < buttons.length; i++ ) {
    if(buttons[i] == null) break;
    if(mode == 0) { // record
      buttons[i].enable();
       buttons[i].show();
     }
    else buttons[i].hide();
  }
  if(mode == mode_record) buttons[hide_button_index-1].enable();  // record
  else buttons[hide_button_index-1].disable();

  if (prev_speed != -1) {
      current_speed = prev_speed;
      prev_speed = -1;
  }

  
  if (device.enabled())  { 
    if(mode == mode_plasma) { // plasma
      ((ColorduinoDevice) device).setModePlasma();
    }
    else if (mode == mode_music) {
	prev_speed = current_speed;
	current_speed = -1;
      ((ColorduinoDevice) device).setModeMusicSync();
    }
    else {
      ((ColorduinoDevice) device).setModePlayFrame();
    }
  }

  if ((mode == mode_snow) || (mode == mode_meteor)) {
      prev_speed = current_speed;
      current_speed = 1;
  }
}

void speed_up() {
  if( current_speed <= 0) return;
  current_speed--;
}

void speed_down() {
  current_speed++;
}







