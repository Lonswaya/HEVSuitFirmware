#include <wiringPi.h>
#include <cstdio>


// BCM gpio numbers please
// If adding more pins, go to Properties -> Build Events -> Remote Post-Build Event command and add gpio export
// so we dont have to run the program with sudo every time


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
	pinMode(PLAY_1_LED, INPUT);
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