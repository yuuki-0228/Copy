#include "Main.h"
#include "Boss.h"
#include "CSoundManager.h"

//�p���`�U���t���O.
void PunchFlag( BOSS* boss, int typeNo, int Dang )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( Dang < 90 ) && ( Dang > -90 ) &&
			( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//�ړI�p�x.
			boss->PunchRAng[typeNo] = RAD( Dang );

			//��������ݒ�.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 25;

			//�U����Ԃ��p���`�̏��������Ԃɂ���.
			boss->AttackNo[typeNo] = enBossAttackNo::BPunchAng;
		}
	}
}

//�����p���`�U���t���O.
void WPunchFlag( BOSS* boss, int Dang )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( Dang < 90 ) && ( Dang > -90 ) &&
			( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			//�ړI�p�x.
			boss->PunchRAng[enBoss::RHand] = RAD( Dang );
			boss->PunchRAng[enBoss::LHand] = RAD( -Dang );

			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {

				//��������ݒ�.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 25;

				//�U����Ԃ��p���`�̏��������Ԃɂ���.
				boss->AttackNo[typeNo] = enBossAttackNo::BPunchAng;
			}
		}
	}
}

//�����ǔ��p���`�U���t���O.
void AtPunchFlag( BOSS* boss, PLAYER player, int typeNo )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//�v���C���[�Ɍ�����.
			boss->PunchRAng[typeNo] = RAD( 90 ) - atan2( ( player.chara.y - boss->chara[typeNo].y ), ( boss->chara[typeNo].x - player.chara.x ) );

			//�p�x��90�x���傫���Ȃ������ׂ�.
			if ( ( DO( boss->PunchRAng[typeNo] ) < 90 ) &&
				( DO( boss->PunchRAng[typeNo] ) > -90 ) ) {

				//��������ݒ�.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 25;

				//�U����Ԃ��p���`�̏��������Ԃɂ���.
				boss->AttackNo[typeNo] = enBossAttackNo::BPunchAng;
			}
		}
	}
}

//�����ǔ������p���`�U���t���O.
void WAtPunchFlag( BOSS* boss, PLAYER player )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//�v���C���[�Ɍ�����.
				boss->PunchRAng[typeNo] = RAD( 90 ) - atan2( ( player.chara.y - boss->chara[typeNo].y ), ( boss->chara[typeNo].x - player.chara.x ) );

				//�p�x��90�x���傫���Ȃ������ׂ�.
				if ( ( DO( boss->PunchRAng[typeNo] ) < 90 ) &&
					( DO( boss->PunchRAng[typeNo] ) > -90 ) ) {

					//��������ݒ�.
					boss->animation[typeNo].cnt = 0;
					boss->animation[typeNo].cntMax = 25;

					//�U����Ԃ��p���`�̏��������Ԃɂ���.
					boss->AttackNo[typeNo] = enBossAttackNo::BPunchAng;
				}
			}
		}
	}
}

//�˂��グ��U��.
void UpRockFlag( BOSS* boss, int typeNo, int numMax, int dirMax, int DAng, int space, int time )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//�e���A�����A�e���m�̊Ԋu.
			boss->rockData[typeNo].Num = numMax;
			boss->rockData[typeNo].Dir = dirMax;
			boss->rockData[typeNo].Ang = DAng;
			boss->rockData[typeNo].Spaa = space;
			boss->rockData[typeNo].Time = time;

			//��������ݒ�.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 40;

			//�U����Ԃ��p���`�̏��������Ԃɂ���.
			boss->AttackNo[typeNo] = enBossAttackNo::BUpRockUp;
		}
	}
}

//�����˂��グ��U��.
void WUpRockFlag( BOSS* boss, int numMax, int dirMax, int DAng, int space, int time )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//�e���A�����A�e���m�̊Ԋu.
				boss->rockData[typeNo].Num = numMax;
				boss->rockData[typeNo].Dir = dirMax;
				boss->rockData[typeNo].Ang = DAng;
				boss->rockData[typeNo].Spaa = space;
				boss->rockData[typeNo].Time = time;

				//��������ݒ�.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//�U����Ԃ��p���`�̏��������Ԃɂ���.
				boss->AttackNo[typeNo] = enBossAttackNo::BUpRockUp;
			}
		}
	}
}

//�~�˂��グ��U��.
void RUpRockFlag( BOSS* boss, int typeNo, int numMax, int dirMax, int space, int time )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//�e���A�����A�e���m�̊Ԋu.
			boss->rockData[typeNo].Num = numMax;
			boss->rockData[typeNo].Dir = dirMax;
			boss->rockData[typeNo].Ang = 270 / dirMax;
			boss->rockData[typeNo].Spaa = space;
			boss->rockData[typeNo].Time = time;

			//��������ݒ�.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 40;

			//�U����Ԃ��p���`�̏��������Ԃɂ���.
			boss->AttackNo[typeNo] = enBossAttackNo::BUpRockUp;
		}
	}
}

//�����~�˂��グ��U��.
void WRUpRockFlag( BOSS* boss, int numMax, int dirMax, int space, int time )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//�e���A�����A�e���m�̊Ԋu.
				boss->rockData[typeNo].Num = numMax;
				boss->rockData[typeNo].Dir = dirMax;
				boss->rockData[typeNo].Ang = 270 / dirMax;
				boss->rockData[typeNo].Spaa = space;
				boss->rockData[typeNo].Time = time;

				//��������ݒ�.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//�U����Ԃ��p���`�̏��������Ԃɂ���.
				boss->AttackNo[typeNo] = enBossAttackNo::BUpRockUp;
			}
		}
	}
}

//��o���U��.
void RockFlag( BOSS* boss, int typeNo, int numMax, int dirMax, int DAng, int space, int iAng )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//�e���A�����A�e���m�̊Ԋu.
			boss->rockData[typeNo].Num = numMax;
			boss->rockData[typeNo].Dir = dirMax;
			boss->rockData[typeNo].Ang = DAng;
			boss->rockData[typeNo].Spaa = space;
			boss->rockData[typeNo].iAng = iAng;

			//��������ݒ�.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 40;

			//�U����Ԃ��p���`�̏��������Ԃɂ���.
			boss->AttackNo[typeNo] = enBossAttackNo::BRockUp;
		}
	}
}

//������o���U��.
void WRockFlag( BOSS* boss, int numMax, int dirMax, int DAng, int space, int iAng )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		boss->rockData[enBoss::RHand].iAng = 180 - iAng;
		boss->rockData[enBoss::LHand].iAng = iAng;

		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//�e���A�����A�e���m�̊Ԋu.
				boss->rockData[typeNo].Num = numMax;
				boss->rockData[typeNo].Dir = dirMax;
				boss->rockData[typeNo].Ang = DAng;
				boss->rockData[typeNo].Spaa = space;

				//��������ݒ�.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//�U����Ԃ��p���`�̏��������Ԃɂ���.
				boss->AttackNo[typeNo] = enBossAttackNo::BRockUp;
			}
		}
	}
}

//�~��o���U��.
void RRockFlag( BOSS* boss, int typeNo, int numMax, int dirMax, int space, int iAng )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//�e���A�����A�e���m�̊Ԋu.
			boss->rockData[typeNo].Num = numMax;
			boss->rockData[typeNo].Dir = dirMax;
			boss->rockData[typeNo].Ang = 270 / dirMax;
			boss->rockData[typeNo].Spaa = space;
			boss->rockData[typeNo].iAng = iAng;

			//��������ݒ�.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 40;

			//�U����Ԃ��p���`�̏��������Ԃɂ���.
			boss->AttackNo[typeNo] = enBossAttackNo::BRockUp;
		}
	}
}

//�����~��o���U��.
void WRRockFlag( BOSS* boss, int numMax, int dirMax, int space, int iAng )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		boss->rockData[enBoss::RHand].iAng = 180 - iAng;
		boss->rockData[enBoss::LHand].iAng = iAng;

		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//�e���A�����A�e���m�̊Ԋu.
				boss->rockData[typeNo].Num = numMax;
				boss->rockData[typeNo].Dir = dirMax;
				boss->rockData[typeNo].Ang = 270 / dirMax;
				boss->rockData[typeNo].Spaa = space;

				//��������ݒ�.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//�U����Ԃ��p���`�̏��������Ԃɂ���.
				boss->AttackNo[typeNo] = enBossAttackNo::BRockUp;
			}
		}
	}
}

//�{�X�{�̃V���b�g�U��.
void BShotFlag( BOSS* boss, int dirMax, int DAng, int sSpd, int iAng )
{
	bool breakFlag = false;	//break�t���O.
	int AngCnt = 0;			//�t���O�𗧂Ă������̒e�J�E���g.

	//���͒l�ŃX�e�[�^�X��ύX.
	boss->status[enBoss::Main].dir = dirMax;
	boss->chara[enBoss::Main].ShotDirAng = DAng;
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		for ( int dirNo = 0; dirNo < boss->BS_DIR; dirNo++ ) {
			boss->shot[numNo][dirNo].sspd = sSpd;
		}
	}

	//��Ԋp�x�𒲂ׂ�.
	double ang = iAng;
	ang -= boss->chara[enBoss::Main].ShotDirAng * ( boss->status[enBoss::Main].dir - 1 ) / 2;
	//�V���b�gSE��炷.
	CSoundManager::PlaySE( CSoundManager::SE_ESNorm );

	//�e���̐��������[�v.
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		if ( boss->NumShotFlag[numNo] == false ) {
			//�����̐��������[�v.
			for ( int dirNo = 0; dirNo < boss->status[enBoss::Main].dir; dirNo++ ) {
				if ( boss->shot[numNo][dirNo].DispShotFlag == false ) {
					boss->shot[numNo][dirNo].x = ( boss->chara[enBoss::Main].x + C_SIZE - S_SIZE / 2 );
					boss->shot[numNo][dirNo].y = ( boss->chara[enBoss::Main].y + C_SIZE - S_SIZE );
					boss->shot[numNo][dirNo].ang = RAD( ang + boss->chara[enBoss::Main].ShotDirAng * AngCnt );	//�p�x�𒲂ׂ�.
					boss->shot[numNo][dirNo].DispShotFlag = true;												//���˃t���O�𗧂Ă�.
					AngCnt++;			//�t���O�𗧂Ă������̒e�J�E���g.

					breakFlag = true;	//break�t���O�𗧂Ă�.
				}
			}
		}
		if ( AngCnt == boss->status[enBoss::Main].dir ) {
			boss->NumShotFlag[numNo] = true;
		}
		if ( breakFlag == true ) {
			AngCnt = 0;					//�t���O�𗧂Ă������̒e�J�E���g��������.

			break;						//�ꔭ��������for���𔲂���.
		}
	}
}

//�{�X�{�̉~�V���b�g�U��.
void RBShotFlag( BOSS* boss, int dirMax, int sSpd, int iAng )
{
	bool breakFlag = false;	//break�t���O.
	int AngCnt = 0;			//�t���O�𗧂Ă������̒e�J�E���g.

	//���͒l�ŃX�e�[�^�X��ύX.
	boss->status[enBoss::Main].dir = dirMax;
	boss->chara[enBoss::Main].ShotDirAng = 270 / dirMax;
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		for ( int dirNo = 0; dirNo < boss->BS_DIR; dirNo++ ) {
			boss->shot[numNo][dirNo].sspd = sSpd;
		}
	}

	//��Ԋp�x�𒲂ׂ�.
	double ang = iAng;
	ang -= boss->chara[enBoss::Main].ShotDirAng * ( boss->status[enBoss::Main].dir - 1 ) / 2;

	//�e���̐��������[�v.
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		if ( boss->NumShotFlag[numNo] == false ) {
			//�����̐��������[�v.
			for ( int dirNo = 0; dirNo < boss->status[enBoss::Main].dir; dirNo++ ) {
				if ( boss->shot[numNo][dirNo].DispShotFlag == false ) {
					boss->shot[numNo][dirNo].x = ( boss->chara[enBoss::Main].x + C_SIZE - S_SIZE / 2 );
					boss->shot[numNo][dirNo].y = ( boss->chara[enBoss::Main].y + C_SIZE - S_SIZE );
					boss->shot[numNo][dirNo].ang = RAD( ang + boss->chara[enBoss::Main].ShotDirAng * AngCnt );	//�p�x�𒲂ׂ�.
					boss->shot[numNo][dirNo].DispShotFlag = true;												//���˃t���O�𗧂Ă�.
					AngCnt++;			//�t���O�𗧂Ă������̒e�J�E���g.

					breakFlag = true;	//break�t���O�𗧂Ă�.
				}
			}
		}
		if ( AngCnt == boss->status[enBoss::Main].dir ) {
			boss->NumShotFlag[numNo] = true;
		}
		if ( breakFlag == true ) {
			AngCnt = 0;					//�t���O�𗧂Ă������̒e�J�E���g��������.

			break;						//�ꔭ��������for���𔲂���.
		}
	}
}

//������΂��U��.
void BlowawayFlag( BOSS* boss, PLAYER player )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//���I�����Ă��� ���� ���̎肪�U�����ł͂Ȃ��ꍇ.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			 ( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {

				//��������ݒ�.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//�U����Ԃ��p���`�̏��������Ԃɂ���.
				boss->AttackNo[typeNo] = enBossAttackNo::BUpHand;
			}
		}
	}
}
