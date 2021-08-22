#include <wiringPi.h>
#include <cstdio>


// LED Pin - wiringPi pin 0 is BCM_GPIO 17.
// we have to use BCM numbering when initializing with wiringPiSetupSys
// when choosing a different pin number please use the BCM numbering, also
// update the Property Pages - Build Events - Remote Post-Build Event command 
// which uses gpio export for setup for wiringPiSetupSys
#define	CHANNEL_TOGGLE_LED	2
#define PLAY_1_LED 3
#define PLAY_2_LED 4
#define PLAY_3_LED 14
#define PLAY_4_LED 15

int main(void)
{
	wiringPiSetupSys();
	//pinMode(CHANNEL_TOGGLE_LED, OUTPUT);
	//pinMode(PLAY_1_LED, OUTPUT);


	pinMode(CHANNEL_TOGGLE_LED, INPUT);
	pinMode(CHANNEL_TOGGLE_LED, PUD_DOWN);
	pinMode(PLAY_1_LED, INPUT);
	pinMode(PLAY_1_LED, PUD_UP);
	pinMode(PLAY_2_LED, INPUT);
	pinMode(PLAY_3_LED, INPUT);
	pinMode(PLAY_4_LED, INPUT);

	// Play init audio
	int count = 0;
	while (true)
	{
		/*delay(500); 
		digitalWrite(CHANNEL_TOGGLE_LED, HIGH);
		digitalWrite(PLAY_1_LED, HIGH);
		delay(500);
		digitalWrite(CHANNEL_TOGGLE_LED, LOW);
		digitalWrite(PLAY_1_LED, LOW);
		printf("cycle\n");*/
		
		int channelToggle = digitalRead(CHANNEL_TOGGLE_LED);
		int play1 = digitalRead(PLAY_1_LED);
		int play2 = digitalRead(PLAY_2_LED);
		int play3 = digitalRead(PLAY_3_LED);
		int play4 = digitalRead(PLAY_4_LED);
		printf("Inputs cycle %d: %d %d %d %d %d\n", ++count, channelToggle, play1, play2, play3, play4);
		delay(250);
	}
	return 0;
}