#include "Main.h"
#include "Shot.h"
#include "CSoundManager.h"

//プレイヤーの弾の発射.
void P_ShotFlag( PLAYER* playerData, enScene Scene, ENDING Ending )
{
	bool breakFlag = false;	//breakフラグ.
	bool FlagSkip = false;	//breakフラグ内の処理を無視する.
	int AngCnt = 0;			//フラグを立てた方向の弾カウント.

	//飛ぶ角度を調べる.
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
		//弾数の数だけループ.
		for ( int numNo = 0; numNo < playerData->status.num; numNo++ ) {
			if ( playerData->NumShotFlag[numNo] == false ) {
				//方向の数だけループ.
				for ( int dirNo = 0; dirNo < playerData->status.dir; dirNo++ ) {
					if ( ( playerData->ability.chg == 0 ) || ( playerData->chara.chgCnt == -1 ) ) {
						if ( playerData->shot[numNo][dirNo].DispShotFlag == false ) {
							//指定の座標に弾の位置を揃える.
							if ( playerData->ability.chg == 0 ) {
								playerData->shot[numNo][dirNo].x = ( playerData->chara.x + ( C_SIZE / 2 - S_SIZE / 2 ) );
							}
							else {
								playerData->shot[numNo][dirNo].x = ( playerData->chara.x + C_SIZE / 2 );
							}
							playerData->shot[numNo][dirNo].y = ( playerData->chara.y + ( C_SIZE / 2 - S_SIZE / 2 ) );
							playerData->shot[numNo][dirNo].ang = RAD( ang + playerData->chara.ShotDirAng * AngCnt );	//角度を調べる.
							playerData->shot[numNo][dirNo].DispShotFlag = true;	//発射フラグを立てる.
							AngCnt++;	//フラグを立てた方向の弾カウント.

							breakFlag = true;	//breakフラグを立てる.
						}
					}
					else if ( playerData->ability.chg != 0 ) {
						//演出用で[4][9]を使用.
						playerData->shot[4][9].x = ( playerData->chara.x + C_SIZE / 2 );
						playerData->shot[4][9].y = ( playerData->chara.y );

						if ( playerData->chara.chgCnt < CHARGEMAX ) {
							playerData->chara.chgCnt += 1 + playerData->ability.sspdUP * 2;
							if ( playerData->chara.chgCnt >= CHARGEMAX ) {
								//チャージ完了音.
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

					//SEを鳴らす.
					SE_PShot( playerData );

					playerData->shotCnt = 0;		//カウントを0にする.
					playerData->chara.chgCnt = 0;	//溜めカウント初期化.
					AngCnt = 0;						//フラグを立てた方向の弾カウントを初期化.
				}
				break;								//一発撃ったらfor文を抜ける.
			}
		}
	}
}

//敵の弾の発射.
void E_ShotFlag( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, double Rang, bool angSkip )
{
	bool breakFlag = false;	//breakフラグ.
	bool FlagSkip = false;	//breakフラグ内の処理を無視する.
	int AngCnt = 0;			//フラグを立てた方向の弾カウント.

	//飛ぶ角度を調べる.
	double ang = IANG;
	ang -= enemyData[typeNo][ptnNo][eNo].chara.ShotDirAng * ( enemyData[typeNo][ptnNo][eNo].status.dir - 1 ) / 2;

	//弾数の数だけループ.
	for ( int numNo = 0; numNo < enemyData[typeNo][ptnNo][eNo].status.num; numNo++ ){
		if ( enemyData[typeNo][ptnNo][eNo].NumShotFlag[numNo] == false ) {
			//方向の数だけループ.
			for ( int dirNo = 0; dirNo < enemyData[typeNo][ptnNo][eNo].status.dir; dirNo++ ) {
				if ( ( enemyData[typeNo][ptnNo][eNo].ability.chg == 0 ) || ( enemyData[typeNo][ptnNo][eNo].chara.chgCnt > CHARGEMAX ) ) {
					if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == false ) {
						//指定の座標に弾の位置を揃える.
						enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = ( enemyData[typeNo][ptnNo][eNo].chara.x + ( C_SIZE / 2 - S_SIZE / 2 ) );
						enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = ( enemyData[typeNo][ptnNo][eNo].chara.y + ( C_SIZE / 2 - S_SIZE / 2 ) );
						if ( angSkip == false ) {
							enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang = RAD( ang + enemyData[typeNo][ptnNo][eNo].chara.ShotDirAng * AngCnt );	//角度を調べる.
						}
						else {
							enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang = Rang;
						}
						enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = true;	//発射フラグを立てる.
						AngCnt++;	//フラグを立てた方向の弾カウント.

						breakFlag = true;	//breakフラグを立てる.
					}
				}
				else if ( enemyData[typeNo][ptnNo][eNo].ability.chg != 0 ) {
					//演出用で[4][9]を使用.
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

					//SEを鳴らす.
					SE_EShot( enemyData, typeNo, ptnNo, eNo );

					AngCnt = 1;	//フラグを立てた方向の弾カウントを初期化.
					enemyData[typeNo][ptnNo][eNo].chara.chgCnt = 0;
				}
				break;		//一発撃ったらfor文を抜ける.
			}
		}
	}
}

//プレイヤーの弾.
void SE_PShot( PLAYER* player )
{
	if		( player->ability.beam != 0 )	{ CSoundManager::PlaySE( CSoundManager::SE_PSBeam ); }
	else if ( player->ability.pene != 0 )	{ CSoundManager::PlaySE( CSoundManager::SE_PSPene ); }
	else if ( player->ability.chg != 0 )	{ CSoundManager::PlaySE( CSoundManager::SE_PSChg );	 }
	else									{ CSoundManager::PlaySE( CSoundManager::SE_PSNorm ); }
}

//敵の弾.
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