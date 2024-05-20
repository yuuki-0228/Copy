#include "Main.h"
#include "CSoundManager.h"

void CntMap( SCROLL* scroll, MAP* map, ENEMY enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX],
			 BOSS* boss, PLAYER* player )
{
	if ( map->cnt < C_SIZE ){
		map->cnt += scroll->spd;
	}
	else{
		map->cnt = 0;
		map->dispCnt++;
		
		//�}�b�v�̕`��.
		DispMap( map, enemy, Plant, gimmick, boss, scroll, player );
	}
}

void DispMap( MAP* map, ENEMY enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX],
			  BOSS* boss, SCROLL* scroll, PLAYER* player )
{
	int idenNo = 0;		// ����No( 100 �̌� ).
	int typeNo = 0;		// ���No(  10 �̌� ).
	int ptnNo  = 0;		//�����No(   1 �̌� ).

	for ( int wNo = 0; wNo < MAP_W; wNo++ ){
		if ( map->Data[map->dispCnt][wNo] != 0 ){
			//������.
			idenNo = map->Data[map->dispCnt][wNo] / 100;
			typeNo = ( map->Data[map->dispCnt][wNo] % 100 ) / 10;
			ptnNo  = map->Data[map->dispCnt][wNo] % 10;

			//����\�����邩.
			switch ( idenNo )
			{
			case 1:	//�G.
				for ( int eNo = 0; eNo < E_MAX; eNo++ ){
					if ( enemy[typeNo - 1][ptnNo - 1][eNo].appFlag == false ){
						//�\��.
						enemy[typeNo - 1][ptnNo - 1][eNo].chara.x		= C_SIZE / 2 * wNo;
						enemy[typeNo - 1][ptnNo - 1][eNo].chara.y		= -C_SIZE;
						enemy[typeNo - 1][ptnNo - 1][eNo].chara.State	= enCharaState::Living;
						enemy[typeNo - 1][ptnNo - 1][eNo].status.hp		= enemy[typeNo - 1][ptnNo - 1][eNo].initial_status.hp;
						enemy[typeNo - 1][ptnNo - 1][eNo].appFlag		= true;

						//�ԂȂ�.
						if ( typeNo == 1 ){
							Plant[ptnNo - 1][eNo].State				= enPlantState::Sleeping;
							Plant[ptnNo - 1][eNo].Sleep[0].x		= enemy[typeNo - 1][ptnNo][eNo].chara.x + C_SIZE / 2 - 16 / 2;
							Plant[ptnNo - 1][eNo].Sleep[0].y		= enemy[typeNo - 1][ptnNo][eNo].chara.y - 16 / 2;
							Plant[ptnNo - 1][eNo].Sleep[0].Alpha	= 255;
							Plant[ptnNo - 1][eNo].Sleep[0].ptnNo	= 2;
							Plant[ptnNo - 1][eNo].Sleep[0].DispFlag = true;
						}
						break;
					}
				}
				break;
			case 2:	//�X�e�[�W�M�~�b�N.
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					if ( gimmick[typeNo - 1][ptnNo - 1][gNo].State == enGimmickState::GWait ) {
						//�\��.
						gimmick[typeNo - 1][ptnNo - 1][gNo].x		= C_SIZE / 2 * wNo + 8;
						gimmick[typeNo - 1][ptnNo - 1][gNo].y		= -gimmick[typeNo - 1][ptnNo - 1][gNo].hSize;
						gimmick[typeNo - 1][ptnNo - 1][gNo].hp		= 10;
						gimmick[typeNo - 1][ptnNo - 1][gNo].State	= enGimmickState::GLiving;

						break;
					}
				}
				break;
			case 3:	//���������G.
				for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
					if ( enemy[typeNo - 1][ptnNo - 1][eNo].BossFlag == false ) {
						//�\��.
						enemy[typeNo - 1][ptnNo - 1][eNo].chara.x		= C_SIZE / 2 * wNo;
						enemy[typeNo - 1][ptnNo - 1][eNo].chara.y		= -C_SIZE;
						enemy[typeNo - 1][ptnNo - 1][eNo].chara.State	= enCharaState::Living;
						enemy[typeNo - 1][ptnNo - 1][eNo].BossFlag		= true;

						//�ԂȂ�.
						if ( typeNo == 1 ){
							Plant[ptnNo - 1][eNo].State				= enPlantState::Sleeping;
							Plant[ptnNo - 1][eNo].Sleep[0].Alpha	= 255;
							Plant[ptnNo - 1][eNo].Sleep[0].ptnNo	= 2;
							Plant[ptnNo - 1][eNo].Sleep[0].DispFlag = true;
						}
						break;
					}
				}
				break;
			case 4:	//�{�X.
				boss->chara[0].x		= boss->initial_xy[0].x;
				boss->chara[0].y		= boss->initial_xy[0].y + C_SIZE * 2;
				boss->chara[0].State	= enCharaState::Anima;	//������.
				boss->chara[1].x		= boss->initial_xy[1].x;
				boss->chara[1].y		= boss->initial_xy[1].y + C_SIZE * 2;
				boss->chara[1].State	= enCharaState::Anima;	//������.
				boss->chara[2].x		= boss->initial_xy[2].x;
				boss->chara[2].y		= boss->initial_xy[2].y + C_SIZE * 2;
				boss->chara[2].State	= enCharaState::Anima;	//������.

				scroll->stop	= true;
				player->KeyStop = true;
				boss->DispFlag	= true;

				//�{�X��BGM�̍Đ�.
				CSoundManager::Stop	   ( CSoundManager::BGM_Main );
				CSoundManager::PlayLoop( CSoundManager::BGM_Boss );

				//�{�X�̓��ޏoSE�̍Đ�.
				CSoundManager::PlaySE( CSoundManager::SE_BEarth );

				break;
			default:
				break;
			}
		}
	}
}