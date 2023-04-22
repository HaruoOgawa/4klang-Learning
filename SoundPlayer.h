#pragma once

namespace sound 
{
	class SoundPlayer
	{
	public:
		SoundPlayer() = default;
		virtual ~SoundPlayer() = default;

		bool Play(unsigned char* pData, bool IsLoop);
		bool Stop();
	};
}