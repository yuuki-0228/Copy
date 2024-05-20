#include "Main.h"
#include "AbltUpdate.h"

//�v���C���[�̔\�͂̌��X�V.
void P_UpdateAblt( PLAYER* playerData )
{
	//�|�C���^�Œ��g���擾(�R�[�h�����ɒ����Ȃ�Ȃ����邽��).
	ABILITY* pAbility = &playerData->ability;

	//�ꎞ�I�ɔ\�͂̏�����������.
	int UpdateTmp[ABLT_TYPE] = {};

	//�������𒲂׏㏑������.
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

	//�v���C���[�̃X�e�[�^�X�̍X�V.
	P_UpdateStatus( playerData );
}

//�G�̔\�͂̌��X�V.
void E_UpdateAblt( ENEMY enemey[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo )
{
	//�|�C���^�Œ��g���擾(�R�[�h�����ɒ����Ȃ�Ȃ����邽��).
	ABILITY* pAbility = &enemey[typeNo][ptnNo][eNo].ability;

	//�ꎞ�I�ɔ\�͂̏�����������.
	int UpdateTmp[ABLT_TYPE] = {};

	//�������𒲂׏㏑������.
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

	//�v���C���[�̃X�e�[�^�X�̍X�V.
	E_UpdateStatus( enemey, typeNo, ptnNo, eNo );
}

//�v���C���[�̃X�e�[�^�X�\�͂ɉ����čX�V.
void P_UpdateStatus( PLAYER* playerData )
{
	//�|�C���^�Œ��g���擾(�R�[�h�����ɒ����Ȃ�Ȃ����邽��).
	STATUS*  pStatus = &playerData->status;
	STATUS*  pI_Status = &playerData->initial_status;
	ABILITY* pAbility = &playerData->ability;

	//�����\�͂ɉ����ăX�e�[�^�X��ω�������.
	pStatus->atk  = pI_Status->atk	+ pAbility->atkUP * 2;
	pStatus->cspd = pI_Status->cspd + pAbility->cspdUP;
	pStatus->dir  = pI_Status->dir	+ pAbility->dirUP * 2;
	pStatus->num  = pI_Status->num	+ pAbility->numUP;
	for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
		for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
			playerData->shot[numNo][dirNo].sspd = ( int )( PS_SPD + pAbility->sspdUP );
		}
	}
	//����\�͂ɂ��ϓ�.
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

//�G�̃X�e�[�^�X��\�͂ɉ����čX�V.
void E_UpdateStatus( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo )
{
	//�|�C���^�Œ��g���擾(�R�[�h�����ɒ����Ȃ�Ȃ����邽��).
	STATUS*  pStatus   = &enemyData[typeNo][ptnNo][eNo].status;
	STATUS*  pI_Status = &enemyData[typeNo][ptnNo][eNo].initial_status;
	ABILITY* pAbility  = &enemyData[typeNo][ptnNo][eNo].ability;

	//�����\�͂ɉ����ăX�e�[�^�X��ω�������.
	pStatus->atk  = pI_Status->atk  + pAbility->atkUP * 2;
	pStatus->cspd = pI_Status->cspd + pAbility->cspdUP;
	pStatus->dir  = pI_Status->dir  + pAbility->dirUP * 2;
	pStatus->num  = pI_Status->num  + pAbility->numUP;
	for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
		for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
			enemyData[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd = ( int )( ES_SPD + pAbility->sspdUP );
		}
	}

	//����\�͂ɂ��ϓ�.
	if ( pAbility->homing != 0 ) {
		enemyData[typeNo][ptnNo][eNo].chara.ShotDirAng = 20 * ( ABLT_MAX - pAbility->dirUP + 1 );
	}
	else {
		enemyData[typeNo][ptnNo][eNo].chara.ShotDirAng = 20;
	}
}