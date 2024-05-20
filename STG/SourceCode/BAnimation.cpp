#include "Main.h"
#include "CSoundManager.h"

//�{�X�̓o��A�j���[�V����.
void DispAnimation( BOSS* boss, SCROLL* scroll, PLAYER* player, CXInput* pXInput )
{
	//�{�X�o���A�j���[�V�����J�E���g+.
	boss->DispAnima.cnt++;
	if ( boss->DispAnima.cnt > boss->DispAnima.cntMax ) {
		if ( boss->DispAnima.No != boss->DispAnima.max ) {

			//�U���J�n.
			pXInput->SetVibration( 30000, 30000 );
			
			boss->DispAnima.No++;
			for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				boss->chara[typeNo].y -= 1.0;
			}

			//��ʗh��.
			scroll->BossDAng	 += 1.0;
			scroll->BossDispYCnt += 1;
			if ( scroll->BossDispYCnt >= 50 ) {
				scroll->BossDispYCnt = 0;
			}
		}
		else {

			//�{�X�̓��ޏoSE�̒�~.
			CSoundManager::Stop( CSoundManager::SE_BEarth );

			//�U����~.
			pXInput->SetVibration( 0, 0 );

			for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				boss->chara[typeNo].State = enCharaState::Living;
			}
			//����J�n.
			player->KeyStop			= false;
			boss->DispFlag			= false;

			//������.
			boss->DispAnima.cnt		= 0;
			boss->DispAnima.No		= 0;
			scroll->BossDAng		= 0.0;
			scroll->BossDispYCnt	= 0;
		}
	}
	scroll->BossDispY = sin( scroll->BossDAng ) * ( 10 - scroll->BossDispYCnt / 5 );
	//scroll->BossDispY = sin( scroll->BossDAng ) * ( 10.0 - ( fmod( scroll->BossDAng, 10.0 ) ) );	//double�^�́��v�Z.
}

//�{�X�̎��S�A�j���[�V����.
bool DeadAnimation( BOSS* boss, SCROLL* scroll, PLAYER* player, CXInput* pXInput )
{
	//�{�X�o���A�j���[�V�����J�E���g+.
	boss->DispAnima.cnt++;
	if ( boss->DispAnima.cnt > boss->DispAnima.cntMax ) {
		if ( boss->DispAnima.No != boss->DispAnima.max ) {

			//�U���J�n.
			pXInput->SetVibration( 30000, 30000 );

			boss->DispAnima.No++;
			for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				boss->chara[typeNo].y += 1.0;
			}

			//��ʗh��.
			scroll->BossDAng += 1.0;
			scroll->BossDispYCnt += 1;
			if ( scroll->BossDispYCnt >= 50 ) {
				scroll->BossDispYCnt = 0;
			}
		}
		else {

			//�{�X�̓��ޏoSE�̒�~.
			CSoundManager::Stop( CSoundManager::SE_BEarth );

			//�U����~.
			pXInput->SetVibration( 0, 0 );

			for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				boss->chara[typeNo].State = enCharaState::Living;
			}
			//����J�n.
			player->KeyStop			= false;
			boss->DispFlag			= false;

			//�A�j���[�V�����S�ďI��.
			return true;
		}
	}
	scroll->BossDispY = sin( scroll->BossDAng ) * ( 10 - scroll->BossDispYCnt / 5 );

	//�܂��A�j���[�V�����̑�������.
	return false;
}