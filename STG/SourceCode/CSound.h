#pragma once

#include <Windows.h>

#pragma comment( lib, "winmm.lib" )	//�����Đ��ŕK�v.

/************************************************
*	�T�E���h�N���X.
*		midi,mp3,wav�`���t�@�C���̍Đ��E��~�����s��.
**/
class CSound
{
public:	//public�F�O������A�N�Z�X�\.
	//�萔�錾.
	static const int STR_BUFF_MAX = 256;	//������o�b�t�@.

	CSound();	//�R���X�g���N�^.
	~CSound();	//�f�X�g���N�^.

	//�����t�@�C�����J��.
	//����(Volume)�̓f�t�H���g��1000�����Ă���.�f�t�H���g�����Ƃ�����.
	bool Open( const char* sFileName, const char* sAlias, HWND hWnd );

	//�����t�@�C�������.
	bool Close();

	//�������Đ�����.
	bool Play( bool bNotify = false );
	//�������Đ�����(SE�Ŏg��).
	bool PlaySE();
	//�������Đ�����(���[�v).
	bool PlayLoop();

	//�������~����.
	bool Stop();

	//��Ԃ��擾����.
	bool GetStatus( char* sStatus );

	//�����̒�~���m�F����.
	bool IsStopped();

	//�Đ��ʒu���ŏ��ɖ߂�.
	bool SeekToStart();

	//�����ݒ�.
	void SetInitParam( const char* sAlias, HWND hWnd );

	//���ʂ�ݒ肷��.
	bool SetVolume( int Volume );

private://private�F�O������A�N�Z�X�s�\.
	HWND	m_hWnd;					//�E�B���h�E�n���h��.
	char	m_sAlias[STR_BUFF_MAX];	//�G�C���A�X.
	int		m_Volume;				//����(0 ~ (�ʏ�)1000).
};