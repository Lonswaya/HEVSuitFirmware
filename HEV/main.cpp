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

constexpr auto CHANNEL_MAX = 5;

int main(void)
{
	wiringPiSetupSys();

	pinMode(CHANNEL_TOGGLE_LED, INPUT);
	pinMode(PLAY_1_LED, INPUT);
	pinMode(PLAY_2_LED, INPUT);
	pinMode(PLAY_3_LED, INPUT);
	pinMode(PLAY_4_LED, INPUT);

	// Play init audio TODO

	int channel = 0;
	bool playing = false;

	int lastChannelToggle;
	int lastPlay1;
	int lastPlay2;
	int lastPlay3;
	int lastPlay4;

	while (true)
	{
		int channelToggle = digitalRead(CHANNEL_TOGGLE_LED);
		int play1 = digitalRead(PLAY_1_LED);
		int play2 = digitalRead(PLAY_2_LED);
		int play3 = digitalRead(PLAY_3_LED);
		int play4 = digitalRead(PLAY_4_LED);
		//printf("Inputs cycle %d: %d %d %d %d %d\n", ++count, channelToggle, play1, play2, play3, play4);
		delay(100);

		if (channelToggle && channelToggle != lastChannelToggle)
		{
			channel = (channel + 1)% CHANNEL_MAX;
			printf("Channel toggle: %d\n", channel);

		}
		if (play1 && play1 != lastPlay1)
		{
			// TODO play
			printf("play1\n");
		}

		if (play2 && play2 != lastPlay2)
		{
			// TODO play
			printf("play2\n");
		}

		if (play3 && play3 != lastPlay3)
		{
			// TODO play
			printf("play3\n");
		}

		if (play4 && play4 != lastPlay4)
		{
			// TODO play
			printf("play4\n");
		}

		lastChannelToggle = channelToggle;
		lastPlay1 = play1;
		lastPlay2 = play2;
		lastPlay3 = play3;
		lastPlay4 = play4;
	}
	return 0;
}