#include "SoundPlayer.h"
#include <Windows.h>

namespace sound 
{
	bool SoundPlayer::Play(unsigned char* pData, bool IsLoop)
	{
		// SND_MEMORY: ����������f�[�^���擾����
		// SND_ASYNC: �񓯊��ōĐ�����(�f�t�H���g��SND_SYNC�œ����Đ�)
		auto Flags = SND_MEMORY | SND_ASYNC;
		if (IsLoop) Flags |= SND_LOOP;
		return PlaySound((LPCTSTR)(&pData[0]), NULL, Flags);
	}

	bool SoundPlayer::Stop()
	{
		return PlaySound(NULL, 0, 0);
	}
}
