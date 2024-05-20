#include "Main.h"

bool GetBEDispData( BOSS* boss )
{
	FILE *fp;
	fp = fopen( "Data\\CSV\\B_EDIDP.csv", "r" );	//�t�@�C�����J��.
	if ( fp == NULL ) {
		//�t�@�C�����J���Ȃ��ꍇ.
		return false;
	}
	for ( int No = 0; No < BEDISPMAX; No++ ) {
		//�t�@�C���̒��̃f�[�^������.
		fscanf( fp, "%d, %d, %d", &boss->BEDisp.CntData[No], &boss->BEDisp.ENoData[No], &boss->BEDisp.DispData[No] );
	}
	//�t�@�C�������.
	fclose( fp );

	//������.
	boss->BEDisp.DispCnt = 0;

	return true;
}