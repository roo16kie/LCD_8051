//實驗四 LCD 顯示器
#include "8051.h"
//data bus P2
// ENABLE P3.4;
// R_W P3.3;
// RS P3.2;
void LCDcmd(char hex);
void LCDdata(char hex);
code char init[] = {0x38,0x0e,0x06,0x02,0x01};
code char ser[]={0xfe,0xfd,0xfb,0xf7};
code int key[]={1,2,3,12,4,5,6,13,7,8,9,14,10,0,11,15};
code char keymsg[]={0xee,0xed,0xeb,0xe7,
 0xde,0xdd,0xdb,0xd7,
 0xbe,0xbd,0xbb,0xb7,
 0x7e,0x7d,0x7b,0x77};
code char msg[] = "0123456789abcdef";
int rx_flag;
char ch;
void delay (int t);
int getkey();
void main()
{
 int i,x;
 P3_2 = 0;
 P3_3 = 0;
 P3_4 = 0;
 SCON=0x50;
 TMOD=0x20;
 TH1=0xfd;
 TR1=1;
IE=0x90;
 rx_flag=0;
 for(i = 0; i < 5; i++)
 {
 LCDcmd(init[i]);
 }
while(1)
 {
 if (rx_flag==1)
 {
 rx_flag = 0;
 SBUF = ch;
 }
 else
 {
 x=getkey();
 LCDcmd(0x80);
 LCDdata(msg[x]);
 }
}
}
void interr_s(void) interrupt 4
{
 if (RI)
 {
 RI = 0;
 rx_flag = 1;
 ch = SBUF;
 LCDcmd(0xc0);
 LCDdata(ch);
 }
 else
 {
 TI = 0;
 }
}
int getkey()
{
 int i,k=-1;
 while(1){
 for(i=0;i<4;i++){
 P1=ser[i];
 if(P1!=ser[i]){
 if(P1==keymsg[i])
 k=key[i];
 else if(P1==keymsg[i+4])
 k=key[i+4];
 else if(P1==keymsg[i+8])
 k=key[i+8];
 else if(P1==keymsg[i+12])
 k=key[i+12];
 }
 if (k!=-1) break ;
 }
 if(k!=-1)
 while(1){
 P1=ser[i];
 if(P1==ser[i])
 return k;
 }
 }
}
void LCDcmd(char hex)
{
 delay(1000);
 P2 = hex;
 P3_4 = 1;
 P3_4 = 0;
}
void LCDdata(char hex)
{
 delay(1000);
 P2 = hex;
 P3_4 = 1;
 P3_2 = 1;
 P3_4 = 0;
P3_2 = 0;
}
void delay(int t)
{
 int i;
 for(i = 0; i < t; i++)
 ;
}