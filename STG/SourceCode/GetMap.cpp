#include "Main.h"

bool GetMap( MAP* map )
{
	char	Path[128]	= "";
	int		MapNo		= map->LastNo;

	//前回のMapNo以外が選ばれるまでループ.
	while ( MapNo == map->LastNo )
	{
		MapNo = rand() % 3 + 1;
	}
	//前回のMapNoを上書き.
	map->LastNo = MapNo;

	//ファイルパスの作成.
 	wsprintf( Path, "Data\\CSV\\MAP%d.csv", MapNo );

	FILE *fp;
	fp = fopen( Path, "r" );	//ファイルを開く.
	if ( fp == NULL ) {
		//ファイルが開けない場合.
		return false;
	}
	for ( int hNo = 0; hNo < MAP_H; hNo++ ) {
		//ファイルの中のデータを入れる.
		fscanf( fp, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			&map->Data[hNo][0], &map->Data[hNo][1], &map->Data[hNo][2], &map->Data[hNo][3], &map->Data[hNo][4], &map->Data[hNo][5], &map->Data[hNo][6], &map->Data[hNo][7], &map->Data[hNo][8], &map->Data[hNo][9], &map->Data[hNo][10],
			&map->Data[hNo][11], &map->Data[hNo][12], &map->Data[hNo][13], &map->Data[hNo][14], &map->Data[hNo][15], &map->Data[hNo][16], &map->Data[hNo][17], &map->Data[hNo][18], &map->Data[hNo][19], &map->Data[hNo][20], &map->Data[hNo][21] );
	}
	//ファイルを閉じる.
	fclose( fp );

	return true;
}