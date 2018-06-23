/*----------------------------------------------------------------
 * Copyright (C) 2011 江苏大学 电气学院416 刘中原
 * 版权所有。 
 * 
 * 文件名： main.c
 * 
 * 文件功能描述：   
 *          MSP430F16x,MSP430F14X系列单片机串口通讯函数库示例程序
 *      说明：  
 *          该程序示范串口通讯函数库内的各函数使用方法，同时可作为
 *          串口测试程序用。该程序的演示需要通过Windows自带的"超级
 *          终端"软件，或是串口调试器进行。
 *
 *          运行后，先输出两个字符串  
 *          UartWriteChar 函数工作正常!
 *          下面测试串口收发函数
 *          然后，将按原字符回复，串口调试器发送什么就接收什么
 *
 * 创建标识：   刘中原20110622
 * 
 * 修改标识：   
 * 修改描述：   
**----------------------------------------------------------------*/
#include <stdint.h>
#include "msp430x14x.h"   //430寄存器头文件
#include "Uart.h"         //串口通讯程序库头文件
#include "Spi.h"
#include "MSP430_AFE_SPI.h"
#include "TI_AFE4400.h"
#include "TI_AFE4400_setting.h"

char str[] = "UartWriteChar"; //ROM中一个字符串
char str1[] = "start conversation!"; //ROM中一个字符串
char str2[] = "stop conversation!"; //ROM中一个字符串
char q = 'Q';

/****************************************************************************
* 名    称：ClkInit
* 功    能：时钟系统初始化  MCLK为8MHz，SMCLK为1MHz
* 入口参数：无
* 出口参数：无
****************************************************************************/
void ClkInit()
{
    char i;
    BCSCTL1 &= ~XT2OFF;             //打开XT2振荡器
    IFG1&=~OFIFG;                   //清除振荡错误标志
    while((IFG1&OFIFG)!=0)
    {
        for(i=0;i<0xff;i++);
        IFG1&=~OFIFG;               //清除振荡错误标志
    }
    BCSCTL2 |= SELM_2+SELS+DIVS_0;  //MCLK为8MHz，SMCLK为1MHz
}

/*******************************************
函数名称：Delays
功    能：延时一会
参    数：无
返回值  ：无
********************************************/
void Delays(uint32_t k)
{
    uint8_t i=100;
    uint32_t j;
    while(i--)
    {
            j=k;
            while(j--);
    }
}

/****************************************************************************
* 名    称：main主程序
* 功    能：设置串口，输出信息，从串口读计算机键盘输入数据，测试串口收发
* 入口参数：无
* 出口参数：无
****************************************************************************/
int main( void )
{
  P4DIR = 0x01;
  P4OUT = 0x01;
  P2DIR = 0xfa;
  P2OUT = 0xf0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  ClkInit();
  
  uint8_t count=3;
  uint8_t read_buf[3];
  unsigned long val;
 // int i = 0;

//  count = CH_DATA_SIZE;                                                        // bytes to read: ADC_DOUT2 - ADCDOUT0

  
  char chr;               //串口测试中，收到的字节
  // 主机模式，波特率4000000,8位数据位，三线模式，时钟模式1（具体见spi.c）
  SpiMasterInit(400000,8,3,0);
  UartInit(115200,'n',8,1);//串口初始化,设置成38400bps,无校验,8位数据,1位停止
  _EINT(); 
  
  P1DIR = 0xff;
  P1SEL = 0x00;  
  Delays(10000);
  TI_AFE4400_WriteRegSettings();  
  P2OUT = 0xd0;
  while(1)                    //串口测试
  {
      chr = UartReadChar();
      switch(chr)
      {
      case 'S':
        UartWriteStr(str1);
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 1, 3); //enable read 0x000001ul
        UartWriteChar(q);
        break;
      case 'T':
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 1, 3);//enable read
        break;
      case 'M':
        for(uint32_t k = 0;k<1000000;k++)
        {
            val = TI_AFE4400_SPIAutoIncReadReg(LED1VAL, count);
            read_buf[0] = val & 0xFF;
            read_buf[1] = (val>>8) & 0xFF;
            read_buf[2] = (val>>16) & 0xFF;
            UartWriteint(read_buf[2]);
            UartWriteint(read_buf[1]);
            UartWriteint(read_buf[0]);
            UartWriteChar(0x0d);    //发送"换行"(\r)"
            UartWriteChar(0x0a);    //发送"回车"(\n)"  
            Delays(160);
        }
        break;
      case 'B':
        for(uint8_t k = 1;k<48;k++)
        {
            val = TI_AFE4400_SPIAutoIncReadReg(k, count);
            read_buf[0] = val & 0xFF;
            read_buf[1] = (val>>8) & 0xFF;
            read_buf[2] = (val>>16) & 0xFF;
            UartWriteint(read_buf[2]);
            UartWriteint(read_buf[1]);
            UartWriteint(read_buf[0]);
            UartWriteChar(0x0d);    //发送"换行"(\r)"
            UartWriteChar(0x0a);    //发送"回车"(\n)"  
            Delays(50000);
        }
        break;
        case 'A':
        for(uint32_t k = 0;k<1000000;k++)
        {
            TI_AFE4400_SPIAutoIncWriteReg(0x00, 0x010203, 3);//enable read 
            Delays(10);
        }
        break;
        case 'E':
        for(uint32_t k = 0;k<1000;k++)
        {
            TI_AFE4400_WriteRegSettings();
            UartWriteChar(0x0d);    //发送"换行"(\r)"
            UartWriteChar(0x0a);    //发送"回车"(\n)"
            Delays(10);
        }
        break;


      }
      
  }

}

