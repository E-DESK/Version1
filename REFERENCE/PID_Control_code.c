
 /*This routine is a PID control based on IRQ constant timing.
 /Resources: TMR1: PID Timebase
 /In your Initialisation, insert Pid_Init
 /In your ISR put Pid_Function
 /Pid function need 2 external object, user created: GetFeedback and PutOutput
 /GetFeedback return an int which is the controlled variable
 /PutOutput passes the output (like PWM) value to Plant
 /These functions are called here to assure minimum jitter, and
 /must have constant execution time.
 */
 
 
 //INCLUDE FILES--------------------
 
 #include <p18f452.h>
 #include <timers.h>
 #include "Global_Conf.h"            //Include in this file #define F_OSC   youroscillator frequency in Hz
 #include <delays.h>
 #include <adc.h>
 
 //PID VARIABLES--------------------
 #pragma    udata
 int    Reference = 0;                    //Reference value 0 to 4000
 int    Feedback = 0;                    //Readed value 0 to 4000      
 int    Error = 0;                        //Error (Reference - Feedback)
 int Pid_Out = 0;                    //Function Output 0 to 4000
 
 int    Kp = 0;                            //Proportional gain  0 to 255
 int    Ki = 0;                            //Integral gain 0 to 255
 int    Kd = 0;                            //Derivative gain 0 to 255
 
 int    PLimit = 0x7FFF;                    //Excursion Limit for Proportional component
 int    ILimit = 0x7FFF;                    //Excursion limit for Integrative component
 int    DLimit = 0x7FFF;                    //Excursion limit for Derivative component
 int    Out_Limit = 0x7FFF;                    //Excursion limit for Output
 
 unsigned int    PidPeriod;                //Period of Pid calculation 0=FFFF multiply of ms
 unsigned int    IPeriod;                //Period for Integrative calculation 0=FFFF multiply of PidPeriod
 unsigned int    DPeriod;                //Period for Derivative calculation 0=FFFF multiply of PidPeriod
 unsigned int    PidCounter = 0;            //Counting for PID occourrence
 unsigned int    ICounter = 0;            //Counter for Integrative  occourrence
 unsigned int    DCounter = 0;            //Counter for Integrative occourrence
 
 int    PTerm = 0;                            //Calculated Proportional term
 int    ITerm = 0;                            //Calculated Integral term
 int    DTerm = 0;                            //Calculated Derivative term
 
 int    IInstant;                            //Instantaneous Integrative Compensation
 int    OldError;                            //Old Derivative Compensation
 
 //PID Constants
 #define K_TMR1   65535 - (F_OSC/32000)
 
 
 #pragma code USER                        //Take place in USER section        
 
 //Function declaration------------------------
 
 //Limit_value_signed: limit value of signed Variable between Min_Value and Max_Value
 //Supply variable Address, Min and Max value within she has to be limited
 //Function return new value if limit take place
 int Limit_value_signed ( int Variable, int Min_Value, int Max_Value)
 {
 if (Variable < Min_Value)
     {Variable = Min_Value;}
 if (Variable > Max_Value)
     {Variable = Max_Value;}
 return Variable;
 }
 
 
 //Limit_value_unsigned: limit value of unsigned Variable between Min_Value and Max_Value
 //Supply variable Address, Min and Max value within she has to be limited
 //Function return new value if limit take place
 int unsigned Limit_value_unsigned ( int Variable,int Min_Value,int Max_Value)
 {
 if (Variable < Min_Value)
     {Variable = Min_Value;}
 if (Variable > Max_Value)
     {Variable = Max_Value;}
 return Variable;
 }
 
 //GetPError: Calculate proportional Term for Pid algorythm
 //Limiting value between Local_Limit_Low and Local_Limit_High
 void  GetPError (int Local_Error, int Local_Limit_Low, int Local_Limit_High)
 {
 PTerm = (Kp * Local_Error)/10;
     if (Local_Error > 0)
     {    if (PTerm < 0) { PTerm = 0x7FFF;}    }
     if (Local_Error < 0)
     {    if (PTerm > 0) { PTerm = 0x8000;}    }
 PTerm = Limit_value_signed (PTerm, Local_Limit_Low , Local_Limit_High);    
 }
 
 //GetIError: Calculate Integral Term for Pid algorythm
 //Limiting value between Local_Limit_Low and Local_Limit_High
 void GetIError (int Local_Error, int Local_Limit_Low, int Local_Limit_High)
 {
 IInstant = (Ki * Local_Error)/10;
     if (Local_Error > 0)
     {    if (IInstant < 0) { IInstant = 0x7FFF;}    }
     if (Local_Error < 0)
     {    if (IInstant > 0) { IInstant = 0x8000;}    }
 ITerm = IInstant + ITerm;
 ITerm = Limit_value_signed (ITerm, Local_Limit_Low, Local_Limit_High);
 }
 
 //GetPError: Calculate Derivative Term for Pid algorythm
 //Limiting value between Local_Limit_Low and Local_Limit_High
 void GetDError (int Local_Error, int Local_Limit_Low, int Local_Limit_High)
 {
 DTerm = ((Local_Error - OldError)*Kd)/10;
     if ((Local_Error - OldError) > 0)
     {    if (DTerm < 0) { DTerm = 0x7FFF;}    }
     if ((Local_Error - OldError) < 0)
     {    if (DTerm > 0) { DTerm = 0x8000;}    }
 OldError = Error;
 DTerm = Limit_value_signed (DTerm, Local_Limit_Low, Local_Limit_High);
 }
 
 
 
 //Initialize PID---------------------
 //Use TMR1 to make timebase of 1ms
 void Pid_Init (void)
 {
     OpenTimer1 (TIMER_INT_ON    &
                 T1_8BIT_RW        &
                 T1_SOURCE_INT    &
                 T1_PS_1_8        &
                 T1_OSC1EN_OFF    &
                 T1_SYNC_EXT_OFF    );
 
     WriteTimer1 (K_TMR1);
 
 }
 
 
 int a=0x3FF;
 int b=0;
 
 //PID Function------------------
 void Pid_Function    (void)
 {
     if (PIR1bits.TMR1IF == 1)                                //Monitor for 1ms timebase IRQ (TMR1IRQ)
     {    
     PIR1bits.TMR1IF=0;                                        //Reset IRQ Flag
     WriteTimer1 (K_TMR1);                                    //Recharge TMR1
 
 //Update Output-----------
 
 
 
     Pid_Out = PTerm + ITerm + DTerm;                        //Update out at once to avoid jitter
     Pid_Out = Limit_value_signed (Pid_Out, b, a);            //Limit Output
     PutOutput(Pid_Out);                                        //Call output function
 
 //Update Input, get Error--------
     Error = Reference - GetFeedback();                            //get Error
 
     if (PidCounter == 0)                                    //PID Period Counter Run out?
         {                                                    //If yes, do Proportional calc
         GetPError ( Error, -PLimit , PLimit);                //Call function, supplying data
         PidCounter = PidPeriod;                                //Recharge Period counter
         if (IPeriod > 0)                                        //Is IPeriod > 0 (ITerm required?)
             {                                                    //If yes do I calculation
             if (ICounter == 0)                                        //I Period Counter Run out?
                 {                                                    //If yes, do Integrative calc
                 GetIError ( Error, b , ILimit);                        //Call funtion, supplying data
                 ICounter = IPeriod;                                    //Recharge period counter
                 }
         else                                                        //If not, decrease I counter    
                 {
                 ICounter--;
                 }
             }    
         else                                                    //If not, then ITerm=0    
             {
             ITerm = 0;
             }    
         if (DPeriod > 0)                                        //Is DPeriod > 0 (DTerm required?)
             {                                                    //If yes do D calculation
             if (DCounter == 0)                                        //D Period Counter Run out?
                 {                                                    //If yes, do Derivative calc
                 GetDError ( Error, -DLimit , DLimit);                //Call funtion, supplying data
                 DCounter = DPeriod;                                    //Recharge period counter
                 }
             else
                 {
                 DCounter--;                                            //If not, decrease D counter
                 }
             }
         else
             {
             DTerm = 0;                                            //If not, then DTerm=0
             }
         }
     else                                                    //If not, decrease Pid counter
         {
         PidCounter--;
         }
     }
 }
 
 