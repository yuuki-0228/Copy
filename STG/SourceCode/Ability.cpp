#include "Main.h"

bool E_Ability(ENEMY enemydata[E_TYPE][E_PTN][E_MAX])
{
	FILE *fp;
	fp = fopen("Data\\CSV\\E_ABILITY.csv", "r");	//ファイルを開く.
	if (fp == NULL) {
		//ファイルが開けない場合.
		return false;
	}
	for ( int type = 0; type < E_TYPE; type++ ) {
		for ( int ptn = 0; ptn < E_PTN; ptn++ ) {
			//ファイルの中のデータを入れる.
			fscanf( fp, "%d, %d, %d, %d", &enemydata[type][ptn][0].chara.abArea[0], &enemydata[type][ptn][0].chara.abArea[1], &enemydata[type][ptn][0].chara.abArea[2], &enemydata[type][ptn][0].chara.abArea[3] );
		}
	}
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
			for ( int eNo = 1; eNo < E_MAX; eNo++ ) {
				for ( int abNo = 0; abNo < ABLT_MAX; abNo++ ){
					enemydata[typeNo][ptnNo][eNo].chara.abArea[abNo] = enemydata[typeNo][ptnNo][0].chara.abArea[abNo];
				}
			}
		}
	}
	//ファイルを閉じる.
	fclose(fp);

	return true;
}