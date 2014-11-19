#include <Colorduino.h> // 匯入Colorduino程式庫

// 定義按鈕開關的腳位，以及儲存按鈕開關狀態的變數
#define BUTTON_LEFT_PIN 0
#define BUTTON_RIGHT_PIN 0
int button_left_status;
int button_right_status;

// 定義遊戲可調整的參數
// falling_delay代表寶石落下的延遲時間，值越少、越快落下
unsigned long falling_delay = 500; // millisecond
// long button_delay代表按鈕開關的反應速度，
// 單位是毫秒，400代表0.4秒內不管按下幾次都算一次
unsigned long button_delay = 400; // millisecond

// 定義最高回合數，當落下TURN_MAX個寶石後且未失敗，便勝出
#define TURN_MAX 50
int turn;

// 定義顏色值
PixelRGB COLOR_BLACK = {0, 0, 0};
PixelRGB COLOR_RED = {255, 0, 0};
PixelRGB COLOR_GREEN = {0, 255, 0};
PixelRGB COLOR_BLUE = {0, 0, 255};
PixelRGB COLOR_YELLOW = {255, 255, 0};
PixelRGB COLOR_CYAN = {0, 255, 255};
PixelRGB COLOR_MAGENTA = {255, 0, 255};
PixelRGB COLOR_WHITE = {255, 255, 255};

// 定義遊戲中落下寶石的顏色數，
#define COLORS_MAX 3
// 定義想要使用哪些顏色的寶石
PixelRGB colors[COLORS_MAX] = {
  COLOR_RED, COLOR_GREEN, COLOR_BLUE, 
};

// pic_winner二維陣列儲存勝利時應顯示的圖案，pic_loser則是輸掉時
PixelRGB pic_winner[ColorduinoScreenWidth][ColorduinoScreenHeight] = {
  {COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK}, 
  {COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_GREEN}, 
  {COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK}, 
  {COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK}, 
  {COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK}, 
  {COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK}, 
  {COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_GREEN}, 
  {COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_GREEN, COLOR_BLACK},
};

PixelRGB pic_loser[ColorduinoScreenWidth][ColorduinoScreenHeight] = {
  {COLOR_RED, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_BLACK}, 
  {COLOR_BLACK, COLOR_RED, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_RED}, 
  {COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_BLACK}, 
  {COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_BLACK, COLOR_RED, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK}, 
  {COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_BLACK, COLOR_RED, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK}, 
  {COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_BLACK}, 
  {COLOR_BLACK, COLOR_RED, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_RED}, 
  {COLOR_RED, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_RED, COLOR_BLACK}, 
};

// 此二為陣列儲存LED矩陣的資料
PixelRGB screen_data[ColorduinoScreenWidth][ColorduinoScreenHeight];

// 此遊戲程式的主體：狀態機（state machine）所需要的狀態
typedef enum{
  StateStart, // 啟動遊戲
  StateRunPrepare, // 正式開始前做一些準備功夫
  StateRun, // 執行遊戲開始遊玩
  StateWinner, // 勝利了
  StateLoser, // 失敗了
} StateEnum;

StateEnum state;

// 儲存落下寶石的xy座標值與顏色
int movingpixel_x;
int movingpixel_y;
PixelRGB movingpixel_color;

// 比較兩個顏色值
boolean colorCompare(const PixelRGB colorA, const PixelRGB colorB)
{
  if(colorA.r == colorB.r && colorA.g == colorB.g && colorA.b == colorB.b){
    return true;
  }
  return false;
}

// 往左移動落下的寶石
void move_pixel_left()
{
  if(movingpixel_x > 0 && colorCompare(screen_data[movingpixel_x-1][movingpixel_y], COLOR_BLACK) == true){
    screen_data[movingpixel_x][movingpixel_y] = COLOR_BLACK;
    movingpixel_x--;
    screen_data[movingpixel_x][movingpixel_y] = movingpixel_color;
    displayPixelRGBData(screen_data);
  }
}

// 往右移動落下的寶石
void move_pixel_right()
{
  if(movingpixel_x < ColorduinoScreenWidth - 1 && colorCompare(screen_data[movingpixel_x+1][movingpixel_y], COLOR_BLACK) == true){
    screen_data[movingpixel_x][movingpixel_y] = COLOR_BLACK;
    movingpixel_x++;
    screen_data[movingpixel_x][movingpixel_y] = movingpixel_color;
    displayPixelRGBData(screen_data);
  }
}

// 檢查寶石是否連成一線（至少三個，縱向或橫向），並且消除
boolean checkAndClear(int mx, int my)
{
    PixelRGB color = screen_data[mx][my];
    if(colorCompare(color, COLOR_BLACK) == true){
      return false;
    }
    
    //check if anything row-3 or col-3 and clear
    //check horizontal, row
    int pos[8];
    int num;
    int x, y;
    
    num = 0;
    pos[num] = mx;
    num++;
    for(x = mx - 1; x >= 0; x--){
      if(colorCompare(screen_data[x][my], color) == true){
        pos[num] = x;
        num++;
      }
      else{
        break;
      }
    }
    for(x = mx + 1; x < ColorduinoScreenWidth; x++){
      if(colorCompare(screen_data[x][my], color) == true){
        pos[num] = x;
        num++;
      }
      else{
        break;
      }
    }
    if(num >= 3){
      for(int i = 0; i < num; i++){
        screen_data[pos[i]][my] = COLOR_BLACK;
      }
      displayPixelRGBData(screen_data);
      delay(400);
      for(int i = 0; i < num; i++){
        for(y = my + 1; y < ColorduinoScreenHeight; y++){
          screen_data[pos[i]][y - 1] = screen_data[pos[i]][y];
        }
      }
      displayPixelRGBData(screen_data);
      delay(400);
      
      for(int w = 0; w < ColorduinoScreenWidth; w++){
        for(int h = 0; h < ColorduinoScreenHeight; h++){
          if(colorCompare(screen_data[w][h], COLOR_BLACK) == false){
            checkAndClear(w, h);
          }
        }
      }
      
      return true;
    }
    
    //check vertical, col
    num = 0;
    pos[num] = my;
    num++;
    for(y = my - 1; y >= 0; y--){
      if(colorCompare(screen_data[mx][y], color) == true){
        pos[num] = y;
        num++;
      }
      else{
        break;
      }
    }
    if(num >= 3){
      for(int i = 0; i < num; i++){
        screen_data[mx][pos[i]] = COLOR_BLACK;
      }
      displayPixelRGBData(screen_data);
      delay(400);
      return true;
    }
    
    return false;
}

// 往下移動落下的寶石
void move_pixel_down()
{
  if(movingpixel_y == 0 || colorCompare(screen_data[movingpixel_x][movingpixel_y - 1], COLOR_BLACK) == false){//to be fixed
    if(checkAndClear(movingpixel_x, movingpixel_y) == true){
      Serial.println("checkAndClear() == true");
      movingpixel_start();
      return;
    }
    if(movingpixel_y == ColorduinoScreenHeight - 1){//at top
      state = StateLoser;
      delay(1000);
      return;
    }
    else{//new moving pixel
      movingpixel_start();
      return;
    }
  }
  
  screen_data[movingpixel_x][movingpixel_y] = COLOR_BLACK;
  movingpixel_y--;
  screen_data[movingpixel_x][movingpixel_y] = movingpixel_color;
  displayPixelRGBData(screen_data);
}

// 初始化落下寶石
void movingpixel_start()
{
  movingpixel_x = random(ColorduinoScreenWidth); // 橫向座標不定
  movingpixel_y = ColorduinoScreenHeight - 1; // 從最高處落下
  movingpixel_color = colors[random(COLORS_MAX)]; // 亂數選取落下寶石的顏色
  
  screen_data[movingpixel_x][movingpixel_y] = movingpixel_color;
  displayPixelRGBData(screen_data);
  
  turn++;
  if(turn == TURN_MAX){
    state = StateWinner;
  }
}

// 傳入含有想要顯示圖案的資料，此函式負責將資料透過Colorduino程式庫顯示在LED矩陣
void displayPixelRGBData(PixelRGB data[ColorduinoScreenWidth][ColorduinoScreenHeight])
{
  for(int w = 0; w < ColorduinoScreenWidth; w++){
    for(int h = 0; h < ColorduinoScreenHeight; h++){
      Colorduino.SetPixel(h, w, data[w][h].r, data[w][h].g, data[w][h].b);
    }
  }
  Colorduino.FlipPage();
}

// 用參數（某顏色）填滿整個LED矩陣
void fillColor(PixelRGB rgb)
{
  for(int w = 0; w < ColorduinoScreenWidth; w++){
    for(int h = 0; h < ColorduinoScreenHeight; h++){
      screen_data[w][h].r = rgb.r;
      screen_data[w][h].g = rgb.g;
      screen_data[w][h].b = rgb.b;
    }
  }
  displayPixelRGBData(screen_data);
}

// 記錄上一次的時間，往前推進遊戲
unsigned long time_previous;
// 記錄上一次按鈕開關起作用的時間，
unsigned long time_previous_for_button;

// 程式一開始會執行一次的設定函式setup()
void setup()
{
  randomSeed(analogRead(0)); // 初始化亂數值
  
  pinMode(BUTTON_LEFT_PIN, INPUT);
  pinMode(BUTTON_RIGHT_PIN, INPUT);
  
  Serial.begin(57600);
  
  // 初始化Colorduino物件，並設定白平衡
  Colorduino.Init();
  // R, G, B的值介於0~63
  unsigned char whiteBalVal[3] = {36,63,63};
  Colorduino.SetWhiteBal(whiteBalVal);
  
  time_previous = millis();
  
  state = StateStart; // 一開始狀態機的狀態為StateStart
}

// 不斷重複執行的loop()，在此測試程式裡
void loop()
{
  // 根據狀態機的狀態，分別執行該執行的動作
  switch(state){ 
    case StateStart: // 啟動遊戲
    {
	  // 根據遊戲所能擁有的全部顏色，填滿整個LED矩陣
	  // 相隔一秒
      for(int i = 0; i < COLORS_MAX; i++){
        fillColor(colors[i]);
        delay(1000);
      }
      state = StateRunPrepare;
    }
    break;
    case StateRunPrepare: // 正式開始前做一些準備功夫
    {
      fillColor(COLOR_BLACK); // 黑色（清空LED矩陣）
      delay(100);
      
      movingpixel_start(); // 初始化落下寶石，開始執行遊戲
      turn = 0;
      state = StateRun;
    }
    break;
    case StateRun: // 執行遊戲開始遊玩
    {
	  // 讀取兩個按鈕開關的狀態，移動落下表時
      if(button_left_status == LOW){
        button_left_status = digitalRead(BUTTON_LEFT_PIN);
      }
      if(button_right_status == LOW){
        button_right_status = digitalRead(BUTTON_RIGHT_PIN);
      }
      
      if(button_left_status == HIGH){
        Serial.println("Left button pressed");
      }
      if(button_right_status == HIGH){
        Serial.println("Right button pressed");
      }
      unsigned long time_now = millis();
      if(time_now - time_previous_for_button > button_delay){
        time_previous_for_button = time_now;
        if(button_left_status == HIGH){
          move_pixel_left();
        }
        if(button_right_status == HIGH){
          move_pixel_right();
        }
        button_left_status = LOW;
        button_right_status = LOW;
      }
      
      if(time_now - time_previous > falling_delay){
        time_previous = time_now;
        move_pixel_down(); // 經過延遲時間後，就讓寶石落下一格
      }
    }
    break;
    case StateWinner: // 勝利了
      displayPixelRGBData(pic_winner);
      delay(5000);
      state = StateStart;
    break;
    case StateLoser: // 失敗了
      displayPixelRGBData(pic_loser);
      delay(5000);
      state = StateStart;
    break;
    default: // error, should not be here, reset
      state = StateStart;
    break;
  }
}


