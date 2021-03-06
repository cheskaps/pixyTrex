//Remove motorspeed map and constrain.

#include <EEPROM.h>
#include <Wire.h>

#define leftMotorDirPin 2
#define leftMotorPWMPin 3
#define leftMotorBreakPin 4

#define rightMotorBreakPin 9
#define rightMotorDirPin 10
#define rightMotorPWMPin 11

byte I2Caddr;

int MotorSpeed=0;
int Input;
byte leftMotorDir,rightMotorDir;

//For troubleshooting delay()
uint32_t pastTime=0;

void setup()
{
        Serial.begin(9600);
        Wire.begin(I2Caddress());
        uint8_t m[]={2,3,4,9,10,11};
        for(int i=0;i<6;i++){
                pinMode(m[i],OUTPUT);
        }
}

byte  I2Caddress()
{

        byte n=EEPROM.read(0);
        if(n!=0x55){
                EEPROM.write(0,0x55);
                EEPROM.write(1,0x07);
        }
        I2Caddr=EEPROM.read(1);
        return I2Caddr;
}

void getSpeed()
{
        Wire.requestFrom(0x06,4);
        if(Wire.available()==4){
                MotorSpeed=Wire.read()<<8|Wire.read();
                Input=Wire.read()<<8|Wire.read();
		MotorSpeed=MotorSpeed>>3;
        }
        else {
                Serial.println("Cannot connect to Master");
        }
	if(MotorSpeed<0){
		leftMotorDir=1;
		rightMotorDir=1;	
	}
	else{
		leftMotorDir=0;
		rightMotorDir=0;
	}
}

void forward(int leftSpeed, int rightSpeed)
{
        digitalWrite(leftMotorBreakPin,0);
        digitalWrite(rightMotorBreakPin,0);
        digitalWrite(leftMotorDirPin,leftMotorDir);
        digitalWrite(rightMotorDirPin,rightMotorDir);
        analogWrite(leftMotorPWMPin,leftSpeed);
        analogWrite(rightMotorPWMPin,rightSpeed);
}

void loop()
{
        delay(100);
        getSpeed();
        forward(MotorSpeed,MotorSpeed);

	troubleShoot();
}

void troubleShoot()
{
	uint32_t currentTime;
	const int interval=500;
	if((currentTime=millis()-pastTime)>=interval){
		Serial.print(Input);
        	Serial.print("\t");
        	Serial.println(MotorSpeed);
		pastTime=millis();
	}
}
