/*
 * File:   custom.c
 * Author: User
 *
 * Created on August 19, 2022, 11:09 AM
 */

// General Thumb sucked tuning rule
//kp = 0.0003; ki = 0.000075
//ki largely affects settling time.
//ki largely affects system stability.
//Increase ki, and increase kp to be at least 3.5*ki
//If you reach unstable point, kp down
//kp = 2*ki
//Speed wise, system is sensitive to ki
//Stability wise, system is more sensitive to kp
//As loop execution time decreases, coefficients have to increase
//As loop execution time increases, coefficients have to decrease

#define     kp_v        0.002256            //Voltage PID proportional component
#define     ki_v        0.0008128            //Voltage PID integral component
#define     kd_v        0.0                 //Voltage PID derivative component
#define     kp_i        0.2256             //Current PID proportional component
#define     ki_i        0.0728             //Current PID integral component
#define     kd_i        0.0                 //Current PID derivative component
#define     MAX_DUTY    159.0               //Controller max duty cycle value
#define     MAX_VOLTAGE 35.0                //Maximum Inductor current
#define     cntrl_max   0.85                 //Maximum duty cycle 0.8 out of 1.0
#define     cntrl_min   0.0                 //Minimum duty cycle is 0.0

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mcc.h"
#include "tmr1.h"
#include "custom.h"

extern float Vchrg;
extern float Vchrg_max;
extern float Vout;
extern float Ichrg;
extern float Iout;

extern int iloop_cnt;
extern int ch_loop_cnt;

extern float Bchrg;
extern float Bchrg_max;

extern float Ini_chrg;


extern float Iin;
extern float Iprev;
extern float Delta_I;

extern float Vin;
extern float Vprev;
extern float Delta_V;

extern float Pin;
extern float Pprev;
extern float Delta_P;
extern float cntrl;

extern float e_v[5];
extern float m_v[5];
extern float e_i[5];
extern float m_i[5];


void MPPT_IC_ROUTINE(void){

    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin
    
    //Sensor stuff
//    Vin = ((float)ADC1BUF0/1023.0)*5.0;
//    Vin = Vactual*((1200+100)/100);
    Vin = (float)ADC1BUF0 * 0.06353861193;
    
//    Iin = ((float)ADC1BUF1/1023.0)*5.0;
//    Iin = (2.5 - Iactual)/0.1;
    Iin = 25 - 0.04887585532*(float)ADC1BUF1;
    //Sensor stuff ends
    
    //MPPT Incremental Conductance algorithm
    Pin = Vin*Iin;
    Delta_I = Iin - Iprev;
    Delta_V = Vin - Vprev;
    Delta_P = Pin - Pprev;
    
    if(Delta_V == 0.0){
        if(Delta_I == 0.0){
            cntrl = cntrl;
        }
        else{
            if(Delta_I > 0.0){
                cntrl = cntrl - 0.01;
            }
            else{
                cntrl = cntrl + 0.01;
            }
        }
    }
    else{
        if((Delta_I/Delta_V) == -1*(Iin/Vin)){
            cntrl = cntrl;
        }
        else{
           if((Delta_I/Delta_V) > -1*(Iin/Vin)){
               cntrl = cntrl - 0.01;
           }
           else{
               cntrl = cntrl + 0.01;
           }
        }
    }
    
    
    //Limiting of the duty cycle
    if(cntrl >= cntrl_max){
        cntrl = cntrl_max;
    }
    else if(cntrl <= cntrl_min){
        cntrl = cntrl_min;
    }
    else{
        cntrl = cntrl;
    }
    
    OC1RS = (int)(MAX_DUTY*cntrl);
    
    //Updating parameters for next cycle
    Vprev = Vin;
    Pprev = Pin;
    Iprev = Iin;
    
}

void MPPT_PO_ROUTINE(void){
    
    //Sensor stuff
//    Vin = ((float)ADC1BUF0/1023.0)*5.0;
//    Vin = Vactual*((1200+100)/100);
    Vin = (float)ADC1BUF0 * 0.06353861193;
    
//    Iin = ((float)ADC1BUF1/1023.0)*5.0;
//    Iin = (2.5 - Iactual)/0.1;
    Iin = 25 - 0.04887585532*(float)ADC1BUF1;
    //Sensor stuff ends
    
    
    //MPPT Perturb and Observe algorithm
    Pin = Vin*Iin;

    Delta_P = Pin - Pprev;
    Delta_V = Vin - Vprev;

    if(Delta_P > 0){
        if(Delta_V > 0){
            cntrl = cntrl - 0.01;
        }
        else{
            cntrl = cntrl + 0.01;
        }
    }
    else{
        if(Delta_V > 0){
            cntrl = cntrl + 0.01;
        }
        else{
            cntrl = cntrl - 0.01;
        }
    }
    
    //Limiting of the duty cycle for the first stage
    if(cntrl >= cntrl_max){
        cntrl = cntrl_max;
    }
    else if(cntrl <= cntrl_min){
        cntrl = cntrl_min;
    }
    else{
        cntrl = cntrl;
    }
    
    OC1RS = (int)(MAX_DUTY*cntrl);
    
    //Update the parameters for the next cycle
    Vprev = Vin;
    Pprev = Pin;
    Iprev = Iin;
    
}

void BATTERY_CHARGER(void){
    
    //Sensor stuff
//    Vout = ((float)ADC1BUF0/1023.0)*5.0;
//    Vout = Vactual*((1200+100)/100);
    Vout = (float)ADC1BUF2 * 0.06353861193;
    
//    Iout = ((float)ADC1BUF1/1023.0)*5.0;
//    Iout = (2.5 - Iactual)/0.1;
    Iout = 25 - 0.04887585532*(float)ADC1BUF3;
    //Sensor stuff ends
    
    ch_loop_cnt++;
//    ch_loop_cnt = 0;
    if(ch_loop_cnt == 10){
        ch_loop_cnt = 0;
        
        //The inner loop
        m_v[1] = m_v[0];
        e_v[2] = e_v[1];
        e_v[1] = e_v[0];

        e_v[0] = Vchrg - Vout;    //Error Calculation

        m_v[0] = (kp_v + ki_v + kd_v)*e_v[0] - (kp_v + 2*kd_v)*e_v[1] + kd_v*e_v[2] + m_v[1];   //PID control step


        //Limiting of the duty cycle
        if(m_v[0] >= cntrl_max){
            m_v[0] = cntrl_max;
        }
        else if(m_v[0] <= cntrl_min){
            m_v[0] = cntrl_min;
        }
        else{
            m_v[0] = m_v[0];
        }

        OC2RS = (int)(MAX_DUTY*m_v[0]);         //Updating the duty cycle

//        Bchrg = Ini_chrg + Iout*4.167e-08;

        iloop_cnt++;
//        iloop_cnt = 0;
        if(iloop_cnt == 10){
            iloop_cnt = 0;        
            
            //The outer loop -> 10 times slower than the inner loop
            m_i[1] = m_i[0];
            e_i[2] = e_i[1];
            e_i[1] = e_i[0];

            e_i[0] = Ichrg - Iout;

            m_i[0] = (kp_i + ki_i + kd_i)*e_i[0] - (kp_i + 2*kd_i)*e_i[1] + kd_i*e_i[2] + m_i[1];

            if(m_i[0] >= 1.0){m_i[0] = 1.0;}
            else if(m_i[0] <= 0.0){m_i[0] = 0.0;}
            else{m_i[0] = m_i[0];}

//            if(Bchrg > 0.85*Bchrg_max){Vchrg_max = 26.9;}

            Vchrg = Vchrg_max*m_i[0];

        }
    }
    
}
