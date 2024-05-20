#pragma once

#include <Windows.h>

#pragma comment( lib, "winmm.lib" )	//音声再生で必要.

/************************************************
*	サウンドクラス.
*		midi,mp3,wav形式ファイルの再生・停止等を行う.
**/
class CSound
{
public:	//public：外部からアクセス可能.
	//定数宣言.
	static const int STR_BUFF_MAX = 256;	//文字列バッファ.

	CSound();	//コンストラクタ.
	~CSound();	//デストラクタ.

	//音声ファイルを開く.
	//音量(Volume)はデフォルトで1000を入れておく.デフォルト引数ともいう.
	bool Open( const char* sFileName, const char* sAlias, HWND hWnd );

	//音声ファイルを閉じる.
	bool Close();

	//音声を再生する.
	bool Play( bool bNotify = false );
	//音声を再生する(SEで使う).
	bool PlaySE();
	//音声を再生する(ループ).
	bool PlayLoop();

	//音声を停止する.
	bool Stop();

	//状態を取得する.
	bool GetStatus( char* sStatus );

	//音声の停止を確認する.
	bool IsStopped();

	//再生位置を最初に戻す.
	bool SeekToStart();

	//初期設定.
	void SetInitParam( const char* sAlias, HWND hWnd );

	//音量を設定する.
	bool SetVolume( int Volume );

private://private：外部からアクセス不可能.
	HWND	m_hWnd;					//ウィンドウハンドル.
	char	m_sAlias[STR_BUFF_MAX];	//エイリアス.
	int		m_Volume;				//音量(0 ~ (通常)1000).
};