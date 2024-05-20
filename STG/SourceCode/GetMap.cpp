#include "Main.h"

bool GetMap( MAP* map )
{
	char	Path[128]	= "";
	int		MapNo		= map->LastNo;

	//�O���MapNo�ȊO���I�΂��܂Ń��[�v.
	while ( MapNo == map->LastNo )
	{
		MapNo = rand() % 3 + 1;
	}
	//�O���MapNo���㏑��.
	map->LastNo = MapNo;

	//�t�@�C���p�X�̍쐬.
 	wsprintf( Path, "Data\\CSV\\MAP%d.csv", MapNo );

	FILE *fp;
	fp = fopen( Path, "r" );	//�t�@�C�����J��.
	if ( fp == NULL ) {
		//�t�@�C�����J���Ȃ��ꍇ.
		return false;
	}
	for ( int hNo = 0; hNo < MAP_H; hNo++ ) {
		//�t�@�C���̒��̃f�[�^������.
		fscanf( fp, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			&map->Data[hNo][0], &map->Data[hNo][1], &map->Data[hNo][2], &map->Data[hNo][3], &map->Data[hNo][4], &map->Data[hNo][5], &map->Data[hNo][6], &map->Data[hNo][7], &map->Data[hNo][8], &map->Data[hNo][9], &map->Data[hNo][10],
			&map->Data[hNo][11], &map->Data[hNo][12], &map->Data[hNo][13], &map->Data[hNo][14], &map->Data[hNo][15], &map->Data[hNo][16], &map->Data[hNo][17], &map->Data[hNo][18], &map->Data[hNo][19], &map->Data[hNo][20], &map->Data[hNo][21] );
	}
	//�t�@�C�������.
	fclose( fp );

	return true;
}