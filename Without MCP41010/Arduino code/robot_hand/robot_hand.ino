#include <Servo.h>

char data;     // Varriabel for reading incoming byte over serial port 
char res[100]; // Character array for storing read bytes
int i;         // Varriable for incrementing char array index


Servo thumb;
Servo pointer;
Servo middle_finger;
Servo ring_finger;
Servo pinky;


 
void setup()
{
  
Serial.begin(9600);

servo_finger_attach();
default_fingers_close();

}

void loop()
{
  
  do_serial();
  
  String r(res);
  
  do_fingers(r);
        
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
}


void default_fingers_open()
{   
 thumb.write(50); // close 
 pointer.write(150); // close
 middle_finger.write(150); // close
 ring_finger.write(150); // close
 pinky.write(150); // close
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
    }

    if(r[0] == '0')
    {
      thumb.write(150);   //close
    }

////////////////////////////////////////////



////////   POINTER  ////////////////////////

    if(r[3] == '1')
    {
      pointer.write(150); // open        
    }

    if(r[3] == '0')
    {
      pointer.write(50); // close      
    }

////////////////////////////////////////////



////////   MIDDLE FINGER  //////////////////

    if(r[6] == '1')
    {
      middle_finger.write(150); // open          
    }

    if(r[6] == '0')
    {
      middle_finger.write(50); // close          
    }

///////////////////////////////////////////




////////   RING FINGER  //////////////////

    if(r[9] == '1')
    {
      ring_finger.write(150); // open            
    }

    if(r[9] == '0')
    {
      ring_finger.write(50); // close            
    }

///////////////////////////////////////////



//////////   Pinky  //////////////////////

    if(r[12] == '1')
    {
      pinky.write(150); // open 
    }

    if(r[12] == '0')
    {
      pinky.write(50); // close 
    }

///////////////////////////////////////////
}

