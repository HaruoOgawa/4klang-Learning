#pragma warning(disable : 4996)

#include <stdint.h>
#include <fstream>

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

bool WriteWaveFile(const char* FileName, void* pData, int32_t nDataSize, int16_t nNumChannels, int32_t nSampleRate, int32_t nBitsPerSample)
{
	// ファイルオブジェクトの生成
	FILE* File = fopen(FileName, "w+b");
	if (!File) return false;

	SMinimalWaveFileHeader waveHeader;

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

	// ファイルにヘッダを書きこむ
	fwrite(&waveHeader, sizeof(SMinimalWaveFileHeader), 1, File);

	// waveデータを書き込む
	fwrite(pData, nDataSize, 1, File);

	// ファイルのエクスポート
	fclose(File);

	return true;
}

int main()
{
	// 実験的にSawtooth waveを生成
	int nSampleRate = 44100; // 1秒あたりのオーディオデータのサンプル数
	// サンプル レートは、オーディオ ストリームに格納できる最大周波数も定義します。保存できる最大周波数は、サンプル レートの半分
	int nNumSeconds = 4; // 再生時間
	int nNumChannels = 1; // オーディオチャンネル数

	int nNumSamples = nSampleRate * nNumChannels * nNumSeconds; // オーディオサンプルの合計
	int32_t* pData = new int32_t[nNumSamples];

	// Wave Data を作成
	int32_t nValue = 0;
	for (int nIndex = 0; nIndex < nNumSamples; nIndex++)
	{
		nValue += 8000000;
		pData[nIndex] = nValue;
	}

	//
	
	WriteWaveFile("E:\\CppDev\\Cpp-Synth-Learning\\out\\test.wav", pData,
		nNumSamples * sizeof(pData[0]), nNumChannels, nSampleRate, sizeof(pData[8]) * 8);

	delete pData;


	return 1;
}