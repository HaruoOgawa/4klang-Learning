#pragma warning(disable : 4996)

#include "SoundPlayer.h"
#include <stdint.h>
#include <Windows.h>
#include <vector>
#include <time.h>

#include "sound/4klang.h"

//this struct is the minimal required header data for a wav file
struct SMinimalWaveFileHeader
{
	//the main chunk
	unsigned char m_szChunkID[4];
	uint32_t m_nChunkSize;
	unsigned char m_szFormat[4];

	//sub chunk 1 "fmt "
	unsigned char m_szSubChunk1ID[4];
	uint32_t m_nSubChunk1Size;
	uint16_t m_nAudioFormat;
	uint16_t m_nNumChannels;
	uint32_t m_nSampleRate;
	uint32_t m_nByteRate;
	uint16_t m_nBlockAlign;
	uint16_t m_nBitsPerSample;

	//sub chunk 2 "data"
	unsigned char m_szSubChunk2ID[4];
	uint32_t m_nSubChunk2Size;

	//then comes the data!
};

void GetWaveHeader(SMinimalWaveFileHeader& waveHeader, int32_t nDataSize, int16_t nNumChannels, int32_t nSampleRate, int32_t nBitsPerSample)
{
	// メインチャンク
	std::memcpy(waveHeader.m_szChunkID, "RIFF", 4);
	waveHeader.m_nChunkSize = nDataSize + 36; // 36はたぶんヘッダの分かな？
	std::memcpy(waveHeader.m_szFormat, "WAVE", 4);

	// サブチャンク 1
	std::memcpy(waveHeader.m_szSubChunk1ID, "fmt ", 4);
	waveHeader.m_nSubChunk1Size = 16;
	waveHeader.m_nAudioFormat = 1;
	waveHeader.m_nNumChannels = nNumChannels; // チャンネル数
	waveHeader.m_nSampleRate = nSampleRate; // サンプリングの間隔
	waveHeader.m_nByteRate = nSampleRate * nNumChannels * nBitsPerSample / 8;
	waveHeader.m_nBlockAlign = nNumChannels * nBitsPerSample / 8;
	waveHeader.m_nBitsPerSample = nBitsPerSample;

	// サブチャンク 2
	std::memcpy(waveHeader.m_szSubChunk2ID, "data", 4);
	waveHeader.m_nSubChunk2Size = nDataSize;
}

bool PlayAudioOnApp(void* pData, int32_t nDataSize, int16_t nNumChannels, int32_t nSampleRate, int32_t nBitsPerSample)
{
	// wavを動的生成
	SMinimalWaveFileHeader waveHeader;
	GetWaveHeader(waveHeader, nDataSize, nNumChannels, nSampleRate, nBitsPerSample);
	size_t HeaderSize = sizeof(SMinimalWaveFileHeader) * 1;

	std::vector<unsigned char> WavData;
	WavData.resize(HeaderSize + nDataSize);
	std::memcpy(&WavData[0], &waveHeader, sizeof(SMinimalWaveFileHeader));
	std::memcpy(&WavData[sizeof(SMinimalWaveFileHeader) * 1], pData, nDataSize);

	//
	sound::SoundPlayer player;
	if(!player.Play(WavData.data(), true)) return false;

	while (true)
	{
		printf("Playing ---------------------\n");

		if (GetKeyState(VK_ESCAPE))
		{
			break;
		}
	}

	player.Stop();
	WavData.clear();

	return true;
}

int main()
{
	int nNumSamples = MAX_SAMPLES * POLYPHONY;

	SAMPLE_TYPE* buffer = new SAMPLE_TYPE[nNumSamples];
	_4klang_render(buffer);
	PlayAudioOnApp(buffer, nNumSamples * sizeof(SAMPLE_TYPE), POLYPHONY, SAMPLE_RATE, sizeof(SAMPLE_TYPE) * 8);

	delete buffer;

	return 1;
}