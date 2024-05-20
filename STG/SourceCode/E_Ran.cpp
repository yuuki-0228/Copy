#include "Main.h"
#include "Enemy.h"

//�G�̓���.
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

		//�R�E�����ƃn�`�ȊO.
		if ( ( type != 3 ) && ( type != 4 ) ) {
			//�X�e�[�W�M�~�b�N[��]�ɂԂ�����.
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

//�ʏ�̓G�̓���.
void E_ran( SCROLL scroll, PLAYER player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int ty, int ptn, int eNo ) {
	if ( ( boss->chara[enBoss::Main].State != enCharaState::Dying ) && ( boss->chara[enBoss::Main].State != enCharaState::Dead ) ){
		if ( ( Enemy[ty][ptn][eNo].chara.State == enCharaState::Living ) || ( Enemy[ty][ptn][eNo].chara.State == enCharaState::Invi ) ) {
			if ( Enemy[ty][ptn][eNo].appFlag == true ) {
				Enemy[ty][ptn][eNo].ran = enEmRanAng::DOWN;
				switch ( ty ) {
				case 0://����.
				{
					//�����׸�.
					E_ran_right( Enemy, Plant, player, scroll, ty, ptn, eNo );
					break;
				}
				case 1://�ʏ�.
				{
					if ( Enemy[ty][ptn][eNo].EshotCnt > 40 ) {
						E_ShotFlag( Enemy, ty, ptn, eNo );
						Enemy[ty][ptn][eNo].EshotCnt = 0;

					}
					break;
				}
				case 2://�ђ�.
				{
					E_ran_up( Enemy, boss, ty, ptn, eNo );
					break;
				}
				case 3://�r�[��.
				{
					E_ran_left( Enemy, boss, ty, ptn, eNo );
					break;
				}
				case 4://�ǔ�.
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
					Enemy[ty][ptn][eNo].chara.y = WND_H;					//��ʂ̊O(��).
					Enemy[ty][ptn][eNo].chara.State = enCharaState::Wait;	//�ҋ@.
					Enemy[ty][ptn][eNo].appFlag = false;
				}
			}
		}
		else if ( ( Enemy[ty][ptn][eNo].chara.State == enCharaState::Dead ) || ( Enemy[ty][ptn][eNo].chara.State == enCharaState::Invi ) ) {
			if ( ( ty == 1 ) || ( scroll.stop == false ) ){
				Enemy[ty][ptn][eNo].chara.y += scroll.spd;
				if ( Enemy[ty][ptn][eNo].chara.y > GWND_H ) {
					Enemy[ty][ptn][eNo].chara.x = WND_W;
					Enemy[ty][ptn][eNo].chara.y = WND_H;					//��ʂ̊O(��).
					Enemy[ty][ptn][eNo].chara.State = enCharaState::Wait;	//�ҋ@.
					Enemy[ty][ptn][eNo].appFlag = false;
				}
			}
		}
	}
}

//���ߓ���.
void E_ran_right( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], PLAYER plarye, SCROLL scroll, int ty, int ptn, int eNo )
{
	Enemy[ty][ptn][eNo].ran = enEmRanAng::STOP;
	Enemy[ty][ptn][eNo].chara.y += scroll.spd;

	//�v���C���[�Ƃ̊p�x�𒲂ׂ�.
	Enemy[ty][ptn][eNo].chara.ang = atan2( ( Enemy[ty][ptn][eNo].chara.y - plarye.chara.y ), ( plarye.chara.x - Enemy[ty][ptn][eNo].chara.x ) );
	if( ( ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 70 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -70 ) ) ||
		( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 110 ) || ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -110 ) ) ) {

		//�Q�Ă���ꍇ�N������.
		if ( Plant[ptn][eNo].State == enPlantState::Sleeping ){
			Plant[ptn][eNo].State			= enPlantState::WokeUp;
			Plant[ptn][eNo].Woke.AnimaX		= 0;
			Plant[ptn][eNo].Woke.AnimaY		= 0;
			Plant[ptn][eNo].Woke.AnimaCnt	= 0;
			Plant[ptn][eNo].Woke.Alpha		= 255;
			Plant[ptn][eNo].Woke.AlphaFlag	= false;
		}

		//�摜�̕ύX.
		if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 110 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 155 ) ) {
			//����.
			Enemy[ty][ptn][eNo].animetion.No = 0;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) > 155 ) || ( DO( Enemy[ty][ptn][eNo].chara.ang ) < -155 ) ) {
			//��.
			Enemy[ty][ptn][eNo].animetion.No = 1;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -155 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -110 ) ) {
			//����.
			Enemy[ty][ptn][eNo].animetion.No = 2;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 25 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 70 ) ) {
			//�E��.
			Enemy[ty][ptn][eNo].animetion.No = 3;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) > -25 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) < 25 ) ) {
			//�E.
			Enemy[ty][ptn][eNo].animetion.No = 4;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -70 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -25 ) ) {
			//�E��.
			Enemy[ty][ptn][eNo].animetion.No = 5;
		}

		//�V���b�g�t���O.
		E_ShotFlag( Enemy, ty, ptn, eNo, ( RAD( 180 ) + Enemy[ty][ptn][eNo].chara.ang ), true );
	}
	else {
		//������Ԃɂ���.
		Plant[ptn][eNo].State				= enPlantState::Sleeping;

		//���߃J�E���g�����炵�Ă���.
		if( Enemy[ty][ptn][eNo].chara.chgCnt > 0 ) {
			Enemy[ty][ptn][eNo].shot[4][9].x = ( Enemy[ty][ptn][eNo].chara.x + C_SIZE / 2 );
			Enemy[ty][ptn][eNo].shot[4][9].y = ( Enemy[ty][ptn][eNo].chara.y + C_SIZE / 2 );
			Enemy[ty][ptn][eNo].chara.chgCnt -= 1;
		}
	}
}

//�ђʓ���.
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
	//�����׸�.
	if( Enemy[ty][ptn][eNo].EshotCnt > 25 ) {
		E_ShotFlag( Enemy, ty, ptn, eNo );
		Enemy[ty][ptn][eNo].EshotCnt = 0;
	}
}

//�r�[������.
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

	//�����׸�.
	if( Enemy[ty][ptn][eNo].EshotCnt > 25 ) {
		E_ShotFlag( Enemy, ty, ptn, eNo );
		Enemy[ty][ptn][eNo].EshotCnt = 0;
	}
}

//�ǔ�.
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

	//�����׸�.
	if( Enemy[ty][ptn][eNo].EshotCnt > 25 ) {
		E_ShotFlag( Enemy, ty, ptn, eNo );
		Enemy[ty][ptn][eNo].EshotCnt = 0;
	}
}

//���������G.
void LBE_ran( SCROLL scroll, PLAYER player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int ty ) {
	if ( ( boss->chara[enBoss::Main].State != enCharaState::Dying ) && ( boss->chara[enBoss::Main].State != enCharaState::Dead ) ){
		if ( ( Enemy[ty][5][0].chara.State == enCharaState::Living ) || ( Enemy[ty][5][0].chara.State == enCharaState::Invi ) ) {
			if ( Enemy[ty][5][0].BossFlag == true ) {
				Enemy[ty][5][0].ran = enEmRanAng::DOWN;
				switch ( ty ) {
				case 1://����.
				{
					LBE_charge( Enemy, Plant, player, scroll, ty, 5, 0 );
					break;
				}
				case 2://�ђ�.
				{
					LBE_penetration( Enemy, ty, 5, 0 );
					break;
				}
				case 3://�r�[��.
				{
					LBE_beam( Enemy, ty, 5, 0 );
					break;
				}
				case 4://�ǔ�.
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
					Enemy[ty][5][0].chara.y = -C_SIZE;					//��ʂ̊O(��).
					Enemy[ty][5][0].chara.State = enCharaState::Wait;	//�ҋ@.
					Enemy[ty][5][0].BossFlag = false;
				}
			}
		}
		else if ( Enemy[ty][5][0].chara.State == enCharaState::Dead ) {
			if ( scroll.stop == false ){
				Enemy[ty][5][0].chara.y += scroll.spd;
				if ( Enemy[ty][5][0].chara.y > GWND_H ) {
					Enemy[ty][5][0].chara.x = WND_W;
					Enemy[ty][5][0].chara.y = WND_H;					//��ʂ̊O(��).
					Enemy[ty][5][0].chara.State = enCharaState::Wait;	//�ҋ@.
					Enemy[ty][5][0].appFlag = false;
				}
			}
		}
	}
}

//LB���ߓ���.
void LBE_charge( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], PLAYER plarye, SCROLL scroll, int ty, int ptn, int eNo )
{
	Enemy[ty][ptn][eNo].ran = enEmRanAng::STOP;
	Enemy[ty][ptn][eNo].chara.y += scroll.spd;

	//�v���C���[�Ƃ̊p�x�𒲂ׂ�.
	Enemy[ty][ptn][eNo].chara.ang = atan2( ( Enemy[ty][ptn][eNo].chara.y - plarye.chara.y ), ( plarye.chara.x - Enemy[ty][ptn][eNo].chara.x ) );
	if( ( ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 70 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -70 ) ) ||
		( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 110 ) || ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -110 ) ) ) {

		//�Q�Ă���ꍇ�N������.
		if ( Plant[ptn][eNo].State == enPlantState::Sleeping ){
			Plant[ptn][eNo].State			= enPlantState::WokeUp;
			Plant[ptn][eNo].Woke.AnimaX		= 0;
			Plant[ptn][eNo].Woke.AnimaY		= 0;
			Plant[ptn][eNo].Woke.AnimaCnt	= 0;
			Plant[ptn][eNo].Woke.Alpha		= 255;
			Plant[ptn][eNo].Woke.AlphaFlag	= false;
		}

		//�摜�̕ύX.
		if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 110 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 155 ) ) {
			//����.
			Enemy[ty][ptn][eNo].animetion.No = 0;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) > 155 ) || ( DO( Enemy[ty][ptn][eNo].chara.ang ) < -155 ) ) {
			//��.
			Enemy[ty][ptn][eNo].animetion.No = 1;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -155 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -110 ) ) {
			//����.
			Enemy[ty][ptn][eNo].animetion.No = 2;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= 25 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= 70 ) ) {
			//�E��.
			Enemy[ty][ptn][eNo].animetion.No = 3;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) > -25 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) < 25 ) ) {
			//�E.
			Enemy[ty][ptn][eNo].animetion.No = 4;
		}
		else if( ( DO( Enemy[ty][ptn][eNo].chara.ang ) >= -70 ) && ( DO( Enemy[ty][ptn][eNo].chara.ang ) <= -25 ) ) {
			//�E��.
			Enemy[ty][ptn][eNo].animetion.No = 5;
		}

		//�V���b�g�t���O.
		E_ShotFlag( Enemy, ty, 5, 0, ( RAD( 180 ) + Enemy[ty][ptn][eNo].chara.ang ), true );
	}
	else {
		//������Ԃɂ���.
		Plant[ptn][eNo].State	= enPlantState::Sleeping;

		//���߃J�E���g�����炵�Ă���.
		if( Enemy[ty][ptn][eNo].chara.chgCnt > 0 ) {
			Enemy[ty][ptn][eNo].shot[4][9].x = ( Enemy[ty][ptn][eNo].chara.x + C_SIZE / 2 );
			Enemy[ty][ptn][eNo].shot[4][9].y = ( Enemy[ty][ptn][eNo].chara.y + C_SIZE / 2 );
			Enemy[ty][ptn][eNo].chara.chgCnt -= 1;
		}
	}
}

//LB�ђʓ���.
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
	//�����׸�.
	if( Enemy[ty][5][0].EshotCnt > 20 ) {
		E_ShotFlag( Enemy, ty, 5, 0 );
		Enemy[ty][5][0].EshotCnt = 0;
	}
}

//LB�r�[������.
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
	//�����׸�.
	if( Enemy[ty][5][0].EshotCnt > 20 ) {
		E_ShotFlag( Enemy, ty, 5, 0 );
		Enemy[ty][5][0].EshotCnt = 0;
	}

}

//LB�ǔ�����.
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
	//�����׸�.
	if( Enemy[ty][5][0].EshotCnt > 20 ) {
		E_ShotFlag( Enemy, ty, 5, 0 );
		Enemy[ty][5][0].EshotCnt = 0;
	}
}
