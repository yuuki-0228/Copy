#include "Main.h"

bool E_Status(ENEMY enemydata[E_TYPE][E_PTN][E_MAX])
{
	FILE *fp;
	fp = fopen("Data\\CSV\\E_STATUS.csv", "r");	//�t�@�C�����J��.
	if (fp == NULL) {
		//�t�@�C�����J���Ȃ��ꍇ.
		return false;
	}
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
			//�t�@�C���̒��̃f�[�^������.
			fscanf( fp, "%d, %lf", &enemydata[typeNo][ptnNo][0].initial_status.hp, &enemydata[typeNo][ptnNo][0].initial_status.cspd );
			enemydata[typeNo][ptnNo][0].initial_status.atk = 1, enemydata[typeNo][ptnNo][0].initial_status.dir = 1, enemydata[typeNo][ptnNo][0].initial_status.num = 1;
		}
	}
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
			for ( int eNo = 1; eNo < E_MAX; eNo++ ) {
				enemydata[typeNo][ptnNo][eNo].initial_status = enemydata[typeNo][ptnNo][0].initial_status;
			}
		}
	}
	//�t�@�C�������.
	fclose(fp);

	return true;
}