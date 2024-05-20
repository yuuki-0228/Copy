#pragma once

//-----------------------------.
//	�֐����������ߐ錾.
//-----------------------------.
/* �U���̔����t���O�֐�. */
//�p���`�U���t���O.
void PunchFlag( BOSS* boss, int typeNo, int Dang = 0 );
//�����p���`�U���t���O.
void WPunchFlag( BOSS* boss, int Dang = 0 );
//�����ǔ��p���`�U���t���O.
void AtPunchFlag( BOSS* boss, PLAYER player, int typeNo );
//�����ǔ�X�p���`�U���t���O.
void WAtPunchFlag( BOSS* boss, PLAYER player );
//�˂��グ��U��.
void UpRockFlag( BOSS* boss, int typeNo, int numMax = 10, int dirMax = 1, int DAng = 20, int space = 8, int time = 100 );
//�����˂��グ��U��.
void WUpRockFlag( BOSS* boss, int numMax = 10, int dirMax = 1, int DAng = 20, int space = 8, int time = 100 );
//�~�˂��グ��U��.
void RUpRockFlag( BOSS* boss, int typeNo, int numMax = 10, int dirMax = 13, int space = 8, int time = 100 );
//�����~�˂��グ��U��.
void WRUpRockFlag( BOSS* boss, int numMax = 10, int dirMax = 13, int space = 8, int time = 100 );
//��o���U��.
void RockFlag( BOSS* boss, int typeNo, int numMax = 5, int dirMax = 1, int DAng = 20, int space = 7, int iAng = 90 );
//������o���U��.
void WRockFlag( BOSS* boss, int numMax = 5, int dirMax = 1, int DAng = 20, int space = 7, int iAng = 90 );
//�~��o���U��.
void RRockFlag( BOSS* boss, int typeNo, int numMax = 5, int dirMax = 1, int space = 7, int iAng = 90 );
//�����~��o���U��.
void WRRockFlag( BOSS* boss, int numMax = 5, int dirMax = 1, int space = 7, int iAng = 90 );
//������΂�.
void BlowawayFlag( BOSS* boss, PLAYER player );
//�{�X�{�̃V���b�g�U��.
void BShotFlag( BOSS* boss, int dirMax = 1, int DAng = 20, int sSpd = 5, int iAng = 90 );
//�{�X�{�̉~�V���b�g�U��.
void RBShotFlag( BOSS* boss, int dirMax = 1, int sSpd = 5, int iAng = 90 );

/* �U���̓���֐�. */
//�p���`����.
void PunchCon( BOSS* boss, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo );
//�˂��グ��U��.
void UpRockCon( BOSS* boss, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int typeNo );
//��o���U��.
void RockCon( BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], PLAYER* Player, int typeNo );
//������΂�.
void Blowaway( BOSS* boss, SCROLL* scroll, PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo );
//�{�X�̒e�U��.
void BShotCon( BOSS* boss );

/* �U���p�^�[�� */
void EarlyForm( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX] );
void MidForm( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX] );
void LateForm( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX] );
void Stick( BOSS* boss, PLAYER player );
void Attacking( BOSS* boss );

/* �{�X�G���A�̓G�̕\�� */
void BEDisp( BOSS* boss, ENEMY enemy[E_TYPE][E_PTN][E_MAX] );
