#define audioPin A0
#define vibratorPin 3

#define adcMax 550
#define pwmMAX 255

void setup(void)
{
	pinMode(vibratorPin, OUTPUT);
}

void loop(void) 
{
	analogWrite(vibratorPin,map(analogRead(audioPin),0,adcMax,0,pwmMAX));
}