#include <NewPing.h> // https://bitbucket.org/teckel12/arduino-new-ping/downloads/

#define sensorPin A3
#define vibratorPin 10

#define MAX_DISTANCE 200
#define triggerPin 7
#define echoPin 8

NewPing sonar(triggerPin, echoPin, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int adcNeutral=700; //define a center value (neutral) for when sensor isn't actuated
int threshold=10; //define a minimum divergence from neutral which will activate the output.
float adcGain=1; //set initial gain to something sensible

#define pwmMAX 255

void setup(void)
{
    Serial.begin(115200);

	pinMode(vibratorPin, OUTPUT);
    pinMode(5, INPUT_PULLUP); //sensor detect pin. This is pulled low when the ping sensor is attached.

}

int state = 0; //0=ADC mode, 1=PING mode.
long distance;
int diff;

void loop(void) 
{
    //set machine state (works on the fly)
    if(digitalRead(5)) state=0;
    else state=1;

    //gain and offset settings (slide pots)
    adcNeutral=analogRead(A0); //directly transferable since all we have is voltage dividers :)
    adcGain=(float)analogRead(A1)/(float)512.0; //divided by 512 to give an effective gain range of 0.0-2.0


    if (state==0) //adc mode
    {
    int adcIn=analogRead(sensorPin);
    diff=abs(adcNeutral-adcIn);
    } //end of adc mode

    else if (state==1) //ping mode.
    {
    int neutralDistance=map(adcNeutral,0,1023,0,MAX_DISTANCE); //map the ADC range to between 0 and 200 cm.

    distance=sonar.ping_cm();

    if (distance >= MAX_DISTANCE     || distance <= 0){
        diff=0;
        Serial.println("Out of range");
        }
    else {

        diff=abs(neutralDistance-distance);
    }
    delay(50);
    } //end of ping mode
    


    delay(5);

    if(diff<threshold) digitalWrite(vibratorPin,LOW); //if divergence between neutral and sensor input is below threshold.
    else analogWrite(vibratorPin,map(diff*adcGain,0,512,0,pwmMAX)); //that 512 is just magic, don't try to do a thing... or you know.. whatever. try.. 

}