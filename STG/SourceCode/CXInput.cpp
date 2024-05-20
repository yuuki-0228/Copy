#include "CXInput.h"

//�l��͈͓��Ɏ��߂�֐�.
template<typename T>
T Clamp( T val, T min, T max )
{
	return ( val < min ) ? min : ( max < val ) ? max : val;
}

//-------------------------------------------------.
//	�R���X�g���N�^.
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
//	�f�X�g���N�^.
//-------------------------------------------------.
CXInput::~CXInput()
{
	EndProc();
}

//-------------------------------------------------.
//	�L�[���͂̍X�V.
//-------------------------------------------------.
bool CXInput::Update()
{
	//�L�[�����X�V����O�ɑޔ�.
	m_stateOld = m_state;
	if( UpdateStatus() == false ){
		return false;
	}
	return true;
}

//-------------------------------------------------.
//	�I������.
//-------------------------------------------------.
void CXInput::EndProc()
{
	SetVibration( 0, 0 );
}

//-------------------------------------------------.
//	�L�[�������ꂽ���Ƃ��擾.
//	������:XINPUT_GAMEPAD_A �Ȃ�.
//-------------------------------------------------.
bool CXInput::IsPressed( WORD GamePad )
{
	if( m_state.Gamepad.wButtons & GamePad ){
		return true;
	}
	return false;
}

//-------------------------------------------------.
//	�������u��.
//	������:XINPUT_GAMEPAD_A �Ȃ�.
//-------------------------------------------------.
bool CXInput::IsKeyDown( WORD GamePad )
{
	//�O�񖢓��͂ō�����́��������u��.
	if ( ( m_stateOld.Gamepad.wButtons & GamePad ) == 0 && 
		 ( m_state.Gamepad.wButtons & GamePad ) != 0 )
	{
		return true;
	}
	return false;
}

//-------------------------------------------------.
//	�������u��.
//	������:XINPUT_GAMEPAD_A �Ȃ�.
//-------------------------------------------------.
bool CXInput::IsKeyUp( WORD GamePad )
{
	//�O����͂ō��񖢓��́��������u��.
	if( ( m_stateOld.Gamepad.wButtons & GamePad ) != 0 &&
		( m_state.Gamepad.wButtons & GamePad ) == 0 )
	{
		return true;
	}
	return false;
}

//-------------------------------------------------.
//	���������Ă���.
//	������:XINPUT_GAMEPAD_A �Ȃ�.
//-------------------------------------------------.
bool CXInput::IsKeyRepeat( WORD GamePad )
{
	//�O����͂ō�����́����������Ă���.
	if( ( m_stateOld.Gamepad.wButtons & GamePad ) != 0 &&
		( m_state.Gamepad.wButtons & GamePad ) != 0 )
	{
		return true;
	}
	return false;
}

//-------------------------------------------------.
//	�U���ݒ�.
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
//	�L�[���͂̍X�V.
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
