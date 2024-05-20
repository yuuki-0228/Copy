#pragma once

#include <Windows.h>
#include <Xinput.h>

#pragma comment( lib, "xinput.lib" )

/********************************************************************************
*   XInputクラス.
**/
class CXInput
{
public:
	//--------------------.
	//	定数宣言.
	//--------------------.
	//トリガー入力範囲.
	static const BYTE	INPUT_TRIGGER_MIN	= 0;
	static const BYTE	INPUT_TRIGGER_MAX	= 255;
	//スティック入力範囲.
	static const SHORT	INPUT_THUMB_MIN		=-32768;
	static const SHORT	INPUT_THUMB_MAX		= 32767;
	//バイブレーション値範囲.
	static const WORD	INPUT_VIBRATION_MIN	= 0;
	static const WORD	INPUT_VIBRATION_MAX	= 65535;
	static const bool	VIBRATION_ON		= false;

	//コンストラクタ.
	CXInput( DWORD padId );
	//デストラクタ.
	~CXInput();

	//キー入力の更新.
	bool Update();
	//終了処理.
	void EndProc();

	//--------------------.
	//	キーが押されたことを取得する.
	//	※引数:XINPUT_GAMEPAD_A など.
	//--------------------.
	bool IsPressed( WORD vkPad );

	//--------------------.
	//	キー入力の状態を細かく取得する.
	//	※引数:XINPUT_GAMEPAD_A など.
	//--------------------.
	//押した瞬間.
	bool IsKeyDown( WORD GamePad );
	//離した瞬間.
	bool IsKeyUp( WORD GamePad );
	//押し続けている.
	bool IsKeyRepeat( WORD GamePad );

	//左トリガー取得.
	BYTE GetLTrigger() const {	return m_state.Gamepad.bLeftTrigger;	}
	//右トリガー取得.
	BYTE GetRTrigger() const {	return m_state.Gamepad.bRightTrigger;	}

	//左スティックX軸取得.
	SHORT GetLThumbX() const {	return m_state.Gamepad.sThumbLX;		}
	//左スティックY軸取得.
	SHORT GetLThumbY() const {	return m_state.Gamepad.sThumbLY;		}

	//右スティックX軸取得.
	SHORT GetRThumbX() const {	return m_state.Gamepad.sThumbRX;		}
	//右スティックY軸取得.
	SHORT GetRThumbY() const {	return m_state.Gamepad.sThumbRY;		}

	//振動設定.
	bool SetVibration( WORD LMotorSpd, WORD RMotorSpd );

	//接続チェック.
	bool IsConnect() const	{	return m_connect;	}
	//パッド番号取得.
	DWORD GetPadID() const	{	return m_padId;		}

private:
	//キー入力の更新.
	bool UpdateStatus();

private:
	DWORD               m_padId;		//パッド番号(0~3).
	XINPUT_STATE        m_state;		//キー入力情報.
	XINPUT_STATE		m_stateOld;		//キー入力情報(キーストローク判定用).
	XINPUT_VIBRATION    m_vibration;	//振動.
	bool				m_connect;		//接続判定.
};
