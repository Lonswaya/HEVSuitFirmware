#include <wiringPi.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

// BCM gpio numbers please
// If adding more pins, go to Properties -> Build Events -> Remote Post-Build Event command and add gpio export
// so we dont have to run the program with sudo every time


#define	CHANNEL_TOGGLE_BUTTON	2
#define PLAY_1_BUTTON 3
#define PLAY_2_BUTTON 4
#define PLAY_3_BUTTON 14
#define PLAY_4_BUTTON 15

constexpr auto BUTTON_COUNT = 4;
constexpr auto AUDIO_PATH = "~/fvox/%s.wav";

std::vector<std::string> audioSamples
{
	"audio1",
	"audio2",
	"audio3",
	"audio4",
	"audio5",
	"audio6",
	"audio7",
	"audio8",
	"audio9",
	"audio10",
	"audio11",
	"audio12",
	"audio13",
	"audio14",
	"audio15",
	"audio16",
};

int channel = 0;
bool playing = false;

void playAudio(int channel, int buttonId)
{
	// Button count is 4
	int songIndex = (channel * (BUTTON_COUNT - 1)) + buttonId;
	char songNameBuff[128];
	std::string selectedSong = audioSamples[songIndex];
	std::sprintf(songNameBuff, AUDIO_PATH, selectedSong);
	printf("SongName: %s\n", songNameBuff);
}

int main(void)
{
	int CHANNEL_MAX = (audioSamples.size() / BUTTON_COUNT);

	wiringPiSetupSys();

	pinMode(CHANNEL_TOGGLE_BUTTON, INPUT);
	pinMode(PLAY_1_BUTTON, INPUT);
	pinMode(PLAY_2_BUTTON, INPUT);
	pinMode(PLAY_3_BUTTON, INPUT);
	pinMode(PLAY_4_BUTTON, INPUT);

	// Play init audio TODO

	int lastChannelToggle;
	int buttonStates[BUTTON_COUNT];
	int lastButtonStates[BUTTON_COUNT];

	while (true)
	{
		delay(100);

		int channelToggle = digitalRead(CHANNEL_TOGGLE_BUTTON);
		buttonStates[0] = digitalRead(PLAY_1_BUTTON);
		buttonStates[1] = digitalRead(PLAY_2_BUTTON);
		buttonStates[2] = digitalRead(PLAY_3_BUTTON);
		buttonStates[3] = digitalRead(PLAY_4_BUTTON);
		//printf("Inputs cycle %d: %d %d %d %d %d\n", ++count, channelToggle, play1, play2, play3, play4);
		
		if (channelToggle && channelToggle != lastChannelToggle)
		{
			channel = (channel + 1) % CHANNEL_MAX;
			printf("Channel toggle: %d\n", channel);
		}

		lastChannelToggle = channelToggle;
		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			int currentState = buttonStates[i];
			int lastButtonState = lastButtonStates[i];
			if (currentState && currentState != lastButtonState)
			{
				playAudio(channel, i);
			}
			lastButtonStates[i] = currentState;
		}
	}
	return 0;
}