#include "Main.h"
#include "Boss.h"
#include "CSoundManager.h"

//ボスの攻撃動作.
void B_AttackControl( BOSS* boss, int typeNo, SCROLL* scroll, PLAYER* player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX] )
{
	if ( boss->chara[enBoss::Main].State != enCharaState::Dying ) {

		//パンチ動作.
		PunchCon( boss, player, gimmick, enemy, typeNo );

		//突き上げ岩動作.
		UpRockCon( boss, player, gimmick, typeNo );

		//岩出現攻撃.
		RockCon( boss, gimmick, player, typeNo );

		//吹き飛ばし.
		Blowaway( boss, scroll, player, gimmick, enemy, typeNo );

		//ボスの弾攻撃.
		BShotCon( boss );
	}
}

//パンチ動作.
void PunchCon( BOSS* boss, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo )
{
	//パンチ角度合わせ.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunchAng ) {
		if ( boss->PunchRAng[typeNo] > boss->chara[typeNo].ang ) {
			boss->chara[typeNo].ang += RAD( 1 );
			//角度に合わせれたら.
			if ( boss->PunchRAng[typeNo] < boss->chara[typeNo].ang ) {
				boss->chara[typeNo].ang = boss->PunchRAng[typeNo];
			}
		}
		else if ( boss->PunchRAng[typeNo] < boss->chara[typeNo].ang ) {
			boss->chara[typeNo].ang -= RAD( 1 );
			//角度に合わせれたら.
			if ( boss->PunchRAng[typeNo] > boss->chara[typeNo].ang ) {
				boss->chara[typeNo].ang = boss->PunchRAng[typeNo];
			}
		}
		else {
			//初期動作へ.
			boss->AttackNo[typeNo] = enBossAttackNo::BPunchInit;
		}
	}
	//パンチ初期動作.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunchInit ) {
		boss->animation[typeNo].cnt += 1;
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ) {
			//腕を後ろに動かす.
			boss->chara[typeNo].x += cos( RAD( 90 ) + boss->chara[typeNo].ang + RAD( 180 ) ) * 1;
			boss->chara[typeNo].y += sin( RAD( 90 ) + boss->chara[typeNo].ang + RAD( 180 ) ) * 1;
		}
		else {
			//パンチ中へ.
			boss->AttackNo[typeNo] = enBossAttackNo::BPunch;
			boss->animation[typeNo].cnt = 0;
			//パンチSEを鳴らす.
			CSoundManager::PlaySE( CSoundManager::SE_BPANT );
		}
	}
	//パンチ中.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunch ) {
		//角度の方向に飛ばす.
		boss->animation[typeNo].cnt += 1;
		boss->chara[typeNo].x += cos( RAD( 90 ) + boss->chara[typeNo].ang ) * ( 2 + boss->animation[typeNo].cnt / 3 );
		boss->chara[typeNo].y += sin( RAD( 90 ) + boss->chara[typeNo].ang ) * ( 2 + boss->animation[typeNo].cnt / 3 );

		//岩と衝突した.
		for( int gty = 0; gty < G_TYPE; gty++ ){
			for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
				if ( CollisionDetection(
					boss->chara[typeNo].x,  boss->chara[typeNo].y,	C_SIZE,						C_SIZE,
					gimmick[1][gty][gNo].x, gimmick[1][gty][gNo].y, gimmick[1][gty][gNo].wSize, gimmick[1][gty][gNo].hSize ) ) {

					//岩破壊音.
					CSoundManager::PlaySE( CSoundManager::SE_GRockDest );

					//岩をなくす.
					gimmick[1][gty][gNo].State = enGimmickState::GWait;
					gimmick[1][gty][gNo].x = WND_W;
					gimmick[1][gty][gNo].y = WND_H;
				}
			}
		}
		//プレイヤーと衝突した.
		if ( Player->chara.State == enCharaState::Living ) {
			if ( CollisionDetection(
				Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
				boss->chara[typeNo].x, boss->chara[typeNo].y, boss->charaSize[typeNo], boss->charaSize[typeNo] ) ) {

				//ダメージSEを鳴らす.
				CSoundManager::PlaySE( CSoundManager::SE_PDamage );

				//HPを減らす.
				Player->status.hp -= boss->status[enBoss::Main].atk * 2;
				if ( Player->status.hp <= 0 ) {
					Player->chara.State			= enCharaState::Dying;	//状態を死亡中にする.
					Player->chara.ExpAnimCnt	= 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
					Player->DeadNo				= enDeedNo::Golem;		//ボスで死亡.
				}
				else {
					//状態を無敵にする.
					Player->chara.State = enCharaState::Invi;
					Player->chara.inviTime = PINVITIME;
				}
			}
		}
		//敵と衝突した.
		for ( int type = 0; type < E_TYPE; type++ ){
			for ( int ptn = 0; ptn < E_PTN; ptn++ ){
				for ( int eNo = 0; eNo < E_MAX; eNo++ ){
					if ( enemy[type][ptn][eNo].chara.State == enCharaState::Dead ){
						if ( CollisionDetection(
							enemy[type][ptn][eNo].chara.x,	enemy[type][ptn][eNo].chara.y,	C_SIZE,						C_SIZE,
							boss->chara[typeNo].x,			boss->chara[typeNo].y,			boss->charaSize[typeNo],	boss->charaSize[typeNo] ) )
						{
							enemy[type][ptn][eNo].chara.x		= WND_W;
							enemy[type][ptn][eNo].chara.y		= WND_H;				//画面の外(上).
							enemy[type][ptn][eNo].chara.State	= enCharaState::Wait;	//待機.
							enemy[type][ptn][eNo].appFlag		= false;

						}
					}
					else if ( enemy[type][ptn][eNo].chara.State == enCharaState::Living ){
						if ( CollisionDetection(
							enemy[type][ptn][eNo].chara.x,	enemy[type][ptn][eNo].chara.y,	C_SIZE,						C_SIZE,
							boss->chara[typeNo].x,			boss->chara[typeNo].y,			boss->charaSize[typeNo],	boss->charaSize[typeNo] ) )
						{
							//敵のダメージSEを鳴らす.
							CSoundManager::PlaySE( CSoundManager::SE_EDamage );

							//HPを減らす.
							enemy[typeNo][ptn][eNo].status.hp -= boss->status[enBoss::Main].atk * 2;
							if ( enemy[typeNo][ptn][eNo].status.hp <= 0 ){
								enemy[typeNo][ptn][eNo].chara.State			= enCharaState::Dying;	//状態を死亡中にする.
								enemy[typeNo][ptn][eNo].chara.ExpAnimCnt	= 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
								enemy[typeNo][ptn][eNo].chara.chgCnt		= 0;
							}
							else{
								//状態を無敵にする.
								enemy[typeNo][ptn][eNo].chara.State		= enCharaState::Invi;
								enemy[typeNo][ptn][eNo].chara.inviTime	= INVITIME;
							}
						}
					}
				}
			}
		}

		//画面の端に触れた.
		if ( ( boss->chara[typeNo].y < 0 ) || ( boss->chara[typeNo].y > GWND_H - C_SIZE ) ||
			 ( boss->chara[typeNo].x < 0 ) || ( boss->chara[typeNo].x > GWND_W - C_SIZE ) ) {
			//パンチ激突音.
			CSoundManager::PlaySE( CSoundManager::SE_BPANTCrush );
			//手の回収へ.
			boss->AttackNo[typeNo] = enBossAttackNo::BPunchReturn;
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 50;	//着弾時の待機時間.
		}
	}
	//手回収.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunchReturn ) {
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ) {
			//着弾時の待機.
			boss->animation[typeNo].cnt += 1;
		}
		else {
			boss->chara[typeNo].x += cos( RAD( 90 ) + boss->chara[typeNo].ang + RAD( 180 ) ) * 3;
			boss->chara[typeNo].y += sin( RAD( 90 ) + boss->chara[typeNo].ang + RAD( 180 ) ) * 3;
			if ( boss->chara[typeNo].y < boss->initial_xy[typeNo].y ) {

				//初期座標にする.
				boss->chara[typeNo].x = boss->initial_xy[typeNo].x;
				boss->chara[typeNo].y = boss->initial_xy[typeNo].y;

				//手の角度を戻すへ.
				boss->AttackNo[typeNo] = enBossAttackNo::BPunchEnd;
			}

			//プレイヤーと衝突した.
			if ( Player->chara.State == enCharaState::Living ) {
				if ( CollisionDetection(
					Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
					boss->chara[typeNo].x, boss->chara[typeNo].y, boss->charaSize[typeNo], boss->charaSize[typeNo] ) ) {
					//ダメージSEを鳴らす.
					CSoundManager::PlaySE( CSoundManager::SE_PDamage );

					//HPを減らす.
					Player->status.hp -= 1;
					if ( Player->status.hp <= 0 ) {
						Player->chara.State			= enCharaState::Dying;	//状態を死亡中にする.
						Player->chara.ExpAnimCnt	= 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
						Player->DeadNo				= enDeedNo::Golem;		//ボスで死亡.
					}
					else {
						//状態を無敵にする.
						Player->chara.State = enCharaState::Invi;
						Player->chara.inviTime = PINVITIME;
					}
				}
			}
		}
	}
	//角度を戻す.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BPunchEnd ) {
		if ( 0 > boss->chara[typeNo].ang ) {
			boss->chara[typeNo].ang += RAD( 1 );
			//角度に合わせれたら.
			if ( 0 < boss->chara[typeNo].ang ) {
				boss->chara[typeNo].ang = 0;
			}
		}
		else if ( 0 < boss->chara[typeNo].ang ) {
			boss->chara[typeNo].ang -= RAD( 1 );
			//角度に合わせれたら.
			if ( 0 > boss->chara[typeNo].ang ) {
				boss->chara[typeNo].ang = 0;
			}
		}
		else {
			//パンチ終了.
			boss->AttackNo[typeNo] = enBossAttackNo::BWait;
			//アニメーション初期化.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 5;
		}
	}
}

//突き上げ岩攻撃.
void UpRockCon( BOSS* boss, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int typeNo )
{
	//手を挙げる.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpRockUp ){
		boss->animation[typeNo].cnt += 1;
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ){
			boss->chara[typeNo].y -= 1;
		}
		else{
			//手を下ろす.
			boss->AttackNo[typeNo] = enBossAttackNo::BUpRockDown;
			boss->animation[typeNo].cnt = 0;
		}
	}
	//手を下ろす.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpRockDown ){
		boss->chara[typeNo].y += 5;
		if ( boss->chara[typeNo].y > boss->initial_xy[typeNo].y ){
			//座標を揃える.
			boss->animation[typeNo].No = 0;
			boss->animation[typeNo].cnt = 0;
			boss->chara[typeNo].y = boss->initial_xy[typeNo].y;
			boss->AttackNo[typeNo] = enBossAttackNo::BUpRock;
		}
		//岩叩きと突き上げSEを鳴らす.
		CSoundManager::PlaySE( CSoundManager::SE_BROCKCrush );
		CSoundManager::PlaySE( CSoundManager::SE_BROCKUP );
	}
	//岩を出していく.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpRock ){
		if ( boss->animation[typeNo].No < boss->rockData[typeNo].Num ){
			double ang = 90;
			ang -= boss->rockData[typeNo].Ang * ( boss->rockData[typeNo].Dir - 1 ) / 2;

			for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].Rang = RAD( ang + boss->rockData[typeNo].Ang * dirNo );	//角度を調べる.
				//座標を揃えフラグを立てる.
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].x = boss->chara[typeNo].x + boss->charaSize[typeNo] / 2 - S_SIZE;
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].y = boss->chara[typeNo].y + boss->charaSize[typeNo];
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].DispFlag = true;
				boss->rock[typeNo][boss->animation[typeNo].No][dirNo].cnt = 5;
			}


			//手から動いた場所に移動.
			for ( int numNo = 0; numNo < boss->animation[typeNo].No + 1; numNo++ ){
				for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
					//岩を移動.
					boss->rock[typeNo][numNo][dirNo].x -= cos( boss->rock[typeNo][boss->animation[typeNo].No][dirNo].Rang ) * ( S_SIZE * boss->rockData[typeNo].Spaa );
					boss->rock[typeNo][numNo][dirNo].y += sin( boss->rock[typeNo][boss->animation[typeNo].No][dirNo].Rang ) * ( S_SIZE * boss->rockData[typeNo].Spaa );
					boss->rock[typeNo][numNo][dirNo].cnt = 10;
				}
			}
			boss->animation[typeNo].No += 1;
		}
		else{
			//待機時間.
			boss->animation[typeNo].cnt += 1;
			if ( boss->animation[typeNo].cnt > 50 ){
				for ( int numNo = 0; numNo < boss->animation[typeNo].No; numNo++ ){
					for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
						if ( boss->rock[typeNo][numNo][dirNo].cnt < 50 ){
							//岩を出していく.
							boss->rock[typeNo][numNo][dirNo].cnt += 2;
							boss->rock[typeNo][numNo][dirNo].y -= C_SIZE / 40;

							//岩(ギミック)と衝突した.
							for ( int gty = 0; gty < G_TYPE; gty++ ) {
								for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
									if ( CollisionDetection(
										boss->rock[typeNo][numNo][dirNo].x, boss->rock[typeNo][numNo][dirNo].y,	S_SIZE * 2,					( S_SIZE / 2 ) * ( boss->rock[typeNo][numNo][dirNo].cnt / 10 + 1 ),
										gimmick[1][gty][gNo].x,				gimmick[1][gty][gNo].y,				gimmick[1][gty][gNo].wSize, gimmick[1][gty][gNo].hSize ) ) {

										//岩をなくす.
										gimmick[1][gty][gNo].State = enGimmickState::GWait;
										gimmick[1][gty][gNo].x = WND_W;
										gimmick[1][gty][gNo].y = WND_H;
									}
								}
							}
						}
						//プレイヤーと衝突した.
						if ( Player->chara.State == enCharaState::Living ) {
							if ( CollisionDetection(
								Player->chara.x + PH_CORR,			Player->chara.y + PH_CORR,			PH_SIZE,		PH_SIZE,
								boss->rock[typeNo][numNo][dirNo].x, boss->rock[typeNo][numNo][dirNo].y, S_SIZE * 2,		( S_SIZE / 2 ) * ( boss->rock[typeNo][numNo][dirNo].cnt / 10 + 1 ) ) ) {

								//ダメージSEを鳴らす.
								CSoundManager::PlaySE( CSoundManager::SE_PDamage );

								//HPを減らす.
								Player->status.hp -= 1;
								if ( Player->status.hp <= 0 ) {
									Player->chara.State			= enCharaState::Dying;	//状態を死亡中にする.
									Player->chara.ExpAnimCnt	= 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
									Player->DeadNo				= enDeedNo::Golem;		//ボスで死亡.
								}
								else {
									//状態を無敵にする.
									Player->chara.State = enCharaState::Invi;
									Player->chara.inviTime = PINVITIME;
								}
							}
						}
					}
				}
			}
			if ( boss->animation[typeNo].cnt > 50 + boss->rockData[typeNo].Time ){
				//岩を無くしていくへ.
				boss->AttackNo[typeNo] = enBossAttackNo::BUpRockEnd;
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].No = 0;
			}
		}
	}
	//岩を無くしていく.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpRockEnd ){
		if ( boss->animation[typeNo].No < boss->rockData[typeNo].Num ){
			boss->animation[typeNo].No += 1;
			for ( int numNo = 0; numNo < boss->animation[typeNo].No; numNo++ ){
				for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
					if ( boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].cnt > 0 ){
						//岩を埋めていく.
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
			//待機時間.
			boss->animation[typeNo].cnt += 1;
			for ( int numNo = 0; numNo < boss->animation[typeNo].No; numNo++ ){
				for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ){
					if ( boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].cnt > 0 ){
						//岩を埋めていく(上の続き).
						boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].cnt -= 2;
						boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].y += C_SIZE / 40;
					}
					else{
						boss->rock[typeNo][boss->rockData[typeNo].Num - 1 - numNo][dirNo].DispFlag = false;
					}
				}
			}
			if ( boss->animation[typeNo].cnt > 50 ){
				//攻撃終了.
				boss->AttackNo[typeNo] = enBossAttackNo::BWait;
			}
		}
	}
}

//岩出現攻撃.
void RockCon( BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], PLAYER* Player, int typeNo )
{

	//手を挙げる.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BRockUp ) {
		boss->animation[typeNo].cnt += 1;
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ) {
			boss->chara[typeNo].y -= 1;
		}
		else {
			//手を下ろす.
			boss->AttackNo[typeNo] = enBossAttackNo::BRockDown;
			boss->animation[typeNo].cnt = 0;
		}
	}
	//手を下ろす.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BRockDown ) {
		boss->chara[typeNo].y += 5;
		if ( boss->chara[typeNo].y > boss->initial_xy[typeNo].y ) {
			//座標を揃える.
			boss->animation[typeNo].No = 0;
			boss->animation[typeNo].cnt = 0;
			boss->chara[typeNo].y = boss->initial_xy[typeNo].y;
			boss->AttackNo[typeNo] = enBossAttackNo::BRock;
			boss->animation[typeNo].Flag = false;
			//地面叩きSEを鳴らす.
			CSoundManager::PlaySE( CSoundManager::SE_BROCKCrush );
		}
	}
	//岩を出していく.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BRock ) {
		if ( boss->animation[typeNo].No < boss->rockData[typeNo].Num ) {
			bool BreakFlag = false;

			double ang = 90;
			ang -= boss->rockData[typeNo].Ang * ( boss->rockData[typeNo].Dir - 1 ) / 2;

			for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ) {
				for ( int gty = 0; gty < G_TYPE; gty++ ) {
					for ( int gno = 0; gno < G_MAX; gno++ ) {
						if ( gimmick[1][gty][gno].State == enGimmickState::GWait ) {
							boss->RockRAngData[boss->animation[typeNo].No][dirNo] = RAD( ang + boss->rockData[typeNo].Ang * dirNo );	//角度を調べる.
							//座標を揃えフラグを立てる.
							gimmick[1][gty][gno].x			= boss->chara[typeNo].x + boss->charaSize[typeNo] / 2 - S_SIZE;
							gimmick[1][gty][gno].y			= boss->chara[typeNo].y + boss->charaSize[typeNo];
							gimmick[1][gty][gno].State		= enGimmickState::GLiving;
							gimmick[1][gty][gno].RockFlag	= true;

							//表示した gNo を保存.
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
			//手から動いた場所に移動.
			for ( int numNo = 0; numNo < boss->animation[typeNo].No + 1; numNo++ ) {
				for ( int dirNo = 0; dirNo < boss->rockData[typeNo].Dir; dirNo++ ) {
					for ( int gty = 0; gty < G_TYPE; gty++ ) {
						if ( gimmick[1][gty][boss->RockDispNoData[boss->animation[typeNo].No][dirNo]].State == enGimmickState::GLiving ) {
							//岩を移動.
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

					//岩を出す.
					for ( int gty = 0; gty < G_TYPE; gty++ ) {
						for ( int gno = 0; gno < G_MAX; gno++ ) {
							gimmick[1][gty][gno].RockFlag = false;
						}
					}

					//岩突き上げSEを鳴らす.
					CSoundManager::PlaySE( CSoundManager::SE_BROCK );

					//表示した gno を初期化.
					for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
						for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
							boss->RockDispNoData[numNo][dirNo] = 0;
						}
					}

					//プレイヤーとぶつかった.
					for ( int gty = 0; gty < G_TYPE; gty++ ) {
						for ( int gno = 0; gno < G_MAX; gno++ ) {
							if ( CollisionDetection(
								Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
								gimmick[1][gty][gno].x, gimmick[1][gty][gno].y, gimmick[1][gty][gno].wSize, gimmick[1][gty][gno].hSize ) )
							{
								//岩の破壊SEを鳴らす.
								CSoundManager::PlaySE( CSoundManager::SE_GRockDest );

								gimmick[1][gty][gno].State = enGimmickState::GWait;
								gimmick[1][gty][gno].x = WND_W;
								gimmick[1][gty][gno].y = WND_H;

								//ダメージSEを鳴らす.
								CSoundManager::PlaySE( CSoundManager::SE_PDamage );

								//HPを減らす.
								Player->status.hp -= 1;
								if ( Player->status.hp <= 0 ) {
									Player->chara.State = enCharaState::Dying;	//状態を死亡中にする.
									Player->chara.ExpAnimCnt = 0;				//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
									Player->DeadNo = enDeedNo::Golem;			//ボスで死亡.
								}
								else {
									//状態を無敵にする.
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

//吹き飛ばし攻撃.
void Blowaway( BOSS* boss, SCROLL* scroll, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo )
{
	//手を挙げる.
	if ( boss->AttackNo[typeNo] == enBossAttackNo::BUpHand ) {
		boss->animation[typeNo].cnt += 1;
		if ( boss->animation[typeNo].cnt < boss->animation[typeNo].cntMax ) {
			boss->chara[typeNo].y -= 1;
		}
		else {
			//手を下ろす.
			boss->AttackNo[typeNo]		= enBossAttackNo::BDownHand;
			boss->animation[typeNo].cnt = 0;

			CSoundManager::PlaySE( CSoundManager::SE_BollA );

			//吹き飛ばしエフェクトの表示.
			boss->BlowAway.y		= ( int ) ( boss->chara[enBoss::LHand].y );
			boss->BlowAway.DispFlag = true;
		}
	}
	//手を下ろす.
	else if ( boss->AttackNo[typeNo] == enBossAttackNo::BDownHand ) {
		boss->chara[typeNo].y += 5;
		if ( boss->chara[typeNo].y > boss->initial_xy[typeNo].y ) {
			//座標を揃える.
			boss->animation[typeNo].No = 0;
			boss->animation[typeNo].cnt = 0;
			boss->chara[typeNo].y = boss->initial_xy[typeNo].y;
			if ( Player->chara.y < WND_H - C_SIZE * 2 ) {
				//プレイヤーの移動.
				Player->chara.y += 10;

				//ステージギミック[岩]にぶつかった.
				for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
					for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
						if ( CollisionDetection(
							Player->chara.x + PH_CORR,	Player->chara.y + PH_CORR,	PH_SIZE,						PH_SIZE,
							gimmick[1][ptnNo][gNo].x,	gimmick[1][ptnNo][gNo].y,	gimmick[1][ptnNo][gNo].wSize,	gimmick[1][ptnNo][gNo].hSize ) )
						{
							//ぶつかった岩の上の座標に揃えダメージ.
							Player->chara.y = gimmick[1][ptnNo][gNo].y - C_SIZE + PH_CORR;

							//ダメージSEを鳴らす.
							CSoundManager::PlaySE( CSoundManager::SE_PDamage );

							//HPを減らす.
							Player->status.hp -= 1;
							if ( Player->status.hp <= 0 ) {
								Player->chara.State = enCharaState::Dying;	//状態を死亡中にする.
								Player->chara.ExpAnimCnt = 0;				//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
								Player->DeadNo = enDeedNo::Golem;			//ボスで死亡.
							}
							else {
								//状態を無敵にする.
								Player->chara.State = enCharaState::Invi;
								Player->chara.inviTime = PINVITIME;
							}

							//強制終了.
							boss->Blowcnt = 0;
							boss->AttackNo[typeNo] = enBossAttackNo::BWait;

							//飛ばす手をランダムで決める.
							int Hand = rand() % 2 + 1;
							AtPunchFlag( boss, *Player, Hand );
						}
					}
				}

				//死亡している敵.
				for ( int type = 0; type < E_TYPE; type++ ){
					for ( int ptn = 0; ptn < E_PTN; ptn++ ){
						for ( int eNo = 0; eNo < E_MAX; eNo++ ){
							if( !( ( type == 4 ) && ( enemy[type][ptn][eNo].chara.State != enCharaState::Dead ) ) ){
								if ( ( enemy[type][ptn][eNo].chara.State == enCharaState::Living ) || ( enemy[type][ptn][eNo].chara.State == enCharaState::Dead ) ) {
									//初期に接触している岩を調べる.
									bool HitRock = false;	//初期に接触している.
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
										//移動.
										enemy[type][ptn][eNo].chara.y += 10;

										//ステージギミック[岩]にぶつかった.
										for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
											for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
												if ( CollisionDetection(
													enemy[type][ptn][eNo].chara.x + PH_CORR, enemy[type][ptn][eNo].chara.y + PH_CORR, PH_SIZE, PH_SIZE,
													gimmick[1][ptnNo][gNo].x, gimmick[1][ptnNo][gNo].y, gimmick[1][ptnNo][gNo].wSize, gimmick[1][ptnNo][gNo].hSize ) )
												{
													enemy[type][ptn][eNo].chara.y = gimmick[1][ptnNo][gNo].y - C_SIZE + PH_CORR;

													if ( enemy[type][ptn][eNo].chara.State == enCharaState::Living ) {
														//敵のダメージSEを鳴らす.
														CSoundManager::PlaySE( CSoundManager::SE_EDamage );

														//HPを減らす.
														enemy[typeNo][ptn][eNo].status.hp -= 1;
														if ( enemy[typeNo][ptn][eNo].status.hp <= 0 ) {
															enemy[typeNo][ptn][eNo].chara.State = enCharaState::Dying;	//状態を死亡中にする.
															enemy[typeNo][ptn][eNo].chara.ExpAnimCnt = 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
															enemy[typeNo][ptn][eNo].chara.chgCnt = 0;
														}
														else {
															//状態を無敵にする.
															enemy[typeNo][ptn][eNo].chara.State = enCharaState::Invi;
															enemy[typeNo][ptn][eNo].chara.inviTime = INVITIME;
														}

													}
												}
											}
										}

										//画面外にでた.
										if ( enemy[type][ptn][eNo].chara.y > GWND_H ) {
											enemy[type][ptn][eNo].chara.x = WND_W;
											enemy[type][ptn][eNo].chara.y = WND_H;				//画面の外(上).
											enemy[type][ptn][eNo].chara.State = enCharaState::Wait;	//待機.
											enemy[type][ptn][eNo].appFlag = false;
											enemy[type][ptn][eNo].BossFlag = false;
										}
									}
								}
							}
						}
					}
				}

				//画面揺れ.
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
				
				//飛ばす手をランダムで決める.
				int Hand = rand() % 2 + 1;
				AtPunchFlag( boss, *Player, Hand );
			}
		}
	}
}

//ボスの弾攻撃.
void BShotCon( BOSS* boss )
{
	int NumShotCnt = 0;	//方向+弾が画面の外へ移動した数.

	//弾数の数だけループ.
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		//方向の数だけループ.
		for ( int dirNo = 0; dirNo < boss->BS_DIR; dirNo++ ) {
			//弾が発射中なら.
			if ( boss->NumShotFlag[numNo] == true ) {
				if ( boss->shot[numNo][dirNo].DispShotFlag == true ) {

					//弾の移動.
					boss->shot[numNo][dirNo].x -= cos( boss->shot[numNo][dirNo].ang ) * boss->shot[numNo][dirNo].sspd;
					boss->shot[numNo][dirNo].y += sin( boss->shot[numNo][dirNo].ang ) * boss->shot[numNo][dirNo].sspd;

					//画面の外へ移動した.
					if ( ( boss->shot[numNo][dirNo].y < -S_SIZE ) || ( boss->shot[numNo][dirNo].y > GWND_H ) ||
						( boss->shot[numNo][dirNo].x < -S_SIZE ) || ( boss->shot[numNo][dirNo].x > GWND_W ) ) {

						boss->shot[numNo][dirNo].DispShotFlag = false;
						boss->shot[numNo][dirNo].x = WND_W;
						boss->shot[numNo][dirNo].y = WND_H;
					}
				}
			}
		}
		//方向+弾全て画面の外へ移動した.
		for ( int dirNo = 0; dirNo < boss->BS_DIR; dirNo++ ) {
			if ( boss->shot[numNo][dirNo].DispShotFlag == false ) {
				NumShotCnt++;
			}
		}
		if ( NumShotCnt == boss->BS_DIR ) {
			boss->NumShotFlag[numNo] = false;	//発射フラグを降ろす.
		}

		NumShotCnt = 0;	//画面の外へ移動した数を初期化.
	}
}