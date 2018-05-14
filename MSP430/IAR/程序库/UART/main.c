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

#include "msp430x16x.h"   //430寄存器头文件
#include "Uart.h"         //串口通讯程序库头文件

char str[] = "UartWriteChar 函数工作正常!"; //ROM中一个字符串

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
    BCSCTL2 |= SELM_2+SELS+DIVS_3;  //MCLK为8MHz，SMCLK为1MHz
}

/****************************************************************************
* 名    称：main主程序
* 功    能：设置串口，输出信息，从串口读计算机键盘输入数据，测试串口收发
* 入口参数：无
* 出口参数：无
****************************************************************************/
void main()
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    
    char chr;               //串口测试中，收到的字节
    
    //串口初始化之前必须调用此函数，SMCLK设为1M，9600以下波特率可以不调用
    ClkInit();
    
    UartInit(38400,'n',8,1);//串口初始化,设置成38400bps,无校验,8位数据,1位停止
    _EINT();
    UartWriteStr(str);
    UartWriteChar(0x0d);    //发送"换行"(\r)"
    UartWriteChar(0x0a);    //发送"回车"(\n)"   
    
    UartWriteStr("下面测试串口收发函数\r\n");
    
    while(1)                    //串口测试
    {
        chr=UartReadChar();     //收1字节
        UartWriteChar(chr);     //将收到的数据返回
    }
}

