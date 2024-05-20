#include "Main.h"
#include "Boss.h"

void BEDisp( BOSS* boss, ENEMY enemy[E_TYPE][E_PTN][E_MAX] )
{
	//カウント追加.
	boss->BEDisp.DispCnt += 1;
	if ( boss->BEDisp.DispCnt >= BECNTMAX * 10 ){
		boss->BEDisp.DispCnt = 0;
	}

	//現在のカウントで出現する敵がいれば.
	for ( int No = 0; No < BEDISPMAX; No++ ){
		if ( boss->BEDisp.DispCnt == boss->BEDisp.CntData[No] * 10 ){
			if ( boss->BEDisp.ENoData[No] != 0 ) {
				int typeNo = 0;		// 種類No(  10 の桁 ).
				int ptnNo = 0;		//ﾊﾟﾀｰﾝNo(   1 の桁 ).

				//桁分け.
				typeNo = ( boss->BEDisp.ENoData[No] % 100 ) / 10;
				ptnNo = boss->BEDisp.ENoData[No] % 10;

				//敵の表示.
				for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
					if ( enemy[typeNo - 1][ptnNo - 1][eNo].appFlag == false ) {
						//表示.
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