#include "CSound.h"
#include <stdio.h>

//�R���X�g���N�^.
//�I�u�W�F�N�g�̐������Ɏ����I�ɌĂяo��������ȃ����o�֐�.
CSound::CSound()
	//�������q���X�g.
	: m_hWnd	( nullptr )
	, m_sAlias	()
	, m_Volume	( 1000 )
{
}

//�f�X�g���N�^.
//�I�u�W�F�N�g���j�����ꂽ�Ƃ��Ɏ����I�ɌĂяo��������ȃ����o�֐�.
//�֐����̓N���X���Ɂu~(�`���_)�v��t����.
CSound::~CSound()
{
}

//�����t�@�C�����J��.
bool CSound::Open( const char* sFileName, const char* sAlias, HWND hWnd )
{
	//�����ݒ�.
	SetInitParam( sAlias, hWnd );

	//�R�}���h.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "open %s alias %s", sFileName, m_sAlias );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//�����t�@�C�������.
bool CSound::Close()
{
	//�R�}���h.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "close %s", m_sAlias );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//�������Đ�����.
bool CSound::Play( bool bNotify )
{
	//�R�}���h.
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
//�������Đ�����(SE�Ŏg��).
bool CSound::PlaySE()
{
	SeekToStart();
	if ( Play() == true ) {
		return true;
	}
	return false;
}
//�������Đ�����(���[�v).
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

//�������~����.
bool CSound::Stop()
{
	//�R�}���h.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "stop %s", m_sAlias );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//��Ԃ��擾����.
bool CSound::GetStatus( char* sStatus )
{
	//�R�}���h.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "status %s mode", m_sAlias );

	if ( mciSendString( cmd, sStatus, STR_BUFF_MAX, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//�����̒�~���m�F����.
bool CSound::IsStopped()
{
	char sStatus[STR_BUFF_MAX] = "";

	//��Ԃ̎擾.
	if ( GetStatus( sStatus ) == true ) {
		//�������r.
		if ( strcmp( sStatus, "stopped" ) == 0 ) {
			return true;
		}
	}
	return false;
}

//�Đ��ʒu���ŏ��ɖ߂�.
bool CSound::SeekToStart()
{
	//�R�}���h.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "seek %s to start", m_sAlias );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}

//�����ݒ�.
void CSound::SetInitParam( const char* sAlias, HWND hWnd )
{
	//�E�B���h�E�n���h����o�^.
	m_hWnd = hWnd;

	//�G�C���A�X��o�^(������̃R�s�[).
	strcpy_s( m_sAlias, sizeof( m_sAlias ), sAlias );
}

//���ʂ�ݒ肷��.
bool CSound::SetVolume( int Volume )
{
	//�͈͓��Ɋۂ߂�.
	if ( Volume < 0 ) { Volume = 0; }
	else if ( 1000 < Volume ) { Volume = 1000; }

	m_Volume = Volume;

	//�R�}���h.
	char cmd[STR_BUFF_MAX] = "";

	sprintf_s( cmd, "setaudio %s volume to %d", m_sAlias, m_Volume );

	if ( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ) {
		return true;
	}
	return false;
}
