#include <wiringPi.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

// BCM gpio numbers please
// If adding more pins, go to Properties -> Build Events -> Remote Post-Build Event command and add gpio export
// so we dont have to run the program with sudo every time
// gpio export 2 out; gpio export 3 out; gpio export 4 out; gpio export 14 out; gpio export 15 out;


#define	CHANNEL_TOGGLE_BUTTON	2
#define PLAY_1_BUTTON 3
#define PLAY_2_BUTTON 4
#define PLAY_3_BUTTON 14
#define PLAY_4_BUTTON 15

#define UNIQUENESS_PID_FILE "/home/pi/.hev_pid"

constexpr auto CYCLING_BUTTON_COUNT = 4;
constexpr auto AUDIO_PATH = "aplay /home/pi/fvox/";
constexpr auto AUDIO_EXT = ".wav";
constexpr auto AUDIO_POST_PID = "& echo $! > ";


vector<vector<string>> audioSamples
{
	{ "safe_day" },
	{ "eleven", "twelve", "thirteen" },
	{ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"},
	{ "blip", },
	{ "boop", },
	{ "buzz", },
};

int channel = 0;

int getLastPlayedPid()
{
	fstream pidInFile(UNIQUENESS_PID_FILE);
	int result = -1;
	if (pidInFile.is_open())
	{
		string line;
		getline(pidInFile, line);
		result = stoi(line.c_str());

		pidInFile.close();
	}
	return result;
}

void killLastAudioProcess()
{
	int lastPid = getLastPlayedPid();
	if (lastPid != -1)
	{
		// This can kill any other process, but killing a fork won't work because that is just another fork in itself that doesnt forward kills
		int killResult = kill(lastPid, 1);
	}
}

int execCommand(const char* command)
{
	// This should execute a command, and spit the pid of that command into the pid file
	int pid = fork();
	if (pid == 0)
	{
		system(command);
		exit(EXIT_SUCCESS);
	}
	if (pid > 0)
	{
		// Wait for pid to be recorded
		wait(NULL);
		return getLastPlayedPid();
	}
}

int audioSampleIndex = -1;
int audioSampleTrackPoint = -1;
int audioTrackSeriesPid = -1;

void playNextAudioTrackSeries()
{
	audioTrackSeriesPid = -1;
	vector<string> sampleArray = audioSamples[audioSampleIndex];

	string selectedSong = sampleArray[audioSampleTrackPoint];
	string path = AUDIO_PATH;
	string ext = AUDIO_EXT;
	// example:  aplay /home/pi/fvox/hev_logon.wav & echo $! > /home/pi/.hev_pid
	string songCommand = path + selectedSong + ext + AUDIO_POST_PID + UNIQUENESS_PID_FILE;
	printf("Song Command: %s\n", songCommand.c_str());

	audioTrackSeriesPid = execCommand(songCommand.c_str());
	audioSampleTrackPoint++;

	if (audioSampleTrackPoint >= sampleArray.size())
	{
		audioTrackSeriesPid = -1;
		audioSampleIndex = -1;
		audioSampleTrackPoint = -1;
		printf("Done playing\n");
	}
}

void initiateAudioSeriesPlay(int channel, int buttonId)
{
	killLastAudioProcess();
	int songIndex = (channel * (CYCLING_BUTTON_COUNT)) + buttonId;
	audioSampleIndex = songIndex;
	audioSampleTrackPoint = 0;
	playNextAudioTrackSeries();
}

bool isProcessRunning(int pid)
{
	// Sending a signal 0 does nothing, but still error checks
	return kill(pid, 0) == 0;
}

int main(void)
{
	/* Ensure program uniqueness */
	//killLastAudioProcess();

	//remove(UNIQUENESS_PID_FILE);

	/* Add our own PID 
	ofstream pidOutFile(UNIQUENESS_PID_FILE, ofstream::trunc);
	if (pidOutFile.is_open())
	{
		pidOutFile << getpid() << "\n";
		pidOutFile.close();
	}*/

	int CHANNEL_MAX = (audioSamples.size() / CYCLING_BUTTON_COUNT);

	wiringPiSetupSys();

	pinMode(CHANNEL_TOGGLE_BUTTON, INPUT);
	pinMode(PLAY_1_BUTTON, INPUT);
	pinMode(PLAY_2_BUTTON, INPUT);
	pinMode(PLAY_3_BUTTON, INPUT);
	pinMode(PLAY_4_BUTTON, INPUT);

	// Play init audio TODO

	int lastChannelToggle;
	int buttonStates[CYCLING_BUTTON_COUNT];
	int lastButtonStates[CYCLING_BUTTON_COUNT];

	int count = 0;

	while (true)
	{
		delay(10);

		int channelToggle = digitalRead(CHANNEL_TOGGLE_BUTTON);
		buttonStates[0] = digitalRead(PLAY_1_BUTTON);
		buttonStates[1] = digitalRead(PLAY_2_BUTTON);
		buttonStates[2] = digitalRead(PLAY_3_BUTTON);
		buttonStates[3] = digitalRead(PLAY_4_BUTTON);
		//printf("Inputs cycle %d: %d %d %d %d %d\n", ++count, channelToggle, buttonStates[0], buttonStates[1], buttonStates[2], buttonStates[3]);

		if (audioTrackSeriesPid != -1 && !isProcessRunning(audioTrackSeriesPid))
		{
			playNextAudioTrackSeries();
		}
		
		if (channelToggle && channelToggle != lastChannelToggle)
		{
			channel = (channel + 1) % CHANNEL_MAX;
			printf("Channel toggle: %d\n", channel);
		}

		lastChannelToggle = channelToggle;
		for (int i = 0; i < CYCLING_BUTTON_COUNT; i++)
		{
			int currentState = buttonStates[i];
			int lastButtonState = lastButtonStates[i];
			if (currentState && currentState != lastButtonState)
			{
				initiateAudioSeriesPlay(channel, i);
			}
			lastButtonStates[i] = currentState;
		}
	}
	return 0;
}