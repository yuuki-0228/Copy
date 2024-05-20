#include "Main.h"
#include "CSoundManager.h"

//�\�͂��̂Ă鎞�̔\�͂̎擾�ɂ�鏉����.
void AbilityReset( PLAYER* Player, int areaNo );

//�v���C���[�̈ړ�.
void PlayerOp( PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], CXInput* pXInput, XInStTrg XInKey, SCROLL* scroll, enScene Scene, ENDING Ending, bool* pause, FADE* Fade ) {
	//�Q�[���X�^�[�g���X���C������ɂ�����.
	if ( scroll->Start == true ) {
		if ( Player->chara.y > GWND_H - C_SIZE * 2 ) {
			//��Ɉړ�.
			Player->chara.y -= 1;
		}
		else {
			//�X�N���[���J�n.
			scroll->stop	= false;
			scroll->Start	= false;
		}
	}
	else {
		//�����m�F.
		if ( ( Player->chara.State == enCharaState::Living ) ||
			 ( Player->chara.State == enCharaState::Invi ) )
		{

			//�p�x��0�ł͂Ȃ��ꍇ������.
			if ( Player->chara.ang != 0 ) {
				Player->chara.ang = 0;
			}

			//�� or LSU.
			if ( GetAsyncKeyState( VK_UP ) & 0x8000 || XInKey.stL_Y >= CXInput::INPUT_THUMB_MAX ) {
				//�����ɐڐG���Ă����𒲂ׂ�.
				bool HitRock[G_TYPE][G_PTN][G_MAX] = {};	//�����ɐڐG���Ă����.
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( ( typeNo != 2 ) && ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GDead ) ) {
								if ( CollisionDetection(
									Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
									gimmick[typeNo][ptnNo][gNo].x, gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) )
								{
									HitRock[typeNo][ptnNo][gNo] = true;
								}
							}
						}
					}
				}

				//�v���C���[�̈ړ�.
				Player->chara.y -= Player->status.cspd;
				if ( Player->chara.y < GWND_H / 4 ) {
					Player->chara.y = GWND_H / 4;
				}
				//�M�~�b�N�ƏՓ˂����ꍇ.
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( ( typeNo != 2 ) && ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GDead ) &&
								( gimmick[typeNo][ptnNo][gNo].RockFlag == false ) && ( HitRock[typeNo][ptnNo][gNo] == false ) )
							{
								if ( CollisionDetection(
									Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
									gimmick[typeNo][ptnNo][gNo].x, gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) )
								{
									Player->chara.y = gimmick[typeNo][ptnNo][gNo].y + gimmick[typeNo][ptnNo][gNo].hSize - PH_CORR;
								}
							}
						}
					}
				}
			}
			//�� or LSD.
			if ( GetAsyncKeyState( VK_DOWN ) & 0x8000 || XInKey.stL_Y <= CXInput::INPUT_THUMB_MIN ) {
				//�����ɐڐG���Ă����𒲂ׂ�.
				bool HitRock[G_TYPE][G_PTN][G_MAX] = {};	//�����ɐڐG���Ă����.
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( ( typeNo != 2 ) && ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GDead ) ) {
								if ( CollisionDetection(
									Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
									gimmick[typeNo][ptnNo][gNo].x, gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) )
								{
									HitRock[typeNo][ptnNo][gNo] = true;
								}
							}
						}
					}
				}

				//�v���C���[�̈ړ�.
				Player->chara.y += Player->status.cspd;
				if ( Player->chara.y > GWND_H - C_SIZE ) {
					Player->chara.y = GWND_H - C_SIZE;
				}
				//�M�~�b�N�ƏՓ˂����ꍇ.
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( ( typeNo != 2 ) && ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GDead ) &&
								 ( gimmick[typeNo][ptnNo][gNo].RockFlag == false ) && ( HitRock[typeNo][ptnNo][gNo] == false ) )
							{
								if ( CollisionDetection(
									Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
									gimmick[typeNo][ptnNo][gNo].x, gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) )
								{
									Player->chara.y = gimmick[typeNo][ptnNo][gNo].y - C_SIZE + PH_CORR;
								}
							}
						}
					}
				}
			}
			//�� or LSL.
			if ( GetAsyncKeyState( VK_LEFT ) & 0x8000 || XInKey.stL_X <= CXInput::INPUT_THUMB_MIN ) {
				//�����ɐڐG���Ă����𒲂ׂ�.
				bool HitRock[G_TYPE][G_PTN][G_MAX] = {};	//�����ɐڐG���Ă����.
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( ( typeNo != 2 ) && ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GDead ) ) {
								if ( CollisionDetection(
									Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
									gimmick[typeNo][ptnNo][gNo].x, gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) )
								{
									HitRock[typeNo][ptnNo][gNo] = true;
								}
							}
						}
					}
				}

				//�v���C���[�̈ړ�.
				Player->chara.x -= Player->status.cspd;
				Player->chara.ang = -0.2;
				if ( Player->chara.x < 0 ) {
					Player->chara.x = 0;
				}
				//�M�~�b�N�ƏՓ˂����ꍇ.
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( ( typeNo != 2 ) && ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GDead ) &&
								( gimmick[typeNo][ptnNo][gNo].RockFlag == false ) && ( HitRock[typeNo][ptnNo][gNo] == false ) )
							{
								if ( CollisionDetection(
									Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
									gimmick[typeNo][ptnNo][gNo].x, gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) )
								{
									Player->chara.x = gimmick[typeNo][ptnNo][gNo].x + gimmick[typeNo][ptnNo][gNo].wSize - PH_CORR;
								}
							}
						}
					}
				}
			}
			//�� or LSR.
			if ( GetAsyncKeyState( VK_RIGHT ) & 0x8000 || XInKey.stL_X >= CXInput::INPUT_THUMB_MAX ) {
				//�����ɐڐG���Ă����𒲂ׂ�.
				bool HitRock[G_TYPE][G_PTN][G_MAX] = {};	//�����ɐڐG���Ă����.
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( ( typeNo != 2 ) && ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GDead ) ) {
								if ( CollisionDetection(
									Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
									gimmick[typeNo][ptnNo][gNo].x, gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) )
								{
									HitRock[typeNo][ptnNo][gNo] = true;
								}
							}
						}
					}
				}

				//�v���C���[�̈ړ�.
				Player->chara.x += Player->status.cspd;
				Player->chara.ang = 0.2;
				if ( Player->chara.x > GWND_W - C_SIZE ) {
					Player->chara.x = GWND_W - C_SIZE;
				}
				//�M�~�b�N�ƏՓ˂����ꍇ.
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( ( typeNo != 2 ) && ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GDead ) &&
								( gimmick[typeNo][ptnNo][gNo].RockFlag == false ) && ( HitRock[typeNo][ptnNo][gNo] == false ) )
							{
								if ( CollisionDetection(
									Player->chara.x + PH_CORR, Player->chara.y + PH_CORR, PH_SIZE, PH_SIZE,
									gimmick[typeNo][ptnNo][gNo].x, gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) )
								{
									Player->chara.x = gimmick[typeNo][ptnNo][gNo].x - C_SIZE + PH_CORR;
								}
							}
						}
					}
				}
			}
			//�X�y�[�X�L�[ or RT.
            if ( ( GetAsyncKeyState( VK_SPACE ) & 0x8000 || XInKey.trgR >= CXInput::INPUT_TRIGGER_MAX ) &&
				 ( ( Player->chgNumCnt == 0 ) || ( Player->chgNumCnt == Player->status.num ) ) ) {

				Player->PushKey[enPushKey::RT] = true;
				Player->PushKey[enPushKey::Space] = true;
				P_ShotFlag( Player, Scene, Ending );
			}
			else if ( Player->ability.chg != 0 ) {
				Player->PushKey[enPushKey::Space] = false;
				Player->PushKey[enPushKey::RT] = false;

				if ( ( Player->chara.chgCnt >= CHARGEMAX ) ||
					( ( Player->chgNumCnt > 0 ) && ( Player->shotCnt > S_TIME ) ) ) {

					//���ˏ����A�j���[�V����CNT������.
					Player->chgAniCnt = 0;
					Player->chgAniFlag = false;

					// chgCnt = -1�F�u -1 �v�Œe�̔���.
					Player->chara.chgCnt = -1;
					Player->chgNumCnt -= 1;
					if ( Player->chgNumCnt < 0 ){
						Player->chgNumCnt = 0;
					}
					P_ShotFlag( Player, Scene, Ending );
				}
				else {
					if ( Player->chara.chgCnt > 0 ) {
						//���o�p��[4][9]���g�p.
						Player->shot[4][9].x = ( Player->chara.x + C_SIZE / 2 );
						Player->shot[4][9].y = ( Player->chara.y );

						Player->chara.chgCnt -= 3;
						if ( Player->chara.chgCnt <= 0 ) {
							Player->chara.chgCnt = 0;
						}
					}
				}
			}
			else {
				Player->PushKey[enPushKey::Space] = false;
				Player->PushKey[enPushKey::RT] = false;
			}
			//A.
			if ( GetAsyncKeyState( 'A' ) & 0x8000 ) {
				if ( Player->PushKey[enPushKey::A] == false ) {
					Player->PushKey[enPushKey::A] = true;

					if ( Player->chara.abArea[0] != 0 ) {
						//�\�͍폜SE��炷.
						CSoundManager::PlaySE( CSoundManager::SE_ADelete );

						//�\�͍폜�ɂ��X�e�[�^�X�̏�����.
						AbilityReset( Player, 0 );

						//�G���A�̍폜.
						Player->DGAbi[0].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 0;
						Player->DGAbi[0].xy.y		= WND_H - ABLT_SIZE - 6;
						Player->DGAbi[0].DispFlag	= true;
						Player->DGAbi[0].ExpaFlag	= false;
						Player->DGAbi[0].AbiNo		= Player->chara.abArea[0];
						Player->DGAbi[0].Expa		= 0;
						Player->DGAbi[0].Alpha		= 200;
						Player->chara.abArea[0]		= 0;

						//�����ڂ�߂�.
						if ( Player->SlimeNo != enSlime::SNormal ){
							Player->NewSlimeNo	= enSlime::SNormal;
							Player->AbGetFlag	= true;
						}

						P_UpdateAblt( Player );
					}
				}
			}
			else { Player->PushKey[enPushKey::A] = false; }
			//S.
			if ( GetAsyncKeyState( 'S' ) & 0x8000 ) {
				if ( Player->PushKey[enPushKey::S] == false ) {
					Player->PushKey[enPushKey::S] = true;

					if ( Player->chara.abArea[1] != 0 ) {
						//�\�͍폜SE��炷.
						CSoundManager::PlaySE( CSoundManager::SE_ADelete );

						//�\�͍폜�ɂ��X�e�[�^�X�̏�����.
						AbilityReset( Player, 1 );

						//�G���A�̍폜.
						Player->DGAbi[1].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 1;
						Player->DGAbi[1].xy.y		= WND_H - ABLT_SIZE - 6;
						Player->DGAbi[1].DispFlag	= true;
						Player->DGAbi[1].ExpaFlag	= false;
						Player->DGAbi[1].AbiNo		= Player->chara.abArea[1];
						Player->DGAbi[1].Expa		= 0;
						Player->DGAbi[1].Alpha		= 200;
						Player->chara.abArea[1]		= 0;

						//�����ڂ�߂�.
						if ( Player->SlimeNo != enSlime::SNormal ){
							if ( Player->chara.abArea[0] == 0 ){
								Player->NewSlimeNo	= enSlime::SNormal;
								Player->AbGetFlag	= true;
							}
						}

						P_UpdateAblt( Player );
					}
				}
			}
			else { Player->PushKey[enPushKey::S] = false; }
			//D.
			if ( GetAsyncKeyState( 'D' ) & 0x8000 ) {
				if ( Player->PushKey[enPushKey::D] == false ) {
					Player->PushKey[enPushKey::D] = true;

					if ( Player->chara.abArea[2] != 0 ) {
						//�\�͍폜SE��炷.
						CSoundManager::PlaySE( CSoundManager::SE_ADelete );

						//�\�͍폜�ɂ��X�e�[�^�X�̏�����.
						AbilityReset( Player, 2 );

						//�G���A�̍폜.
						Player->DGAbi[2].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 2;
						Player->DGAbi[2].xy.y		= WND_H - ABLT_SIZE - 6;
						Player->DGAbi[2].DispFlag	= true;
						Player->DGAbi[2].ExpaFlag	= false;
						Player->DGAbi[2].AbiNo		= Player->chara.abArea[2];
						Player->DGAbi[2].Expa		= 0;
						Player->DGAbi[2].Alpha		= 200;
						Player->chara.abArea[2]		= 0;

						//�����ڂ�߂�.
						if ( Player->SlimeNo != enSlime::SNormal ){
							if ( Player->chara.abArea[0] == 0 ){
								Player->NewSlimeNo	= enSlime::SNormal;
								Player->AbGetFlag	= true;
							}
						}

						P_UpdateAblt( Player );
					}
				}
			}
			else { Player->PushKey[enPushKey::D] = false; }
			//F.
			if ( GetAsyncKeyState( 'F' ) & 0x8000 ) {
				if ( Player->PushKey[enPushKey::F] == false ) {
					Player->PushKey[enPushKey::F] = true;

					if ( Player->chara.abArea[3] != 0 ) {
						//�\�͍폜SE��炷.
						CSoundManager::PlaySE( CSoundManager::SE_ADelete );

						//�\�͍폜�̂��X�e�[�^�X�̏�����.
						AbilityReset( Player, 3 );

						//�G���A�̍폜.
						Player->DGAbi[3].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 3;
						Player->DGAbi[3].xy.y		= WND_H - ABLT_SIZE - 6;
						Player->DGAbi[3].DispFlag	= true;
						Player->DGAbi[3].ExpaFlag	= false;
						Player->DGAbi[3].AbiNo		= Player->chara.abArea[3];
						Player->DGAbi[3].Expa		= 0;
						Player->DGAbi[3].Alpha		= 200;
						Player->chara.abArea[3]		= 0;

						//�����ڂ�߂�.
						if ( Player->SlimeNo != enSlime::SNormal ){
							if ( Player->chara.abArea[0] == 0 ){
								Player->NewSlimeNo	= enSlime::SNormal;
								Player->AbGetFlag	= true;
							}
						}

						P_UpdateAblt( Player );
					}
				}
			}
			else { Player->PushKey[enPushKey::F] = false; }
			//ESC,.
			if ( GetAsyncKeyState( VK_ESCAPE ) & 0x8000 || pXInput->IsKeyDown( XINPUT_GAMEPAD_START ) ){
				//�t�F�[�h�C��.
				*pause			= true;
				Fade->InOutFlag = false;
				Fade->FadeState = enFade::in;
				Fade->EndAlpha	= 127;
			}
			//�R���g���[�����ڑ�����Ă���Ƃ�.
			if ( pXInput->IsConnect() == true ) {
				//LB.
				if ( pXInput->IsKeyDown( XINPUT_GAMEPAD_LEFT_SHOULDER ) ) {
					//�I���G���A�����Ɉړ�.
					Player->XInArea -= 1;
					if ( Player->XInArea < 0 ) {
						Player->XInArea = 3;
					}
				}
				//RB.
				if ( pXInput->IsKeyDown( XINPUT_GAMEPAD_RIGHT_SHOULDER ) ) {
					//�I���G���A���E�Ɉړ�.
					Player->XInArea += 1;
					if ( Player->XInArea > 3 ) {
						Player->XInArea = 0;
					}
				}
				//LT.
				if ( XInKey.trgL >= CXInput::INPUT_TRIGGER_MAX ) {
					if ( Player->PushKey[enPushKey::LT] == false ) {
						Player->PushKey[enPushKey::LT] = true;

						if ( Player->chara.abArea[Player->XInArea] != 0 ) {
							//�\�͍폜SE��炷.
							CSoundManager::PlaySE( CSoundManager::SE_ADelete );

							//�\�͍폜�ɂ��X�e�[�^�X�̏�����.
							AbilityReset( Player, Player->XInArea );

							//�G���A�̍폜.
							Player->DGAbi[Player->XInArea].xy.x			= 5 + ( ABLT_SIZE + 5 ) * Player->XInArea;
							Player->DGAbi[Player->XInArea].xy.y			= WND_H - ABLT_SIZE - 6;
							Player->DGAbi[Player->XInArea].DispFlag		= true;
							Player->DGAbi[Player->XInArea].ExpaFlag		= false;
							Player->DGAbi[Player->XInArea].AbiNo		= Player->chara.abArea[Player->XInArea];
							Player->DGAbi[Player->XInArea].Expa			= 0;
							Player->DGAbi[Player->XInArea].Alpha		= 200;
							Player->chara.abArea[Player->XInArea]		= 0;

							//�����ڂ�߂�.
							if ( Player->SlimeNo != enSlime::SNormal ){
								if ( ( Player->XInArea == 0 ) || ( Player->chara.abArea[0] == 0 ) ){
									Player->NewSlimeNo		= enSlime::SNormal;
									Player->AbGetFlag	= true;
								}
							}

							P_UpdateAblt( Player );
						}
					}
				}
				else{ Player->PushKey[enPushKey::LT] = false; }
			}
		}
	}
}

//�\�͂��̂Ă鎞�̔\�͂̎擾�ɂ�鏉����.
void AbilityReset( PLAYER* Player, int areaNo )
{
	//���߂������.
	if ( Player->chara.abArea[areaNo] == enAbility::chg ) {
		Player->chgAniCnt	= 0;
		Player->chgNumCnt	= 0;
		Player->chgAniFlag	= false;
		
		//�`��p�̒e�̏�����.
		Player->shot[4][9].x = -WND_W;
		Player->shot[4][9].y = -WND_H;
	}
	//�r�[���������.
	else if ( Player->chara.abArea[areaNo] == enAbility::beam ){
		for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ){
			for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ){
				//�r�[���̃q�b�g����������.
				Player->shot[numNo][dirNo].beamCnt = BEAMMAX;
			}
		}
	}
	//�e��+�������.
	else if ( Player->chara.abArea[areaNo] == enAbility::numUP ) {
		for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
			for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ){
				//�̂Ă��e���S�Ẵt���O�����낵������.
				Player->shot[numNo][dirNo].DispShotFlag = false;
				Player->shot[numNo][dirNo].x = -WND_W;
				Player->shot[numNo][dirNo].y = -WND_H;

				//���߂������.
				if ( Player->chara.abArea[0] == enAbility::chg ) {
					Player->chgAniCnt = 0;
					Player->chgNumCnt = 0;
					Player->chgAniFlag = false;

					//�`��p�̒e�̏�����.
					Player->shot[4][9].x = -WND_W;
					Player->shot[4][9].y = -WND_H;
				}
			}
		}
	}
	//����+�������.
	else if ( Player->chara.abArea[areaNo] == enAbility::dirUP ) {
		for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
			for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
				//�̂Ă������S�Ẵt���O�����낵������.
				Player->shot[numNo][dirNo].DispShotFlag = false;
				Player->shot[numNo][dirNo].x = WND_W;
				Player->shot[numNo][dirNo].y = WND_H;

				//���߂������.
				if ( Player->chara.abArea[0] == enAbility::chg ) {
					Player->chgAniCnt = 0;
					Player->chgNumCnt = 0;
					Player->chgAniFlag = false;

					//�`��p�̒e�̏�����.
					Player->shot[4][9].x = -WND_W;
					Player->shot[4][9].y = -WND_H;
				}
			}
		}
	}
}