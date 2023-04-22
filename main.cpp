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
	// �t�@�C���I�u�W�F�N�g�̐���
	FILE* File = fopen(FileName, "w+b");
	if (!File) return false;

	SMinimalWaveFileHeader waveHeader;

	// ���C���`�����N
	std::memcpy(waveHeader.m_szChunkID, "RIFF", 4);
	waveHeader.m_nChunkSize = nDataSize + 36; // 36�͂��Ԃ�w�b�_�̕����ȁH
	std::memcpy(waveHeader.m_szFormat, "WAVE", 4);

	// �T�u�`�����N 1
	std::memcpy(waveHeader.m_szSubChunk1ID, "fmt ", 4);
	waveHeader.m_nSubChunk1Size = 16;
	waveHeader.m_nAudioFormat = 1;
	waveHeader.m_nNumChannels = nNumChannels; // �`�����l����
	waveHeader.m_nSampleRate = nSampleRate; // �T���v�����O�̊Ԋu
	waveHeader.m_nByteRate = nSampleRate * nNumChannels * nBitsPerSample / 8;
	waveHeader.m_nBlockAlign = nNumChannels * nBitsPerSample / 8;
	waveHeader.m_nBitsPerSample = nBitsPerSample;

	// �T�u�`�����N 2
	std::memcpy(waveHeader.m_szSubChunk2ID, "data", 4);
	waveHeader.m_nSubChunk2Size = nDataSize;

	// �t�@�C���Ƀw�b�_����������
	fwrite(&waveHeader, sizeof(SMinimalWaveFileHeader), 1, File);

	// wave�f�[�^����������
	fwrite(pData, nDataSize, 1, File);

	// �t�@�C���̃G�N�X�|�[�g
	fclose(File);

	return true;
}

int main()
{
	// �����I��Sawtooth wave�𐶐�
	int nSampleRate = 44100; // 1�b������̃I�[�f�B�I�f�[�^�̃T���v����
	// �T���v�� ���[�g�́A�I�[�f�B�I �X�g���[���Ɋi�[�ł���ő���g������`���܂��B�ۑ��ł���ő���g���́A�T���v�� ���[�g�̔���
	int nNumSeconds = 4; // �Đ�����
	int nNumChannels = 1; // �I�[�f�B�I�`�����l����

	int nNumSamples = nSampleRate * nNumChannels * nNumSeconds; // �I�[�f�B�I�T���v���̍��v
	int32_t* pData = new int32_t[nNumSamples];

	// Wave Data ���쐬
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