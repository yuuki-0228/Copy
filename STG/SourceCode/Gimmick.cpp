#include "Main.h"
#include "CSoundManager.h"

//�M�~�b�N�̈ړ�.
void Gimmick_Ran( GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], SCROLL scroll, PLAYER* Player )
{
	//�M�~�b�N.
	if ( scroll.stop != true ) {
		for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
			for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					if ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GWait ) {
						gimmick[typeNo][ptnNo][gNo].y += scroll.spd;

						//��ʊO�ɏo��.
						if ( ( ( typeNo != 0 ) && ( gimmick[typeNo][ptnNo][gNo].y > GWND_H ) ) ||
							 ( ( typeNo == 0 ) && ( gimmick[typeNo][ptnNo][gNo].y - C_SIZE * 2 > GWND_H ) ) ) {

							gimmick[typeNo][ptnNo][gNo].x = WND_W;
							gimmick[typeNo][ptnNo][gNo].y = WND_H;
							if ( typeNo == 0 ) {
								gimmick[typeNo][ptnNo][gNo].wSize = C_SIZE;
								gimmick[typeNo][ptnNo][gNo].hSize = C_SIZE + C_SIZE / 2;
							}
							gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GWait;
						}

						//�v���C���[�ƏՓ˂����ꍇ.
						if ( ( Player->chara.State == enCharaState::Living ) ||
							 ( Player->chara.State == enCharaState::Invi ) ) {

							if ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GDead ) {
								if ( CollisionDetection(
									Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
									gimmick[typeNo][ptnNo][gNo].x, gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) )
								{
									if ( typeNo != 2 ) {
										//�����o������.
										Player->chara.y += scroll.spd;
										if ( Player->chara.y + C_SIZE > WND_H ) {		//��ʊO�ɏo��.
											//�v���C���[�̃_���[�WSE��炷.
											CSoundManager::PlaySE( CSoundManager::SE_PDamage );

											Player->chara.State			= enCharaState::Dying;	//���S���ɂ���.
											Player->chara.ExpAnimCnt	= 0;					//������Ұ��ݶ�����0�ɂ���.
											Player->DeadNo				= enDeedNo::Gimmick;	//�M�~�b�N�Ŏ��S.
										}
									}
									else {
										if ( Player->chara.State != enCharaState::Invi ) {
											//�v���C���[�̃_���[�WSE��炷.
											CSoundManager::PlaySE( CSoundManager::SE_PDamage );

											//�_���[�W����.
											Player->status.hp -= 1;
											if ( Player->status.hp <= 0 ) {
												Player->chara.State			= enCharaState::Dying;	//��Ԃ����S���ɂ���.
												Player->chara.ExpAnimCnt	= 0;					//������Ұ��ݶ�����0�ɂ���.
												Player->DeadNo				= enDeedNo::Gimmick;	//�M�~�b�N�Ŏ��S.
											}
											else {
												//��Ԃ𖳓G�ɂ���.
												Player->chara.State = enCharaState::Invi;
												Player->chara.inviTime = PINVITIME;
											}
										}
									}
								}
								if ( ( typeNo == 0 ) && ( gimmick[typeNo][ptnNo][gNo].TreeAlpha > 127 ) &&
									( CollisionDetection(
										Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
										gimmick[typeNo][ptnNo][gNo].x - C_SIZE / 2, gimmick[typeNo][ptnNo][gNo].y - C_SIZE - C_SIZE / 2 + 5, C_SIZE * 2, C_SIZE * 2 ) ) ) {

									//���߂��Ă���.
									gimmick[typeNo][ptnNo][gNo].TreeAlpha -= 5;
									if ( gimmick[typeNo][ptnNo][gNo].TreeAlpha < 127 ) {
										gimmick[typeNo][ptnNo][gNo].TreeAlpha = 127;
									}
								}
								else if ( gimmick[typeNo][ptnNo][gNo].TreeAlpha < 255 ) {

									//���߂𒼂��Ă���.
									gimmick[typeNo][ptnNo][gNo].TreeAlpha += 5;
									if ( gimmick[typeNo][ptnNo][gNo].TreeAlpha > 255 ) {
										gimmick[typeNo][ptnNo][gNo].TreeAlpha = 255;
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