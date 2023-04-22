#include "SoundPlayer.h"
#include <Windows.h>

namespace sound 
{
	bool SoundPlayer::Play(unsigned char* pData, bool IsLoop)
	{
		// SND_MEMORY: メモリからデータを取得する
		// SND_ASYNC: 非同期で再生する(デフォルトはSND_SYNCで同期再生)
		auto Flags = SND_MEMORY | SND_ASYNC;
		if (IsLoop) Flags |= SND_LOOP;
		return PlaySound((LPCTSTR)(&pData[0]), NULL, Flags);
	}

	bool SoundPlayer::Stop()
	{
		return PlaySound(NULL, 0, 0);
	}
}
