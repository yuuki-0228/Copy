#include "Main.h"
#include "Shot.h"
#include "CSoundManager.h"

//�v���C���[�̒e�̔���.
void P_ShotFlag( PLAYER* playerData, enScene Scene, ENDING Ending )
{
	bool breakFlag = false;	//break�t���O.
	bool FlagSkip = false;	//break�t���O���̏����𖳎�����.
	int AngCnt = 0;			//�t���O�𗧂Ă������̒e�J�E���g.

	//��Ԋp�x�𒲂ׂ�.
	double ang;
	if ( Scene == enScene::Ending ){
		if ( Ending.Slime.Action == enAction::MoveLeft ){
			ang = IANG * 2;
		}
		else if ( Ending.Slime.Action == enAction::MoveRight ){
			ang = 0;
		}
	}
	else {
		ang = IANG;
	}
	ang -= playerData->chara.ShotDirAng * ( playerData->status.dir - 1 ) / 2;

	if ( playerData->shotCnt > S_TIME ) {
		//�e���̐��������[�v.
		for ( int numNo = 0; numNo < playerData->status.num; numNo++ ) {
			if ( playerData->NumShotFlag[numNo] == false ) {
				//�����̐��������[�v.
				for ( int dirNo = 0; dirNo < playerData->status.dir; dirNo++ ) {
					if ( ( playerData->ability.chg == 0 ) || ( playerData->chara.chgCnt == -1 ) ) {
						if ( playerData->shot[numNo][dirNo].DispShotFlag == false ) {
							//�w��̍��W�ɒe�̈ʒu�𑵂���.
							if ( playerData->ability.chg == 0 ) {
								playerData->shot[numNo][dirNo].x = ( playerData->chara.x + ( C_SIZE / 2 - S_SIZE / 2 ) );
							}
							else {
								playerData->shot[numNo][dirNo].x = ( playerData->chara.x + C_SIZE / 2 );
							}
							playerData->shot[numNo][dirNo].y = ( playerData->chara.y + ( C_SIZE / 2 - S_SIZE / 2 ) );
							playerData->shot[numNo][dirNo].ang = RAD( ang + playerData->chara.ShotDirAng * AngCnt );	//�p�x�𒲂ׂ�.
							playerData->shot[numNo][dirNo].DispShotFlag = true;	//���˃t���O�𗧂Ă�.
							AngCnt++;	//�t���O�𗧂Ă������̒e�J�E���g.

							breakFlag = true;	//break�t���O�𗧂Ă�.
						}
					}
					else if ( playerData->ability.chg != 0 ) {
						//���o�p��[4][9]���g�p.
						playerData->shot[4][9].x = ( playerData->chara.x + C_SIZE / 2 );
						playerData->shot[4][9].y = ( playerData->chara.y );

						if ( playerData->chara.chgCnt < CHARGEMAX ) {
							playerData->chara.chgCnt += 1 + playerData->ability.sspdUP * 2;
							if ( playerData->chara.chgCnt >= CHARGEMAX ) {
								//�`���[�W������.
								CSoundManager::PlaySE( CSoundManager::SE_SChgCnd );
							}
							FlagSkip = true;
							breakFlag = true;
							break;
						}
						else {
							playerData->chgNumCnt = playerData->status.num;
							if ( playerData->chgAniFlag == false ) {
								playerData->chgAniCnt += 2;
								if ( playerData->chgAniCnt >= 80 ) {
									playerData->chgAniCnt	= 80;
									playerData->chgAniFlag	= true;
								}
							}
							else {
								playerData->chgAniCnt -= 2;
								if ( playerData->chgAniCnt <= 0 ) {
									playerData->chgAniCnt	= 0;
									playerData->chgAniFlag	= false;
								}
							}
						}
					}
				}
			}
			if ( AngCnt == playerData->status.dir ) {
				playerData->NumShotFlag[numNo] = true;
			}
			if ( breakFlag == true ){
				if ( FlagSkip == false ){

					//SE��炷.
					SE_PShot( playerData );

					playerData->shotCnt = 0;		//�J�E���g��0�ɂ���.
					playerData->chara.chgCnt = 0;	//���߃J�E���g������.
					AngCnt = 0;						//�t���O�𗧂Ă������̒e�J�E���g��������.
				}
				break;								//�ꔭ��������for���𔲂���.
			}
		}
	}
}

//�G�̒e�̔���.
void E_ShotFlag( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, double Rang, bool angSkip )
{
	bool breakFlag = false;	//break�t���O.
	bool FlagSkip = false;	//break�t���O���̏����𖳎�����.
	int AngCnt = 0;			//�t���O�𗧂Ă������̒e�J�E���g.

	//��Ԋp�x�𒲂ׂ�.
	double ang = IANG;
	ang -= enemyData[typeNo][ptnNo][eNo].chara.ShotDirAng * ( enemyData[typeNo][ptnNo][eNo].status.dir - 1 ) / 2;

	//�e���̐��������[�v.
	for ( int numNo = 0; numNo < enemyData[typeNo][ptnNo][eNo].status.num; numNo++ ){
		if ( enemyData[typeNo][ptnNo][eNo].NumShotFlag[numNo] == false ) {
			//�����̐��������[�v.
			for ( int dirNo = 0; dirNo < enemyData[typeNo][ptnNo][eNo].status.dir; dirNo++ ) {
				if ( ( enemyData[typeNo][ptnNo][eNo].ability.chg == 0 ) || ( enemyData[typeNo][ptnNo][eNo].chara.chgCnt > CHARGEMAX ) ) {
					if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == false ) {
						//�w��̍��W�ɒe�̈ʒu�𑵂���.
						enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = ( enemyData[typeNo][ptnNo][eNo].chara.x + ( C_SIZE / 2 - S_SIZE / 2 ) );
						enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = ( enemyData[typeNo][ptnNo][eNo].chara.y + ( C_SIZE / 2 - S_SIZE / 2 ) );
						if ( angSkip == false ) {
							enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang = RAD( ang + enemyData[typeNo][ptnNo][eNo].chara.ShotDirAng * AngCnt );	//�p�x�𒲂ׂ�.
						}
						else {
							enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang = Rang;
						}
						enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = true;	//���˃t���O�𗧂Ă�.
						AngCnt++;	//�t���O�𗧂Ă������̒e�J�E���g.

						breakFlag = true;	//break�t���O�𗧂Ă�.
					}
				}
				else if ( enemyData[typeNo][ptnNo][eNo].ability.chg != 0 ) {
					//���o�p��[4][9]���g�p.
					enemyData[typeNo][ptnNo][eNo].shot[4][9].x = ( enemyData[typeNo][ptnNo][eNo].chara.x + C_SIZE / 2 );
					enemyData[typeNo][ptnNo][eNo].shot[4][9].y = ( enemyData[typeNo][ptnNo][eNo].chara.y + C_SIZE / 2);
					enemyData[typeNo][ptnNo][eNo].chara.chgCnt += 1.0;

					FlagSkip = true;
					breakFlag = true;
					break;
				}
				enemyData[typeNo][ptnNo][eNo].NumShotFlag[numNo] = true;
			}
			if ( breakFlag == true ){
				if ( FlagSkip == false ){

					//SE��炷.
					SE_EShot( enemyData, typeNo, ptnNo, eNo );

					AngCnt = 1;	//�t���O�𗧂Ă������̒e�J�E���g��������.
					enemyData[typeNo][ptnNo][eNo].chara.chgCnt = 0;
				}
				break;		//�ꔭ��������for���𔲂���.
			}
		}
	}
}

//�v���C���[�̒e.
void SE_PShot( PLAYER* player )
{
	if		( player->ability.beam != 0 )	{ CSoundManager::PlaySE( CSoundManager::SE_PSBeam ); }
	else if ( player->ability.pene != 0 )	{ CSoundManager::PlaySE( CSoundManager::SE_PSPene ); }
	else if ( player->ability.chg != 0 )	{ CSoundManager::PlaySE( CSoundManager::SE_PSChg );	 }
	else									{ CSoundManager::PlaySE( CSoundManager::SE_PSNorm ); }
}

//�G�̒e.
void SE_EShot( ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo )
{
	switch ( typeNo )
	{
	case 0:	CSoundManager::PlaySE( CSoundManager::SE_ESChg );		break;
	case 1:	CSoundManager::PlaySE( CSoundManager::SE_ESNorm );		break;
	case 2:	CSoundManager::PlaySE( CSoundManager::SE_ESPene );		break;
	case 3:	CSoundManager::PlaySE( CSoundManager::SE_ESBeam );		break;
	case 4:	CSoundManager::PlaySE( CSoundManager::SE_EShoming );	break;
	default:														break;
	}
}