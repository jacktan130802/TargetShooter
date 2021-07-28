#include "SoftwareSerial.h"
SoftwareSerial mySerial(4, 5);
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

# define ACTIVATED HIGH


int pins[] = {11, 10, 9}; // an array of pins! This is similar to int pin0 = 11; int pin1=10; int pin2=9;
long values[3]; // make an array of values but don't give them a value just yet
long current_values[3]; // make an array of current values, but don't give them a value yet



int winP = 2; //initialize win pin
int loseP = 3; //initialize lose pin

boolean isPlaying = false;

int count=0, count2=0;



void setup () {

randomSeed(analogRead(0)); // get some unpredictable value to start off our random number generator
 // otherwise, we'd get the same random numbers each time (boring!)
 for (int i=0; i <3; i++) { // pins 0 to less than 3
 values[i] = random(255); // pick a random number between 0 and 255 for a pin (red, green, or blue)
 current_values[i] = values[i]; // make our current value the same
 analogWrite(pins[i], current_values[i]); // set the LED to our initial choice
 values[i] = random(255); // pick a new random number for our next color
 }
 

pinMode(winP, INPUT);
digitalWrite(winP,LOW);
pinMode(loseP, INPUT);
digitalWrite(loseP,LOW);

mySerial.begin (9600);
delay(1000);
playFirst(); //play first track (background music)
isPlaying = true; 


}



void loop () { 

  while (digitalRead (winP)==LOW && digitalRead (loseP)==LOW)
  {
    ledBackground(); //run background led code
  }

 if (digitalRead(winP) == ACTIVATED && count==0)
  {
    if(isPlaying)
    {
      playNext(); //play cheering music (2nd track on microSD)
      ledWin(); //run winning led code
      count ++; //increase count to stop loop
    }
  }

 if (digitalRead(loseP) == ACTIVATED && count==0)
  {
    if(isPlaying)
    {
      playNext();
      playNext(); // play losing music (3rd track on microSD)
      ledLose(); // run losing led code
      count++; //increase count to stop loop
    }
  }
}


void ledWin() //winning led code
{
  int gLed=11, rLed=10, bLed=9; //reinitialise pins
  
  pinMode (gLed, OUTPUT); // declare output pins
  pinMode (rLed, OUTPUT);
  pinMode (bLed, OUTPUT);

while(count2<20)
{
  digitalWrite (gLed, LOW);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, HIGH);
  delay(30);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, HIGH);
  delay (50);
  
  digitalWrite (gLed, LOW);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, HIGH);
  delay(30);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, HIGH);
  delay (50);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, LOW);
  digitalWrite (bLed, HIGH);
  delay(30);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, HIGH);
  delay (50);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, LOW);
  digitalWrite (bLed, HIGH);
  delay(30);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, HIGH);
  delay (50);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, LOW);
  delay(30);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, HIGH);
  delay (50);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, LOW);
  delay(30);

  digitalWrite (gLed, HIGH);
  digitalWrite (rLed, HIGH);
  digitalWrite (bLed, HIGH);
  delay (50);
  
  count2++;
}

  digitalWrite (gLed, HIGH);
}

void ledLose() //losing led code
{
    int gLed=11, rLed=10, bLed=9; //reinitialise pins
    pinMode (gLed, OUTPUT);
    pinMode (rLed, OUTPUT);
    pinMode (bLed, OUTPUT);
 
    digitalWrite (gLed, HIGH);
    digitalWrite (bLed, HIGH);

    while (count2<17)
    {
      digitalWrite (rLed, LOW);
      delay (200);
      digitalWrite (rLed, HIGH);
      delay (200);

      count2++;
    }

    digitalWrite (rLed, HIGH);
    
}


void ledBackground() // background led code
{
  for (int i=0; i <3; i++) {
 if (values[i] > current_values[i]){ // if our new color is a larger number than our current color ...
 current_values[i]++; // get just a little bit closer to the new color by adding 1
 analogWrite(pins[i], current_values[i]); // set the LED to the new current color
 delay(2); // wait a little bit
 }

 if (values[i] < current_values[i]){ // if our new color is a smaller number than our current color ...
 current_values[i]--; // get just a little bit closer to the new color by subtracting 1
 analogWrite(pins[i], current_values[i]); // set the LED to the new current color
 delay(2); // wait a little bit

 }
 if (values[i] == current_values[i]){ // if our new color and our current color are the same ...
 analogWrite(pins[i], current_values[i]); // make sure the LED is set to the new color
 values[i] = random(255); // pick a new color
 }
 }
}


void playFirst() //function to play first track
{
  execute_CMD(0x3F, 0, 0);
  delay(500);
  setVolume(80);
  delay(500);
  execute_CMD(0x11,0,1); 
  delay(500);
}


void playNext() //function to play next track
{
  execute_CMD(0x01,0,1);
  delay(500);
}

void pause() //function to pause playback
{
  execute_CMD(0x0E,0,0);
  delay(500);
}

void play() //function to start playback
{
  execute_CMD(0x0D,0,1); 
  delay(500);
}

void setVolume(int volume) //function to set volume
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}
