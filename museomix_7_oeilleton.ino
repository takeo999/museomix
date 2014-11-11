/*****************************************************************************/
//	Function: control the seeedstudo Grove MP3 player
//      Hardware: Grove - Serial MP3 Player
/*******************************************************************************/
#include <SoftwareSerial.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic(8);//modify this with the connector you are using.
SoftwareSerial mp3(2, 3);//modify this with the connector you are using.
void setup()
{
	mp3.begin(9600);
	Serial.begin(9600); 
	pinMode(6, OUTPUT);//modify this with the connector you are using.
	pinMode(3, OUTPUT);//modify this with the connector you are using
	digitalWrite(6,LOW);
	digitalWrite(3,LOW);
	delay(100);
	if (true ==SetPlayMode(0x01))
		Serial.println("Set The Play Mode to 0x01, Single Loop Mode.");
	else
		Serial.println("Playmode Set Error");

}
void loop()
{
	SetMusicPlay(00,01);
	delay(1000);
	SetVolume(0x14);
	delay(1000);
	SetPlayMode(0x01);
	int mem = 0;
	int current = 0;
	while(1){
		ultrasonic.MeasureInCentimeters();
		// On adapte current à la position courante de la personne
		if(ultrasonic.RangeInCentimeters < 40){
			current = 1;
		} else {
			current = 0;
		}

		// On modifie l'interrupteur ouvert si la situation a changé
		if(current!=mem){
			if(current == 1){
				digitalWrite(6, HIGH);
				digitalWrite(3,LOW);
			}else{
				digitalWrite(6, LOW);
				digitalWrite(3,HIGH);
			}
		}
		mem=current;
		delay(1000);
	}
}
//Set the music index to play, the index is decided by the input sequence
//of the music;
//hbyte: the high byte of the index;
//lbyte: the low byte of the index;
boolean SetMusicPlay(uint8_t hbyte,uint8_t lbyte)
{
	mp3.write(0x7E);
	mp3.write(0x04);
	mp3.write(0xA0);
	mp3.write(hbyte);
	mp3.write(lbyte);
	mp3.write(0x7E);
	delay(10);
	while(mp3.available())
	{
		if (0xA0==mp3.read())
			return true;
		else 
			return false;
	}
}
// Pause on/off  the current music
boolean PauseOnOffCurrentMusic(void)
{
	mp3.write(0x7E);
	mp3.write(0x02);
	mp3.write(0xA3);
	mp3.write(0x7E);
	delay(10);
	while(mp3.available())
	{
		if (0xA3==mp3.read())
			return true;
		else 
			return false;
	}
}

//Set the volume, the range is 0x00 to 0x1F
boolean SetVolume(uint8_t volume)
{
	mp3.write(0x7E);
	mp3.write(0x03);
	mp3.write(0xA7);
	mp3.write(volume);
	mp3.write(0x7E);
	delay(10);
	while(mp3.available())
	{
		if (0xA7==mp3.read())
			return true;
		else 
			return false;
	}
}

boolean SetPlayMode(uint8_t playmode)
{
	if (((playmode==0x00)|(playmode==0x01)|(playmode==0x02)|(playmode==0x03))==false)
	{
		Serial.println("PlayMode Parameter Error! ");
		return false;
	}
	mp3.write(0x7E);
	mp3.write(0x03);
	mp3.write(0xA9);
	mp3.write(playmode);
	mp3.write(0x7E);
	delay(10);
	while(mp3.available())
	{
		if (0xA9==mp3.read())
			return true;
		else 
			return false;
	}
}
