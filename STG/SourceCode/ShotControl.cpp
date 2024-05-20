#include "Main.h"
#include "Shot.h"

//プレイヤーの弾の動作.
void P_ShotControl(
	PLAYER* playerData, BOSS* boss, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], enScene Scene )
{
 	int NumShotCnt = 0;	//方向+弾が画面の外へ移動した数.

	//弾数の数だけループ.
 	for ( int numNo = 0; numNo < playerData->status.num; numNo++ ) {
		//方向の数だけループ.
		for ( int dirNo = 0; dirNo < playerData->status.dir; dirNo++ ) {
			//弾が発射中なら.
			if ( playerData->NumShotFlag[numNo] == true ) {

				//追尾を取得中なら.
				if ( playerData->ability.homing != 0 ){
					P_homing( playerData, boss, enemyData, Scene, numNo, dirNo );
				}

				//弾の移動.
				playerData->shot[numNo][dirNo].x += cos( playerData->shot[numNo][dirNo].ang ) * ( playerData->shot[numNo][dirNo].sspd / BEAMMAX ) * playerData->shot[numNo][dirNo].beamCnt;
				playerData->shot[numNo][dirNo].y -= sin( playerData->shot[numNo][dirNo].ang ) * ( playerData->shot[numNo][dirNo].sspd / BEAMMAX ) * playerData->shot[numNo][dirNo].beamCnt;

				//追尾終了.
				if ( playerData->shot[numNo][dirNo].y < playerData->target.y ) {
					playerData->shot[numNo][dirNo].homingCnt = HOMINGMAX;
				}

				//画面の外へ移動した.
				if ( Scene == enScene::Ending ){
					if ( ( playerData->shot[numNo][dirNo].y < -S_SIZE ) || ( playerData->shot[numNo][dirNo].y > WND_H ) ||
						 ( playerData->shot[numNo][dirNo].x < -S_SIZE ) || ( playerData->shot[numNo][dirNo].x > WND_W ) ) {

						//ビームなら.
						if ( playerData->ability.beam != 0 ){
							playerData->shot[numNo][dirNo].beamCnt = BEAMMAX;		//ビームのヒット回数を初期化.
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

						//ビームなら.
						if ( playerData->ability.beam != 0 ){
							playerData->shot[numNo][dirNo].beamCnt = BEAMMAX;		//ビームのヒット回数を初期化.
						}
						playerData->shot[numNo][dirNo].DispShotFlag = false;
						playerData->shot[numNo][dirNo].x = -WND_W;
						playerData->shot[numNo][dirNo].y = -WND_H;
						NumShotCnt++;
					}
				}
			}
		}
		//方向+弾全て画面の外へ移動した.
		if ( NumShotCnt == playerData->status.dir ) {
			playerData->NumShotFlag[numNo] = false;			//発射フラグを降ろす.
			for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ){
				playerData->shot[numNo][dirNo].homingCnt = 0;			//追尾カウント初期化.
			}
		}
		NumShotCnt = 0;	//画面の外へ移動した数を初期化.
	}
}

//敵の弾の動作.
void E_ShotControl(
	ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, PLAYER playerData )
{
	int NumShotCnt = 0;	//方向+弾が画面の外へ移動した数.

	//弾数の数だけループ.
	for ( int numNo = 0; numNo < enemyData[typeNo][ptnNo][eNo].status.num; numNo++ ) {
		//方向の数だけループ.
		for ( int dirNo = 0; dirNo < enemyData[typeNo][ptnNo][eNo].status.dir; dirNo++ ) {
			//弾が発射中なら.
			if ( enemyData[typeNo][ptnNo][eNo].NumShotFlag[numNo] == true ) {
 
				//追尾を取得中なら.
				if ( enemyData[typeNo][ptnNo][eNo].ability.homing != 0 ){
					E_homing( enemyData, typeNo, ptnNo, eNo, numNo, dirNo, playerData );
				}

				//弾の移動.
				enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x -= cos( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang ) * enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd;
				enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y += sin( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang ) * enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd;

				//追尾終了.
				if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y > enemyData[typeNo][ptnNo][eNo].target.y - C_SIZE * 2 ) {
					enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt = HOMINGMAX;
				}

				//画面の外へ移動した.
				if ( ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y < -S_SIZE ) || ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y > GWND_H ) ||
					 ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x < -S_SIZE ) || ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x > GWND_W ) ){

					//ビームなら.
					if ( enemyData[typeNo][ptnNo][eNo].ability.beam != 0 ){
						enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt = BEAMMAX;		//ビームのヒット回数を初期化.
					}
					enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag	= false;
					enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x				= WND_W;
					enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y				= WND_H;
					NumShotCnt++;
				}
			}
		}
		//方向+弾全て画面の外へ移動した.
		if ( NumShotCnt == enemyData[typeNo][ptnNo][eNo].status.dir ){
			enemyData[typeNo][ptnNo][eNo].NumShotFlag[numNo] = false;	//発射フラグを降ろす.
			for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
				enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt = 0;			//追尾カウント初期化.
			}
		}
		NumShotCnt = 0;	//画面の外へ移動した数を初期化.
	}
}

//プレイヤーの追尾制御.
void P_homing( PLAYER* playerData, BOSS* boss, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], enScene Scene, int numNo, int dirNo )
{
	playerData->shot[numNo][dirNo].homingCnt += 1;

	if ( ( playerData->shot[numNo][dirNo].homingCnt > 5 ) && 
		 ( playerData->shot[numNo][dirNo].homingCnt < HOMINGMAX ) ){

		//一番近い敵を探し、ターゲットにする.
		if ( playerData->shot[numNo][dirNo].homingCnt == 6 ) {
			if ( boss->chara[enBoss::Main].State == enCharaState::Wait ) {
				P_TargetUpdate( playerData, enemyData, numNo, dirNo );
			}
			else {
				P_BTargetUpdate( playerData, boss, numNo, dirNo );
			}
		}
		if ( playerData->target.y > playerData->shot[numNo][dirNo].y - S_SIZE * 2 ) {
			//追尾終了.
			playerData->shot[numNo][dirNo].homingCnt = HOMINGMAX;
		}

		//角度を調べる.
		double Mang = atan2( ( playerData->shot[numNo][dirNo].y - ( playerData->target.y + C_SIZE / 2 - S_SIZE / 2 ) ), ( ( playerData->target.x + C_SIZE / 2 - S_SIZE / 2 ) - playerData->shot[numNo][dirNo].x ) );

		//角度の変更.
		if ( playerData->shot[numNo][dirNo].ang > Mang ){
			playerData->shot[numNo][dirNo].ang -= 0.1;
		}
		if ( playerData->shot[numNo][dirNo].ang < Mang ) {
			playerData->shot[numNo][dirNo].ang += 0.1;
		}
	}
}

//敵の追尾制御.
void E_homing( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, int numNo, int dirNo, PLAYER playerData )
{
	enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt += 1;

	if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt < HOMINGMAX ) {

		//プレイヤーをターゲットにする.
		E_TargetUpdate( enemyData, typeNo, ptnNo, eNo, numNo, dirNo, playerData );

		//角度を調べる.
		double Mang = atan2( ( ( enemyData[typeNo][ptnNo][eNo].target.y + C_SIZE / 2 - S_SIZE / 2 ) - enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ), ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - ( enemyData[typeNo][ptnNo][eNo].target.x + C_SIZE / 2 - S_SIZE / 2 ) ) );

		//角度の変更.
		if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang > Mang ){
			enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang -= 0.1;
		}
		if ( enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang < Mang ) {
			enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang += 0.1;
		}
	}
}

//プレイヤーのターゲットの更新.
void P_TargetUpdate( PLAYER* playerData, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int numNo, int dirNo )
{
	//ターゲットを初期化.
	playerData->target.x = playerData->chara.x + C_SIZE / 2 - S_SIZE / 2;
	playerData->target.y = -C_SIZE;

	//一番近い敵を探し、ターゲットにする.
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ){
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ){
			for ( int eNo = 0; eNo < E_MAX; eNo++ ){
				if ( ( enemyData[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) ||
					 ( enemyData[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) ){
					if ( ( enemyData[typeNo][ptnNo][eNo].chara.y + C_SIZE < playerData->chara.y - C_SIZE ) &&
						 ( enemyData[typeNo][ptnNo][eNo].chara.y + C_SIZE < playerData->shot[numNo][dirNo].y - S_SIZE ) &&
						 ( enemyData[typeNo][ptnNo][eNo].chara.y > playerData->target.y ) ){

						//ターゲットを更新.
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
	//ターゲットを初期化.
	playerData->target.x = GWND_W / 2 - C_SIZE / 2;
	playerData->target.y = -C_SIZE;

	//一番近い敵を探し、ターゲットにする.
	if ( ( boss->chara[enBoss::Main].State == enCharaState::Living ) ||
		 ( boss->chara[enBoss::Main].State == enCharaState::Invi ) ) {
		if ( ( boss->chara[enBoss::Main].y + C_SIZE < playerData->chara.y - C_SIZE ) &&
			 ( boss->chara[enBoss::Main].y + C_SIZE < playerData->shot[numNo][dirNo].y - S_SIZE ) &&
			 ( boss->chara[enBoss::Main].y > playerData->target.y ) ) {

			//ターゲットを更新.
			playerData->target.x = boss->chara[enBoss::Main].x + ( boss->charaSize[enBoss::Main] / 2 - C_SIZE / 2 );
			playerData->target.y = boss->chara[enBoss::Main].y + ( boss->charaSize[enBoss::Main] / 2 - C_SIZE / 2 );
		}
	}	
}

//敵のターゲットの更新.
void E_TargetUpdate( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, int numNo, int dirNo, PLAYER playerData )
{
	//ターゲットを初期化.
	enemyData[typeNo][ptnNo][eNo].target.x = GWND_W / 2 - C_SIZE / 2;
	enemyData[typeNo][ptnNo][eNo].target.y = -C_SIZE;

	//一番近い敵を探し、ターゲットにする.
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ){
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ){
			for ( int eNo = 0; eNo < E_MAX; eNo++ ){
				//ターゲットを更新.
				enemyData[typeNo][ptnNo][eNo].target.x = playerData.chara.x;
				enemyData[typeNo][ptnNo][eNo].target.y = playerData.chara.y;
			}
		}
	}
}