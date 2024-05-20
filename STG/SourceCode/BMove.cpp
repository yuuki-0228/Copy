#include "Main.h"
#include "Boss.h"
#include "CSoundManager.h"

void B_AttackMove( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX] )
{
	switch ( boss->AttackMove )
	{
	case enBossMoveNo::EarlyForm1:
		EarlyForm( boss, player, gimmick );
		break;
	case enBossMoveNo::MidForm2:
		MidForm( boss, player, gimmick );
		break;
	case enBossMoveNo::LateForm3:
		LateForm( boss, player, gimmick );
		break;
	default:
		break;
	}

	//攻撃.
	Attacking( boss );

	//張り付き防止.
	Stick( boss, player );

	//敵の表示.
	BEDisp( boss, enemy );
}

//形態1.
void EarlyForm( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX] )
{
	if( boss->AttackCnt == 100 ) {
		WAtPunchFlag( boss, player );
	}

	//本体.
	if( boss->AttackCnt == 200 || boss->AttackCnt == 500 ) {
		BShotFlag( boss, 5, 30, 2 );
	}
	if( boss->AttackCnt == 350 || boss->AttackCnt == 650 ) {
		BShotFlag( boss, 4, 20, 2 );
	}


	//右手.
	if( boss->AttackCnt == 550 ) {
		if( boss->PatternCnt % 2 == 0 ) {
			RockFlag( boss, enBoss::RHand, 4, 2 );
		}
		else {
			AtPunchFlag( boss, player, enBoss::RHand );
		}
	}
	if( boss->PatternCnt % 2 == 0 ) {
		if( boss->AttackCnt == 800 ) {
			UpRockFlag( boss, enBoss::RHand, 6, 2, 25 );
		}
	}

	//左手.
	if( boss->AttackCnt == 550 ) {
		if( boss->PatternCnt % 2 == 0 ) {
			AtPunchFlag( boss, player, enBoss::LHand );
		}
		else {
			RockFlag( boss, enBoss::LHand, 4, 2 );
		}
	}
	if( boss->PatternCnt % 2 != 0 ) {
		if( boss->AttackCnt == 800 ) {
			UpRockFlag( boss, enBoss::LHand, 7, 2, 25, 4 );
		}
	}

	//振り出しｶｳﾝﾄ.
	if( boss->AttackCnt == 1000 ) {
		BShotFlag( boss, 5, 25, 2 );
		boss->AttackCnt = 0;
		boss->PatternCnt++;
		if( boss->PatternCnt == 3 ) {
			boss->PatternCnt = 1;
		}
	}
}

//形態2.
void MidForm( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX] )
{
	//本体.
	if( ( boss->AttackCnt == 50 ) || ( boss->AttackCnt == 70 ) || ( boss->AttackCnt == 250 ) || ( boss->AttackCnt == 400 ) )
	{
		BShotFlag( boss, 5, 30, 2 );
	}
	if( ( boss->AttackCnt == 60 ) || ( boss->AttackCnt == 200 || ( boss->AttackCnt == 550 ) ) )
	{
		BShotFlag( boss, 4, 30, 2 );
	}

	//両手
	if( boss->AttackCnt == 150 ) {
		if( boss->PatternCnt % 2 == 0 ) {
			WRockFlag( boss, 4, 2, 30, 7 );
		}
		else {
			WUpRockFlag( boss, 6, 2, 30, 5 );
		}
	}
	//右手.
	if( boss->AttackCnt == 650 ) {
		if( boss->PatternCnt % 2 == 0 ) {
			AtPunchFlag( boss, player, enBoss::RHand );
		}
		else {
			RUpRockFlag( boss, enBoss::RHand, 10, 7, 4 );
		}
	}

	//左手.
	if( boss->AttackCnt == 700 ) {
		if( boss->PatternCnt % 2 == 0 ) {
			RUpRockFlag( boss, enBoss::LHand, 10, 9, 4 );
		}
		else {
			AtPunchFlag( boss, player, enBoss::LHand );
		}
	}


	//振り出しｶｳﾝﾄ.
	if( boss->AttackCnt == 1000 ) {
		boss->AttackCnt = 50;
		boss->PatternCnt++;
		if( boss->PatternCnt == 3 ) {
			boss->PatternCnt = 1;
		}
	}

}

//形態3.
void LateForm( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX] )
{
	//本体.
	if( ( boss->AttackCnt >= 100 ) && ( boss->AttackCnt <= 400 ) && ( boss->AttackCnt % 15 == 0 ) ) {
		BShotFlag( boss, 2, 250 - boss->AttackCnt, 2 );
	}
	if( boss->AttackCnt == 500 || boss->AttackCnt == 510 || boss->AttackCnt == 710
		|| boss->AttackCnt == 720 || boss->AttackCnt == 730 || boss->AttackCnt == 890 ) {
		BShotFlag( boss, 7, 30, 2 );
	}
	if( boss->AttackCnt == 505 || boss->AttackCnt == 515 || boss->AttackCnt == 715
		|| boss->AttackCnt == 725 || boss->AttackCnt == 735 || boss->AttackCnt == 900 ) {
		BShotFlag( boss, 6, 25, 2 );
	}


	//右手.
	if( boss->AttackCnt == 200 ) {
		if( boss->PatternCnt % 2 == 0 ) {
			AtPunchFlag( boss, player, enBoss::RHand );
		}
		else {
			RUpRockFlag( boss, enBoss::RHand );
		}
	}

	//左手.
	if( boss->AttackCnt == 250 ) {
		if( boss->PatternCnt % 2 == 0 ) {
			RUpRockFlag( boss, enBoss::LHand, 10, 13, 4 );
		}
		else {
			AtPunchFlag( boss, player, enBoss::LHand );
		}
	}

	//両手.
	if( boss->AttackCnt == 700 ) {
		if( boss->PatternCnt % 2 == 0 ) {
			WUpRockFlag( boss, 4, 9, 15, 8 );
		}
		else {
			WAtPunchFlag( boss, player );
		}
	}
	//振り出しｶｳﾝﾄ.
	if( boss->AttackCnt == 1000 ) {
		boss->AttackCnt = 50;
		boss->PatternCnt++;
		if( boss->PatternCnt == 3 ) {
			boss->PatternCnt = 1;
		}
	}
}

//張り付き防止.
void Stick( BOSS* boss, PLAYER player ) {
	if ( player.chara.y < GWND_H / 2 + C_SIZE ) {
		if ( player.chara.y < GWND_H / 4 + C_SIZE ) {
			boss->Blowcnt += 5;
			if ( boss->Blowcnt >= 300 ) {
				boss->Blowcnt = 300;
				BlowawayFlag( boss, player );
			}
		}
		else {
			boss->Blowcnt++;
			if ( boss->Blowcnt >= 300 ) {
				boss->Blowcnt = 300;
				BlowawayFlag( boss, player );
			}

		}
	}
	else {
		if ( boss->Blowcnt != 0 ) {
			boss->Blowcnt--;
		}
	}
}

//時間による形態変化.
void Attacking( BOSS* boss ) {
	if ( boss->status[enBoss::Main].hp >= 100 || boss->AttackMovecnt < 2000 ) {
		boss->AttackMove = enBossMoveNo::EarlyForm1;
		if ( boss->status[enBoss::Main].hp <= 100 ) {
			boss->AttackMovecnt = 2000;
		}
	}
	if ( boss->status[enBoss::Main].hp >= 50 && boss->status[enBoss::Main].hp < 100 || boss->AttackMovecnt > 2000 && boss->AttackMovecnt < 4000 ) {
		boss->AttackMove = enBossMoveNo::MidForm2;
		if ( boss->status[enBoss::Main].hp <= 50 ) {
			boss->AttackMovecnt = 4000;
		}

	}
	if ( boss->status[enBoss::Main].hp <= 50 || boss->AttackMovecnt > 4000 && boss->AttackMovecnt < 5500 ) {
		boss->AttackMove = enBossMoveNo::LateForm3;
	}

	if ( boss->AttackMovecnt >= 5500 ) {
		//ボスの入退出SEの再生.
		CSoundManager::PlaySE( CSoundManager::SE_BEarth );

		boss->chara[enBoss::Main].State = enCharaState::Dying;
		boss->AttackMovecnt = 0;
		//本体がやられた場合両腕を死亡中にする.
		for ( int i = enBoss::RHand; i < boss->BOSS_TYPE; i++ ) {
			if ( ( boss->chara[i].State == enCharaState::Living ) || ( boss->chara[i].State == enCharaState::Invi ) ) {
				boss->chara[i].State = enCharaState::Dying;		//状態を死亡中にする.
				boss->chara[i].ExpAnimCnt = 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
				boss->chara[i].chgCnt = 0;
			}
		}
	}
}