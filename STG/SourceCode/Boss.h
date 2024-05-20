#pragma once

//-----------------------------.
//	関数のﾌﾟﾛﾄﾀｲﾌﾟ宣言.
//-----------------------------.
/* 攻撃の発生フラグ関数. */
//パンチ攻撃フラグ.
void PunchFlag( BOSS* boss, int typeNo, int Dang = 0 );
//同時パンチ攻撃フラグ.
void WPunchFlag( BOSS* boss, int Dang = 0 );
//初期追尾パンチ攻撃フラグ.
void AtPunchFlag( BOSS* boss, PLAYER player, int typeNo );
//初期追尾Xパンチ攻撃フラグ.
void WAtPunchFlag( BOSS* boss, PLAYER player );
//突き上げ岩攻撃.
void UpRockFlag( BOSS* boss, int typeNo, int numMax = 10, int dirMax = 1, int DAng = 20, int space = 8, int time = 100 );
//同時突き上げ岩攻撃.
void WUpRockFlag( BOSS* boss, int numMax = 10, int dirMax = 1, int DAng = 20, int space = 8, int time = 100 );
//円突き上げ岩攻撃.
void RUpRockFlag( BOSS* boss, int typeNo, int numMax = 10, int dirMax = 13, int space = 8, int time = 100 );
//同時円突き上げ岩攻撃.
void WRUpRockFlag( BOSS* boss, int numMax = 10, int dirMax = 13, int space = 8, int time = 100 );
//岩出現攻撃.
void RockFlag( BOSS* boss, int typeNo, int numMax = 5, int dirMax = 1, int DAng = 20, int space = 7, int iAng = 90 );
//同時岩出現攻撃.
void WRockFlag( BOSS* boss, int numMax = 5, int dirMax = 1, int DAng = 20, int space = 7, int iAng = 90 );
//円岩出現攻撃.
void RRockFlag( BOSS* boss, int typeNo, int numMax = 5, int dirMax = 1, int space = 7, int iAng = 90 );
//同時円岩出現攻撃.
void WRRockFlag( BOSS* boss, int numMax = 5, int dirMax = 1, int space = 7, int iAng = 90 );
//吹き飛ばし.
void BlowawayFlag( BOSS* boss, PLAYER player );
//ボス本体ショット攻撃.
void BShotFlag( BOSS* boss, int dirMax = 1, int DAng = 20, int sSpd = 5, int iAng = 90 );
//ボス本体円ショット攻撃.
void RBShotFlag( BOSS* boss, int dirMax = 1, int sSpd = 5, int iAng = 90 );

/* 攻撃の動作関数. */
//パンチ動作.
void PunchCon( BOSS* boss, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo );
//突き上げ岩攻撃.
void UpRockCon( BOSS* boss, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int typeNo );
//岩出現攻撃.
void RockCon( BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], PLAYER* Player, int typeNo );
//吹き飛ばし.
void Blowaway( BOSS* boss, SCROLL* scroll, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo );
//ボスの弾攻撃.
void BShotCon( BOSS* boss );

/* 攻撃パターン */
void EarlyForm( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX] );
void MidForm( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX] );
void LateForm( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX] );
void Stick( BOSS* boss, PLAYER player );
void Attacking( BOSS* boss );

/* ボスエリアの敵の表示 */
void BEDisp( BOSS* boss, ENEMY enemy[E_TYPE][E_PTN][E_MAX] );
