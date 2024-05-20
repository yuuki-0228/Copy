#pragma once

//-----------------------------.
//	関数のﾌﾟﾛﾄﾀｲﾌﾟ宣言.
//-----------------------------.
//プレイヤーのステータスの更新.
void P_UpdateStatus( PLAYER* playerData );
//敵のステータスの更新.
void E_UpdateStatus( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo );
