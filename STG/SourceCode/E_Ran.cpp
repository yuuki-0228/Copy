#include "Main.h"
#include "Enemy.h"

//敵の動作.
void Es_Ran( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int type, int ptn, int eNo )
{
	switch ( Enemy[type][ptn][eNo].ran ) {
	case enEmRanAng::STOP:
		break;
	case enEmRanAng::UP:
		Enemy[type][ptn][eNo].chara.y -= Enemy[type][ptn][eNo].status.cspd;
		break;
	case enEmRanAng::DOWN:
		Enemy[type][ptn][eNo].chara.y += Enemy[type][ptn][eNo].status.cspd;

		//コウモリとハチ以外.
		if ( ( type != 3 ) && ( type != 4 ) ) {
			//ステージギミック[岩]にぶつかった.
			for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					if ( CollisionDetection(
						Enemy[type][ptn][eNo].chara.x + PH_CORR, Enemy[type][ptn][eNo].chara.y + PH_CORR, PH_SIZE, PH_SIZE,
						gimmick[1][ptnNo][gNo].x, gimmick[1][ptnNo][gNo].y, gimmick[1][ptnNo][gNo].wSize, gimmick[1][ptnNo][gNo].hSize ) )
					{
						Enemy[type][ptn][eNo].chara.y = gimmick[1][ptnNo][gNo].y - C_SIZE + PH_CORR;
					}
				}
			}
		}
		break;
	case enEmRanAng::RIGHT:
		Enemy[type][ptn][eNo].chara.x += Enemy[type][ptn][eNo].status.cspd;
		break;
	case enEmRanAng::LEFT:
		Enemy[type][ptn][eNo].chara.x -= Enemy[type][ptn][eNo].status.cspd;
		break;
	default:
		break;
	}
}

//通常の敵の動き.
void E_ran( SCROLL scroll, PLAYER player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int ty, int ptn, int eNo ) {
	if ( ( boss->chara[enBoss::Main].State != enCharaState::Dying ) && ( boss->chara[enBoss::Main].State != enCharaState::Dead ) ){
		if ( ( Enemy[ty][ptn][eNo].chara.State == enCharaState::Living ) || ( Enemy[ty][ptn][eNo].chara.State == enCharaState::Invi ) ) {
			if ( Enemy[ty][ptn][eNo].appFlag == true ) {
				Enemy[ty][ptn][eNo].ran = enEmRanAng::DOWN;
				switch ( ty ) {
				case 0://溜め.
				{
					//ｼｮｯﾄﾌﾗｸﾞ.
					E_ran_right( Enemy, Plant, player, scroll, ty, ptn, eNo );
					break;
				}
				case 1://通常.
				{
					if ( Enemy[ty][ptn][eNo].EshotCnt > 40 ) {
						E_ShotFlag( Enemy, ty, ptn, eNo );
						Enemy[ty][ptn][eNo].EshotCnt = 0;

					}
					break;
				}
				case 2://貫通.
				{
					E_ran_up( Enemy, boss, ty, ptn, eNo );
					break;
				}
				case 3://ビーム.
				{
					E_ran_left( Enemy, boss, ty, ptn, eNo );
					break;
				}
				case 4://追尾.
				{
					E_ran_circle( Enemy, ty, ptn, eNo );
					break;
				}
				default:
					break;
				}
				Es_Ran( Enemy, gimmick, ty, ptn, eNo );

				if ( ( Enemy[ty][ptn][eNo].chara.y < -C_SIZE ) || ( Enemy[ty][ptn][eNo].chara.y > GWND_H ) ||
					( Enemy[ty][ptn][eNo].chara.x < -C_SIZE ) || ( Enemy[ty][ptn][eNo].chara.x > GWND_W ) )
				{
					Enemy[ty][ptn][eNo].chara.x = WND_W;
					Enemy[ty][ptn][eNo].chara.y = WND_H;					//画面の外(上).
					Enemy[ty][ptn][eNo].chara.State = enCharaState::Wait;	//待機.
					Enemy[ty][ptn][eNo].appFlag = false;
				}
			}
		}
		else if ( ( Enemy[ty][ptn][eNo].chara.State == enCharaState::Dead ) || ( Enemy[ty][ptn][eNo].chara.State == enCharaState::Invi ) ) {
			if ( ( ty == 1 ) || ( scroll.stop == false ) ){
				Enemy[ty][ptn][eNo].chara.y += scroll.spd;
				if ( Enemy[ty][ptn][eNo].chara.y > GWND_H ) {
					Enemy[ty][ptn][eNo].chara.x = WND_W;
					Enemy[ty][ptn][eNo].chara.y = WND_H;					//画面の外(上).
					Enemy[ty][ptn][eNo].chara.State = enCharaState::Wait;	//待機.
					Enemy[ty][ptn][eNo].appFlag = false;
				}
			}
		}
	}
}

//溜め動き.
void E_ran_right( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], PLAYER plarye, SCROLL scroll, int ty, int ptn, int eNo )
{
	Enemy[ty][ptn][eNo].ran = enEmRanAng::STOP;
	Enemy[ty][ptn][eNo].chara.y += scroll.spd;

	//プレイヤーとの角度を調べる.
	Enemy[ty][ptn][eNo].chara.ang = atan2( ( Enemy[ty][ptn][eNo].chara.y - plarye.chara.y ), ( plarye.chara.x - Enemy[ty][ptn][eNo].chara.x ) );
	if( ( ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 70 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -70 ) ) ||
		( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 110 ) || ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -110 ) ) ) {

		//寝ている場合起床する.
		if ( Plant[ptn][eNo].State == enPlantState::Sleeping ){
			Plant[ptn][eNo].State			= enPlantState::WokeUp;
			Plant[ptn][eNo].Woke.AnimaX		= 0;
			Plant[ptn][eNo].Woke.AnimaY		= 0;
			Plant[ptn][eNo].Woke.AnimaCnt	= 0;
			Plant[ptn][eNo].Woke.Alpha		= 255;
			Plant[ptn][eNo].Woke.AlphaFlag	= false;
		}

		//画像の変更.
		if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 110 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 155 ) ) {
			//左上.
			Enemy[ty][ptn][eNo].animetion.No = 0;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) > 155 ) || ( DO( Enemy[ty][ptn][eNo].chara.ang ) < -155 ) ) {
			//左.
			Enemy[ty][ptn][eNo].animetion.No = 1;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -155 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -110 ) ) {
			//左下.
			Enemy[ty][ptn][eNo].animetion.No = 2;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 25 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 70 ) ) {
			//右上.
			Enemy[ty][ptn][eNo].animetion.No = 3;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) > -25 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) < 25 ) ) {
			//右.
			Enemy[ty][ptn][eNo].animetion.No = 4;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -70 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -25 ) ) {
			//右下.
			Enemy[ty][ptn][eNo].animetion.No = 5;
		}

		//ショットフラグ.
		E_ShotFlag( Enemy, ty, ptn, eNo, ( RAD( 180 ) + Enemy[ty][ptn][eNo].chara.ang ), true );
	}
	else {
		//睡眠状態にする.
		Plant[ptn][eNo].State				= enPlantState::Sleeping;

		//溜めカウントを減らしていく.
		if( Enemy[ty][ptn][eNo].chara.chgCnt > 0 ) {
			Enemy[ty][ptn][eNo].shot[4][9].x = ( Enemy[ty][ptn][eNo].chara.x + C_SIZE / 2 );
			Enemy[ty][ptn][eNo].shot[4][9].y = ( Enemy[ty][ptn][eNo].chara.y + C_SIZE / 2 );
			Enemy[ty][ptn][eNo].chara.chgCnt -= 1;
		}
	}
}

//貫通動き.
void E_ran_up( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], BOSS* boss, int ty, int ptn, int eNo )
{
	if ( boss->chara[enBoss::Main].State != enCharaState::Wait ) {
		if ( Enemy[ty][ptn][eNo].chara.y > 150 ) {
			Enemy[ty][ptn][eNo].ran = enEmRanAng::STOP;
		}
		if ( Enemy[ty][ptn][eNo].EmRanCnt > 510 ) {
			Enemy[ty][ptn][eNo].ran = enEmRanAng::UP;
		}
	}
	else {
		if ( Enemy[ty][ptn][eNo].chara.y > 100 ) {
			Enemy[ty][ptn][eNo].ran = enEmRanAng::STOP;
		}
		if ( Enemy[ty][ptn][eNo].EmRanCnt > 460 ) {
			Enemy[ty][ptn][eNo].ran = enEmRanAng::UP;
		}
	}
	//ｼｮｯﾄﾌﾗｸﾞ.
	if( Enemy[ty][ptn][eNo].EshotCnt > 25 ) {
		E_ShotFlag( Enemy, ty, ptn, eNo );
		Enemy[ty][ptn][eNo].EshotCnt = 0;
	}
}

//ビーム動き.
void E_ran_left( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], BOSS* boss, int ty, int ptn, int eNo )
{
	if ( boss->chara[enBoss::Main].State != enCharaState::Wait ) {
		Enemy[ty][ptn][eNo].chara.y = C_SIZE * 3;
	}
	else {
		Enemy[ty][ptn][eNo].chara.y = C_SIZE * 2;
	}

	if( Enemy[ty][ptn][eNo].EmRanCnt == 0 ) {
		Enemy[ty][ptn][eNo].chara.x = -C_SIZE;
	}
	Enemy[ty][ptn][eNo].ran = enEmRanAng::RIGHT;

	//ｼｮｯﾄﾌﾗｸﾞ.
	if( Enemy[ty][ptn][eNo].EshotCnt > 25 ) {
		E_ShotFlag( Enemy, ty, ptn, eNo );
		Enemy[ty][ptn][eNo].EshotCnt = 0;
	}
}

//追尾.
void E_ran_circle( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], int ty, int ptn, int eNo )
{
	double E_ang = 0;
	double M_x = Enemy[ty][ptn][eNo].half_rot.M_x, M_y = Enemy[ty][ptn][eNo].half_rot.M_y;
	double I_spd = Enemy[ty][ptn][eNo].chara.x - M_x;

	Enemy[ty][ptn][eNo].ran = enEmRanAng::STOP;
	M_x -= cos( Enemy[ty][ptn][eNo].half_rot.E_height * 3.14 / 180 ) * I_spd;
	M_y += sin( Enemy[ty][ptn][eNo].half_rot.E_height * 3.14 / 180 ) * I_spd;
	E_ang = atan2( ( M_y - Enemy[ty][ptn][eNo].chara.y ), ( Enemy[ty][ptn][eNo].chara.x - M_x ) );
	Enemy[ty][ptn][eNo].chara.y += sin( E_ang ) * 2;
	Enemy[ty][ptn][eNo].chara.x -= cos( E_ang ) * 2;

	//ｼｮｯﾄﾌﾗｸﾞ.
	if( Enemy[ty][ptn][eNo].EshotCnt > 25 ) {
		E_ShotFlag( Enemy, ty, ptn, eNo );
		Enemy[ty][ptn][eNo].EshotCnt = 0;
	}
}

//少し強い敵.
void LBE_ran( SCROLL scroll, PLAYER player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int ty ) {
	if ( ( boss->chara[enBoss::Main].State != enCharaState::Dying ) && ( boss->chara[enBoss::Main].State != enCharaState::Dead ) ){
		if ( ( Enemy[ty][5][0].chara.State == enCharaState::Living ) || ( Enemy[ty][5][0].chara.State == enCharaState::Invi ) ) {
			if ( Enemy[ty][5][0].BossFlag == true ) {
				Enemy[ty][5][0].ran = enEmRanAng::DOWN;
				switch ( ty ) {
				case 1://溜め.
				{
					LBE_charge( Enemy, Plant, player, scroll, ty, 5, 0 );
					break;
				}
				case 2://貫通.
				{
					LBE_penetration( Enemy, ty, 5, 0 );
					break;
				}
				case 3://ビーム.
				{
					LBE_beam( Enemy, ty, 5, 0 );
					break;
				}
				case 4://追尾.
				{
					LBE_tracking( Enemy, ty, 5, 0 );
					break;
				}
				default:
					break;
				}
				Es_Ran( Enemy, gimmick, ty, 5, 0 );
				if ( ( Enemy[ty][5][0].chara.y < -C_SIZE ) || ( Enemy[ty][5][0].chara.y > GWND_H ) ||
					( Enemy[ty][5][0].chara.x < -C_SIZE ) || ( Enemy[ty][5][0].chara.x > GWND_W ) )
				{
					Enemy[ty][5][0].chara.x = 0;
					Enemy[ty][5][0].chara.y = -C_SIZE;					//画面の外(上).
					Enemy[ty][5][0].chara.State = enCharaState::Wait;	//待機.
					Enemy[ty][5][0].BossFlag = false;
				}
			}
		}
		else if ( Enemy[ty][5][0].chara.State == enCharaState::Dead ) {
			if ( scroll.stop == false ){
				Enemy[ty][5][0].chara.y += scroll.spd;
				if ( Enemy[ty][5][0].chara.y > GWND_H ) {
					Enemy[ty][5][0].chara.x = WND_W;
					Enemy[ty][5][0].chara.y = WND_H;					//画面の外(上).
					Enemy[ty][5][0].chara.State = enCharaState::Wait;	//待機.
					Enemy[ty][5][0].appFlag = false;
				}
			}
		}
	}
}

//LB溜め動き.
void LBE_charge( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], PLAYER plarye, SCROLL scroll, int ty, int ptn, int eNo )
{
	Enemy[ty][ptn][eNo].ran = enEmRanAng::STOP;
	Enemy[ty][ptn][eNo].chara.y += scroll.spd;

	//プレイヤーとの角度を調べる.
	Enemy[ty][ptn][eNo].chara.ang = atan2( ( Enemy[ty][ptn][eNo].chara.y - plarye.chara.y ), ( plarye.chara.x - Enemy[ty][ptn][eNo].chara.x ) );
	if( ( ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 70 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -70 ) ) ||
		( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 110 ) || ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -110 ) ) ) {

		//寝ている場合起床する.
		if ( Plant[ptn][eNo].State == enPlantState::Sleeping ){
			Plant[ptn][eNo].State			= enPlantState::WokeUp;
			Plant[ptn][eNo].Woke.AnimaX		= 0;
			Plant[ptn][eNo].Woke.AnimaY		= 0;
			Plant[ptn][eNo].Woke.AnimaCnt	= 0;
			Plant[ptn][eNo].Woke.Alpha		= 255;
			Plant[ptn][eNo].Woke.AlphaFlag	= false;
		}

		//画像の変更.
		if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 110 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 155 ) ) {
			//左上.
			Enemy[ty][ptn][eNo].animetion.No = 0;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) > 155 ) || ( DO( Enemy[ty][ptn][eNo].chara.ang ) < -155 ) ) {
			//左.
			Enemy[ty][ptn][eNo].animetion.No = 1;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -155 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -110 ) ) {
			//左下.
			Enemy[ty][ptn][eNo].animetion.No = 2;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 25 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 70 ) ) {
			//右上.
			Enemy[ty][ptn][eNo].animetion.No = 3;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) > -25 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) < 25 ) ) {
			//右.
			Enemy[ty][ptn][eNo].animetion.No = 4;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -70 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -25 ) ) {
			//右下.
			Enemy[ty][ptn][eNo].animetion.No = 5;
		}

		//ショットフラグ.
		E_ShotFlag( Enemy, ty, 5, 0, ( RAD( 180 ) + Enemy[ty][ptn][eNo].chara.ang ), true );
	}
	else {
		//睡眠状態にする.
		Plant[ptn][eNo].State	= enPlantState::Sleeping;

		//溜めカウントを減らしていく.
		if( Enemy[ty][ptn][eNo].chara.chgCnt > 0 ) {
			Enemy[ty][ptn][eNo].shot[4][9].x = ( Enemy[ty][ptn][eNo].chara.x + C_SIZE / 2 );
			Enemy[ty][ptn][eNo].shot[4][9].y = ( Enemy[ty][ptn][eNo].chara.y + C_SIZE / 2 );
			Enemy[ty][ptn][eNo].chara.chgCnt -= 1;
		}
	}
}

//LB貫通動き.
void LBE_penetration( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], int ty, int ptn, int eNo )
{
	if( Enemy[ty][5][0].EmRanCnt > 100 ) {
		Enemy[ty][5][0].ran = enEmRanAng::LEFT;
	}
	if( Enemy[ty][5][0].EmRanCnt > 300 ) {
		Enemy[ty][5][0].ran = enEmRanAng::DOWN;
	}
	if( Enemy[ty][5][0].EmRanCnt > 400 ) {
		Enemy[ty][5][0].ran = enEmRanAng::RIGHT;
	}
	if( Enemy[ty][5][0].EmRanCnt > 800 ) {
		Enemy[ty][5][0].ran = enEmRanAng::UP;
	}
	if( Enemy[ty][5][0].EmRanCnt > 900 ) {
		Enemy[ty][5][0].ran = enEmRanAng::LEFT;
	}
	if( Enemy[ty][5][0].EmRanCnt > 1100 ) {
		Enemy[ty][5][0].ran = enEmRanAng::UP;
	}
	//ｼｮｯﾄﾌﾗｸﾞ.
	if( Enemy[ty][5][0].EshotCnt > 20 ) {
		E_ShotFlag( Enemy, ty, 5, 0 );
		Enemy[ty][5][0].EshotCnt = 0;
	}
}

//LBビーム動き.
void LBE_beam( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], int ty, int ptn, int eNo )
{
	if( Enemy[ty][5][0].EmRanCnt > 250 ) {
		Enemy[ty][5][0].ran = enEmRanAng::RIGHT;
	}
	if( Enemy[ty][5][0].EmRanCnt > 350 ) {
		Enemy[ty][5][0].ran = enEmRanAng::UP;
	}
	if( Enemy[ty][5][0].EmRanCnt > 450 ) {
		Enemy[ty][5][0].ran = enEmRanAng::RIGHT;
	}
	if( Enemy[ty][5][0].EmRanCnt > 550 ) {
		Enemy[ty][5][0].ran = enEmRanAng::DOWN;
	}
	if( Enemy[ty][5][0].EmRanCnt > 650 ) {
		Enemy[ty][5][0].ran = enEmRanAng::RIGHT;
	}
	if( Enemy[ty][5][0].EmRanCnt > 750 ) {
		Enemy[ty][5][0].ran = enEmRanAng::UP;
	}
	if( Enemy[ty][5][0].EmRanCnt > 850 ) {
		Enemy[ty][5][0].ran = enEmRanAng::RIGHT;
	}
	if( Enemy[ty][5][0].EmRanCnt > 950 ) {
		Enemy[ty][5][0].ran = enEmRanAng::DOWN;
	}
	if( Enemy[ty][5][0].EmRanCnt > 1050 ) {
		Enemy[ty][5][0].ran = enEmRanAng::RIGHT;
	}
	if( Enemy[ty][5][0].EmRanCnt > 1150 ) {
		Enemy[ty][5][0].ran = enEmRanAng::UP;
	}
	//ｼｮｯﾄﾌﾗｸﾞ.
	if( Enemy[ty][5][0].EshotCnt > 20 ) {
		E_ShotFlag( Enemy, ty, 5, 0 );
		Enemy[ty][5][0].EshotCnt = 0;
	}

}

//LB追尾動き.
void LBE_tracking( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], int ty, int ptn, int eNo )
{
	double LBC_x = ( GWND_W / 2 ) - ( C_SIZE / 2 ), LBC_y = C_SIZE * 2;
	double length_x = 248, length_y = 100;
	if( Enemy[ty][5][0].EmMoveCnt != 527 ) {
		if( Enemy[ty][5][0].EmRanCnt > 127 ) {
			double radius = Enemy[ty][5][0].LBE_ang * 3.14 / 180;
			double add_x = cos( radius ) * length_x;
			double add_y = sin( radius ) * length_y;
			Enemy[ty][5][0].chara.x = LBC_x + add_x;
			Enemy[ty][5][0].chara.y = LBC_y + add_y;
			Enemy[ty][5][0].LBE_ang += 1;
			Enemy[ty][5][0].EmMoveCnt += 1;
		}
	}
	else {
		Enemy[ty][5][0].ran = enEmRanAng::UP;
	}
	//ｼｮｯﾄﾌﾗｸﾞ.
	if( Enemy[ty][5][0].EshotCnt > 20 ) {
		E_ShotFlag( Enemy, ty, 5, 0 );
		Enemy[ty][5][0].EshotCnt = 0;
	}
}
