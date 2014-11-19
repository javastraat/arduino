sbit LOAD=P1^2;          //MAX7219片選           12腳
sbit DIN=P1^1;           //MAX7219串列資料       1腳
sbit CLK=P1^0;           //MAX7219串列時鐘        13腳
//寄存器巨集定義
#define DECODE_MODE  0x09   //解碼控制寄存器
#define INTENSITY    0x0A   //亮度控制寄存器
#define SCAN_LIMIT   0x0B   //掃描界限寄存器
#define SHUT_DOWN    0x0C   //關斷模式寄存器
#define DISPLAY_TEST 0x0F   //測試控制寄存器         
//函數聲明
void Write7219(unsigned char address,unsigned char dat);
void Initial(void);
//位址、資料發送副程式
void Write7219(unsigned char address,unsigned char dat)
{   
    unsigned char i;
    LOAD=0;    //拉低片選線，選中器件
    //發送地址
    for (i=0;i<8;i++)        //移位迴圈8次              
    {   
       CLK=0;        //清零時鐘匯流排
       DIN=(bit)(address&0x80); //每次取高位元組      
       address<<=1;             //左移一位
       CLK=1;        //時鐘上升沿，發送位址
    }
    //發送資料
    for (i=0;i<8;i++)               
    {   
       CLK=0;
       DIN=(bit)(dat&0x80);     
       dat<<=1;  
       CLK=1;        //時鐘上升沿，發送資料
    }
    LOAD=1;    //發送結束，上升沿鎖存資料                       
}
//MAX7219初始化，設置MAX7219內部的控制寄存器
void Initial(void)                 
{ 
    Write7219(SHUT_DOWN,0x01);         //開啟正常工作模式（0xX1）
    Write7219(DISPLAY_TEST,0x00);      //選擇工作模式（0xX0）
    Write7219(DECODE_MODE,0xff);       //選用全解碼模式
    Write7219(SCAN_LIMIT,0x07);        //8只LED全用
    Write7219(INTENSITY,0x04);          //設置初始亮度      
}
測試程式
void main(void)
{
    unsigned char i; 
    Initial();               //MAX7219初始化
    while(1)
    {  
       for(i=1;i<9;i++)
       {
           Write7219(i,i);      //數碼管顯示1~8
       }
    } 
}
