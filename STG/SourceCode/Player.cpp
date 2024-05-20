#include "Main.h"
#include "CSoundManager.h"

//能力を捨てる時の能力の取得による初期化.
void AbilityReset( PLAYER* Player, int areaNo );

//プレイヤーの移動.
void PlayerOp( PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], CXInput* pXInput, XInStTrg XInKey, SCROLL* scroll, enScene Scene, ENDING Ending, bool* pause, FADE* Fade ) {
	//ゲームスタート時スライムを上にあげる.
	if ( scroll->Start == true ) {
		if ( Player->chara.y > GWND_H - C_SIZE * 2 ) {
			//上に移動.
			Player->chara.y -= 1;
		}
		else {
			//スクロール開始.
			scroll->stop	= false;
			scroll->Start	= false;
		}
	}
	else {
		//生存確認.
		if ( ( Player->chara.State == enCharaState::Living ) ||
			 ( Player->chara.State == enCharaState::Invi ) )
		{

			//角度が0ではない場合初期化.
			if ( Player->chara.ang != 0 ) {
				Player->chara.ang = 0;
			}

			//↑ or LSU.
			if ( GetAsyncKeyState( VK_UP ) & 0x8000 || XInKey.stL_Y >= CXInput::INPUT_THUMB_MAX ) {
				//初期に接触している岩を調べる.
				bool HitRock[G_TYPE][G_PTN][G_MAX] = {};	//初期に接触している岩.
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

				//プレイヤーの移動.
				Player->chara.y -= Player->status.cspd;
				if ( Player->chara.y < GWND_H / 4 ) {
					Player->chara.y = GWND_H / 4;
				}
				//ギミックと衝突した場合.
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
			//↓ or LSD.
			if ( GetAsyncKeyState( VK_DOWN ) & 0x8000 || XInKey.stL_Y <= CXInput::INPUT_THUMB_MIN ) {
				//初期に接触している岩を調べる.
				bool HitRock[G_TYPE][G_PTN][G_MAX] = {};	//初期に接触している岩.
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

				//プレイヤーの移動.
				Player->chara.y += Player->status.cspd;
				if ( Player->chara.y > GWND_H - C_SIZE ) {
					Player->chara.y = GWND_H - C_SIZE;
				}
				//ギミックと衝突した場合.
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
			//← or LSL.
			if ( GetAsyncKeyState( VK_LEFT ) & 0x8000 || XInKey.stL_X <= CXInput::INPUT_THUMB_MIN ) {
				//初期に接触している岩を調べる.
				bool HitRock[G_TYPE][G_PTN][G_MAX] = {};	//初期に接触している岩.
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

				//プレイヤーの移動.
				Player->chara.x -= Player->status.cspd;
				Player->chara.ang = -0.2;
				if ( Player->chara.x < 0 ) {
					Player->chara.x = 0;
				}
				//ギミックと衝突した場合.
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
			//→ or LSR.
			if ( GetAsyncKeyState( VK_RIGHT ) & 0x8000 || XInKey.stL_X >= CXInput::INPUT_THUMB_MAX ) {
				//初期に接触している岩を調べる.
				bool HitRock[G_TYPE][G_PTN][G_MAX] = {};	//初期に接触している岩.
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

				//プレイヤーの移動.
				Player->chara.x += Player->status.cspd;
				Player->chara.ang = 0.2;
				if ( Player->chara.x > GWND_W - C_SIZE ) {
					Player->chara.x = GWND_W - C_SIZE;
				}
				//ギミックと衝突した場合.
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
			//スペースキー or RT.
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

					//発射準備アニメーションCNT初期化.
					Player->chgAniCnt = 0;
					Player->chgAniFlag = false;

					// chgCnt = -1：「 -1 」で弾の発射.
					Player->chara.chgCnt = -1;
					Player->chgNumCnt -= 1;
					if ( Player->chgNumCnt < 0 ){
						Player->chgNumCnt = 0;
					}
					P_ShotFlag( Player, Scene, Ending );
				}
				else {
					if ( Player->chara.chgCnt > 0 ) {
						//演出用で[4][9]を使用.
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
						//能力削除SEを鳴らす.
						CSoundManager::PlaySE( CSoundManager::SE_ADelete );

						//能力削除によるステータスの初期化.
						AbilityReset( Player, 0 );

						//エリアの削除.
						Player->DGAbi[0].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 0;
						Player->DGAbi[0].xy.y		= WND_H - ABLT_SIZE - 6;
						Player->DGAbi[0].DispFlag	= true;
						Player->DGAbi[0].ExpaFlag	= false;
						Player->DGAbi[0].AbiNo		= Player->chara.abArea[0];
						Player->DGAbi[0].Expa		= 0;
						Player->DGAbi[0].Alpha		= 200;
						Player->chara.abArea[0]		= 0;

						//見た目を戻す.
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
						//能力削除SEを鳴らす.
						CSoundManager::PlaySE( CSoundManager::SE_ADelete );

						//能力削除によるステータスの初期化.
						AbilityReset( Player, 1 );

						//エリアの削除.
						Player->DGAbi[1].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 1;
						Player->DGAbi[1].xy.y		= WND_H - ABLT_SIZE - 6;
						Player->DGAbi[1].DispFlag	= true;
						Player->DGAbi[1].ExpaFlag	= false;
						Player->DGAbi[1].AbiNo		= Player->chara.abArea[1];
						Player->DGAbi[1].Expa		= 0;
						Player->DGAbi[1].Alpha		= 200;
						Player->chara.abArea[1]		= 0;

						//見た目を戻す.
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
						//能力削除SEを鳴らす.
						CSoundManager::PlaySE( CSoundManager::SE_ADelete );

						//能力削除によるステータスの初期化.
						AbilityReset( Player, 2 );

						//エリアの削除.
						Player->DGAbi[2].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 2;
						Player->DGAbi[2].xy.y		= WND_H - ABLT_SIZE - 6;
						Player->DGAbi[2].DispFlag	= true;
						Player->DGAbi[2].ExpaFlag	= false;
						Player->DGAbi[2].AbiNo		= Player->chara.abArea[2];
						Player->DGAbi[2].Expa		= 0;
						Player->DGAbi[2].Alpha		= 200;
						Player->chara.abArea[2]		= 0;

						//見た目を戻す.
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
						//能力削除SEを鳴らす.
						CSoundManager::PlaySE( CSoundManager::SE_ADelete );

						//能力削除のよるステータスの初期化.
						AbilityReset( Player, 3 );

						//エリアの削除.
						Player->DGAbi[3].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 3;
						Player->DGAbi[3].xy.y		= WND_H - ABLT_SIZE - 6;
						Player->DGAbi[3].DispFlag	= true;
						Player->DGAbi[3].ExpaFlag	= false;
						Player->DGAbi[3].AbiNo		= Player->chara.abArea[3];
						Player->DGAbi[3].Expa		= 0;
						Player->DGAbi[3].Alpha		= 200;
						Player->chara.abArea[3]		= 0;

						//見た目を戻す.
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
				//フェードイン.
				*pause			= true;
				Fade->InOutFlag = false;
				Fade->FadeState = enFade::in;
				Fade->EndAlpha	= 127;
			}
			//コントローラが接続されているとき.
			if ( pXInput->IsConnect() == true ) {
				//LB.
				if ( pXInput->IsKeyDown( XINPUT_GAMEPAD_LEFT_SHOULDER ) ) {
					//選択エリアを左に移動.
					Player->XInArea -= 1;
					if ( Player->XInArea < 0 ) {
						Player->XInArea = 3;
					}
				}
				//RB.
				if ( pXInput->IsKeyDown( XINPUT_GAMEPAD_RIGHT_SHOULDER ) ) {
					//選択エリアを右に移動.
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
							//能力削除SEを鳴らす.
							CSoundManager::PlaySE( CSoundManager::SE_ADelete );

							//能力削除によるステータスの初期化.
							AbilityReset( Player, Player->XInArea );

							//エリアの削除.
							Player->DGAbi[Player->XInArea].xy.x			= 5 + ( ABLT_SIZE + 5 ) * Player->XInArea;
							Player->DGAbi[Player->XInArea].xy.y			= WND_H - ABLT_SIZE - 6;
							Player->DGAbi[Player->XInArea].DispFlag		= true;
							Player->DGAbi[Player->XInArea].ExpaFlag		= false;
							Player->DGAbi[Player->XInArea].AbiNo		= Player->chara.abArea[Player->XInArea];
							Player->DGAbi[Player->XInArea].Expa			= 0;
							Player->DGAbi[Player->XInArea].Alpha		= 200;
							Player->chara.abArea[Player->XInArea]		= 0;

							//見た目を戻す.
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

//能力を捨てる時の能力の取得による初期化.
void AbilityReset( PLAYER* Player, int areaNo )
{
	//溜めがあれば.
	if ( Player->chara.abArea[areaNo] == enAbility::chg ) {
		Player->chgAniCnt	= 0;
		Player->chgNumCnt	= 0;
		Player->chgAniFlag	= false;
		
		//描画用の弾の初期化.
		Player->shot[4][9].x = -WND_W;
		Player->shot[4][9].y = -WND_H;
	}
	//ビームがあれば.
	else if ( Player->chara.abArea[areaNo] == enAbility::beam ){
		for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ){
			for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ){
				//ビームのヒット数を初期化.
				Player->shot[numNo][dirNo].beamCnt = BEAMMAX;
			}
		}
	}
	//弾数+があれば.
	else if ( Player->chara.abArea[areaNo] == enAbility::numUP ) {
		for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
			for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ){
				//捨てた弾数全てのフラグを下ろし初期化.
				Player->shot[numNo][dirNo].DispShotFlag = false;
				Player->shot[numNo][dirNo].x = -WND_W;
				Player->shot[numNo][dirNo].y = -WND_H;

				//溜めがあれば.
				if ( Player->chara.abArea[0] == enAbility::chg ) {
					Player->chgAniCnt = 0;
					Player->chgNumCnt = 0;
					Player->chgAniFlag = false;

					//描画用の弾の初期化.
					Player->shot[4][9].x = -WND_W;
					Player->shot[4][9].y = -WND_H;
				}
			}
		}
	}
	//方向+があれば.
	else if ( Player->chara.abArea[areaNo] == enAbility::dirUP ) {
		for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
			for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
				//捨てた方向全てのフラグを下ろし初期化.
				Player->shot[numNo][dirNo].DispShotFlag = false;
				Player->shot[numNo][dirNo].x = WND_W;
				Player->shot[numNo][dirNo].y = WND_H;

				//溜めがあれば.
				if ( Player->chara.abArea[0] == enAbility::chg ) {
					Player->chgAniCnt = 0;
					Player->chgNumCnt = 0;
					Player->chgAniFlag = false;

					//描画用の弾の初期化.
					Player->shot[4][9].x = -WND_W;
					Player->shot[4][9].y = -WND_H;
				}
			}
		}
	}
}