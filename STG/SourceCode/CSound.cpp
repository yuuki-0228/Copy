#include "CSound.h"
#include <stdio.h>

//コンストラクタ.
//オブジェクトの生成時に自動的に呼び出される特殊なメンバ関数.
CSound::CSound()
	//初期化子リスト.
	: m_hWnd	( nullptr )
	, m_sAlias	()
	, m_Volume	( 1000 )
{
}

//デストラクタ.
//オブジェクトが破棄されたときに自動的に呼び出される特殊なメンバ関数.
//関数名はクラス名に「~(チルダ)」を付ける.
CSound::~CSound()
{
}

//音声ファイルを開く.
bool CSound::Open( const char* sFileName, const char* sAlias, HWND hWnd )
{
	//初期設定.
	SetInitParam( sAlias, hWnd );

	//コマンド.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "open %s alias %s", sFileName, m_sAlias );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//音声ファイルを閉じる.
bool CSound::Close()
{
	//コマンド.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "close %s", m_sAlias );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//音声を再生する.
bool CSound::Play( bool bNotify )
{
	//コマンド.
	char cmd[STR_BUFF_MAX] = "";

	if ( bNotify == true ) {
		sprintf_s( cmd, "play %s notify", m_sAlias );
	}
	else {
		sprintf_s( cmd, "play %s", m_sAlias );
	}

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}
//音声を再生する(SEで使う).
bool CSound::PlaySE()
{
	SeekToStart();
	if ( Play() == true ) {
		return true;
	}
	return false;
}
//音声を再生する(ループ).
bool CSound::PlayLoop()
{
	if ( IsStopped() == true ) {
		SeekToStart();
		if ( Play( true ) == true ) {
			return true;
		}
	}
	return false;
}

//音声を停止する.
bool CSound::Stop()
{
	//コマンド.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "stop %s", m_sAlias );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//状態を取得する.
bool CSound::GetStatus( char* sStatus )
{
	//コマンド.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "status %s mode", m_sAlias );

	if ( mciSendString( cmd, sStatus, STR_BUFF_MAX, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//音声の停止を確認する.
bool CSound::IsStopped()
{
	char sStatus[STR_BUFF_MAX] = "";

	//状態の取得.
	if ( GetStatus( sStatus ) == true ) {
		//文字列比較.
		if ( strcmp( sStatus, "stopped" ) == 0 ) {
			return true;
		}
	}
	return false;
}

//再生位置を最初に戻す.
bool CSound::SeekToStart()
{
	//コマンド.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "seek %s to start", m_sAlias );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//初期設定.
void CSound::SetInitParam( const char* sAlias, HWND hWnd )
{
	//ウィンドウハンドルを登録.
	m_hWnd = hWnd;

	//エイリアスを登録(文字列のコピー).
	strcpy_s( m_sAlias, sizeof( m_sAlias ), sAlias );
}

//音量を設定する.
bool CSound::SetVolume( int Volume )
{
	//範囲内に丸める.
	if ( Volume < 0 ) { Volume = 0; }
	else if ( 1000 < Volume ) { Volume = 1000; }

	m_Volume = Volume;

	//コマンド.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "setaudio %s volume to %d", m_sAlias, m_Volume );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}
