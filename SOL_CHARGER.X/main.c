/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ64GA002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"

/*
                         Main application
 */

float Vchrg = 20.0;          //Initializing the charge voltage
float Vchrg_max = 20.0;     //The maximum charging voltage of the battery
float Ichrg = 6.8;           //The charging current of the battery

float Bchrg = 0.0;
float Bchrg_max = 15.0;
float Ini_chrg = 0.0;

float Iout = 0.0;           //Measured Output Current
float Vout = 0.0;           //Measured Output Voltage

float e_v[5] = {0.0};       //Voltage error array
float m_v[5] = {0.0};       //Voltage PID output array

float e_i[5] = {0.0};       //Current error array
float m_i[5] = {0.0};       //Current PID output array

int iloop_cnt = 0;          //count variable for the outer current loop
int ch_loop_cnt = 0;



float Iin = 0.0;            //Input Current
float Iprev = 0.0;          //Previous Current
float Delta_I = 0.0;        //Change in Current

float Vin = 0.0;            //Input Voltage
float Vprev = 0.0;          //Previous Voltage
float Delta_V = 0.0;        //Change in Voltage

float Pin = 0.0;            //Input Power
float Pprev = 0.0;          //Previous Power
float Delta_P = 0.0;        //Change in Power

float cntrl = 0.1;          //10% duty cycle

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    while (1)
    {
        // Add your application code
    }

    return 1;
}
/**
 End of File
*/

