//�����ӳ����ṩ���������������ο�
// *******************************************************************
// Website:  http://wch.cn
// Email:    tech@wch.cn
// Author:   TECH  2008.12.05
// *******************************************************************

//ICCAVR�±���

// **************************************
// ����ͷ�ļ�
// **************************************
#include <iom16v.h>
#include <macros.h>

// **************************************
// CH423����
// **************************************
#define				CH423_SDA_OUT   	{ DDRC |= (1<<PC1); }
#define				CH423_SDA_IN		{ PORTC |= (1<<PC1); DDRC &= ~(1<<PC1); }
#define			 	CH423_SDA_SET		PORTC |= (1<<PC1)
#define			 	CH423_SDA_CLR		PORTC&=~(1<<PC1)
#define			 	CH423_SDA			( PINC & (1<<PC1) )
#define			 	CH423_SCL_OUT		{ PORTC |= (1<<PC0); DDRC |= (1<<PC0); }
#define			 	CH423_SCL_SET		PORTC |= (1<<PC0)
#define			 	CH423_SCL_CLR		PORTC &= ~(1<<PC0) 
#define				DELAY			    { NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP(); }

#define 			DOT_				0x80
#define             CH423_SYSTEM        0x4800
#define             CH423_OCL           0x4400
#define             CH423_OCH           0x4600
#define             CH423_IO        	0x6000
#define             CH423_DIG0        	0x6000 
#define             CH423_DIG1        	0x6200 
#define             CH423_DIG2        	0x6400
#define             CH423_DIG3        	0x6600
#define             CH423_DIG4        	0x6800 
#define             CH423_DIG5        	0x6A00 
#define             CH423_DIG6        	0x6C00
#define             CH423_DIG7        	0x6E00
#define             CH423_DIG8        	0x7000 
#define             CH423_DIG9        	0x7200 
#define             CH423_DIG10        	0x7400
#define             CH423_DIG11        	0x7600
#define             CH423_DIG12        	0x7800 
#define             CH423_DIG13        	0x7A00 
#define             CH423_DIG14        	0x7C00
#define             CH423_DIG15        	0x7E00
#define             CH423_RD        	0x4D00
#define             CH423_SYS0        	0x00
#define             CH423_SYS1        	0x01
#define             CH423_SYS2        	0x17

// **************************************
// ��ʱ���룬��׼
// **************************************
void mDelaymS( unsigned char ms )    // ��ʱ���룬��׼
{
    unsigned short i;
    while ( ms -- )
    {
        for( i = 0; i != 3000; i++ );
    }
}

//     7 1 F 8 9 A
//6 5 4 2 3  |  C E D 0 B
// **************************************
// BCD���
// **************************************
const unsigned char BCD_decode_tab[ 29 ] = { 
                                               0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 
                                               0X77, 0X7C, 0X58, 0X5E, 0X79, 0X71, 0x00, 0x46, 0x40, 0x41, 
											   0x39, 0x0F, 0x08, 0x76, 0x38, 0x73, 0x80, 0xFF, 0x00 
										   };

// **************************************
// I2C START
// **************************************									   
void CH423_I2c_Start( void )
{
	CH423_SDA_SET; /*������ʼ�����������ź�*/
	CH423_SDA_OUT;  /* ����SDAΪ������� */
 	DELAY;
	
	CH423_SCL_SET;/* ����SCLΪ������� */
	DELAY;
	
	CH423_SDA_CLR;   /*������ʼ�ź�*/
	DELAY;    
	  
	CH423_SCL_CLR;  /*ǯסI2C���ߣ�׼�����ͻ�������� */
	DELAY;
}

// **************************************
// I2C STOP
// **************************************	
void CH423_I2c_Stop( void )
{
	CH423_SDA_CLR; 
	CH423_SDA_OUT; /* ����SDAΪ������� */
	DELAY;
	
	CH423_SCL_SET; 
	DELAY;
	
	CH423_SDA_SET; /*����I2C���߽����ź�*/
	DELAY;   
	
	CH423_SDA_IN; /* ����SDAΪ���뷽�� */
	DELAY; 
}

// **************************************
// I2C дһ���ֽ�
// **************************************
void CH423_I2c_WrByte( unsigned char c )	//дһ���ֽ�����
{
	unsigned char i;
	
	CH423_SDA_SET;
	CH423_SDA_OUT; /* ����SDAΪ������� */
	DELAY;
	
	for( i = 0; i != 8; i++ ) 
	{
		if( c & 0x80 ) { CH423_SDA_SET; }
		else { CH423_SDA_CLR; }
		DELAY;
		
		CH423_SCL_SET;
		
		c <<= 1;
		DELAY;
		
		CH423_SCL_CLR;
	}
	
	CH423_SDA_IN;   /* ����SDAΪ���뷽�� */
	DELAY;
	
	CH423_SCL_SET;  
	DELAY;
	
	CH423_SCL_CLR;
	DELAY;
}

// **************************************
// I2C ��һ���ֽ�
// **************************************
unsigned char  CH423_I2c_RdByte( void )		//��һ���ֽ�����
{
	unsigned char c, i;
	
	CH423_SDA_IN;   /* ����SDAΪ���뷽�� */
	
	c = 0;
	for( i = 0; i != 8; i++ )  // ����8λ����
	{
		CH423_SCL_SET;
		DELAY;
		
		c <<= 1;
		if( CH423_SDA ) c++;  // ����1λ
		
		CH423_SCL_CLR;
		DELAY;
	}
	
	CH423_SDA_SET;
	CH423_SDA_OUT;
	
	CH423_SCL_SET;  // ������ЧӦ��
	DELAY;
	
	CH423_SCL_CLR;
	DELAY;
	
	return(c);
}

// **************************************
// CH423д����
// **************************************
void CH423_Write( unsigned short command )	//д����
{	
     CH423_I2c_Start();               /*��������*/
   	 CH423_I2c_WrByte( (unsigned char)(command>>8) ); 
   	 CH423_I2c_WrByte( (unsigned char)command );               /*��������*/
  	 CH423_I2c_Stop();                 /*��������*/
}

// **************************************
// CH423������
// **************************************
unsigned char CH423_Read( void )	//������
{	
     unsigned short command = CH423_RD;
	 unsigned char val;
	 
     CH423_I2c_Start();               /*��������*/
   	 CH423_I2c_WrByte( (unsigned char)(command>>8) ); 
   	 val = CH423_I2c_RdByte( );               /*������*/
  	 CH423_I2c_Stop();                 /*��������*/
	 
	 return val;
}

// **************************************
// CH423����ģʽ0
// **************************************
void CH423_SetMode0( void )
{
 	 CH423_Write( CH423_SYSTEM | CH423_SYS0 );//����IO��չ���ܣ�IO7-IO0Ϊ���룬OC15-OC0Ϊ��©���
}

// **************************************
// CH423����ģʽ1
// **************************************
void CH423_SetMode1( void )
{
 	 CH423_Write( CH423_SYSTEM | CH423_SYS1 );//����IO��չ���ܣ�IO7-IO0Ϊ�����OC15-OC0Ϊ��©���
}

// **************************************
// CH423����ģʽ2
// **************************************
void CH423_SetMode2( void )
{
 	 CH423_Write( CH423_SYSTEM | CH423_SYS2 );//����������Զ�ɨ�蹦�ܣ�SEG7��SEG0 ��DIG15��DIG0 Ϊ���
}

// **************************************
// CH423��IO7-IO0�������
// **************************************
void CH423_IO_Output( unsigned char output )
{
 	 CH423_Write( CH423_IO | output );//IO7-IO0���
}

// **************************************
// CH423��OC15-OC8�������
// **************************************
void CH423_OCH_Output( unsigned char output )
{
 	 CH423_Write( CH423_OCH | output );//OC15-OC8���
}

// **************************************
// CH423��OC7-OC0�������
// **************************************
void CH423_OCL_Output( unsigned char output )
{
 	 CH423_Write( CH423_OCL | output );//OC7-OC0���
}

// **************************************
// ��CH423��IO7-IO0���������
// **************************************
unsigned char CH423_IO_Input( void )
{
 	 return CH423_Read();//��IO7-IO0���������
}

// **************************************
// CH423��ʼ��
// **************************************
void CH423_Init( void )
{
 	 CH423_SCL_OUT;
	 CH423_SDA_SET;
	 CH423_SDA_OUT;
	 
	 mDelaymS( 30 );
	 
	 CH423_SetMode2();//����������Զ�ɨ�蹦�ܣ�SEG7��SEG0 ��DIG3��DIG0 Ϊ���
	 
	 CH423_Write( CH423_DIG0 | BCD_decode_tab[0] );
	 CH423_Write( CH423_DIG1 | BCD_decode_tab[1] );
	 CH423_Write( CH423_DIG2 | BCD_decode_tab[2] );
	 CH423_Write( CH423_DIG3 | BCD_decode_tab[3] );
	 CH423_Write( CH423_DIG4 | BCD_decode_tab[4] );
	 CH423_Write( CH423_DIG5 | BCD_decode_tab[5] );
	 CH423_Write( CH423_DIG6 | BCD_decode_tab[6] );
	 CH423_Write( CH423_DIG7 | BCD_decode_tab[7] );
	 CH423_Write( CH423_DIG8 | BCD_decode_tab[8] );
	 CH423_Write( CH423_DIG9 | BCD_decode_tab[9] );
	 CH423_Write( CH423_DIG10 | BCD_decode_tab[10] );
	 CH423_Write( CH423_DIG11 | BCD_decode_tab[11] );
	 CH423_Write( CH423_DIG12 | BCD_decode_tab[12] );
	 CH423_Write( CH423_DIG13 | BCD_decode_tab[13] );
	 CH423_Write( CH423_DIG14 | BCD_decode_tab[14] );
	 CH423_Write( CH423_DIG15 | BCD_decode_tab[15] );
	 
	 mDelaymS( 255 );
	 mDelaymS( 255 );
	 mDelaymS( 255 );
	 mDelaymS( 255 );
	 
	 CH423_Write( CH423_DIG0 | BCD_decode_tab[0] | DOT_ );
	 CH423_Write( CH423_DIG1 | BCD_decode_tab[1] | DOT_ );
	 CH423_Write( CH423_DIG2 | BCD_decode_tab[2] | DOT_ );
	 CH423_Write( CH423_DIG3 | BCD_decode_tab[3] | DOT_ );
	 CH423_Write( CH423_DIG4 | BCD_decode_tab[4] | DOT_ );
	 CH423_Write( CH423_DIG5 | BCD_decode_tab[5] | DOT_ );
	 CH423_Write( CH423_DIG6 | BCD_decode_tab[6] | DOT_ );
	 CH423_Write( CH423_DIG7 | BCD_decode_tab[7] | DOT_ );
	 CH423_Write( CH423_DIG8 | BCD_decode_tab[8] | DOT_ );
	 CH423_Write( CH423_DIG9 | BCD_decode_tab[9] | DOT_ );
	 CH423_Write( CH423_DIG10 | BCD_decode_tab[10] | DOT_ );
	 CH423_Write( CH423_DIG11 | BCD_decode_tab[11] | DOT_ );
	 CH423_Write( CH423_DIG12 | BCD_decode_tab[12] | DOT_ );
	 CH423_Write( CH423_DIG13 | BCD_decode_tab[13] | DOT_ );
	 CH423_Write( CH423_DIG14 | BCD_decode_tab[14] | DOT_ );
	 CH423_Write( CH423_DIG15 | BCD_decode_tab[15] | DOT_ );
	 
	 mDelaymS( 255 );
	 mDelaymS( 255 );
	 mDelaymS( 255 );
	 mDelaymS( 255 );
}

// **************************************
// CH423��DEMO0
// **************************************
void CH423_Demo0( void )
{
 	 unsigned char input;
	 
 	 CH423_SetMode0();//����IO��չ���ܣ�IO7-IO0Ϊ���룬OC3-OC0Ϊ��©���
	 mDelaymS( 10 );
	 
	 do
	 {
	   	 input = CH423_IO_Input();//��CH423��IO7-IO0���������
		 
	 	 if( input & 0x01 )//IO0����1
	 	 {
		  	 CH423_OCH_Output( 0x00 );//OC15-OC8����͵�ƽ
	 	  	 CH423_OCL_Output( 0x00 );//OC7-OC0����͵�ƽ
	 	 }
	 	 else//IO0����0
	 	 {
		  	 CH423_OCH_Output( 0xFF );//OC15-OC8�����
		  	 CH423_OCL_Output( 0xFF );//OC7-OC0�����
	 	 }
		 
		 mDelaymS( 255 );
     }
	 while(1);
}

// **************************************
// CH423��DEMO1
// **************************************
void CH423_Demo1( void )
{
 	 CH423_SetMode1();//����IO��չ���ܣ�IO7-IO0Ϊ�����OC3-OC0Ϊ��©���
	 mDelaymS( 10 );
	 
	 while(1)
	 {
	  	 CH423_IO_Output( 0x55 );//IO6,IO4,IO2,IO0���1��IO7,IO5,IO3,IO1���0
		 mDelaymS( 255 );
		 
		 CH423_IO_Output( 0xAA );//IO6,IO4,IO2,IO0���0��IO7,IO5,IO3,IO1���1
		 mDelaymS( 255 );
		 
		 CH423_OCH_Output( 0x00 );//OC15-OC8����͵�ƽ
		 CH423_OCL_Output( 0x00 );//OC7-OC0����͵�ƽ
		 mDelaymS( 255 );
		 
		 CH423_OCH_Output( 0xFF );//OC15-OC8�����
		 CH423_OCL_Output( 0xFF );//OC7-OC0�����
		 mDelaymS( 255 );
	 }
}

// **************************************
// ������
// **************************************
void main( void )
{
    unsigned char demo = 0;
	
 	CH423_Init();//CH423��ʼ��
	
	if( demo == 0 )
	{
     	CH423_Demo1();//���ģʽDemo
    }
	else if( demo == 1 )
	{
     	CH423_Demo0();//����ģʽDemo
	}
}


