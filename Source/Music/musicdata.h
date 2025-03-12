#pragma once

#include<string>
using namespace std;

//音楽情報構造体定義
//ファイル名、名前、曲全体の長さ(秒数)、BPM
struct MusicData
{
	string FileName;
	string Name;
	int SongLength;
	int BPM;
};
