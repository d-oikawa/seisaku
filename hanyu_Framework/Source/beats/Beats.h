//CSV‚©‚ç‹È‚Ìî•ñ‚ğæ“¾‚µ‚ÄA•ˆ–Ê‚ğì‚éƒNƒ‰ƒX
#pragma once
#include "Fwk/Graphics.h"
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

#include "Fwk/File/CSVFile.h"
#include "Fwk/Audio/SoundSource.h"
#include "Fwk/Audio/MP3Sound.h"

class Beats
{
public:
	void Init();
	void Update();
	void Render();
	void Term();
	int GetBeats();
	float GetTimer();
	float GetSongLength();
	bool IsPlaying();
	void CreateMusic(CSVData* pCsvData);
	void StopBeats();

private:
	string musicName;
	string musicFileName;
	float mSongLength;
	float mTimer;
	float mBeatTimer;
	float mBeats;
	int mBPM;
	int mBeatsCounts;
	SoundSource mSoundSource;
	MP3Sound mSound;

};