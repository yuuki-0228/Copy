#include "Main.h"
#include "Boss.h"
#include "CSoundManager.h"

//�{�X�̍U������.
void B_AttackControl( BOSS* boss, int typeNo, SCROLL* scroll, PLAYER* player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX] )
{
	if ( boss->chara[enBoss::Main].State != enCharaState::Dying ) {

		//�p���`����.
		PunchCon( boss, player, gimmick, enemy, typeNo );

		//�˂��グ�⓮��.
		UpRockCon( boss, player, gimmick, typeNo );

		//��o���U��.
		RockCon( boss, gimmick, player, typeNo );

		//������΂�.
		Blowaway( boss, scroll, player, gimmick, enemy, typeNo );

		//�{�X�̒e�U��.
		BShotCon( boss );
	}
}

//�p���`����.
void PunchCon( BOSS* boss, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo )
{
	//�p���`�p�x���킹.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunchAng ) {
		if ( boss->PunchRAng[typeNo] > boss->chara[typeNo].ang ) {
			boss->chara[typeNo].ang += RAD( 1 );
			//�p�x�ɍ��킹�ꂽ��.
			if ( boss->PunchRAng[typeNo] < boss->chara[typeNo].ang ) {
				boss->chara[typeNo].ang = boss->PunchRAng[typeNo];
			}
		}
		else if ( boss->PunchRAng[typeNo] < boss->chara[typeNo].ang ) {
			boss->chara[typeNo].ang -= RAD( 1 );
			//�p�x�ɍ��킹�ꂽ��.
			if ( boss->PunchRAng[typeNo] > boss->chara[typeNo].ang ) {
				boss->chara[typeNo].ang = boss->PunchRAng[typeNo];
			}
		}
		else {
			//���������.
			boss->AttackNo[typeNo] = enBossAttackNo::BPunchInit;
		}
	}
	//�p���`��������.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunchInit ) {
		boss->animation[typeNo].cnt += 1;
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ) {
			//�r�����ɓ�����.
			boss->chara[typeNo].x += cos( RAD( 90 ) + boss->chara[typeNo].ang + RAD( 180 ) ) * 1;
			boss->chara[typeNo].y += sin( RAD( 90 ) + boss->chara[typeNo].ang + RAD( 180 ) ) * 1;
		}
		else {
			//�p���`����.
			boss->AttackNo[typeNo] = enBossAttackNo::BPunch;
			boss->animation[typeNo].cnt = 0;
			//�p���`SE��炷.
			CSoundManager::PlaySE( CSoundManager::SE_BPANT );
		}
	}
	//�p���`��.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunch ) {
		//�p�x�̕����ɔ�΂�.
		boss->animation[typeNo].cnt += 1;
		boss->chara[typeNo].x += cos( RAD( 90 ) + boss->chara[typeNo].ang ) * ( 2 + boss->animation[typeNo].cnt / 3 );
		boss->chara[typeNo].y += sin( RAD( 90 ) + boss->chara[typeNo].ang ) * ( 2 + boss->animation[typeNo].cnt / 3 );

		//��ƏՓ˂���.
		for( int gty = 0; gty < G_TYPE; gty++ ){
			for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
				if ( CollisionDetection(
					boss->chara[typeNo].x,  boss->chara[typeNo].y,	C_SIZE,						C_SIZE,
					gimmick[1][gty][gNo].x, gimmick[1][gty][gNo].y, gimmick[1][gty][gNo].wSize, gimmick[1][gty][gNo].hSize ) ) {

					//��j��.
					CSoundManager::PlaySE( CSoundManager::SE_GRockDest );

					//����Ȃ���.
					gimmick[1][gty][gNo].State = enGimmickState::GWait;
					gimmick[1][gty][gNo].x = WND_W;
					gimmick[1][gty][gNo].y = WND_H;
				}
			}
		}
		//�v���C���[�ƏՓ˂���.
		if ( Player->chara.State == enCharaState::Living ) {
			if ( CollisionDetection(
				Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
				boss->chara[typeNo].x, boss->chara[typeNo].y, boss->charaSize[typeNo], boss->charaSize[typeNo] ) ) {

				//�_���[�WSE��炷.
				CSoundManager::PlaySE( CSoundManager::SE_PDamage );

				//HP�����炷.
				Player->status.hp -= boss->status[enBoss::Main].atk * 2;
				if ( Player->status.hp <= 0 ) {
					Player->chara.State			= enCharaState::Dying;	//��Ԃ����S���ɂ���.
					Player->chara.ExpAnimCnt	= 0;					//������Ұ��ݶ�����0�ɂ���.
					Player->DeadNo				= enDeedNo::Golem;		//�{�X�Ŏ��S.
				}
				else {
					//��Ԃ𖳓G�ɂ���.
					Player->chara.State = enCharaState::Invi;
					Player->chara.inviTime = PINVITIME;
				}
			}
		}
		//�G�ƏՓ˂���.
		for ( int type = 0; type < E_TYPE; type++ ){
			for ( int ptn = 0; ptn < E_PTN; ptn++ ){
				for ( int eNo = 0; eNo < E_MAX; eNo++ ){
					if ( enemy[type][ptn][eNo].chara.State == enCharaState::Dead ){
						if ( CollisionDetection(
							enemy[type][ptn][eNo].chara.x,	enemy[type][ptn][eNo].chara.y,	C_SIZE,						C_SIZE,
							boss->chara[typeNo].x,			boss->chara[typeNo].y,			boss->charaSize[typeNo],	boss->charaSize[typeNo] ) )
						{
							enemy[type][ptn][eNo].chara.x		= WND_W;
							enemy[type][ptn][eNo].chara.y		= WND_H;				//��ʂ̊O(��).
							enemy[type][ptn][eNo].chara.State	= enCharaState::Wait;	//�ҋ@.
							enemy[type][ptn][eNo].appFlag		= false;

						}
					}
					else if ( enemy[type][ptn][eNo].chara.State == enCharaState::Living ){
						if ( CollisionDetection(
							enemy[type][ptn][eNo].chara.x,	enemy[type][ptn][eNo].chara.y,	C_SIZE,						C_SIZE,
							boss->chara[typeNo].x,			boss->chara[typeNo].y,			boss->charaSize[typeNo],	boss->charaSize[typeNo] ) )
						{
							//�G�̃_���[�WSE��炷.
							CSoundManager::PlaySE( CSoundManager::SE_EDamage );

							//HP�����炷.
							enemy[typeNo][ptn][eNo].status.hp -= boss->status[enBoss::Main].atk * 2;
							if ( enemy[typeNo][ptn][eNo].status.hp <= 0 ){
								enemy[typeNo][ptn][eNo].chara.State			= enCharaState::Dying;	//��Ԃ����S���ɂ���.
								enemy[typeNo][ptn][eNo].chara.ExpAnimCnt	= 0;					//������Ұ��ݶ�����0�ɂ���.
								enemy[typeNo][ptn][eNo].chara.chgCnt		= 0;
							}
							else{
								//��Ԃ𖳓G�ɂ���.
								enemy[typeNo][ptn][eNo].chara.State		= enCharaState::Invi;
								enemy[typeNo][ptn][eNo].chara.inviTime	= INVITIME;
							}
						}
					}
				}
			}
		}

		//��ʂ̒[�ɐG�ꂽ.
		if ( ( boss->chara[typeNo].y < 0 ) || ( boss->chara[typeNo].y > GWND_H - C_SIZE ) ||
			 ( boss->chara[typeNo].x < 0 ) || ( boss->chara[typeNo].x > GWND_W - C_SIZE ) ) {
			//�p���`���ˉ�.
			CSoundManager::PlaySE( CSoundManager::SE_BPANTCrush );
			//��̉����.
			boss->AttackNo[typeNo] = enBossAttackNo::BPunchReturn;
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 50;	//���e���̑ҋ@����.
		}
	}
	//����.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunchReturn ) {
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ) {
			//���e���̑ҋ@.
			boss->animation[typeNo].cnt += 1;
		}
		else {
			boss->chara[typeNo].x += cos( RAD( 90 ) + boss->chara[typeNo].ang + RAD( 180 ) ) * 3;
			boss->chara[typeNo].y += sin( RAD( 90 ) + boss->chara[typeNo].ang + RAD( 180 ) ) * 3;
			if ( boss->chara[typeNo].y < boss->initial_xy[typeNo].y ) {

				//�������W�ɂ���.
				boss->chara[typeNo].x = boss->initial_xy[typeNo].x;
				boss->chara[typeNo].y = boss->initial_xy[typeNo].y;

				//��̊p�x��߂���.
				boss->AttackNo[typeNo] = enBossAttackNo::BPunchEnd;
			}

			//�v���C���[�ƏՓ˂���.
			if ( Player->chara.State == enCharaState::Living ) {
				if ( CollisionDetection(
					Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
					boss->chara[typeNo].x, boss->chara[typeNo].y, boss->charaSize[typeNo], boss->charaSize[typeNo] ) ) {
					//�_���[�WSE��炷.
					CSoundManager::PlaySE( CSoundManager::SE_PDamage );

					//HP�����炷.
					Player->status.hp -= 1;
					if ( Player->status.hp <= 0 ) {
						Player->chara.State			= enCharaState::Dying;	//��Ԃ����S���ɂ���.
						Player->chara.ExpAnimCnt	= 0;					//������Ұ��ݶ�����0�ɂ���.
						Player->DeadNo				= enDeedNo::Golem;		//�{�X�Ŏ��S.
					}
					else {
						//��Ԃ𖳓G�ɂ���.
						Player->chara.State = enCharaState::Invi;
						Player->chara.inviTime = PINVITIME;
					}
				}
			}
		}
	}
	//�p�x��߂�.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunchEnd ) {
		if ( 0 > boss->chara[typeNo].ang ) {
			boss->chara[typeNo].ang += RAD( 1 );
			//�p�x�ɍ��킹�ꂽ��.
			if ( 0 < boss->chara[typeNo].ang ) {
				boss->chara[typeNo].ang = 0;
			}
		}
		else if ( 0 < boss->chara[typeNo].ang ) {
			boss->chara[typeNo].ang -= RAD( 1 );
			//�p�x�ɍ��킹�ꂽ��.
			if ( 0 > boss->chara[typeNo].ang ) {
				boss->chara[typeNo].ang = 0;
			}
		}
		else {
			//�p���`�I��.
			boss->AttackNo[typeNo] = enBossAttackNo::BWait;
			//�A�j���[�V����������.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 5;
		}
	}
}

//�˂��グ��U��.
void UpRockCon( BOSS* boss, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int typeNo )
{
	//���������.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpRockUp ){
		boss->animation[typeNo].cnt += 1;
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ){
			boss->chara[typeNo].y -= 1;
		}
		else{
			//������낷.
			boss->AttackNo[typeNo] = enBossAttackNo::BUpRockDown;
			boss->animation[typeNo].cnt = 0;
		}
	}
	//������낷.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpRockDown ){
		boss->chara[typeNo].y += 5;
		if ( boss->chara[typeNo].y > boss->initial_xy[typeNo].y ){
			//���W�𑵂���.
			boss->animation[typeNo].No = 0;
			boss->animation[typeNo].cnt = 0;
			boss->chara[typeNo].y = boss->initial_xy[typeNo].y;
			boss->AttackNo[typeNo] = enBossAttackNo::BUpRock;
		}
		//��@���Ɠ˂��グSE��炷.
		CSoundManager::PlaySE( CSoundManager::SE_BROCKCrush );
		CSoundManager::PlaySE( CSoundManager::SE_BROCKUP );
	}
	//����o���Ă���.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpRock ){
		if ( boss->animation[typeNo].No < boss->rockData[typeNo].Num ){
			double ang = 90;
			ang -= boss->rockData[typeNo].Ang * ( boss->rockData[typeNo].Dir - 1 ) / 2;

			for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].Rang = RAD( ang + boss->rockData[typeNo].Ang * dirNo );	//�p�x�𒲂ׂ�.
				//���W�𑵂��t���O�𗧂Ă�.
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].x = boss->chara[typeNo].x + boss->charaSize[typeNo] / 2 - S_SIZE;
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].y = boss->chara[typeNo].y + boss->charaSize[typeNo];
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].DispFlag = true;
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].cnt = 5;
			}


			//�肩�瓮�����ꏊ�Ɉړ�.
			for ( int numNo = 0; numNo < boss->animation[typeNo].No + 1; numNo++ ){
				for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
					//����ړ�.
					boss->rock[typeNo][numNo][dirNo].x -= cos( boss->rock[typeNo][boss->animation[typeNo].No][dirNo].Rang ) * ( S_SIZE * boss->rockData[typeNo].Spaa );
					boss->rock[typeNo][numNo][dirNo].y += sin( boss->rock[typeNo][boss->animation[typeNo].No][dirNo].Rang ) * ( S_SIZE * boss->rockData[typeNo].Spaa );
					boss->rock[typeNo][numNo][dirNo].cnt = 10;
				}
			}
			boss->animation[typeNo].No += 1;
		}
		else{
			//�ҋ@����.
			boss->animation[typeNo].cnt += 1;
			if ( boss->animation[typeNo].cnt > 50 ){
				for ( int numNo = 0; numNo < boss->animation[typeNo].No; numNo++ ){
					for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
						if ( boss->rock[typeNo][numNo][dirNo].cnt < 50 ){
							//����o���Ă���.
							boss->rock[typeNo][numNo][dirNo].cnt += 2;
							boss->rock[typeNo][numNo][dirNo].y -= C_SIZE / 40;

							//��(�M�~�b�N)�ƏՓ˂���.
							for ( int gty = 0; gty < G_TYPE; gty++ ) {
								for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
									if ( CollisionDetection(
										boss->rock[typeNo][numNo][dirNo].x, boss->rock[typeNo][numNo][dirNo].y,	S_SIZE * 2,					( S_SIZE / 2 ) * ( boss->rock[typeNo][numNo][dirNo].cnt / 10 + 1 ),
										gimmick[1][gty][gNo].x,				gimmick[1][gty][gNo].y,				gimmick[1][gty][gNo].wSize, gimmick[1][gty][gNo].hSize ) ) {

										//����Ȃ���.
										gimmick[1][gty][gNo].State = enGimmickState::GWait;
										gimmick[1][gty][gNo].x = WND_W;
										gimmick[1][gty][gNo].y = WND_H;
									}
								}
							}
						}
						//�v���C���[�ƏՓ˂���.
						if ( Player->chara.State == enCharaState::Living ) {
							if ( CollisionDetection(
								Player->chara.x + PH_CORR,			Player->chara.y + PH_CORR,			PH_SIZE,		PH_SIZE,
								boss->rock[typeNo][numNo][dirNo].x, boss->rock[typeNo][numNo][dirNo].y, S_SIZE * 2,		( S_SIZE / 2 ) * ( boss->rock[typeNo][numNo][dirNo].cnt / 10 + 1 ) ) ) {

								//�_���[�WSE��炷.
								CSoundManager::PlaySE( CSoundManager::SE_PDamage );

								//HP�����炷.
								Player->status.hp -= 1;
								if ( Player->status.hp <= 0 ) {
									Player->chara.State			= enCharaState::Dying;	//��Ԃ����S���ɂ���.
									Player->chara.ExpAnimCnt	= 0;					//������Ұ��ݶ�����0�ɂ���.
									Player->DeadNo				= enDeedNo::Golem;		//�{�X�Ŏ��S.
								}
								else {
									//��Ԃ𖳓G�ɂ���.
									Player->chara.State = enCharaState::Invi;
									Player->chara.inviTime = PINVITIME;
								}
							}
						}
					}
				}
			}
			if ( boss->animation[typeNo].cnt > 50 + boss->rockData[typeNo].Time ){
				//��𖳂����Ă�����.
				boss->AttackNo[typeNo] = enBossAttackNo::BUpRockEnd;
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].No = 0;
			}
		}
	}
	//��𖳂����Ă���.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpRockEnd ){
		if ( boss->animation[typeNo].No < boss->rockData[typeNo].Num ){
			boss->animation[typeNo].No += 1;
			for ( int numNo = 0; numNo < boss->animation[typeNo].No; numNo++ ){
				for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
					if ( boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].cnt > 0 ){
						//��𖄂߂Ă���.
						boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].cnt -= 2;
						boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].y += C_SIZE / 40;
					}
					else{
						boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].DispFlag = false;
					}
				}
			}
		}
		else{
			//�ҋ@����.
			boss->animation[typeNo].cnt += 1;
			for ( int numNo = 0; numNo < boss->animation[typeNo].No; numNo++ ){
				for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
					if ( boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].cnt > 0 ){
						//��𖄂߂Ă���(��̑���).
						boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].cnt -= 2;
						boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].y += C_SIZE / 40;
					}
					else{
						boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].DispFlag = false;
					}
				}
			}
			if ( boss->animation[typeNo].cnt > 50 ){
				//�U���I��.
				boss->AttackNo[typeNo] = enBossAttackNo::BWait;
			}
		}
	}
}

//��o���U��.
void RockCon( BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], PLAYER* Player, int typeNo )
{

	//���������.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BRockUp ) {
		boss->animation[typeNo].cnt += 1;
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ) {
			boss->chara[typeNo].y -= 1;
		}
		else {
			//������낷.
			boss->AttackNo[typeNo] = enBossAttackNo::BRockDown;
			boss->animation[typeNo].cnt = 0;
		}
	}
	//������낷.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BRockDown ) {
		boss->chara[typeNo].y += 5;
		if ( boss->chara[typeNo].y > boss->initial_xy[typeNo].y ) {
			//���W�𑵂���.
			boss->animation[typeNo].No = 0;
			boss->animation[typeNo].cnt = 0;
			boss->chara[typeNo].y = boss->initial_xy[typeNo].y;
			boss->AttackNo[typeNo] = enBossAttackNo::BRock;
			boss->animation[typeNo].Flag = false;
			//�n�ʒ@��SE��炷.
			CSoundManager::PlaySE( CSoundManager::SE_BROCKCrush );
		}
	}
	//����o���Ă���.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BRock ) {
		if ( boss->animation[typeNo].No < boss->rockData[typeNo].Num ) {
			bool BreakFlag = false;

			double ang = 90;
			ang -= boss->rockData[typeNo].Ang * ( boss->rockData[typeNo].Dir - 1 ) / 2;

			for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ) {
				for ( int gty = 0; gty < G_TYPE; gty++ ) {
					for ( int gno = 0; gno < G_MAX; gno++ ) {
						if ( gimmick[1][gty][gno].State == enGimmickState::GWait ) {
							boss->RockRAngData[boss->animation[typeNo].No][dirNo] = RAD( ang + boss->rockData[typeNo].Ang * dirNo );	//�p�x�𒲂ׂ�.
							//���W�𑵂��t���O�𗧂Ă�.
							gimmick[1][gty][gno].x			= boss->chara[typeNo].x + boss->charaSize[typeNo] / 2 - S_SIZE;
							gimmick[1][gty][gno].y			= boss->chara[typeNo].y + boss->charaSize[typeNo];
							gimmick[1][gty][gno].State		= enGimmickState::GLiving;
							gimmick[1][gty][gno].RockFlag	= true;

							//�\������ gNo ��ۑ�.
							boss->RockDispNoData[boss->animation[typeNo].No][dirNo] = gno;

							BreakFlag = true;
							break;
						}
					}
					if ( BreakFlag == true ) {
						BreakFlag = false;
						break;
					}
				}
			}
			//�肩�瓮�����ꏊ�Ɉړ�.
			for ( int numNo = 0; numNo < boss->animation[typeNo].No + 1; numNo++ ) {
				for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ) {
					for ( int gty = 0; gty < G_TYPE; gty++ ) {
						if ( gimmick[1][gty][boss->RockDispNoData[boss->animation[typeNo].No][dirNo]].State == enGimmickState::GLiving ) {
							//����ړ�.
							gimmick[1][gty][boss->RockDispNoData[boss->animation[typeNo].No][dirNo]].x -= cos( boss->RockRAngData[numNo][dirNo] ) * ( S_SIZE * boss->rockData[typeNo].Spaa );
							gimmick[1][gty][boss->RockDispNoData[boss->animation[typeNo].No][dirNo]].y += sin( boss->RockRAngData[numNo][dirNo] ) * ( S_SIZE * boss->rockData[typeNo].Spaa );
							break;
						}
					}
				}
			}
			boss->animation[typeNo].No += 1;
		}
		else {
			if ( boss->animation[typeNo].No < 50 ) {
				boss->animation[typeNo].cnt		= 10;
				boss->animation[typeNo].cntMax	= 50;
				boss->animation[typeNo].No += 1;
			}
			else {
				boss->animation[typeNo].cnt += 1;

				if ( boss->animation[typeNo].cnt >= boss->animation[typeNo].cntMax ) {

					//����o��.
					for ( int gty = 0; gty < G_TYPE; gty++ ) {
						for ( int gno = 0; gno < G_MAX; gno++ ) {
							gimmick[1][gty][gno].RockFlag = false;
						}
					}

					//��˂��グSE��炷.
					CSoundManager::PlaySE( CSoundManager::SE_BROCK );

					//�\������ gno ��������.
					for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
						for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
							boss->RockDispNoData[numNo][dirNo] = 0;
						}
					}

					//�v���C���[�ƂԂ�����.
					for ( int gty = 0; gty < G_TYPE; gty++ ) {
						for ( int gno = 0; gno < G_MAX; gno++ ) {
							if ( CollisionDetection(
								Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
								gimmick[1][gty][gno].x, gimmick[1][gty][gno].y, gimmick[1][gty][gno].wSize, gimmick[1][gty][gno].hSize ) )
							{
								//��̔j��SE��炷.
								CSoundManager::PlaySE( CSoundManager::SE_GRockDest );

								gimmick[1][gty][gno].State = enGimmickState::GWait;
								gimmick[1][gty][gno].x = WND_W;
								gimmick[1][gty][gno].y = WND_H;

								//�_���[�WSE��炷.
								CSoundManager::PlaySE( CSoundManager::SE_PDamage );

								//HP�����炷.
								Player->status.hp -= 1;
								if ( Player->status.hp <= 0 ) {
									Player->chara.State = enCharaState::Dying;	//��Ԃ����S���ɂ���.
									Player->chara.ExpAnimCnt = 0;				//������Ұ��ݶ�����0�ɂ���.
									Player->DeadNo = enDeedNo::Golem;			//�{�X�Ŏ��S.
								}
								else {
									//��Ԃ𖳓G�ɂ���.
									Player->chara.State = enCharaState::Invi;
									Player->chara.inviTime = PINVITIME;
								}

							}
						}
					}

					boss->AttackNo[typeNo] = enBossAttackNo::BWait;
					boss->animation[typeNo].cnt = 0;
					boss->animation[typeNo].No = 0;
					boss->animation[typeNo].Flag = false;
				}
			}
		}
	}
}

//������΂��U��.
void Blowaway( BOSS* boss, SCROLL* scroll, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo )
{
	//���������.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpHand ) {
		boss->animation[typeNo].cnt += 1;
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ) {
			boss->chara[typeNo].y -= 1;
		}
		else {
			//������낷.
			boss->AttackNo[typeNo]		= enBossAttackNo::BDownHand;
			boss->animation[typeNo].cnt = 0;

			CSoundManager::PlaySE( CSoundManager::SE_BollA );

			//������΂��G�t�F�N�g�̕\��.
			boss->BlowAway.y		= ( int ) ( boss->chara[enBoss::LHand].y );
			boss->BlowAway.DispFlag = true;
		}
	}
	//������낷.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BDownHand ) {
		boss->chara[typeNo].y += 5;
		if ( boss->chara[typeNo].y > boss->initial_xy[typeNo].y ) {
			//���W�𑵂���.
			boss->animation[typeNo].No = 0;
			boss->animation[typeNo].cnt = 0;
			boss->chara[typeNo].y = boss->initial_xy[typeNo].y;
			if ( Player->chara.y < WND_H - C_SIZE * 2 ) {
				//�v���C���[�̈ړ�.
				Player->chara.y += 10;

				//�X�e�[�W�M�~�b�N[��]�ɂԂ�����.
				for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
					for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
						if ( CollisionDetection(
							Player->chara.x + PH_CORR,	Player->chara.y + PH_CORR,	PH_SIZE,						PH_SIZE,
							gimmick[1][ptnNo][gNo].x,	gimmick[1][ptnNo][gNo].y,	gimmick[1][ptnNo][gNo].wSize,	gimmick[1][ptnNo][gNo].hSize ) )
						{
							//�Ԃ�������̏�̍��W�ɑ����_���[�W.
							Player->chara.y = gimmick[1][ptnNo][gNo].y - C_SIZE + PH_CORR;

							//�_���[�WSE��炷.
							CSoundManager::PlaySE( CSoundManager::SE_PDamage );

							//HP�����炷.
							Player->status.hp -= 1;
							if ( Player->status.hp <= 0 ) {
								Player->chara.State = enCharaState::Dying;	//��Ԃ����S���ɂ���.
								Player->chara.ExpAnimCnt = 0;				//������Ұ��ݶ�����0�ɂ���.
								Player->DeadNo = enDeedNo::Golem;			//�{�X�Ŏ��S.
							}
							else {
								//��Ԃ𖳓G�ɂ���.
								Player->chara.State = enCharaState::Invi;
								Player->chara.inviTime = PINVITIME;
							}

							//�����I��.
							boss->Blowcnt = 0;
							boss->AttackNo[typeNo] = enBossAttackNo::BWait;

							//��΂���������_���Ō��߂�.
							int Hand = rand() % 2 + 1;
							AtPunchFlag( boss, *Player, Hand );
						}
					}
				}

				//���S���Ă���G.
				for ( int type = 0; type < E_TYPE; type++ ){
					for ( int ptn = 0; ptn < E_PTN; ptn++ ){
						for ( int eNo = 0; eNo < E_MAX; eNo++ ){
							if( !( ( type == 4 ) && ( enemy[type][ptn][eNo].chara.State != enCharaState::Dead ) ) ){
								if ( ( enemy[type][ptn][eNo].chara.State == enCharaState::Living ) || ( enemy[type][ptn][eNo].chara.State == enCharaState::Dead ) ) {
									//�����ɐڐG���Ă����𒲂ׂ�.
									bool HitRock = false;	//�����ɐڐG���Ă���.
									bool BreakFlag = false;
									for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
										for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
											if ( CollisionDetection(
												enemy[type][ptn][eNo].chara.x + PH_CORR, enemy[type][ptn][eNo].chara.y + PH_CORR, PH_SIZE, PH_SIZE,
												gimmick[1][ptnNo][gNo].x, gimmick[1][ptnNo][gNo].y, gimmick[1][ptnNo][gNo].wSize, gimmick[1][ptnNo][gNo].hSize ) )
											{
												HitRock = true;
												BreakFlag = true;
												break;
											}
										}
										if ( BreakFlag == true ) {
											break;
										}
									}

									if ( HitRock == false ) {
										//�ړ�.
										enemy[type][ptn][eNo].chara.y += 10;

										//�X�e�[�W�M�~�b�N[��]�ɂԂ�����.
										for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
											for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
												if ( CollisionDetection(
													enemy[type][ptn][eNo].chara.x + PH_CORR, enemy[type][ptn][eNo].chara.y + PH_CORR, PH_SIZE, PH_SIZE,
													gimmick[1][ptnNo][gNo].x, gimmick[1][ptnNo][gNo].y, gimmick[1][ptnNo][gNo].wSize, gimmick[1][ptnNo][gNo].hSize ) )
												{
													enemy[type][ptn][eNo].chara.y = gimmick[1][ptnNo][gNo].y - C_SIZE + PH_CORR;

													if ( enemy[type][ptn][eNo].chara.State == enCharaState::Living ) {
														//�G�̃_���[�WSE��炷.
														CSoundManager::PlaySE( CSoundManager::SE_EDamage );

														//HP�����炷.
														enemy[typeNo][ptn][eNo].status.hp -= 1;
														if ( enemy[typeNo][ptn][eNo].status.hp <= 0 ) {
															enemy[typeNo][ptn][eNo].chara.State = enCharaState::Dying;	//��Ԃ����S���ɂ���.
															enemy[typeNo][ptn][eNo].chara.ExpAnimCnt = 0;					//������Ұ��ݶ�����0�ɂ���.
															enemy[typeNo][ptn][eNo].chara.chgCnt = 0;
														}
														else {
															//��Ԃ𖳓G�ɂ���.
															enemy[typeNo][ptn][eNo].chara.State = enCharaState::Invi;
															enemy[typeNo][ptn][eNo].chara.inviTime = INVITIME;
														}

													}
												}
											}
										}

										//��ʊO�ɂł�.
										if ( enemy[type][ptn][eNo].chara.y > GWND_H ) {
											enemy[type][ptn][eNo].chara.x = WND_W;
											enemy[type][ptn][eNo].chara.y = WND_H;				//��ʂ̊O(��).
											enemy[type][ptn][eNo].chara.State = enCharaState::Wait;	//�ҋ@.
											enemy[type][ptn][eNo].appFlag = false;
											enemy[type][ptn][eNo].BossFlag = false;
										}
									}
								}
							}
						}
					}
				}

				//��ʗh��.
				scroll->BossDAng += 1.0;
				scroll->BossDispYCnt += 1;
				if ( scroll->BossDispYCnt >= 1 ) {
					scroll->BossDispYCnt = 0;
				}
				scroll->BossDispY = sin( scroll->BossDAng ) * ( 10 - scroll->BossDispYCnt / 5 );
			}
			else {
				boss->Blowcnt = 0;
				boss->AttackNo[typeNo] = enBossAttackNo::BWait;
				
				//��΂���������_���Ō��߂�.
				int Hand = rand() % 2 + 1;
				AtPunchFlag( boss, *Player, Hand );
			}
		}
	}
}

//�{�X�̒e�U��.
void BShotCon( BOSS* boss )
{
	int NumShotCnt = 0;	//����+�e����ʂ̊O�ֈړ�������.

	//�e���̐��������[�v.
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		//�����̐��������[�v.
		for ( int dirNo = 0; dirNo < boss->BS_DIR; dirNo++ ) {
			//�e�����˒��Ȃ�.
			if ( boss->NumShotFlag[numNo] == true ) {
				if ( boss->shot[numNo][dirNo].DispShotFlag == true ) {

					//�e�̈ړ�.
					boss->shot[numNo][dirNo].x -= cos( boss->shot[numNo][dirNo].ang ) * boss->shot[numNo][dirNo].sspd;
					boss->shot[numNo][dirNo].y += sin( boss->shot[numNo][dirNo].ang ) * boss->shot[numNo][dirNo].sspd;

					//��ʂ̊O�ֈړ�����.
					if ( ( boss->shot[numNo][dirNo].y < -S_SIZE ) || ( boss->shot[numNo][dirNo].y > GWND_H ) ||
						( boss->shot[numNo][dirNo].x < -S_SIZE ) || ( boss->shot[numNo][dirNo].x > GWND_W ) ) {

						boss->shot[numNo][dirNo].DispShotFlag = false;
						boss->shot[numNo][dirNo].x = WND_W;
						boss->shot[numNo][dirNo].y = WND_H;
					}
				}
			}
		}
		//����+�e�S�ĉ�ʂ̊O�ֈړ�����.
		for ( int dirNo = 0; dirNo < boss->BS_DIR; dirNo++ ) {
			if ( boss->shot[numNo][dirNo].DispShotFlag == false ) {
				NumShotCnt++;
			}
		}
		if ( NumShotCnt == boss->BS_DIR ) {
			boss->NumShotFlag[numNo] = false;	//���˃t���O���~�낷.
		}

		NumShotCnt = 0;	//��ʂ̊O�ֈړ���������������.
	}
}