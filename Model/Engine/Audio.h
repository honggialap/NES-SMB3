#pragma once
#ifndef __AUDIO_H__
#define __AUDIO_H__

#pragma region INCLUDE
#include <dsound.h>
#include <string>
#pragma endregion

class CAudio
{
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

	LPDIRECTSOUND8 _device = NULL;
	LPDIRECTSOUNDBUFFER _primaryBuffer = NULL;

public:
	LPDIRECTSOUND8 GetDevice() { return _device; }

	bool Initialize(
		HWND hWnd
	);
	void Shutdown();

	bool LoadSoundFromFile(
		LPDIRECTSOUNDBUFFER8 secondaryBuffer,
		std::string soundFilePath
	);
};
typedef CAudio* pAudio;

#endif // !__AUDIO_H__
