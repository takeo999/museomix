/*****************************************************************************/
//	Function: control the seeedstudo Grove MP3 player
//      Hardware: Grove - Serial MP3 Player
/*******************************************************************************/
#include <SoftwareSerial.h>

SoftwareSerial mp3(2, 3);//modify this with the connector you are using.
void setup()
{
	mp3.begin(9600);
	Serial.begin(9600); 
	//Connexion avec le tapis de détection de mouvement
	pinMode(12,INPUT);

	delay(100);
	if (true ==SetPlayMode(0x01))
		Serial.println("Set The Play Mode to 0x01, Single Loop Mode.");
	else
		Serial.println("Playmode Set Error");       
}
void loop()
{
	SetPlayMode(0x01);
	delay(200);
	SetMusicPlay(00,01);
	delay(300);
	SetVolume(0x1F);
	/* On utilise plusieurs mémoire afin de s'assurer de ne pas prendre en
	   compte les perturbations liées au tapis. */
	int current = 1;
	int mem0 = 1;
	int mem1 = 1;
	int mem2 = 1;
	int mem3 = 1;
	int state = 2;
	int compteur = 0;
	while(1){
		current = digitalRead(12);
		Serial.println(current);
		delay(200);
		// Mode par défaut qui lance le son d'appel
		if(state==2){
			SetPlayMode(0x01);
			delay(200);
			SetMusicPlay(0,2);
			delay(200);
			state = 1;
		/* Mode correspondant à une personne présente
		   Comportement choisi : lecture complète du son. */
		}else if(state == 0){
			compteur++;
			if(compteur>=368){
				compteur=0;
				state=2;
				SetMusicPlay(0,2);
				delay(200);
				SetPlayMode(0x01);
				delay(200);
			}
		// Mode correspondant à une absence de personne
		}else{
			compteur++;
			mem3 = mem2;
			mem2 = mem1;
			mem1=mem0;
			mem0 = current;
			current = digitalRead(12);
			// 10s de lecture minimum
			if(mem0 == current && mem1 == mem0 && mem2 == mem1 && mem2 == mem3 
					&& current!=state && compteur > 50){
				state = current;
				SetPlayMode(0x02);
				delay(200);
				SetMusicPlay(0,1);
				delay(200);
			}
		}
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
