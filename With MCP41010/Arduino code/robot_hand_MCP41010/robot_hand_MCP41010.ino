#include <Servo.h>
#include <SPI.h>
#include <Smoothed.h>   // Include the library

byte address = 0x11; // 0B00010001
int CS= 10;

int pot_value;

char data;     // Varriabel for reading incoming byte over serial port 
char res[100]; // Character array for storing read bytes
int i;         // Varriable for incrementing char array index

bool relay;

Servo thumb;
Servo pointer;
Servo middle_finger;
Servo ring_finger;
Servo pinky;


Smoothed <int> voltage; 
float smoothedVoltageValueAvg;

float v_max = 6.0;   //  4.8v to 6v is a valide serov voltage range for readbacks to keep the output Enabled.
float v_low = 4.8;   //

void setup()
{
  pinMode(7, OUTPUT);       // Output Relay or MOSFET
  digitalWrite(7, LOW);     // Output Off 
  
  voltage.begin(SMOOTHED_AVERAGE, 100); 
  voltage.clear();
  
  pinMode (CS, OUTPUT);
  SPI.begin();  
  Serial.begin(9600);

  servo_finger_attach();
  default_fingers_close();


  pot_value = 27; /// will set PSU to 5V   
  digitalPotWrite(pot_value);

  relay = false;

  delay(5000);   // give a 5 second delay for the Power supply to settle down to voltage set point
  
  //digitalWrite(7, HIGH);
}

void loop()
{
  digitalPotWrite(pot_value);
  int v_value = analogRead(A0);
    
  // Add the new value to both sensor value stores
  voltage.add(v_value);
    
  // Get the smoothed values
  smoothedVoltageValueAvg = voltage.get() * 0.0544086021505376;

  
  Serial.println(smoothedVoltageValueAvg); 

  if(smoothedVoltageValueAvg >= v_low && smoothedVoltageValueAvg <= v_max)
  {
   digitalWrite(7, HIGH);
   relay = true;
   //Serial.println("valid");
  }
  else
  {
   digitalWrite(7, LOW); 
   relay = false;
   //Serial.println("not valid");
  }

  if(relay == true)
  {
  do_serial();
  
  String r(res);
  
  do_fingers(r);
  }
        
}


void servo_finger_attach()
{
  thumb.attach(2);     
  pointer.attach(3);     
  middle_finger.attach(4);      
  ring_finger.attach(5);
  pinky.attach(6);  
}


void default_fingers_close()
{   
 thumb.write(150); // close 
 pointer.write(50); // close
 middle_finger.write(50); // close
 ring_finger.write(50); // close
 pinky.write(50); // close

 delay(10);
}


void default_fingers_open()
{   
 thumb.write(50); // close 
 pointer.write(150); // close
 middle_finger.write(150); // close
 ring_finger.write(150); // close
 pinky.write(150); // close
 
 delay(10);
}


void do_serial()
{

  i=0;   
  while( data!='$')     
  {
    while(Serial.available())
    {
      data=Serial.read();
      res[i]=data;
      i++;
    }
  }
  data=0;

}


void do_fingers(String r)
{
  ////////  THUMB   //////////////////////////

    if(r[0] == '1')
    {
      thumb.write(50);   //open
      delay(10);
    }

    if(r[0] == '0')
    {
      thumb.write(150);   //close
      delay(10);
    }

////////////////////////////////////////////



////////   POINTER  ////////////////////////

    if(r[3] == '1')
    {
      pointer.write(150); // open
      delay(10);         
    }

    if(r[3] == '0')
    {
      pointer.write(50); // close
      delay(10);       
    }

////////////////////////////////////////////



////////   MIDDLE FINGER  //////////////////

    if(r[6] == '1')
    {
      middle_finger.write(150); // open 
      delay(10);          
    }

    if(r[6] == '0')
    {
      middle_finger.write(50); // close 
      delay(10);          
    }

///////////////////////////////////////////




////////   RING FINGER  //////////////////

    if(r[9] == '1')
    {
      ring_finger.write(150); // open 
      delay(10);            
    }

    if(r[9] == '0')
    {
      ring_finger.write(50); // close  
      delay(10);           
    }

///////////////////////////////////////////



//////////   Pinky  //////////////////////

    if(r[12] == '1')
    {
      pinky.write(150); // open 
      delay(15);
    }

    if(r[12] == '0')
    {
      pinky.write(50); // close 
      delay(15);
    }

///////////////////////////////////////////
}




int digitalPotWrite(int value)
{
   digitalWrite(CS,LOW); // select the chip
   SPI.transfer(address); // command byte
   SPI.transfer(value); // data byte 
   digitalWrite(CS,HIGH); // de-select the chip
}

