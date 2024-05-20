#include "CXInput.h"

//値を範囲内に収める関数.
template<typename T>
T Clamp( T val, T min, T max )
{
	return ( val < min ) ? min : ( max < val ) ? max : val;
}

//-------------------------------------------------.
//	コンストラクタ.
//-------------------------------------------------.
CXInput::CXInput( DWORD padId )
	: m_padId		( padId )
	, m_state		()
	, m_stateOld	()
	, m_vibration	()
	, m_connect		( false )
{
}

//-------------------------------------------------.
//	デストラクタ.
//-------------------------------------------------.
CXInput::~CXInput()
{
	EndProc();
}

//-------------------------------------------------.
//	キー入力の更新.
//-------------------------------------------------.
bool CXInput::Update()
{
	//キー情報を更新する前に退避.
	m_stateOld = m_state;
	if( UpdateStatus() == false ){
		return false;
	}
	return true;
}

//-------------------------------------------------.
//	終了処理.
//-------------------------------------------------.
void CXInput::EndProc()
{
	SetVibration( 0, 0 );
}

//-------------------------------------------------.
//	キーが押されたことを取得.
//	※引数:XINPUT_GAMEPAD_A など.
//-------------------------------------------------.
bool CXInput::IsPressed( WORD GamePad )
{
	if( m_state.Gamepad.wButtons & GamePad ){
		return true;
	}
	return false;
}

//-------------------------------------------------.
//	押した瞬間.
//	※引数:XINPUT_GAMEPAD_A など.
//-------------------------------------------------.
bool CXInput::IsKeyDown( WORD GamePad )
{
	//前回未入力で今回入力→押した瞬間.
	if ( ( m_stateOld.Gamepad.wButtons & GamePad ) == 0 && 
		 ( m_state.Gamepad.wButtons & GamePad ) != 0 )
	{
		return true;
	}
	return false;
}

//-------------------------------------------------.
//	離した瞬間.
//	※引数:XINPUT_GAMEPAD_A など.
//-------------------------------------------------.
bool CXInput::IsKeyUp( WORD GamePad )
{
	//前回入力で今回未入力→離した瞬間.
	if( ( m_stateOld.Gamepad.wButtons & GamePad ) != 0 &&
		( m_state.Gamepad.wButtons & GamePad ) == 0 )
	{
		return true;
	}
	return false;
}

//-------------------------------------------------.
//	押し続けている.
//	※引数:XINPUT_GAMEPAD_A など.
//-------------------------------------------------.
bool CXInput::IsKeyRepeat( WORD GamePad )
{
	//前回入力で今回入力→押し続けている.
	if( ( m_stateOld.Gamepad.wButtons & GamePad ) != 0 &&
		( m_state.Gamepad.wButtons & GamePad ) != 0 )
	{
		return true;
	}
	return false;
}

//-------------------------------------------------.
//	振動設定.
//-------------------------------------------------.	
bool CXInput::SetVibration( WORD LMotorSpd, WORD RMotorSpd )
{
	if ( VIBRATION_ON == true ) {
		m_vibration.wLeftMotorSpeed = Clamp( LMotorSpd, INPUT_VIBRATION_MIN, INPUT_VIBRATION_MAX );
		m_vibration.wRightMotorSpeed = Clamp( RMotorSpd, INPUT_VIBRATION_MIN, INPUT_VIBRATION_MAX );

		if ( ERROR_SUCCESS == XInputSetState(
			m_padId, &m_vibration ) ) {
			return true;
		}
	}
	return false;
}

//-------------------------------------------------.
//	キー入力の更新.
//-------------------------------------------------.
bool CXInput::UpdateStatus()
{
	m_connect = false;
	if( ERROR_SUCCESS == XInputGetState(
		m_padId,
		&m_state ) )
	{
		m_connect = true;
		return true;
	}
	return false;
}
