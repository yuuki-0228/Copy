#include "Main.h"
#include "Shot.h"
#include "CSoundManager.h"

void S_CollisionDetection( PLAYER* PlayerData, ENEMY EnemyData[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss )
{
	PStoE_DC( PlayerData, EnemyData );					//�v���C���[�̒e�ƓG�̓����蔻��.
	PStoB_DC( PlayerData, boss );						//�v���C���[�̒e�ƃ{�X�̓����蔻��.
	PStoG_DC( PlayerData, gimmick, WoodLeaf, boss );	//�v���C���[�̒e�ƃX�e�[�W�M�~�b�N�̓����蔻��.
	PStoEB_DC( PlayerData, EnemyData );					//�v���C���[�̒e�ƓG�̃o���A�̓����蔻��.
	EStoP_DC( PlayerData, EnemyData );					//�G�̒e�ƃv���C���[�̓����蔻��.
	EStoG_DC( EnemyData, gimmick, WoodLeaf, boss );		//�G�̒e�ƃX�e�[�W�M�~�b�N�̓����蔻��.
	EStoPB_DC( PlayerData, EnemyData );					//�G�̒e�ƃv���C���[�̃o���A�̓����蔻��.
	BStoP_DC( PlayerData, boss );						//�{�X�̒e�ƃv���C���[�̓����蔻��.
	BStoPB_DC( PlayerData, boss );						//�{�X�̒e�ƃv���C���[�̃o���A�̓����蔻��( �{�X�̒e��[�ђ�]���������Ă���ꍇ�͎g�p���Ȃ� ).
	PStoES_DC( PlayerData, EnemyData );					//�v���C���[�̒e�ƓG�̒e�̓����蔻��.
	PStoBS_DC( PlayerData, boss );						//�v���C���[�̒e�ƃ{�X�̒e�̓����蔻��.
}

//�v���C���[�̒e�ƓG�̓����蔻��.
void PStoE_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
		for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
			//���@�̒e�����˂���Ă���.
			if ( Player->shot[numNo][dirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							//�G�@�̐����m�F.
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) {
								//���@�̒e�ƓG�@�̓����蔻��.
								if ( ( ( Player->ability.chg == 0 ) && ( CollisionDetection(
									( int )Player->shot[numNo][dirNo].x,			( int )Player->shot[numNo][dirNo].y,			S_SIZE,			S_SIZE,
									( int )Enemy[typeNo][ptnNo][eNo].chara.x,		( int )Enemy[typeNo][ptnNo][eNo].chara.y,		C_SIZE,			C_SIZE ) ) ) ||

									( ( Player->ability.chg != 0 ) && ( CollisionDetection(
									( int ) Player->shot[numNo][dirNo].x - S_SIZE,  ( int ) Player->shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,		S_SIZE * 2,
									( int ) Enemy[typeNo][ptnNo][eNo].chara.x,		( int ) Enemy[typeNo][ptnNo][eNo].chara.y,		C_SIZE,			C_SIZE ) ) ) ) {

									//�G�̃_���[�WSE��炷.
									CSoundManager::PlaySE( CSoundManager::SE_EDamage );

									//HP�����炷.
									Enemy[typeNo][ptnNo][eNo].status.hp -= Player->status.atk;
									if ( Enemy[typeNo][ptnNo][eNo].status.hp <= 0 ){
										Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Dying;	//��Ԃ����S���ɂ���.
										Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt = 0;					//������Ұ��ݶ�����0�ɂ���.
										Enemy[typeNo][ptnNo][eNo].chara.chgCnt = 0;

										//�ǔ��I��.
										Player->shot[numNo][dirNo].homingCnt = HOMINGMAX;
									}
									else{
										//��Ԃ𖳓G�ɂ���.
										Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Invi;
										Enemy[typeNo][ptnNo][eNo].chara.inviTime = INVITIME;
									}

									//���@�̒e�̒��e��̏���.
									if ( Player->ability.pene == 0 ){	//�ђʂ������Ă��Ȃ�.
										if ( Player->ability.beam != 0 ){
											//�r�[���̃q�b�g��+2.
											Player->shot[numNo][dirNo].beamCnt -= 2;
											if ( Player->shot[numNo][dirNo].beamCnt == 0 ){

												Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
												Player->shot[numNo][dirNo].beamCnt = BEAMMAX;		//�r�[���̃q�b�g�񐔂�������.
												Player->shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
												Player->shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
											}
										}
										else{
											Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
											Player->shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
											Player->shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
										}
									}

									break;	//�������for���𔲂���.
								}
							}
						}
					}
				}
			}
		}
	}
}

//�v���C���[�̒e�ƃ{�X�̓����蔻��.
void PStoB_DC( PLAYER* Player, BOSS* boss )
{
	for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
		for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
			//���@�̒e�����˂���Ă���.
			if ( Player->shot[numNo][dirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
					//�{�X�̐����m�F.
					if ( boss->chara[typeNo].State == enCharaState::Living ) {
						//���@�̒e�ƓG�@�̓����蔻��.
						if ( ( ( Player->ability.chg == 0 ) && ( CollisionDetection(
							( int ) Player->shot[numNo][dirNo].x,	( int ) Player->shot[numNo][dirNo].y,	S_SIZE,					 S_SIZE,
							( int ) boss->chara[typeNo].x,			( int ) boss->chara[typeNo].y,			boss->charaSize[typeNo], boss->charaSize[typeNo] ) ) ) ||

							( ( Player->ability.chg != 0 ) && ( CollisionDetection(
							( int ) Player->shot[numNo][dirNo].x - S_SIZE,	( int ) Player->shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,					S_SIZE * 2,
							( int ) boss->chara[typeNo].x,					( int ) boss->chara[typeNo].y,					boss->charaSize[typeNo],	boss->charaSize[typeNo] ) ) ) ) {

							//�{�X�̃_���[�WSE��炷.
							CSoundManager::PlaySE( CSoundManager::SE_EDamage );

							//HP�����炷.
							boss->status[typeNo].hp -= Player->status.atk;
							if ( boss->status[typeNo].hp <= 0 ) {
								boss->chara[typeNo].State		= enCharaState::Dying;	//��Ԃ����S���ɂ���.
								boss->chara[typeNo].ExpAnimCnt	= 0;					//������Ұ��ݶ�����0�ɂ���.
								boss->chara[typeNo].chgCnt		= 0;
								boss->AttackNo[typeNo]			= enBossAttackNo::BWait;
								//�{�̂����ꂽ�ꍇ���r�����S���ɂ���.
								if ( typeNo == 0 ) {
									//�{�X�̓��ޏoSE�̍Đ�.
									CSoundManager::PlaySE( CSoundManager::SE_BEarth );

									for ( int i = enBoss::RHand; i < boss->BOSS_TYPE; i++ ) {
										if ( ( boss->chara[i].State == enCharaState::Living ) || ( boss->chara[i].State == enCharaState::Invi ) ) {
											boss->chara[i].State = enCharaState::Dying;		//��Ԃ����S���ɂ���.
											boss->chara[i].ExpAnimCnt = 0;					//������Ұ��ݶ�����0�ɂ���.
											boss->chara[i].chgCnt = 0;
										}
									}
								}

								//�ǔ��I��.
								Player->shot[numNo][dirNo].homingCnt = HOMINGMAX;
							}
							else {
								//��Ԃ𖳓G�ɂ���.
								boss->chara[typeNo].State = enCharaState::Invi;
								boss->chara[typeNo].inviTime = INVITIME;
							}

							//���@�̒e�̒��e��̏���.
							if ( Player->ability.pene == 0 ) {	//�ђʂ������Ă��Ȃ�.
								if ( Player->ability.beam != 0 ) {
									//�r�[���̃q�b�g��+2.
									Player->shot[numNo][dirNo].beamCnt -= 2;
									if ( Player->shot[numNo][dirNo].beamCnt == 0 ) {

										Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
										Player->shot[numNo][dirNo].beamCnt = BEAMMAX;		//�r�[���̃q�b�g�񐔂�������.
										Player->shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
										Player->shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
									}
								}
								else {
									Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
									Player->shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
									Player->shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
								}
							}
							break;	//�������for���𔲂���.
						}
					}
				}
			}
		}
	}
}

//�v���C���[�̒e�ƃX�e�[�W�M�~�b�N�̓����蔻��.
void PStoG_DC( PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss )
{
	for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
		for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
			//���@�̒e�����˂���Ă���.
			if ( Player->shot[numNo][dirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GWait ) {
								//���@�̒e�ƃX�e�[�W�M�~�b�N�̓����蔻��.
								if ( Player->ability.chg == 0 ) {
									if ( CollisionDetection(
										( int ) Player->shot[numNo][dirNo].x, ( int ) Player->shot[numNo][dirNo].y, S_SIZE, S_SIZE,
										( int ) gimmick[typeNo][ptnNo][gNo].x, ( int ) gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) ) {

										//�X�e�[�W�M�~�b�N�̐����m�F.
										if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GLiving ) {
											//�؂Ȃ�.
											if ( typeNo == 0 ) {
												if ( boss->chara[enBoss::Main].State == enCharaState::Wait ){
													gimmick[typeNo][ptnNo][gNo].hp -= Player->status.atk;

													//�t���Ϗo��.
													int DispNum = rand() % 2;
													int DispCnt = 0;
													for ( int LNo = 0; LNo < W_HITMAX; LNo++ ){
														if ( WoodLeaf[LNo].DispFlag == false ){
															WoodLeaf[LNo].DispFlag = true;
															WoodLeaf[LNo].x = ( int )( gimmick[typeNo][ptnNo][gNo].x + rand() % C_SIZE );
															WoodLeaf[LNo].y = ( int )( gimmick[typeNo][ptnNo][gNo].y + rand() % C_SIZE );
															WoodLeaf[LNo].FallPower = rand() % 3;
															WoodLeaf[LNo].Speed = rand() % 7 + 2;
															WoodLeaf[LNo].PtnX = rand() % 3;
															WoodLeaf[LNo].PtnY = rand() % 2;
															WoodLeaf[LNo].Ang = 0;

															DispCnt++;
														}
														if ( DispCnt >= DispNum ){
															break;
														}
													}

													if ( gimmick[typeNo][ptnNo][gNo].hp <= 0 ) {

														//�؂̔j��SE��炷.
														CSoundManager::PlaySE( CSoundManager::SE_GTreeDest );

														gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GDead;	//��Ԃ����S�ɂ���.
														gimmick[typeNo][ptnNo][gNo].wSize = C_SIZE;
														gimmick[typeNo][ptnNo][gNo].hSize = C_SIZE;
													}
													else {

														//�M�~�b�N�̃_���[�WSE��炷.
														CSoundManager::PlaySE( CSoundManager::SE_GDamage );

														gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GInvi;
														gimmick[typeNo][ptnNo][gNo].inviTime = INVITIME;
													}
												}
											}
											//���@�̒e�̒��e��̏���.
											if ( typeNo < 2 ) {
												if ( Player->ability.pene == 0 ) {	//�ђʂ������Ă��Ȃ�.
													if ( Player->ability.beam != 0 ) {
														//�r�[���̃q�b�g��+2.
														Player->shot[numNo][dirNo].beamCnt -= 2;
														if ( Player->shot[numNo][dirNo].beamCnt == 0 ) {

															Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
															Player->shot[numNo][dirNo].beamCnt = BEAMMAX;		//�r�[���̃q�b�g�񐔂�������.
															Player->shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
															Player->shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
														}
													}
													else {
														Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
														Player->shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
														Player->shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
													}
												}
											}
										}
									}
								}
								else {
									if ( CollisionDetection(
										( int ) Player->shot[numNo][dirNo].x - S_SIZE, ( int ) Player->shot[numNo][dirNo].y - S_SIZE, S_SIZE * 2, S_SIZE * 2,
										( int ) gimmick[typeNo][ptnNo][gNo].x, ( int ) gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) ) {

										switch ( typeNo )
										{
										case 0:	//��.
											if ( boss->chara[enBoss::Main].State == enCharaState::Wait ){
												if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GDead ) {	//�؂芔.

													//�M�~�b�N�̃_���[�WSE��炷.
													CSoundManager::PlaySE( CSoundManager::SE_GDamage );

													gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GWait;
													gimmick[typeNo][ptnNo][gNo].x = -WND_W;
													gimmick[typeNo][ptnNo][gNo].y = -WND_H;
													gimmick[typeNo][ptnNo][gNo].wSize = C_SIZE;
													gimmick[typeNo][ptnNo][gNo].hSize = C_SIZE + C_SIZE / 2;
												}
												else {	//��.

													//�؂̔j��SE��炷.
													CSoundManager::PlaySE( CSoundManager::SE_GTreeDest );

													gimmick[typeNo][ptnNo][gNo].hp = 0;
													gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GDead;	//��Ԃ����S�ɂ���.
													gimmick[typeNo][ptnNo][gNo].wSize = C_SIZE;
													gimmick[typeNo][ptnNo][gNo].hSize = C_SIZE;

													//���e�㏈��.
													Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
													Player->shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
													Player->shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.

													//�t���Ϗo��.
													int DispNum = rand() % 3 + 1;
													int DispCnt = 0;
													for ( int LNo = 0; LNo < W_HITMAX; LNo++ ){
														if ( WoodLeaf[LNo].DispFlag == false ){
															WoodLeaf[LNo].DispFlag = true;
															WoodLeaf[LNo].x = ( int )( gimmick[typeNo][ptnNo][gNo].x + rand() % C_SIZE );
															WoodLeaf[LNo].y = ( int )( gimmick[typeNo][ptnNo][gNo].y + rand() % C_SIZE );
															WoodLeaf[LNo].FallPower = rand() % 3;
															WoodLeaf[LNo].Speed = rand() % 7 + 2;
															WoodLeaf[LNo].PtnX = rand() % 3;
															WoodLeaf[LNo].PtnY = rand() % 2;
															WoodLeaf[LNo].Ang = 0;

															DispCnt++;
														}
														if ( DispCnt >= DispNum ){
															break;
														}
													}
												}
											}
											break;
										case 1:	//��.

											//��̔j��SE��炷.
											CSoundManager::PlaySE( CSoundManager::SE_GRockDest );

											gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GWait;
											gimmick[typeNo][ptnNo][gNo].x = -WND_W;
											gimmick[typeNo][ptnNo][gNo].y = -WND_H;

											//���e�㏈��.
											Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
											Player->shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
											Player->shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
											break;
										default:
											if ( !( ( typeNo == 2 ) && ( ptnNo == 1 ) ) ){
												//�M�~�b�N�̃_���[�WSE��炷.
												CSoundManager::PlaySE( CSoundManager::SE_GDamage );

												gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GWait;
												gimmick[typeNo][ptnNo][gNo].x = -WND_W;
												gimmick[typeNo][ptnNo][gNo].y = -WND_H;
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//�v���C���[�̒e�ƓG�̃o���A�̓����蔻��.
void PStoEB_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
		for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
			//���@�̒e�����˂���Ă���.
			if ( Player->shot[numNo][dirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							if ( ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) || ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) ){
								//�o���A�������Ă���.
								if ( Enemy[typeNo][ptnNo][eNo].ability.barrUP != 0 ){
									for ( int bNo = 0; bNo < Enemy[typeNo][ptnNo][eNo].ability.barrUP * BARRMAX; bNo++ ) {
										//���@�̒e�ƓG�̃o���A�̓����蔻��.
										if ( ( ( Player->ability.chg == 0 ) && ( CollisionDetection(
											 ( int )Player->shot[numNo][dirNo].x,				( int )Player->shot[numNo][dirNo].y,				S_SIZE,		S_SIZE,
											 ( int )Enemy[typeNo][ptnNo][eNo].barrier[bNo].x,	( int )Enemy[typeNo][ptnNo][eNo].barrier[bNo].y,	S_SIZE,		S_SIZE ) ) ) ||

											 ( ( Player->ability.chg != 0 ) && ( CollisionDetection(
											 ( int )Player->shot[numNo][dirNo].x - S_SIZE,		( int )Player->shot[numNo][dirNo].y - S_SIZE,		S_SIZE * 2, S_SIZE * 2,
											 ( int )Enemy[typeNo][ptnNo][eNo].barrier[bNo].x,	( int )Enemy[typeNo][ptnNo][eNo].barrier[bNo].y,	S_SIZE,		S_SIZE ) ) ) ) {

											//���@�̒e�̒��e��̏���.
											if ( Player->ability.pene == 0 ){	//�ђʂ������Ă��Ȃ�.

												//�o���ASE��炷.
												CSoundManager::PlaySE( CSoundManager::SE_Barr );

												if ( Player->ability.beam != 0 ){
													//�r�[���̃q�b�g��+2.
													Player->shot[numNo][dirNo].beamCnt -= 2;
													if ( Player->shot[numNo][dirNo].beamCnt == 0 ){

														Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
														Player->shot[numNo][dirNo].beamCnt		= BEAMMAX;	//�r�[���̃q�b�g�񐔂�������.
														Player->shot[numNo][dirNo].x			= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
														Player->shot[numNo][dirNo].y			= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
													}
												}
												else{
													Player->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
													Player->shot[numNo][dirNo].x			= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
													Player->shot[numNo][dirNo].y			= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//�G�̒e�ƃv���C���[�̓����蔻��.
void EStoP_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
			for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
				for ( int numNo = 0; numNo < Enemy[typeNo][ptnNo][eNo].status.num; numNo++ ) {
					for ( int dirNo = 0; dirNo < Enemy[typeNo][ptnNo][eNo].status.dir; dirNo++ ) {
						//�G�̒e�����˂���Ă���.
						if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) {
							//�v���C���[�̐����m�F.
							if ( Player->chara.State == enCharaState::Living ) {
								//���@�̒e�ƓG�@�̓����蔻��.
								if ( ( ( typeNo == 1 ) && ( CollisionDetection(
									( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE / 2, ( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2, S_SIZE * 2, S_SIZE * 2,
									( int )Player->chara.x + PH_CORR,									( int )Player->chara.y + PH_CORR,									PH_SIZE,	PH_SIZE ) ) ) ||

									( ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
									( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x,			( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y,			S_SIZE,			S_SIZE,
									( int )Player->chara.x + PH_CORR,								( int )Player->chara.y + PH_CORR,								PH_SIZE,		PH_SIZE ) ) ) ||

									( ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
									( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE, ( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,		S_SIZE * 2,
									( int )Player->chara.x + PH_CORR,								( int )Player->chara.y + PH_CORR,								PH_SIZE,		PH_SIZE ) ) ) ){

									//�v���C���[�̃_���[�WSE��炷.
									CSoundManager::PlaySE( CSoundManager::SE_PDamage );

									//HP�����炷.
									Player->status.hp -= Enemy[typeNo][ptnNo][eNo].status.atk;
									if ( Player->status.hp <= 0 ){
										Player->chara.State			= enCharaState::Dying;	//��Ԃ����S���ɂ���.
										Player->chara.ExpAnimCnt	= 0;					//������Ұ��ݶ�����0�ɂ���.
										Player->DeadNo				= typeNo;				//�G(���)�Ŏ��S.
									}
									else{
										//��Ԃ𖳓G�ɂ���.
										Player->chara.State = enCharaState::Invi;
										Player->chara.inviTime = PINVITIME;
									}

									//�G�̒e�̒��e��̏���.
									if ( Enemy[typeNo][ptnNo][eNo].ability.pene == 0 ){	//�ђʂ������Ă��Ȃ�.
										if ( Enemy[typeNo][ptnNo][eNo].ability.beam != 0 ){
											//�r�[���̃q�b�g��+2.
											Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt -= 2;
											if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt == 0 ){

												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt = BEAMMAX;		//�r�[���̃q�b�g�񐔂�������.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
											}
										}
										else{
											Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
											Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
											Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
										}
									}

									break;	//�������for���𔲂���.
								}
							}
						}
					}
				}
			}
		}
	}
}

//�G�̒e�ƃX�e�[�W�M�~�b�N�̓����蔻��.
void EStoG_DC( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss )
{
	for ( int EtypeNo = 0; EtypeNo < E_TYPE; EtypeNo++ ) {
		for ( int EptnNo = 0; EptnNo < E_PTN; EptnNo++ ) {
			for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
				for ( int numNo = 0; numNo < Enemy[EtypeNo][EptnNo][eNo].status.num; numNo++ ) {
					for ( int dirNo = 0; dirNo < Enemy[EtypeNo][EptnNo][eNo].status.dir; dirNo++ ) {
						//���@�̒e�����˂���Ă���.
						if ( Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) {
							for ( int GtypeNo = 0; GtypeNo < G_TYPE; GtypeNo++ ) {
								for ( int GptnNo = 0; GptnNo < G_PTN; GptnNo++ ) {
									for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
										if ( gimmick[GtypeNo][GptnNo][gNo].State != enGimmickState::GWait ) {
											//���@�̒e�ƃX�e�[�W�M�~�b�N�̓����蔻��.
											if ( Enemy[EtypeNo][EptnNo][eNo].ability.chg == 0 ) {
												if ( ( ( EtypeNo == 1 ) && ( CollisionDetection(
													( int )Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x - S_SIZE / 2,	( int )Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2,	S_SIZE * 2,								S_SIZE * 2,
													( int )gimmick[GtypeNo][GptnNo][gNo].x,									( int )gimmick[GtypeNo][GptnNo][gNo].y,									gimmick[GtypeNo][GptnNo][gNo].wSize,	gimmick[GtypeNo][GptnNo][gNo].hSize ) ) ) ||

													( ( EtypeNo != 1 ) && ( CollisionDetection(
													( int ) Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x,	( int ) Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y,	S_SIZE,									S_SIZE,
													( int ) gimmick[GtypeNo][GptnNo][gNo].x,					( int ) gimmick[GtypeNo][GptnNo][gNo].y,					gimmick[GtypeNo][GptnNo][gNo].wSize,	gimmick[GtypeNo][GptnNo][gNo].hSize ) ) ) ) {

													//�X�e�[�W�M�~�b�N�̐����m�F.
													if ( gimmick[GtypeNo][GptnNo][gNo].State == enGimmickState::GLiving ) {
														//�؂Ȃ�.
														if ( GtypeNo == 0 ) {
															if ( boss->chara[enBoss::Main].State == enCharaState::Wait ){
																gimmick[GtypeNo][GptnNo][gNo].hp -= Enemy[EtypeNo][EptnNo][eNo].status.atk;

																//�t���Ϗo��.
																int DispNum = rand() % 2;
																int DispCnt = 0;
																for ( int LNo = 0; LNo < W_HITMAX; LNo++ ){
																	if ( WoodLeaf[LNo].DispFlag == false ){
																		WoodLeaf[LNo].DispFlag = true;
																		WoodLeaf[LNo].x = ( int )( gimmick[GtypeNo][GptnNo][gNo].x + rand() % C_SIZE );
																		WoodLeaf[LNo].y = ( int )( gimmick[GtypeNo][GptnNo][gNo].y + rand() % C_SIZE );
																		WoodLeaf[LNo].FallPower = rand() % 3;
																		WoodLeaf[LNo].Speed = rand() % 7 + 2;
																		WoodLeaf[LNo].PtnX = rand() % 3;
																		WoodLeaf[LNo].PtnY = rand() % 2;
																		WoodLeaf[LNo].Ang = 0;

																		DispCnt++;
																	}
																	if ( DispCnt >= DispNum ){
																		break;
																	}
																}

																if ( gimmick[GtypeNo][GptnNo][gNo].hp <= 0 ) {

																	//�ؔj��SE��炷.
																	CSoundManager::PlaySE( CSoundManager::SE_GTreeDest );

																	gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GDead;	//��Ԃ����S�ɂ���.
																	gimmick[GtypeNo][GptnNo][gNo].wSize = C_SIZE;
																	gimmick[GtypeNo][GptnNo][gNo].hSize = C_SIZE;
																}
																else {

																	//�M�~�b�N�̃_���[�WSE��炷.
																	CSoundManager::PlaySE( CSoundManager::SE_GDamage );

																	gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GInvi;
																	gimmick[GtypeNo][GptnNo][gNo].inviTime = INVITIME;
																}
															}
														}
														//���@�̒e�̒��e��̏���.
														if ( GtypeNo < 2 ) {
															if ( Enemy[EtypeNo][EptnNo][eNo].ability.pene == 0 ) {	//�ђʂ������Ă��Ȃ�.
																if ( Enemy[EtypeNo][EptnNo][eNo].ability.beam != 0 ) {
																	//�r�[���̃q�b�g��+2.
																	Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].beamCnt -= 2;
																	if ( Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].beamCnt == 0 ) {

																		Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
																		Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].beamCnt = BEAMMAX;		//�r�[���̃q�b�g�񐔂�������.
																		Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
																		Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
																	}
																}
																else {
																	Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;		//�����׸ނ��~�낷.
																	Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x = -WND_W;					//���@�̒ex���W:��ʂ̊O��.
																	Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y = -WND_H;					//���@�̒ey���W:��ʂ̊O��.
																}
															}
														}
													}
												}
											}
											else {
												if ( CollisionDetection(
													( int ) Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x - S_SIZE,	( int ) Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,								S_SIZE * 2,
													( int ) gimmick[GtypeNo][GptnNo][gNo].x,							( int ) gimmick[GtypeNo][GptnNo][gNo].y,							gimmick[GtypeNo][GptnNo][gNo].wSize,	gimmick[GtypeNo][GptnNo][gNo].hSize ) ) {

													switch ( GtypeNo )
													{
													case 0:	//��.
														if ( boss->chara[enBoss::Main].State == enCharaState::Wait ){
															if ( gimmick[GtypeNo][GptnNo][gNo].State == enGimmickState::GDead ) {	//�؂芔.

																//�M�~�b�N�̃_���[�WSE��炷.
																CSoundManager::PlaySE( CSoundManager::SE_GDamage );

																gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GWait;
																gimmick[GtypeNo][GptnNo][gNo].x = -WND_W;
																gimmick[GtypeNo][GptnNo][gNo].y = -WND_H;
																gimmick[GtypeNo][GptnNo][gNo].wSize = C_SIZE;
																gimmick[GtypeNo][GptnNo][gNo].hSize = C_SIZE + C_SIZE / 2;
															}
															else {	//��.

																//�ؔj��SE��炷.
																CSoundManager::PlaySE( CSoundManager::SE_GTreeDest );

																gimmick[GtypeNo][GptnNo][gNo].hp = 0;
																gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GDead;	//��Ԃ����S�ɂ���.
																gimmick[GtypeNo][GptnNo][gNo].wSize = C_SIZE;
																gimmick[GtypeNo][GptnNo][gNo].hSize = C_SIZE;

																//���e�㏈��.
																Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
																Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x = -WND_W;	//���@�̒ex���W:��ʂ̊O��.
																Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y = -WND_H;	//���@�̒ey���W:��ʂ̊O��.

																//�t���Ϗo��.
																int DispNum = rand() % 3 + 1;
																int DispCnt = 0;
																for ( int LNo = 0; LNo < W_HITMAX; LNo++ ){
																	if ( WoodLeaf[LNo].DispFlag == false ){
																		WoodLeaf[LNo].DispFlag = true;
																		WoodLeaf[LNo].x = ( int )( gimmick[GtypeNo][GptnNo][gNo].x + rand() % C_SIZE );
																		WoodLeaf[LNo].y = ( int )( gimmick[GtypeNo][GptnNo][gNo].y + rand() % C_SIZE );
																		WoodLeaf[LNo].FallPower = rand() % 3;
																		WoodLeaf[LNo].Speed = rand() % 7 + 2;
																		WoodLeaf[LNo].PtnX = rand() % 3;
																		WoodLeaf[LNo].PtnY = rand() % 2;
																		WoodLeaf[LNo].Ang = 0;

																		DispCnt++;
																	}
																	if ( DispCnt >= DispNum ){
																		break;
																	}
																}
															}
														}
														break;
													case 1:	//��.

														//��j��SE��炷.
														CSoundManager::PlaySE( CSoundManager::SE_GRockDest );

														gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GWait;
														gimmick[GtypeNo][GptnNo][gNo].x		= -WND_W;
														gimmick[GtypeNo][GptnNo][gNo].y		= -WND_H;

														//���e�㏈��.
														Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
														Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x			= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
														Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y			= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
														break;
													default:
														if ( !( ( GtypeNo == 2 ) && ( GptnNo == 1 ) ) ){

															//�M�~�b�N�̃_���[�WSE��炷.
															CSoundManager::PlaySE( CSoundManager::SE_GDamage );

															gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GWait;
															gimmick[GtypeNo][GptnNo][gNo].x = -WND_W;
															gimmick[GtypeNo][GptnNo][gNo].y = -WND_H;
															break;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//�G�̒e�ƃv���C���[�̃o���A�̓����蔻��.
void EStoPB_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	//�v���C���[���o���A�������Ă���.
	if ( Player->ability.barrUP != 0 ){
		for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
			for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
				for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
					for ( int numNo = 0; numNo < Enemy[typeNo][ptnNo][eNo].status.num; numNo++ ) {
						for ( int dirNo = 0; dirNo < Enemy[typeNo][ptnNo][eNo].status.dir; dirNo++ ) {
							//�G�̒e�����˂���Ă���.
							if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) {
								for ( int bNo = 0; bNo < Player->ability.barrUP * BARRMAX; bNo++ ) {
									//�G�̒e�ƃv���C���[�̃o���A�̓����蔻��.
									if ( ( ( typeNo == 1 ) && ( CollisionDetection(
										( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE / 2, ( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2, S_SIZE * 2, S_SIZE * 2,
										( int )Player->barrier[bNo].x,										( int )Player->barrier[bNo].y,										S_SIZE,		S_SIZE ) ) ) ||

										( ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
										( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x,			( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y,			S_SIZE,		S_SIZE,
										( int )Player->barrier[bNo].x,									( int )Player->barrier[bNo].y,									S_SIZE,		S_SIZE ) ) ) ||

										( ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
										( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE, ( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2, S_SIZE * 2,
										( int )Player->barrier[bNo].x,									( int )Player->barrier[bNo].y,									S_SIZE,		S_SIZE ) ) ) ){

										//�G�̒e�̒��e��̏���.
										if ( Enemy[typeNo][ptnNo][eNo].ability.pene == 0 ){	//�ђʂ������Ă��Ȃ�.

											//�o���ASE��炷.
											CSoundManager::PlaySE( CSoundManager::SE_Barr );

											if ( Enemy[typeNo][ptnNo][eNo].ability.beam != 0 ){
												//�r�[���̃q�b�g��+2.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt -= 2;
												if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt == 0 ){

													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt = BEAMMAX;		//�r�[���̃q�b�g�񐔂�������.
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
												}
											}
											else{
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//���@�̒ex���W:��ʂ̊O��.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//���@�̒ey���W:��ʂ̊O��.
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//�{�X�̒e�ƃv���C���[�̓����蔻��.
void BStoP_DC( PLAYER* Player, BOSS* boss )
{
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		for ( int dirNo = 0; dirNo < boss->status[enBoss::Main].dir; dirNo++ ) {
			//�G�̒e�����˂���Ă���.
			if ( boss->shot[numNo][dirNo].DispShotFlag == true ) {
				//�v���C���[�̐����m�F.
				if ( Player->chara.State == enCharaState::Living ) {
					//���@�̒e�ƓG�@�̓����蔻��.
					if ( CollisionDetection(
						( int ) boss->shot[numNo][dirNo].x, ( int ) boss->shot[numNo][dirNo].y,			S_SIZE,		S_SIZE,
						( int ) Player->chara.x + PH_CORR,			 ( int ) Player->chara.y + PH_CORR,	PH_SIZE,	PH_SIZE ) ) {

						//�_���[�WSE��炷.
						CSoundManager::PlaySE( CSoundManager::SE_PDamage );

						//HP�����炷.
						Player->status.hp -= boss->status[enBoss::Main].atk;
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

						//�G�̒e�̒��e��̏���.
						boss->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
						boss->shot[numNo][dirNo].x			  = -WND_W;	//���@�̒ex���W:��ʂ̊O��.
						boss->shot[numNo][dirNo].y			  = -WND_H;	//���@�̒ey���W:��ʂ̊O��.

						break;	//�������for���𔲂���.
					}
				}
			}
		}
	}
}

//�{�X�̒e�ƃv���C���[�̃o���A�̓����蔻��.
void BStoPB_DC( PLAYER* Player, BOSS* boss )
{
	//�v���C���[���o���A�������Ă���.
	if ( Player->ability.barrUP != 0 ) {
		for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
			for ( int dirNo = 0; dirNo < boss->status[enBoss::Main].dir; dirNo++ ) {
				//�G�̒e�����˂���Ă���.
				if ( boss->shot[numNo][dirNo].DispShotFlag == true ) {
					for ( int bNo = 0; bNo < Player->ability.barrUP * BARRMAX; bNo++ ) {
						//�G�̒e�ƃv���C���[�̃o���A�̓����蔻��.
						if ( CollisionDetection(
							 ( int ) boss->shot[numNo][dirNo].x - S_SIZE,	( int ) boss->shot[numNo][dirNo].y - S_SIZE,	S_SIZE, S_SIZE,
							 ( int ) Player->barrier[bNo].x,				( int ) Player->barrier[bNo].y,					S_SIZE,	S_SIZE ) ) {

							//�o���ASE��炷.
							CSoundManager::PlaySE( CSoundManager::SE_Barr );

							//�G�̒e�̒��e��̏���.
							boss->shot[numNo][dirNo].DispShotFlag = false;	//�����׸ނ��~�낷.
							boss->shot[numNo][dirNo].x			  = -WND_W;	//���@�̒ex���W:��ʂ̊O��.
							boss->shot[numNo][dirNo].y			  = -WND_H;	//���@�̒ey���W:��ʂ̊O��.
						}
					}
				}
			}
		}
	}
}

//�v���C���[�̒e�ƓG�̒e�̓����蔻��.
void PStoES_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int PnumNo = 0; PnumNo < Player->status.num; PnumNo++ ) {
		for ( int PdirNo = 0; PdirNo < Player->status.dir; PdirNo++ ) {
			//���@�̒e�����˂���Ă���.
			if ( Player->shot[PnumNo][PdirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							for ( int EnumNo = 0; EnumNo < Enemy[typeNo][ptnNo][eNo].status.num; EnumNo++ ) {
								for ( int EdirNo = 0; EdirNo < Enemy[typeNo][ptnNo][eNo].status.dir; EdirNo++ ) {
									//�G�̒e�����˂���Ă���.
									if ( Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].DispShotFlag == true ) {
										//���@�̒e�ƓG�@�̓����蔻��.
										if ( ( ( Player->ability.chg == 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x,							 ( int )Player->shot[PnumNo][PdirNo].y,								S_SIZE,		S_SIZE,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x,			 ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y,			S_SIZE,		S_SIZE ) ) ) ||

											 ( ( Player->ability.chg == 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x,							 ( int )Player->shot[PnumNo][PdirNo].y,								S_SIZE,		S_SIZE,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x - S_SIZE, ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y - S_SIZE,	S_SIZE * 2, S_SIZE * 2 ) ) ) ||
											
											 ( ( Player->ability.chg == 0 ) && ( typeNo == 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x,								 ( int )Player->shot[PnumNo][PdirNo].y,									S_SIZE,		S_SIZE,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x - S_SIZE / 2, ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y - S_SIZE / 2,	S_SIZE * 2, S_SIZE * 2 ) ) ) ||

											 ( ( Player->ability.chg != 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x - S_SIZE,					 ( int )Player->shot[PnumNo][PdirNo].y - S_SIZE,					S_SIZE * 2, S_SIZE * 2,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x,			 ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y,			S_SIZE,		S_SIZE ) ) ) ||

											 ( ( Player->ability.chg != 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x - S_SIZE,					 ( int )Player->shot[PnumNo][PdirNo].y - S_SIZE,					S_SIZE * 2, S_SIZE * 2,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x - S_SIZE, ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y - S_SIZE,	S_SIZE * 2, S_SIZE * 2 ) ) ) ||
												 
											 ( ( Player->ability.chg != 0 ) && ( typeNo == 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x - S_SIZE,						 ( int )Player->shot[PnumNo][PdirNo].y - S_SIZE,						S_SIZE * 2, S_SIZE * 2,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x - S_SIZE / 2, ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y - S_SIZE / 2,	S_SIZE * 2, S_SIZE * 2 ) ) ) )
										{
											//���@�̒e�̒��e��̏���.
											if ( !( ( Player->ability.pene != 0 ) ||
												    ( ( Player->ability.beam != 0 ) || ( Player->ability.chg != 0 ) ) && ( Enemy[typeNo][ptnNo][eNo].ability.pene == 0 ) ||
												    ( ( Player->ability.pene == 0 ) && ( Player->ability.beam == 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) &&
													  ( Enemy[typeNo][ptnNo][eNo].ability.beam == 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.beam == 0 ) && ( Player->ability.chg == 0 ) ) ) )
											{
												if ( Player->ability.beam != 0 ){
													//�r�[���̃q�b�g��+2.
													Player->shot[PnumNo][PdirNo].beamCnt -= 2;
													if ( Player->shot[PnumNo][PdirNo].beamCnt == 0 ){

														Player->shot[PnumNo][PdirNo].DispShotFlag	= false;	//�����׸ނ��~�낷.
														Player->shot[PnumNo][PdirNo].beamCnt		= BEAMMAX;	//�r�[���̃q�b�g�񐔂�������.
														Player->shot[PnumNo][PdirNo].x				= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
														Player->shot[PnumNo][PdirNo].y				= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
													}
												}
												else{
													Player->shot[PnumNo][PdirNo].DispShotFlag		= false;	//�����׸ނ��~�낷.
													Player->shot[PnumNo][PdirNo].x					= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
													Player->shot[PnumNo][PdirNo].y					= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
												}
											}

											//�G�̒e�̒��e��̏���.
											if ( !( ( ( ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) || ( Enemy[typeNo][ptnNo][eNo].ability.beam != 0 ) ) &&
													  ( ( Player->ability.chg == 0 ) && ( Player->ability.beam == 0 ) && ( Player->ability.pene == 0 ) ) ) ||
													( ( Enemy[typeNo][ptnNo][eNo].ability.pene != 0 ) && ( Player->ability.pene == 0 ) ) ) )
											{
												if ( Enemy[typeNo][ptnNo][eNo].ability.beam != 0 ){
													//�r�[���̃q�b�g��+2.
													Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].beamCnt -= 2;
													if ( Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].beamCnt == 0 ){

														Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].DispShotFlag	= false;	//�����׸ނ��~�낷.
														Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].beamCnt		= BEAMMAX;	//�r�[���̃q�b�g�񐔂�������.
														Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x			= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
														Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y			= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
													}
												}
												else{
													Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].DispShotFlag		= false;	//�����׸ނ��~�낷.
													Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x				= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
													Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y				= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
												}
											}
											break;	//�������for���𔲂���.
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//�v���C���[�̒e�ƃ{�X�̒e�̓����蔻��.
void PStoBS_DC( PLAYER* Player, BOSS* boss )
{
	for ( int PnumNo = 0; PnumNo < Player->status.num; PnumNo++ ) {
		for ( int PdirNo = 0; PdirNo < Player->status.dir; PdirNo++ ) {
			//���@�̒e�����˂���Ă���.
			if ( Player->shot[PnumNo][PdirNo].DispShotFlag == true ) {
				for ( int BnumNo = 0; BnumNo < boss->BS_NUM; BnumNo++ ) {
					for ( int BdirNo = 0; BdirNo < boss->status[enBoss::Main].dir; BdirNo++ ) {
						//�G�̒e�����˂���Ă���.
						if ( boss->shot[BnumNo][BdirNo].DispShotFlag == true ) {
							//���@�̒e�ƓG�@�̓����蔻��.
							if ( ( ( Player->ability.chg == 0 ) && ( CollisionDetection(
								 ( int ) Player->shot[PnumNo][PdirNo].x,			( int ) Player->shot[PnumNo][PdirNo].y,			 S_SIZE,	 S_SIZE,
								 ( int ) boss->shot[BnumNo][BdirNo].x,				( int ) boss->shot[BnumNo][BdirNo].y,			 S_SIZE,	 S_SIZE ) ) ) ||

								 ( ( Player->ability.chg != 0 ) && ( CollisionDetection(
								 ( int ) Player->shot[PnumNo][PdirNo].x - S_SIZE,	( int ) Player->shot[PnumNo][PdirNo].y - S_SIZE, S_SIZE * 2, S_SIZE * 2,
								 ( int ) boss->shot[BnumNo][BdirNo].x,				( int ) boss->shot[BnumNo][BdirNo].y,			 S_SIZE,	 S_SIZE ) ) ) ) {

								//���@�̒e�̒��e��̏���.
								if ( Player->ability.pene == 0 ){
									if ( Player->ability.beam != 0 ) {
										//�r�[���̃q�b�g��+2.
										Player->shot[PnumNo][PdirNo].beamCnt -= 2;
										if ( Player->shot[PnumNo][PdirNo].beamCnt == 0 ) {

											Player->shot[PnumNo][PdirNo].DispShotFlag	= false;	//�����׸ނ��~�낷.
											Player->shot[PnumNo][PdirNo].beamCnt		= BEAMMAX;	//�r�[���̃q�b�g�񐔂�������.
											Player->shot[PnumNo][PdirNo].x				= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
											Player->shot[PnumNo][PdirNo].y				= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
										}
									}
									else {
										Player->shot[PnumNo][PdirNo].DispShotFlag		= false;	//�����׸ނ��~�낷.
										Player->shot[PnumNo][PdirNo].x					= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
										Player->shot[PnumNo][PdirNo].y					= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
									}
								}

								//�G�̒e�̒��e��̏���.
								boss->shot[BnumNo][BdirNo].DispShotFlag					= false;	//�����׸ނ��~�낷.
								boss->shot[BnumNo][BdirNo].x							= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
								boss->shot[BnumNo][BdirNo].y							= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
								break;	//�������for���𔲂���.
							}
						}
					}
				}
			}
		}
	}
}
