#include "Main.h"
#include "Shot.h"

//�v���C���[�̒e�̓���.
void P_ShotControl(
	PLAYER* playerData, BOSS* boss, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], enScene Scene )
{
 	int NumShotCnt = 0;	//����+�e����ʂ̊O�ֈړ�������.

	//�e���̐��������[�v.
 	for ( int numNo = 0; numNo < playerData->status.num; numNo++ ) {
		//�����̐��������[�v.
		for ( int dirNo = 0; dirNo < playerData->status.dir; dirNo++ ) {
			//�e�����˒��Ȃ�.
			if ( playerData->NumShotFlag[numNo] == true ) {

				//�ǔ����擾���Ȃ�.
				if ( playerData->ability.homing != 0 ){
					P_homing( playerData, boss, enemyData, Scene, numNo, dirNo );
				}

				//�e�̈ړ�.
				playerData->shot[numNo][dirNo].x += cos( playerData->shot[numNo][dirNo].ang ) * ( playerData->shot[numNo][dirNo].sspd / BEAMMAX ) * playerData->shot[numNo][dirNo].beamCnt;
				playerData->shot[numNo][dirNo].y -= sin( playerData->shot[numNo][dirNo].ang ) * ( playerData->shot[numNo][dirNo].sspd / BEAMMAX ) * playerData->shot[numNo][dirNo].beamCnt;

				//�ǔ��I��.
				if ( playerData->shot[numNo][dirNo].y < playerData->target.y ) {
					playerData->shot[numNo][dirNo].homingCnt = HOMINGMAX;
				}

				//��ʂ̊O�ֈړ�����.
				if ( Scene == enScene::Ending ){
					if ( ( playerData->shot[numNo][dirNo].y < -S_SIZE ) || ( playerData->shot[numNo][dirNo].y > WND_H ) ||
						 ( playerData->shot[numNo][dirNo].x < -S_SIZE ) || ( playerData->shot[numNo][dirNo].x > WND_W ) ) {

						//�r�[���Ȃ�.
						if ( playerData->ability.beam != 0 ){
							playerData->shot[numNo][dirNo].beamCnt = BEAMMAX;		//�r�[���̃q�b�g�񐔂�������.
						}
						playerData->shot[numNo][dirNo].DispShotFlag = false;
						playerData->shot[numNo][dirNo].x = -WND_W;
						playerData->shot[numNo][dirNo].y = -WND_H;
						NumShotCnt++;
					}
				}
				else{
					if ( ( playerData->shot[numNo][dirNo].y < -S_SIZE ) || ( playerData->shot[numNo][dirNo].y > GWND_H ) ||
						 ( playerData->shot[numNo][dirNo].x < -S_SIZE ) || ( playerData->shot[numNo][dirNo].x > GWND_W ) ) {

						//�r�[���Ȃ�.
						if ( playerData->ability.beam != 0 ){
							playerData->shot[numNo][dirNo].beamCnt = BEAMMAX;		//�r�[���̃q�b�g�񐔂�������.
						}
						playerData->shot[numNo][dirNo].DispShotFlag = false;
						playerData->shot[numNo][dirNo].x = -WND_W;
						playerData->shot[numNo][dirNo].y = -WND_H;
						NumShotCnt++;
					}
				}
			}
		}
		//����+�e�S�ĉ�ʂ̊O�ֈړ�����.
		if ( NumShotCnt == playerData->status.dir ) {
			playerData->NumShotFlag[numNo] = false;			//���˃t���O���~�낷.
			for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ){
				playerData->shot[numNo][dirNo].homingCnt = 0;			//�ǔ��J�E���g������.
			}
		}
		NumShotCnt = 0;	//��ʂ̊O�ֈړ���������������.
	}
}

//�G�̒e�̓���.
void E_ShotControl(
	ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, PLAYER playerData )
{
	int NumShotCnt = 0;	//����+�e����ʂ̊O�ֈړ�������.

	//�e���̐��������[�v.
	for ( int numNo = 0; numNo < enemyData[typeNo][ptnNo][eNo].status.num; numNo++ ) {
		//�����̐��������[�v.
		for ( int dirNo = 0; dirNo < enemyData[typeNo][ptnNo][eNo].status.dir; dirNo++ ) {
			//�e�����˒��Ȃ�.
			if ( enemyData[typeNo][ptnNo][eNo].NumShotFlag[numNo] == true ) {
 
				//�ǔ����擾���Ȃ�.
				if ( enemyData[typeNo][ptnNo][eNo].ability.homing != 0 ){
					E_homing( enemyData, typeNo, ptnNo, eNo, numNo, dirNo, playerData );
				}

				//�e�̈ړ�.
				enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x -= cos( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang ) * enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd;
				enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y += sin( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang ) * enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd;

				//�ǔ��I��.
				if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y > enemyData[typeNo][ptnNo][eNo].target.y - C_SIZE * 2 ) {
					enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt = HOMINGMAX;
				}

				//��ʂ̊O�ֈړ�����.
				if ( ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y < -S_SIZE ) || ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y > GWND_H ) ||
					 ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x < -S_SIZE ) || ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x > GWND_W ) ){

					//�r�[���Ȃ�.
					if ( enemyData[typeNo][ptnNo][eNo].ability.beam != 0 ){
						enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt = BEAMMAX;		//�r�[���̃q�b�g�񐔂�������.
					}
					enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag	= false;
					enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x				= WND_W;
					enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y				= WND_H;
					NumShotCnt++;
				}
			}
		}
		//����+�e�S�ĉ�ʂ̊O�ֈړ�����.
		if ( NumShotCnt == enemyData[typeNo][ptnNo][eNo].status.dir ){
			enemyData[typeNo][ptnNo][eNo].NumShotFlag[numNo] = false;	//���˃t���O���~�낷.
			for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
				enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt = 0;			//�ǔ��J�E���g������.
			}
		}
		NumShotCnt = 0;	//��ʂ̊O�ֈړ���������������.
	}
}

//�v���C���[�̒ǔ�����.
void P_homing( PLAYER* playerData, BOSS* boss, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], enScene Scene, int numNo, int dirNo )
{
	playerData->shot[numNo][dirNo].homingCnt += 1;

	if ( ( playerData->shot[numNo][dirNo].homingCnt > 5 ) && 
		 ( playerData->shot[numNo][dirNo].homingCnt < HOMINGMAX ) ){

		//��ԋ߂��G��T���A�^�[�Q�b�g�ɂ���.
		if ( playerData->shot[numNo][dirNo].homingCnt == 6 ) {
			if ( boss->chara[enBoss::Main].State == enCharaState::Wait ) {
				P_TargetUpdate( playerData, enemyData, numNo, dirNo );
			}
			else {
				P_BTargetUpdate( playerData, boss, numNo, dirNo );
			}
		}
		if ( playerData->target.y > playerData->shot[numNo][dirNo].y - S_SIZE * 2 ) {
			//�ǔ��I��.
			playerData->shot[numNo][dirNo].homingCnt = HOMINGMAX;
		}

		//�p�x�𒲂ׂ�.
		double Mang = atan2( ( playerData->shot[numNo][dirNo].y - ( playerData->target.y + C_SIZE / 2 - S_SIZE / 2 ) ), ( ( playerData->target.x + C_SIZE / 2 - S_SIZE / 2 ) - playerData->shot[numNo][dirNo].x ) );

		//�p�x�̕ύX.
		if ( playerData->shot[numNo][dirNo].ang > Mang ){
			playerData->shot[numNo][dirNo].ang -= 0.1;
		}
		if ( playerData->shot[numNo][dirNo].ang < Mang ) {
			playerData->shot[numNo][dirNo].ang += 0.1;
		}
	}
}

//�G�̒ǔ�����.
void E_homing( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, int numNo, int dirNo, PLAYER playerData )
{
	enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt += 1;

	if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt < HOMINGMAX ) {

		//�v���C���[���^�[�Q�b�g�ɂ���.
		E_TargetUpdate( enemyData, typeNo, ptnNo, eNo, numNo, dirNo, playerData );

		//�p�x�𒲂ׂ�.
		double Mang = atan2( ( ( enemyData[typeNo][ptnNo][eNo].target.y + C_SIZE / 2 - S_SIZE / 2 ) - enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ), ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - ( enemyData[typeNo][ptnNo][eNo].target.x + C_SIZE / 2 - S_SIZE / 2 ) ) );

		//�p�x�̕ύX.
		if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang > Mang ){
			enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang -= 0.1;
		}
		if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang < Mang ) {
			enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang += 0.1;
		}
	}
}

//�v���C���[�̃^�[�Q�b�g�̍X�V.
void P_TargetUpdate( PLAYER* playerData, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int numNo, int dirNo )
{
	//�^�[�Q�b�g��������.
	playerData->target.x = playerData->chara.x + C_SIZE / 2 - S_SIZE / 2;
	playerData->target.y = -C_SIZE;

	//��ԋ߂��G��T���A�^�[�Q�b�g�ɂ���.
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ){
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ){
			for ( int eNo = 0; eNo < E_MAX; eNo++ ){
				if ( ( enemyData[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) ||
					 ( enemyData[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) ){
					if ( ( enemyData[typeNo][ptnNo][eNo].chara.y + C_SIZE < playerData->chara.y - C_SIZE ) &&
						 ( enemyData[typeNo][ptnNo][eNo].chara.y + C_SIZE < playerData->shot[numNo][dirNo].y - S_SIZE ) &&
						 ( enemyData[typeNo][ptnNo][eNo].chara.y > playerData->target.y ) ){

						//�^�[�Q�b�g���X�V.
						playerData->target.x = enemyData[typeNo][ptnNo][eNo].chara.x + ( C_SIZE / 2 - C_SIZE / 2 );
						playerData->target.y = enemyData[typeNo][ptnNo][eNo].chara.y + ( C_SIZE / 2 - C_SIZE / 2 );
					}
				}
			}
		}
	}
}
void P_BTargetUpdate( PLAYER* playerData, BOSS* boss, int numNo, int dirNo )
{
	//�^�[�Q�b�g��������.
	playerData->target.x = GWND_W / 2 - C_SIZE / 2;
	playerData->target.y = -C_SIZE;

	//��ԋ߂��G��T���A�^�[�Q�b�g�ɂ���.
	if ( ( boss->chara[enBoss::Main].State == enCharaState::Living ) ||
		 ( boss->chara[enBoss::Main].State == enCharaState::Invi ) ) {
		if ( ( boss->chara[enBoss::Main].y + C_SIZE < playerData->chara.y - C_SIZE ) &&
			 ( boss->chara[enBoss::Main].y + C_SIZE < playerData->shot[numNo][dirNo].y - S_SIZE ) &&
			 ( boss->chara[enBoss::Main].y > playerData->target.y ) ) {

			//�^�[�Q�b�g���X�V.
			playerData->target.x = boss->chara[enBoss::Main].x + ( boss->charaSize[enBoss::Main] / 2 - C_SIZE / 2 );
			playerData->target.y = boss->chara[enBoss::Main].y + ( boss->charaSize[enBoss::Main] / 2 - C_SIZE / 2 );
		}
	}	
}

//�G�̃^�[�Q�b�g�̍X�V.
void E_TargetUpdate( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, int numNo, int dirNo, PLAYER playerData )
{
	//�^�[�Q�b�g��������.
	enemyData[typeNo][ptnNo][eNo].target.x = GWND_W / 2 - C_SIZE / 2;
	enemyData[typeNo][ptnNo][eNo].target.y = -C_SIZE;

	//��ԋ߂��G��T���A�^�[�Q�b�g�ɂ���.
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ){
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ){
			for ( int eNo = 0; eNo < E_MAX; eNo++ ){
				//�^�[�Q�b�g���X�V.
				enemyData[typeNo][ptnNo][eNo].target.x = playerData.chara.x;
				enemyData[typeNo][ptnNo][eNo].target.y = playerData.chara.y;
			}
		}
	}
}