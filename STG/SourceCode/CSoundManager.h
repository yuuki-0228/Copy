#pragma once
#include "CSound.h"


/************************************************
*	�T�E���h�}�l�[�W���[�N���X(�T�E���h�Ǘ��N���X).
*		singleton(�V���O���g��)�ō쐬.
**/
class CSoundManager
{
public:
	//�T�E���h���X�g�񋓑�.
	enum enList
	{
		BGM_Title,		//�^�C�g��.
		BGM_Main,		//���C��.
		BGM_Boss,		//�{�X��.
		BGM_GameOver,	//�Q�[���I�[�o�[.
		BGM_Ending,		//�G���f�B���O.

		SE_PSNorm,		//�v���C���[�̒ʏ�e.
		SE_PSChg,		//�v���C���[�̗��ߒe.
		SE_PSPene,		//�v���C���[�̊ђʒe.
		SE_PSBeam,		//�v���C���[�̃r�[���e.
		SE_ESChg,		//�G�̗��ߒe.
		SE_ESNorm,		//�G�̒ʏ�e.
		SE_ESPene,		//�G�̊ђʒe.
		SE_ESBeam,		//�G�̃r�[���e.
		SE_EShoming,	//�G�̒ǔ��e.
		SE_SChgCnd,		//�`���[�W����.
		SE_PDamage,		//�v���C���[�̃_���[�W.
		SE_EDamage,		//�G�̃_���[�W.
		SE_GDamage,		//�M�~�b�N�_���[�W.
		SE_GRockDest,	//�M�~�b�N[��]�j��.
		SE_GTreeDest,	//�M�~�b�N[��]�j��.
		SE_Heel,		//�L������.
		SE_Barr,		//�L�����o���A�Ŗh��.
		SE_AGet,		//�\�͎擾.
		SE_ADelete,		//�\�͍폜.

		SE_BEarth,		//�{�X�̓��ޏo.
		SE_BPANT,		//�{�X�̃p���`.
		SE_BPANTCrush,	//�{�X�p���`����.
		SE_BROCK,		//��o����.
		SE_BROCKUP,		//��˂��o��.
		SE_BROCKCrush,	//�n�ʒ@��.
		SE_BollA,		//������΂�.

		SE_Jump,		//�W�����v.
		SE_Landing,		//���n��.
		SE_Best,		//�ō�Hit���X�V.

		SE_Decision,	//����.

		//�����������炱���ɒǉ����Ă�������.
		Max
	};

public:
	//�C���X�^���X�擾(�B��̃A�N�Z�X�o�H).
	static CSoundManager* GetInstance()
	{
		//�B��̃C���X�^���X���쐬����.
		//��static�ō쐬���ꂽ�̂�2��ڈȍ~�͖��������.
		static CSoundManager s_Instance;//s_:�X�^�e�B�b�N�̈Ӗ�.
		return &s_Instance;
	}

	~CSoundManager();

	//�T�E���h�f�[�^�Ǎ��֐�.
	bool LoadData();
	//�T�E���h�f�[�^����֐�.
	void ReleaseData();

	//�E�B���h�E�n���h����ݒ�.
	void SetWnd( HWND hWnd ) { m_hWnd = hWnd; }
	//�T�E���h���擾.
	CSound* GetSound( enList list ) { return m_pSound[list]; }

	//SE���Đ�����.
	static void PlaySE( enList list )	{ CSoundManager::GetInstance()->m_pSound[list]->PlaySE(); }
	//���[�v�ōĐ�����.
	static void PlayLoop( enList list ) { CSoundManager::GetInstance()->m_pSound[list]->PlayLoop(); }
	//��~����.
	static void Stop( enList list )		{ CSoundManager::GetInstance()->m_pSound[list]->Stop(); }

private://�O������A�N�Z�X�s�\.
	//������R�s�[���֎~����.
	CSoundManager();
	CSoundManager( const CSoundManager& rhs ) = delete;
	CSoundManager& operator = ( const CSoundManager& rhs ) = delete;

private:
	CSound*	m_pSound[enList::Max];
	HWND	m_hWnd;
};