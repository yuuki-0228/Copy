#include "Main.h"
#include "AbltUpdate.h"

//プレイヤーの能力の個数更新.
void P_UpdateAblt( PLAYER* playerData )
{
	//ポインタで中身を取得(コードが横に長くならなくするため).
	ABILITY* pAbility = &playerData->ability;

	//一時的に能力の所持個数を入れる.
	int UpdateTmp[ABLT_TYPE] = {};

	//所持個数を調べ上書きする.
	for ( int i = 0; i < ABLT_MAX; i++ ) {
		for( int type = 1; type <= ABLT_TYPE; type++ ){
			if ( playerData->chara.abArea[i] == type ) {
				UpdateTmp[type - 1] += 1;
			}
		}
	}
	pAbility->beam	 = UpdateTmp[0];
	pAbility->pene	 = UpdateTmp[1];
	pAbility->chg	 = UpdateTmp[2];
	pAbility->homing = UpdateTmp[3];
	pAbility->dirUP	 = UpdateTmp[4];
	pAbility->numUP	 = UpdateTmp[5];
	pAbility->hpUP	 = UpdateTmp[6];
	pAbility->barrUP = UpdateTmp[7];
	pAbility->atkUP	 = UpdateTmp[8];
	pAbility->cspdUP = UpdateTmp[9];
	pAbility->sspdUP = UpdateTmp[10];

	//プレイヤーのステータスの更新.
	P_UpdateStatus( playerData );
}

//敵の能力の個数更新.
void E_UpdateAblt( ENEMY enemey[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo )
{
	//ポインタで中身を取得(コードが横に長くならなくするため).
	ABILITY* pAbility = &enemey[typeNo][ptnNo][eNo].ability;

	//一時的に能力の所持個数を入れる.
	int UpdateTmp[ABLT_TYPE] = {};

	//所持個数を調べ上書きする.
	for ( int i = 0; i < ABLT_MAX; i++ ) {
		for ( int type = 1; type <= ABLT_TYPE; type++ ){
			if ( enemey[typeNo][ptnNo][eNo].chara.abArea[i] == type ) {
				UpdateTmp[type - 1] += 1;
			}
		}
	}
	pAbility->beam   = UpdateTmp[0];
	pAbility->pene   = UpdateTmp[1];
	pAbility->chg    = UpdateTmp[2];
	pAbility->homing = UpdateTmp[3];
	pAbility->dirUP  = UpdateTmp[4];
	pAbility->numUP  = UpdateTmp[5];
	pAbility->hpUP   = UpdateTmp[6];
	pAbility->barrUP = UpdateTmp[7];
	pAbility->atkUP	 = UpdateTmp[8];
	pAbility->cspdUP = UpdateTmp[9];
	pAbility->sspdUP = UpdateTmp[10];

	//プレイヤーのステータスの更新.
	E_UpdateStatus( enemey, typeNo, ptnNo, eNo );
}

//プレイヤーのステータス能力に応じて更新.
void P_UpdateStatus( PLAYER* playerData )
{
	//ポインタで中身を取得(コードが横に長くならなくするため).
	STATUS*  pStatus = &playerData->status;
	STATUS*  pI_Status = &playerData->initial_status;
	ABILITY* pAbility = &playerData->ability;

	//所持能力に応じてステータスを変化させる.
	pStatus->atk  = pI_Status->atk	+ pAbility->atkUP * 2;
	pStatus->cspd = pI_Status->cspd + pAbility->cspdUP;
	pStatus->dir  = pI_Status->dir	+ pAbility->dirUP * 2;
	pStatus->num  = pI_Status->num	+ pAbility->numUP;
	for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
		for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
			playerData->shot[numNo][dirNo].sspd = ( int )( PS_SPD + pAbility->sspdUP );
		}
	}
	//特殊能力による変動.
	if ( pAbility->homing != 0 ){
		playerData->chara.ShotDirAng = 20 * ( ABLT_MAX - pAbility->dirUP + 1 );
	}
	else {
		playerData->chara.ShotDirAng = 20;
	}
	if ( pAbility->chg != 0 ) {
		pStatus->atk *= 3;
	}
}

//敵のステータスを能力に応じて更新.
void E_UpdateStatus( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo )
{
	//ポインタで中身を取得(コードが横に長くならなくするため).
	STATUS*  pStatus   = &enemyData[typeNo][ptnNo][eNo].status;
	STATUS*  pI_Status = &enemyData[typeNo][ptnNo][eNo].initial_status;
	ABILITY* pAbility  = &enemyData[typeNo][ptnNo][eNo].ability;

	//所持能力に応じてステータスを変化させる.
	pStatus->atk  = pI_Status->atk  + pAbility->atkUP * 2;
	pStatus->cspd = pI_Status->cspd + pAbility->cspdUP;
	pStatus->dir  = pI_Status->dir  + pAbility->dirUP * 2;
	pStatus->num  = pI_Status->num  + pAbility->numUP;
	for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
		for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
			enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd = ( int )( ES_SPD + pAbility->sspdUP );
		}
	}

	//特殊能力による変動.
	if ( pAbility->homing != 0 ) {
		enemyData[typeNo][ptnNo][eNo].chara.ShotDirAng = 20 * ( ABLT_MAX - pAbility->dirUP + 1 );
	}
	else {
		enemyData[typeNo][ptnNo][eNo].chara.ShotDirAng = 20;
	}
}