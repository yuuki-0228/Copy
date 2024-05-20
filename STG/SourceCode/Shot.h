#pragma once
#include <math.h>

//定数宣言.
#define IANG	90			//1発の時の弾の飛ぶ方向.

//-----------------------------.
//	関数のﾌﾟﾛﾄﾀｲﾌﾟ宣言.
//-----------------------------.
/*当たり判定*/
//プレイヤーの弾と敵の当たり判定.
void PStoE_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//プレイヤーの弾とボスの当たり判定.
void PStoB_DC( PLAYER* Player, BOSS* boss );

//プレイヤーの弾とステージギミックの当たり判定.
void PStoG_DC( PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss );

//プレイヤーの弾と敵のバリアの当たり判定.
void PStoEB_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//敵の弾とプレイヤーの当たり判定.
void EStoP_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//敵の弾とステージギミックの当たり判定.
void EStoG_DC( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss );

//敵の弾とプレイヤーのバリアの当たり判定.
void EStoPB_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//ボスの弾とプレイヤーの当たり判定.
void BStoP_DC( PLAYER* Player, BOSS* boss );

//ボスの弾とプレイヤーのバリアの当たり判定.
void BStoPB_DC( PLAYER* Player, BOSS* boss );

//プレイヤーの弾と敵の弾の当たり判定.
void PStoES_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//プレイヤーの弾とボスの弾の当たり判定.
void PStoBS_DC( PLAYER* Player, BOSS* boss );

/*追尾*/
//プレイヤーの追尾制御.
void P_homing( PLAYER* playerData, BOSS* boss, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], enScene Scene, int numNo, int dirNo );

//敵の追尾制御.
void E_homing( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, int numNo, int dirNo, PLAYER playerData );

//プレイヤーのターゲットの更新.
void P_TargetUpdate ( PLAYER* playerData, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int numNo, int dirNo );
void P_BTargetUpdate( PLAYER* playerData, BOSS* boss, int numNo, int dirNo );

//敵のターゲットの更新.
void E_TargetUpdate( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, int numNo, int dirNo, PLAYER playerData );

/* SE */
//プレイヤーの弾.
void SE_PShot( PLAYER* player );

//敵の弾.
void SE_EShot( ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo );
