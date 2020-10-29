#include<reg52.h>
#include<intrins.h>
sbit laba=P0^0;
sbit p20=P2^0;
sbit p21=P2^1;
sbit p22=P2^2;
sbit p23=P2^3;
sbit p24=P2^4;
sbit p25=P2^5;
sbit p26=P2^6;
sbit p27=P2^7;

sbit DI1=P1^0;
sbit DI2=P1^1;
sbit DI3=P1^2;
sbit DI4=P1^3;
sbit DIO=P1^4;
sbit RCK=P1^5;
sbit SCLK=P1^6;

unsigned char code smgtab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};	//共阴极数码管0~9
unsigned char th00,time,tl00;

// 频率-半周期数据表 高八位
code unsigned char FREQH[] = {
    0xF2, 0xF3, 0xF5, 0xF5, 0xF6, 0xF7, 0xF8,    //低音1234567
    0xF9, 0xF9, 0xFA, 0xFA, 0xFB, 0xFB, 0xFC,    //1,2,3,4,5,6,7
    0xFC, 0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE,    //高音 1234567
    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF};   //超高音 1234567
// 频率-半周期数据表 低八位
code unsigned char FREQL[] = {
    0x42, 0xC1, 0x17, 0xB6, 0xD0, 0xD1, 0xB6,    //低音1234567
    0x21, 0xE1, 0x8C, 0xD8, 0x68, 0xE9, 0x5B,    //1,2,3,4,5,6,7
    0x8F, 0xEE, 0x44, 0x6B, 0xB4, 0xF4, 0x2D,    //高音 1234567
    0x47, 0x77, 0xA2, 0xB6, 0xDA, 0xFA, 0x16};   //超高音 1234567
code unsigned char song[] = {	//欢乐颂
		3,2,2,	3,2,2,	4,2,2,	5,2,2,	5,2,2,	4,2,2,	3,2,2,	2,2,2,	1,2,2,	1,2,2,	2,2,2,	3,2,2,	3,2,3,	2,2,1,	2,2,5,
		3,2,2,	3,2,2,	4,2,2,	5,2,2,	5,2,2,	4,2,2,	3,2,2,	2,2,2,	1,2,2,	1,2,2,	2,2,2,	3,2,2,	2,2,3,	1,2,1,	1,2,5,
		2,2,2,	2,2,2,	3,2,2,	1,2,2,	2,2,2,	3,2,1,	4,2,1,	3,2,2,	1,2,2,	2,2,2,	3,2,1,	4,2,1,	3,2,2,	2,2,2,	1,2,2,	2,2,2,	5,1,3,
		3,2,2,	3,2,2,	3,2,2,	4,2,2,	5,2,2,	5,2,2,	4,2,2,	3,2,1,	4,2,1,	2,2,2,
		1,2,2,	1,2,2,	2,2,2,	3,2,2,	2,2,3,	1,2,1,	1,2,6,
		0xff,0xff,0xff,
};
//一个音符有三个数字。前为第几个音、中为第几个八度、后为时长（以半拍为单位）。
void timedelay(unsigned char t)
{
	unsigned char t1;
   unsigned long t2;
   for(t1 = 0; t1 < t; t1++)    //双重循环, 共延时t个半拍
     for(t2 = 0; t2 < 4000; t2++); //延时期间, 可进入T0中断去发音
   TR0 = 0;                        //关闭T0, 停止发音
}
void delay(unsigned char x)//延时x ms
{
	unsigned char i;
	for(;x>0;x--)
		for(i=0;i<110;i++);
}
unsigned char keyscan()	  //键盘扫描程序
{
	unsigned char key=0;
	if(p20==0)
	{
		delay(100);
		if(p20==0)
			key=1;
	}
	else if(p21==0)
		{
		delay(100);
		if(p21==0)
			key=2;
		}
	else if(p22==0)
		{
		delay(100);
		if(p22==0)
			key=3;
		}
	else if(p23==0)
		{
		delay(100);
		if(p23==0)
			key=4;
		}
	else if(p24==0)
		{
		delay(100);
		if(p24==0)
			key=5;
		}
	else if(p25==0)
		{
		delay(100);
		if(p25==0)
			key=6;
		}
	else if(p26==0)
		{
		delay(100);
		if(p26==0)
			key=7;
		}
	else if(p27==0)
		{
		delay(100);
		if(p27==0)
			key=8;
		}
	return key;
}
void smg(unsigned char key)
{
	unsigned char tem,i;
	for(i=0;i<=8;i++)	//键位判断
	{
		if(key==i)
			tem=smgtab[i];
	}
	for(i=0;i<8;i++)	//74HC595输出控制程序
	{
		DIO=tem>>7;
		tem<<=1;
		SCLK=0;
		_nop_();	//延时1us
		_nop_();
		SCLK=1;
	}
	RCK=0;
	_nop_();
	_nop_();
	RCK=1;

	if(key==1||key==5)
		{DI1=1;DI2=0;DI3=0;DI4=0;}
	else if(key==2||key==6)
		{DI1=0;DI2=1;DI3=0;DI4=0;}
	else if(key==3||key==7)
		{DI1=0;DI2=0;DI3=1;DI4=0;}
	else if(key==4||key==8)
		{DI1=0;DI2=0;DI3=0;DI4=1;}
}
void timer0()
{
	TMOD=0x01;
	TR0=0;
	ET0=1;
	EA=1;
}
void sound()                      //演奏一个音符
{
    TH0 = th00;               //控制音调
    TL0 = tl00;
    TR0 = 1;                     //启动T0, 由T0输出方波去发音
    timedelay(time);                 //控制时间长度
}
void main()
{
	unsigned char key=0,keytem1,keytem2,i,j;
	DI1=1;DI2=0;DI3=0;DI4=0;
	smg(key);
	timer0();
	while(1)
	{	
		keytem1=key;
		key=keyscan();
		keytem2=key;
		if(keytem1!=key)
			smg(key);
		if(key>=1&&key<=7)
		{
			i=key-1;
			time=1;
			while(time)
			{
				if(key!=keyscan())
					break;
				j =i;
        th00 = FREQH[j+14];	//发出高音1-7
        tl00 = FREQL[j+14];     
        sound();                  
			}
		}
		else if(key==8)
		{
			i=0;
			time=1;
			while(time)
			{
				if(song[i]==0xff)
					break;
				if(keyscan()!=key&&keyscan()!=0)
					break;
				j = song[i] + 7 * song[i + 1] - 1;//第i个是音符, 第i+1个是第几个八度
        th00 = FREQH[j];      //从数据表中读出频率数值
        tl00 = FREQL[j];      //实际上, 是定时的时间长度
        time = song[i + 2];   //读出时间长度数值
        i += 3;
        sound();                  //发出一个音符
			}
		}
		delay(6);
	}
}
void time0_in()interrupt 1
{
	TR0=0;
	laba=~laba;
	TH0=th00;
	TL0=tl00;
	TR0=1;
}