#pragma once
#include <math.h>

//�萔�錾.
#define IANG	90			//1���̎��̒e�̔�ԕ���.

//-----------------------------.
//	�֐����������ߐ錾.
//-----------------------------.
/*�����蔻��*/
//�v���C���[�̒e�ƓG�̓����蔻��.
void PStoE_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//�v���C���[�̒e�ƃ{�X�̓����蔻��.
void PStoB_DC( PLAYER* Player, BOSS* boss );

//�v���C���[�̒e�ƃX�e�[�W�M�~�b�N�̓����蔻��.
void PStoG_DC( PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss );

//�v���C���[�̒e�ƓG�̃o���A�̓����蔻��.
void PStoEB_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//�G�̒e�ƃv���C���[�̓����蔻��.
void EStoP_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//�G�̒e�ƃX�e�[�W�M�~�b�N�̓����蔻��.
void EStoG_DC( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss );

//�G�̒e�ƃv���C���[�̃o���A�̓����蔻��.
void EStoPB_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//�{�X�̒e�ƃv���C���[�̓����蔻��.
void BStoP_DC( PLAYER* Player, BOSS* boss );

//�{�X�̒e�ƃv���C���[�̃o���A�̓����蔻��.
void BStoPB_DC( PLAYER* Player, BOSS* boss );

//�v���C���[�̒e�ƓG�̒e�̓����蔻��.
void PStoES_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] );

//�v���C���[�̒e�ƃ{�X�̒e�̓����蔻��.
void PStoBS_DC( PLAYER* Player, BOSS* boss );

/*�ǔ�*/
//�v���C���[�̒ǔ�����.
void P_homing( PLAYER* playerData, BOSS* boss, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], enScene Scene, int numNo, int dirNo );

//�G�̒ǔ�����.
void E_homing( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, int numNo, int dirNo, PLAYER playerData );

//�v���C���[�̃^�[�Q�b�g�̍X�V.
void P_TargetUpdate ( PLAYER* playerData, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int numNo, int dirNo );
void P_BTargetUpdate( PLAYER* playerData, BOSS* boss, int numNo, int dirNo );

//�G�̃^�[�Q�b�g�̍X�V.
void E_TargetUpdate( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, int numNo, int dirNo, PLAYER playerData );

/* SE */
//�v���C���[�̒e.
void SE_PShot( PLAYER* player );

//�G�̒e.
void SE_EShot( ENEMY enemy[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo );
