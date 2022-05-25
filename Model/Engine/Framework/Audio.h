#pragma once
#ifndef __AUDIO_H__
#define __AUDIO_H__

#pragma region INCLUDE
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <unordered_map>
#pragma endregion

struct CSoundClip
{
	~CSoundClip();

	LPDIRECTSOUNDBUFFER8 _secondaryBuffer = NULL;

	bool Play();
};
typedef CSoundClip* pSoundClip;

class CAudio
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	LPDIRECTSOUND8 _directSound = NULL;
	LPDIRECTSOUNDBUFFER _primaryBuffer = NULL;
	std::unordered_map<unsigned int, pSoundClip> _secondaryBuffers;

public:
	void Initialize(HWND hWnd);
	void Shutdown();

	pSoundClip LoadSoundClip(std::string source);
};
typedef CAudio* pAudio;

#endif // !__AUDIO_H__
