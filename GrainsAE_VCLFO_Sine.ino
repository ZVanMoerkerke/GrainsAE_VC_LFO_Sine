//VC LFO for AE Grains / Arduino Nano. By Zeno Van Moerkerke / Keurslager Kurt on 26/11/2021


//Problem: pot debouncing werkt nog niet

//lookup table with a 256 bit sinewave. There is programming room for a bigger sinewave, but this makes the max sinewave speed slower, as the arduino has to cycle through more values.
#include "sinewavedata.h"

//Declaring output & input pins.

int Output1 = 11;

int Input1 = A2;
int Input2 = A1;
int Input3 = A0;
int Input4 = A3;  



/* Old part of the code to calculate frequency numbers to microseconds needed for each sine step.
 *  
long freq = 10;
long freqms = 1000000/(freq*256);
long minfreq = 1000000/26;   //0.1Hz = +-38462micros
long maxfreq = 1000000/(40*256); //40Hz = +- 98 micros

*/

//initializing some variables that are used later on.

long freqms = 10000;
int var1 = 0;
long timerold = micros();
signed int offset = 0;
int oldinputRead1 = 0;
int oldinputRead2 = 0;
int oldinputRead3 = 0;
int oldinputRead4 = 0;
int toggle = 0;
float scaling = 1.0;
int minval = 0;
int maxval = 255;
int inputReadTime = 0;
volatile uint16_t sample1 = 0;


//Setup of the pins. Also setting the PWM frequency.
void setup()
{
  //Serial.begin(9600);
  pinMode(Output1, OUTPUT);

  pinMode(Input1, INPUT);
  pinMode(Input2, INPUT);
  pinMode(Input3, INPUT);
  pinMode(Input4, INPUT);

  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  TCCR1B = TCCR1B & B11111000 | B00000001; // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz  
}

//Let's loop. Max frequency is determined by max loop speed. Each loop, the microcontroller checks whether enough time has passed to go on to the next Sine step. 
//The inputs get pulled and if the inputs differ enough from the last value, the value gets updated.
//The first analog input sets the frequency by changing the time that needs to pass before the sine wave gets updated.
//Second analog input changes the scaling of the sinewave data using the map() function.
//The third analog input changes the offset of the sinewave by adding or substracting a flat value to/from the sinewave.
//In the end the sinewave gets constrained to values between 0-255, because these are the accepted inputs for the duty cycle of the PWM output. The duty cycle determines the output voltage and thus the sine.

void loop()
{    
    
    int inputRead1 = analogRead(Input1);
    int inputRead2 = analogRead(Input2);
    int inputRead3 = analogRead(Input3);
    int inputRead4 = analogRead(Input4);

    //This loop contains some rescaling to make the LFO finer to adjust in the fast frequency range. This feels more natural to me.
    if(abs(inputRead1 - oldinputRead1) > 3){
      if( inputRead1 < 256){
        freqms = map(inputRead1,0,255,38462,19182); //first quarter of the pot handles slowest half of oscillations
      }
      else{
        if(inputRead1 < 512){
          freqms = map(inputRead1,256,511,19182,9591); //second quarter does half of the next half etc.
        }
        else{
          if(inputRead1 < 900){
            freqms = map(inputRead1,512,900,9591,200);
          }
            else{
              freqms = map(inputRead1,900,1023,200,50);
            }         
        }
      }
      oldinputRead1 = inputRead1;
    }

    //Setting the min and max value of the map() function according to the Attenuator pot input.
    if(abs(inputRead2 - oldinputRead2) > 3){

      int scalingPre = map(inputRead2, 0, 1023, 0, 10);
      scaling = scalingPre/10.0;
      minval = 122.0-scaling*122.0;
      maxval = scaling*122.0 + 122.0;
      oldinputRead2 = inputRead2;
      
    }

    //Calculating offset
    //if(abs(inputRead3 - oldinputRead3) > 3){
      offset = map(inputRead3, 0, 1023, -122, 122);
      oldinputRead3 = inputRead3;
      //Serial.println(offset);
    //}
    
  long timer = micros();


  //Hope to get the last input working as sync input.
  if((inputRead4 - oldinputRead4 > 200 || inputRead4 - oldinputRead4 < 200) && (timer- inputReadTime > 10000)){
    if(inputRead4 >700 && toggle == 0){
      toggle = 1;
      oldinputRead4 = inputRead4;
      sample1 = -1;
    }
    else{
      if(inputRead4 < 700 && toggle == 1){
        toggle = 0;
        oldinputRead4 = inputRead4;
      }
    }
  }

  //Finally determine the Sinewave value between 0-255.
  if(timer - timerold >= freqms){
    timerold = timer;
    if (sample1 >= sinewave_length) {  
      sample1 = -1;
    }
    else {
      
      var1 = pgm_read_byte(&sinewave_data[sample1]);
      int minvalSin = constrain(minval+offset,0,255);
      int maxvalSin = constrain(maxval+offset,0,255);      
      var1 = map(var1, 0,255,minvalSin,maxvalSin);      
      var1 = constrain(var1,0,255);
      
    }
    //Update sinewave step
    ++sample1;

    //Update output PWM pin.
    analogWrite(Output1, var1);
    
  }
}
