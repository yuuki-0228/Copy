#pragma once

#include <Windows.h>
#include <Xinput.h>

#pragma comment( lib, "xinput.lib" )

/********************************************************************************
*   XInput�N���X.
**/
class CXInput
{
public:
	//--------------------.
	//	�萔�錾.
	//--------------------.
	//�g���K�[���͔͈�.
	static const BYTE	INPUT_TRIGGER_MIN	= 0;
	static const BYTE	INPUT_TRIGGER_MAX	= 255;
	//�X�e�B�b�N���͔͈�.
	static const SHORT	INPUT_THUMB_MIN		=-32768;
	static const SHORT	INPUT_THUMB_MAX		= 32767;
	//�o�C�u���[�V�����l�͈�.
	static const WORD	INPUT_VIBRATION_MIN	= 0;
	static const WORD	INPUT_VIBRATION_MAX	= 65535;
	static const bool	VIBRATION_ON		= false;

	//�R���X�g���N�^.
	CXInput( DWORD padId );
	//�f�X�g���N�^.
	~CXInput();

	//�L�[���͂̍X�V.
	bool Update();
	//�I������.
	void EndProc();

	//--------------------.
	//	�L�[�������ꂽ���Ƃ��擾����.
	//	������:XINPUT_GAMEPAD_A �Ȃ�.
	//--------------------.
	bool IsPressed( WORD vkPad );

	//--------------------.
	//	�L�[���͂̏�Ԃ��ׂ����擾����.
	//	������:XINPUT_GAMEPAD_A �Ȃ�.
	//--------------------.
	//�������u��.
	bool IsKeyDown( WORD GamePad );
	//�������u��.
	bool IsKeyUp( WORD GamePad );
	//���������Ă���.
	bool IsKeyRepeat( WORD GamePad );

	//���g���K�[�擾.
	BYTE GetLTrigger() const {	return m_state.Gamepad.bLeftTrigger;	}
	//�E�g���K�[�擾.
	BYTE GetRTrigger() const {	return m_state.Gamepad.bRightTrigger;	}

	//���X�e�B�b�NX���擾.
	SHORT GetLThumbX() const {	return m_state.Gamepad.sThumbLX;		}
	//���X�e�B�b�NY���擾.
	SHORT GetLThumbY() const {	return m_state.Gamepad.sThumbLY;		}

	//�E�X�e�B�b�NX���擾.
	SHORT GetRThumbX() const {	return m_state.Gamepad.sThumbRX;		}
	//�E�X�e�B�b�NY���擾.
	SHORT GetRThumbY() const {	return m_state.Gamepad.sThumbRY;		}

	//�U���ݒ�.
	bool SetVibration( WORD LMotorSpd, WORD RMotorSpd );

	//�ڑ��`�F�b�N.
	bool IsConnect() const	{	return m_connect;	}
	//�p�b�h�ԍ��擾.
	DWORD GetPadID() const	{	return m_padId;		}

private:
	//�L�[���͂̍X�V.
	bool UpdateStatus();

private:
	DWORD               m_padId;		//�p�b�h�ԍ�(0~3).
	XINPUT_STATE        m_state;		//�L�[���͏��.
	XINPUT_STATE		m_stateOld;		//�L�[���͏��(�L�[�X�g���[�N����p).
	XINPUT_VIBRATION    m_vibration;	//�U��.
	bool				m_connect;		//�ڑ�����.
};
