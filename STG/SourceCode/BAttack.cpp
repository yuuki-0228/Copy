#include "Main.h"
#include "Boss.h"
#include "CSoundManager.h"

//パンチ攻撃フラグ.
void PunchFlag( BOSS* boss, int typeNo, int Dang )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( Dang < 90 ) && ( Dang > -90 ) &&
			( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//目的角度.
			boss->PunchRAng[typeNo] = RAD( Dang );

			//初期動作設定.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 25;

			//攻撃状態をパンチの初期動作状態にする.
			boss->AttackNo[typeNo] = enBossAttackNo::BPunchAng;
		}
	}
}

//同時パンチ攻撃フラグ.
void WPunchFlag( BOSS* boss, int Dang )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( Dang < 90 ) && ( Dang > -90 ) &&
			( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			//目的角度.
			boss->PunchRAng[enBoss::RHand] = RAD( Dang );
			boss->PunchRAng[enBoss::LHand] = RAD( -Dang );

			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {

				//初期動作設定.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 25;

				//攻撃状態をパンチの初期動作状態にする.
				boss->AttackNo[typeNo] = enBossAttackNo::BPunchAng;
			}
		}
	}
}

//初期追尾パンチ攻撃フラグ.
void AtPunchFlag( BOSS* boss, PLAYER player, int typeNo )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//プレイヤーに向ける.
			boss->PunchRAng[typeNo] = RAD( 90 ) - atan2( ( player.chara.y - boss->chara[typeNo].y ), ( boss->chara[typeNo].x - player.chara.x ) );

			//角度が90度より大きくないか調べる.
			if ( ( DO( boss->PunchRAng[typeNo] ) < 90 ) &&
				( DO( boss->PunchRAng[typeNo] ) > -90 ) ) {

				//初期動作設定.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 25;

				//攻撃状態をパンチの初期動作状態にする.
				boss->AttackNo[typeNo] = enBossAttackNo::BPunchAng;
			}
		}
	}
}

//初期追尾同時パンチ攻撃フラグ.
void WAtPunchFlag( BOSS* boss, PLAYER player )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//プレイヤーに向ける.
				boss->PunchRAng[typeNo] = RAD( 90 ) - atan2( ( player.chara.y - boss->chara[typeNo].y ), ( boss->chara[typeNo].x - player.chara.x ) );

				//角度が90度より大きくないか調べる.
				if ( ( DO( boss->PunchRAng[typeNo] ) < 90 ) &&
					( DO( boss->PunchRAng[typeNo] ) > -90 ) ) {

					//初期動作設定.
					boss->animation[typeNo].cnt = 0;
					boss->animation[typeNo].cntMax = 25;

					//攻撃状態をパンチの初期動作状態にする.
					boss->AttackNo[typeNo] = enBossAttackNo::BPunchAng;
				}
			}
		}
	}
}

//突き上げ岩攻撃.
void UpRockFlag( BOSS* boss, int typeNo, int numMax, int dirMax, int DAng, int space, int time )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//弾数、方向、弾同士の間隔.
			boss->rockData[typeNo].Num = numMax;
			boss->rockData[typeNo].Dir = dirMax;
			boss->rockData[typeNo].Ang = DAng;
			boss->rockData[typeNo].Spaa = space;
			boss->rockData[typeNo].Time = time;

			//初期動作設定.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 40;

			//攻撃状態をパンチの初期動作状態にする.
			boss->AttackNo[typeNo] = enBossAttackNo::BUpRockUp;
		}
	}
}

//同時突き上げ岩攻撃.
void WUpRockFlag( BOSS* boss, int numMax, int dirMax, int DAng, int space, int time )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//弾数、方向、弾同士の間隔.
				boss->rockData[typeNo].Num = numMax;
				boss->rockData[typeNo].Dir = dirMax;
				boss->rockData[typeNo].Ang = DAng;
				boss->rockData[typeNo].Spaa = space;
				boss->rockData[typeNo].Time = time;

				//初期動作設定.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//攻撃状態をパンチの初期動作状態にする.
				boss->AttackNo[typeNo] = enBossAttackNo::BUpRockUp;
			}
		}
	}
}

//円突き上げ岩攻撃.
void RUpRockFlag( BOSS* boss, int typeNo, int numMax, int dirMax, int space, int time )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//弾数、方向、弾同士の間隔.
			boss->rockData[typeNo].Num = numMax;
			boss->rockData[typeNo].Dir = dirMax;
			boss->rockData[typeNo].Ang = 270 / dirMax;
			boss->rockData[typeNo].Spaa = space;
			boss->rockData[typeNo].Time = time;

			//初期動作設定.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 40;

			//攻撃状態をパンチの初期動作状態にする.
			boss->AttackNo[typeNo] = enBossAttackNo::BUpRockUp;
		}
	}
}

//同時円突き上げ岩攻撃.
void WRUpRockFlag( BOSS* boss, int numMax, int dirMax, int space, int time )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//弾数、方向、弾同士の間隔.
				boss->rockData[typeNo].Num = numMax;
				boss->rockData[typeNo].Dir = dirMax;
				boss->rockData[typeNo].Ang = 270 / dirMax;
				boss->rockData[typeNo].Spaa = space;
				boss->rockData[typeNo].Time = time;

				//初期動作設定.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//攻撃状態をパンチの初期動作状態にする.
				boss->AttackNo[typeNo] = enBossAttackNo::BUpRockUp;
			}
		}
	}
}

//岩出現攻撃.
void RockFlag( BOSS* boss, int typeNo, int numMax, int dirMax, int DAng, int space, int iAng )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//弾数、方向、弾同士の間隔.
			boss->rockData[typeNo].Num = numMax;
			boss->rockData[typeNo].Dir = dirMax;
			boss->rockData[typeNo].Ang = DAng;
			boss->rockData[typeNo].Spaa = space;
			boss->rockData[typeNo].iAng = iAng;

			//初期動作設定.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 40;

			//攻撃状態をパンチの初期動作状態にする.
			boss->AttackNo[typeNo] = enBossAttackNo::BRockUp;
		}
	}
}

//同時岩出現攻撃.
void WRockFlag( BOSS* boss, int numMax, int dirMax, int DAng, int space, int iAng )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		boss->rockData[enBoss::RHand].iAng = 180 - iAng;
		boss->rockData[enBoss::LHand].iAng = iAng;

		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//弾数、方向、弾同士の間隔.
				boss->rockData[typeNo].Num = numMax;
				boss->rockData[typeNo].Dir = dirMax;
				boss->rockData[typeNo].Ang = DAng;
				boss->rockData[typeNo].Spaa = space;

				//初期動作設定.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//攻撃状態をパンチの初期動作状態にする.
				boss->AttackNo[typeNo] = enBossAttackNo::BRockUp;
			}
		}
	}
}

//円岩出現攻撃.
void RRockFlag( BOSS* boss, int typeNo, int numMax, int dirMax, int space, int iAng )
{
	if ( boss->chara[typeNo].State != enCharaState::Dead ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( typeNo != enBoss::Main ) && ( boss->AttackNo[typeNo] == enBossAttackNo::BWait ) ) {
			//弾数、方向、弾同士の間隔.
			boss->rockData[typeNo].Num = numMax;
			boss->rockData[typeNo].Dir = dirMax;
			boss->rockData[typeNo].Ang = 270 / dirMax;
			boss->rockData[typeNo].Spaa = space;
			boss->rockData[typeNo].iAng = iAng;

			//初期動作設定.
			boss->animation[typeNo].cnt = 0;
			boss->animation[typeNo].cntMax = 40;

			//攻撃状態をパンチの初期動作状態にする.
			boss->AttackNo[typeNo] = enBossAttackNo::BRockUp;
		}
	}
}

//同時円岩出現攻撃.
void WRRockFlag( BOSS* boss, int numMax, int dirMax, int space, int iAng )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		boss->rockData[enBoss::RHand].iAng = 180 - iAng;
		boss->rockData[enBoss::LHand].iAng = iAng;

		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				//弾数、方向、弾同士の間隔.
				boss->rockData[typeNo].Num = numMax;
				boss->rockData[typeNo].Dir = dirMax;
				boss->rockData[typeNo].Ang = 270 / dirMax;
				boss->rockData[typeNo].Spaa = space;

				//初期動作設定.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//攻撃状態をパンチの初期動作状態にする.
				boss->AttackNo[typeNo] = enBossAttackNo::BRockUp;
			}
		}
	}
}

//ボス本体ショット攻撃.
void BShotFlag( BOSS* boss, int dirMax, int DAng, int sSpd, int iAng )
{
	bool breakFlag = false;	//breakフラグ.
	int AngCnt = 0;			//フラグを立てた方向の弾カウント.

	//入力値でステータスを変更.
	boss->status[enBoss::Main].dir = dirMax;
	boss->chara[enBoss::Main].ShotDirAng = DAng;
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		for ( int dirNo = 0; dirNo < boss->BS_DIR; dirNo++ ) {
			boss->shot[numNo][dirNo].sspd = sSpd;
		}
	}

	//飛ぶ角度を調べる.
	double ang = iAng;
	ang -= boss->chara[enBoss::Main].ShotDirAng * ( boss->status[enBoss::Main].dir - 1 ) / 2;
	//ショットSEを鳴らす.
	CSoundManager::PlaySE( CSoundManager::SE_ESNorm );

	//弾数の数だけループ.
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		if ( boss->NumShotFlag[numNo] == false ) {
			//方向の数だけループ.
			for ( int dirNo = 0; dirNo < boss->status[enBoss::Main].dir; dirNo++ ) {
				if ( boss->shot[numNo][dirNo].DispShotFlag == false ) {
					boss->shot[numNo][dirNo].x = ( boss->chara[enBoss::Main].x + C_SIZE - S_SIZE / 2 );
					boss->shot[numNo][dirNo].y = ( boss->chara[enBoss::Main].y + C_SIZE - S_SIZE );
					boss->shot[numNo][dirNo].ang = RAD( ang + boss->chara[enBoss::Main].ShotDirAng * AngCnt );	//角度を調べる.
					boss->shot[numNo][dirNo].DispShotFlag = true;												//発射フラグを立てる.
					AngCnt++;			//フラグを立てた方向の弾カウント.

					breakFlag = true;	//breakフラグを立てる.
				}
			}
		}
		if ( AngCnt == boss->status[enBoss::Main].dir ) {
			boss->NumShotFlag[numNo] = true;
		}
		if ( breakFlag == true ) {
			AngCnt = 0;					//フラグを立てた方向の弾カウントを初期化.

			break;						//一発撃ったらfor文を抜ける.
		}
	}
}

//ボス本体円ショット攻撃.
void RBShotFlag( BOSS* boss, int dirMax, int sSpd, int iAng )
{
	bool breakFlag = false;	//breakフラグ.
	int AngCnt = 0;			//フラグを立てた方向の弾カウント.

	//入力値でステータスを変更.
	boss->status[enBoss::Main].dir = dirMax;
	boss->chara[enBoss::Main].ShotDirAng = 270 / dirMax;
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		for ( int dirNo = 0; dirNo < boss->BS_DIR; dirNo++ ) {
			boss->shot[numNo][dirNo].sspd = sSpd;
		}
	}

	//飛ぶ角度を調べる.
	double ang = iAng;
	ang -= boss->chara[enBoss::Main].ShotDirAng * ( boss->status[enBoss::Main].dir - 1 ) / 2;

	//弾数の数だけループ.
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		if ( boss->NumShotFlag[numNo] == false ) {
			//方向の数だけループ.
			for ( int dirNo = 0; dirNo < boss->status[enBoss::Main].dir; dirNo++ ) {
				if ( boss->shot[numNo][dirNo].DispShotFlag == false ) {
					boss->shot[numNo][dirNo].x = ( boss->chara[enBoss::Main].x + C_SIZE - S_SIZE / 2 );
					boss->shot[numNo][dirNo].y = ( boss->chara[enBoss::Main].y + C_SIZE - S_SIZE );
					boss->shot[numNo][dirNo].ang = RAD( ang + boss->chara[enBoss::Main].ShotDirAng * AngCnt );	//角度を調べる.
					boss->shot[numNo][dirNo].DispShotFlag = true;												//発射フラグを立てる.
					AngCnt++;			//フラグを立てた方向の弾カウント.

					breakFlag = true;	//breakフラグを立てる.
				}
			}
		}
		if ( AngCnt == boss->status[enBoss::Main].dir ) {
			boss->NumShotFlag[numNo] = true;
		}
		if ( breakFlag == true ) {
			AngCnt = 0;					//フラグを立てた方向の弾カウントを初期化.

			break;						//一発撃ったらfor文を抜ける.
		}
	}
}

//吹き飛ばし攻撃.
void BlowawayFlag( BOSS* boss, PLAYER player )
{
	if ( ( boss->chara[enBoss::RHand].State != enCharaState::Dead ) && ( boss->chara[enBoss::LHand].State != enCharaState::Dead ) ) {
		//手を選択している かつ その手が攻撃中ではない場合.
		if ( ( boss->AttackNo[enBoss::RHand] == enBossAttackNo::BWait ) &&
			 ( boss->AttackNo[enBoss::LHand] == enBossAttackNo::BWait ) ) {
			for ( int typeNo = enBoss::RHand; typeNo < boss->BOSS_TYPE; typeNo++ ) {

				//初期動作設定.
				boss->animation[typeNo].cnt = 0;
				boss->animation[typeNo].cntMax = 40;

				//攻撃状態をパンチの初期動作状態にする.
				boss->AttackNo[typeNo] = enBossAttackNo::BUpHand;
			}
		}
	}
}
