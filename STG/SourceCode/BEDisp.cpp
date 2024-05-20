#include "Main.h"
#include "Boss.h"

void BEDisp( BOSS* boss, ENEMY enemy[E_TYPE][E_PTN][E_MAX] )
{
	//�J�E���g�ǉ�.
	boss->BEDisp.DispCnt += 1;
	if ( boss->BEDisp.DispCnt >= BECNTMAX * 10 ){
		boss->BEDisp.DispCnt = 0;
	}

	//���݂̃J�E���g�ŏo������G�������.
	for ( int No = 0; No < BEDISPMAX; No++ ){
		if ( boss->BEDisp.DispCnt == boss->BEDisp.CntData[No] * 10 ){
			if ( boss->BEDisp.ENoData[No] != 0 ) {
				int typeNo = 0;		// ���No(  10 �̌� ).
				int ptnNo = 0;		//�����No(   1 �̌� ).

				//������.
				typeNo = ( boss->BEDisp.ENoData[No] % 100 ) / 10;
				ptnNo = boss->BEDisp.ENoData[No] % 10;

				//�G�̕\��.
				for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
					if ( enemy[typeNo - 1][ptnNo - 1][eNo].appFlag == false ) {
						//�\��.
						enemy[typeNo - 1][ptnNo - 1][eNo].chara.x = C_SIZE / 2 * boss->BEDisp.DispData[No] + 8;
						enemy[typeNo - 1][ptnNo - 1][eNo].chara.y = -C_SIZE;
						enemy[typeNo - 1][ptnNo - 1][eNo].chara.State = enCharaState::Living;
						enemy[typeNo - 1][ptnNo - 1][eNo].status.hp = enemy[typeNo - 1][ptnNo - 1][eNo].initial_status.hp;
						enemy[typeNo - 1][ptnNo - 1][eNo].appFlag = true;
						if ( ptnNo == 6 ) {
							enemy[typeNo - 1][ptnNo - 1][eNo].BossFlag = true;
						}

						break;
					}
				}
			}
		}
	}
}