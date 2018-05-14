/*----------------------------------------------------------------
 * Copyright (C) 2011 江苏大学 电气学院416 刘中原
 * 版权所有。 
 * 
 * 文件名： main.c
 * 
 * 文件功能描述：   
 *          MSP430F16x,MSP430F14X系列单片机数码管程序演示
 *      说明：  
 *          该程序完成调用数码管程序，完成在数码管中显示的，完成
 *          数码管程序的演示;还包括数码管的printf的使用。
 *          
 *          
 * 创建标识：   刘中原20110822
 * 
 * 修改标识：   
 * 修改描述：   
**----------------------------------------------------------------*/

#include <msp430x16x.h>
#include <stdio.h>
#include "Nixietube.h"

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
* 功    能：数码管程序演示
* 入口参数：无
* 出口参数：无
****************************************************************************/
void main( void )
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    ClkInit();
    NixiettubeInit();
    _EINT();
    //while(1)
    {
        NixiettubeDisplayChar(AA,5);
        NixiettubeDisplayChar(DOT,5);
        NixiettubeInsertChar(2);
        NixiettubeInsertChar(DOT);
        NixiettubeInsertChar(2);
        printf("%1.2f",1.2);
    }
}
