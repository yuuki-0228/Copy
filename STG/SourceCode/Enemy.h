#pragma once

//-----------------------------.
//	�֐����������ߐ錾.
//-----------------------------.
//�G�ړ�.
void Es_Ran( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int type, int ptn, int eNo );
//�ړ��p�^�[��.
void E_ran_right( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], PLAYER player, SCROLL scroll, int ty, int ptn, int eNo );
void E_ran_up( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], BOSS* boss, int ty, int ptn, int eNo );
void E_ran_left( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], BOSS* boss, int ty, int ptn, int eNo );
void E_ran_circle( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], int ty, int ptn, int eNo );
//�G���A�{�X�ړ��p�^�[��.
void LBE_penetration( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], int ty, int ptn, int eNo );
void LBE_beam( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], int ty, int ptn, int eNo );
void LBE_charge( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], PLAYER plarye, SCROLL scroll, int ty, int ptn, int eNo );
void LBE_tracking( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], int ty, int ptn, int eNo );
