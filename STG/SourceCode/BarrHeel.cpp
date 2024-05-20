#include "Main.h"
#include "CSoundManager.h"

//�v���C���[�̃o���A.
void P_Barr( PLAYER* Player )
{
	if ( Player->ability.barrUP != 0 ) {
		for ( int bNo = 0; bNo < ( ABLT_MAX - 1 ) * BARRMAX; bNo++ ) {
			//���W�����킹��.
			Player->barrier[bNo].C_x = Player->chara.x + C_SIZE / 2 - 6;
			Player->barrier[bNo].C_y = Player->chara.y + C_SIZE / 2 - 6;

			//�ړ���𒲂ׂ�.
			double radius = Player->barrier[bNo].ang * 3.14 / 180;
			double add_x = cos( radius ) * Player->barrier[bNo].length_x;
			double add_y = sin( radius ) * Player->barrier[bNo].length_y;

			//�ړ�(��]).
			Player->barrier[bNo].x = Player->barrier[bNo].C_x + add_x;
			Player->barrier[bNo].y = Player->barrier[bNo].C_y + add_y;

			Player->barrier[bNo].ang += 2;
		}
	}
}

//�G�̃o���A.
void E_Barr( ENEMY enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
			for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
				if ( enemy[typeNo][ptnNo][eNo].ability.barrUP != 0 ) {
					if ( enemy[typeNo][ptnNo][eNo].ability.barrUP != 0 ) {
						for ( int bNo = 0; bNo < enemy[typeNo][ptnNo][eNo].ability.barrUP * BARRMAX; bNo++ ) {
							//���W�����킹��.
							enemy[typeNo][ptnNo][eNo].barrier[bNo].C_x = enemy[typeNo][ptnNo][eNo].chara.x + C_SIZE / 2 - 6;
							enemy[typeNo][ptnNo][eNo].barrier[bNo].C_y = enemy[typeNo][ptnNo][eNo].chara.y + C_SIZE / 2 - 6;

							//�ړ���𒲂ׂ�.
							double radius = enemy[typeNo][ptnNo][eNo].barrier[bNo].ang * 3.14 / 180;
							double add_x = cos( radius ) * enemy[typeNo][ptnNo][eNo].barrier[bNo].length_x;
							double add_y = sin( radius ) * enemy[typeNo][ptnNo][eNo].barrier[bNo].length_y;

							//�ړ�(��]).
							enemy[typeNo][ptnNo][eNo].barrier[bNo].x = enemy[typeNo][ptnNo][eNo].barrier[bNo].C_x + add_x;
							enemy[typeNo][ptnNo][eNo].barrier[bNo].y = enemy[typeNo][ptnNo][eNo].barrier[bNo].C_y + add_y;

							enemy[typeNo][ptnNo][eNo].barrier[bNo].ang += 1;
						}
					}
				}
			}
		}
	}
}

//�v���C���[��HP�̉�.
void P_heal( PLAYER* Player )
{
	if ( Player->ability.hpUP != 0 ){
		Player->chara.healCnt += 1;
		//�񕜏���.
		if ( Player->chara.healCnt >= FPS * ( 6 - Player->ability.hpUP ) ) {
			Player->chara.healCnt = 0;
			//�񕜂����ꍇHP��100�𒴂��Ȃ��ꍇ�񕜂���.
			if ( Player->status.hp < Player->initial_status.hp ) {

				//��SE��炷.
				CSoundManager::PlaySE( CSoundManager::SE_Heel );

				Player->status.hp += 1;

				//�񕜃G�t�F�N�g�̕\��.
				for ( int hNo = 0; hNo < H_MAX; hNo++ ){
					Player->HeelAnima[hNo].x			= rand() % ( C_SIZE - C_SIZE / 2 ) + Player->chara.x;
					Player->HeelAnima[hNo].y			= rand() % ( C_SIZE / 3 ) + ( Player->chara.y + ( C_SIZE - C_SIZE / 2 ) );
					Player->HeelAnima[hNo].PtnX			= rand() % 2;
					Player->HeelAnima[hNo].PtnY			= rand() % 2;
					Player->HeelAnima[hNo].DispFlag		= true;
				}
			}
		}
	}
}

//�G��HP�̉�.
void E_heal( ENEMY enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
			for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
				if ( ( enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) ||
					 ( enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) )
				{
					if ( enemy[typeNo][ptnNo][eNo].ability.hpUP != 0 ) {
						enemy[typeNo][ptnNo][eNo].chara.healCnt += 1;
						//�񕜏���.
						if ( enemy[typeNo][ptnNo][eNo].chara.healCnt >= FPS * ( 6 - enemy[typeNo][ptnNo][eNo].ability.hpUP ) ) {
							enemy[typeNo][ptnNo][eNo].chara.healCnt = 0;
							//�񕜂����ꍇHP��100�𒴂��Ȃ��ꍇ�񕜂���.
							if ( enemy[typeNo][ptnNo][eNo].status.hp < enemy[typeNo][ptnNo][eNo].initial_status.hp ) {
								enemy[typeNo][ptnNo][eNo].status.hp += 1;

								//�񕜃G�t�F�N�g�̕\��.
								for ( int hNo = 0; hNo < H_MAX; hNo++ ) {
									enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].x = rand() % ( C_SIZE - C_SIZE / 2 ) + enemy[typeNo][ptnNo][eNo].chara.x;
									enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].y = rand() % ( C_SIZE / 3 ) + ( enemy[typeNo][ptnNo][eNo].chara.y + ( C_SIZE - C_SIZE / 2 ) );
									enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].PtnX = rand() % 2;
									enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].PtnY = rand() % 2;
									enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].DispFlag = true;
								}
							}
						}
					}
				}
			}
		}
	}
}