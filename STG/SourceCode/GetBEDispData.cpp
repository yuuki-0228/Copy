#include "Main.h"

bool GetBEDispData( BOSS* boss )
{
	FILE *fp;
	fp = fopen( "Data\\CSV\\B_EDIDP.csv", "r" );	//ファイルを開く.
	if ( fp == NULL ) {
		//ファイルが開けない場合.
		return false;
	}
	for ( int No = 0; No < BEDISPMAX; No++ ) {
		//ファイルの中のデータを入れる.
		fscanf( fp, "%d, %d, %d", &boss->BEDisp.CntData[No], &boss->BEDisp.ENoData[No], &boss->BEDisp.DispData[No] );
	}
	//ファイルを閉じる.
	fclose( fp );

	//初期化.
	boss->BEDisp.DispCnt = 0;

	return true;
}