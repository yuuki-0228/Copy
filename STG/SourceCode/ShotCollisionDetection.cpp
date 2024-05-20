#include "Main.h"
#include "Shot.h"
#include "CSoundManager.h"

void S_CollisionDetection( PLAYER* PlayerData, ENEMY EnemyData[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss )
{
	PStoE_DC( PlayerData, EnemyData );					//プレイヤーの弾と敵の当たり判定.
	PStoB_DC( PlayerData, boss );						//プレイヤーの弾とボスの当たり判定.
	PStoG_DC( PlayerData, gimmick, WoodLeaf, boss );	//プレイヤーの弾とステージギミックの当たり判定.
	PStoEB_DC( PlayerData, EnemyData );					//プレイヤーの弾と敵のバリアの当たり判定.
	EStoP_DC( PlayerData, EnemyData );					//敵の弾とプレイヤーの当たり判定.
	EStoG_DC( EnemyData, gimmick, WoodLeaf, boss );		//敵の弾とステージギミックの当たり判定.
	EStoPB_DC( PlayerData, EnemyData );					//敵の弾とプレイヤーのバリアの当たり判定.
	BStoP_DC( PlayerData, boss );						//ボスの弾とプレイヤーの当たり判定.
	BStoPB_DC( PlayerData, boss );						//ボスの弾とプレイヤーのバリアの当たり判定( ボスの弾が[貫通]を所持している場合は使用しない ).
	PStoES_DC( PlayerData, EnemyData );					//プレイヤーの弾と敵の弾の当たり判定.
	PStoBS_DC( PlayerData, boss );						//プレイヤーの弾とボスの弾の当たり判定.
}

//プレイヤーの弾と敵の当たり判定.
void PStoE_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
		for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
			//自機の弾が発射されている.
			if ( Player->shot[numNo][dirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							//敵機の生存確認.
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) {
								//自機の弾と敵機の当たり判定.
								if ( ( ( Player->ability.chg == 0 ) && ( CollisionDetection(
									( int )Player->shot[numNo][dirNo].x,			( int )Player->shot[numNo][dirNo].y,			S_SIZE,			S_SIZE,
									( int )Enemy[typeNo][ptnNo][eNo].chara.x,		( int )Enemy[typeNo][ptnNo][eNo].chara.y,		C_SIZE,			C_SIZE ) ) ) ||

									( ( Player->ability.chg != 0 ) && ( CollisionDetection(
									( int ) Player->shot[numNo][dirNo].x - S_SIZE,  ( int ) Player->shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,		S_SIZE * 2,
									( int ) Enemy[typeNo][ptnNo][eNo].chara.x,		( int ) Enemy[typeNo][ptnNo][eNo].chara.y,		C_SIZE,			C_SIZE ) ) ) ) {

									//敵のダメージSEを鳴らす.
									CSoundManager::PlaySE( CSoundManager::SE_EDamage );

									//HPを減らす.
									Enemy[typeNo][ptnNo][eNo].status.hp -= Player->status.atk;
									if ( Enemy[typeNo][ptnNo][eNo].status.hp <= 0 ){
										Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Dying;	//状態を死亡中にする.
										Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt = 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
										Enemy[typeNo][ptnNo][eNo].chara.chgCnt = 0;

										//追尾終了.
										Player->shot[numNo][dirNo].homingCnt = HOMINGMAX;
									}
									else{
										//状態を無敵にする.
										Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Invi;
										Enemy[typeNo][ptnNo][eNo].chara.inviTime = INVITIME;
									}

									//自機の弾の着弾後の処理.
									if ( Player->ability.pene == 0 ){	//貫通をもっていない.
										if ( Player->ability.beam != 0 ){
											//ビームのヒット回数+2.
											Player->shot[numNo][dirNo].beamCnt -= 2;
											if ( Player->shot[numNo][dirNo].beamCnt == 0 ){

												Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
												Player->shot[numNo][dirNo].beamCnt = BEAMMAX;		//ビームのヒット回数を初期化.
												Player->shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
												Player->shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
											}
										}
										else{
											Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
											Player->shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
											Player->shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
										}
									}

									break;	//当たればfor文を抜ける.
								}
							}
						}
					}
				}
			}
		}
	}
}

//プレイヤーの弾とボスの当たり判定.
void PStoB_DC( PLAYER* Player, BOSS* boss )
{
	for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
		for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
			//自機の弾が発射されている.
			if ( Player->shot[numNo][dirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
					//ボスの生存確認.
					if ( boss->chara[typeNo].State == enCharaState::Living ) {
						//自機の弾と敵機の当たり判定.
						if ( ( ( Player->ability.chg == 0 ) && ( CollisionDetection(
							( int ) Player->shot[numNo][dirNo].x,	( int ) Player->shot[numNo][dirNo].y,	S_SIZE,					 S_SIZE,
							( int ) boss->chara[typeNo].x,			( int ) boss->chara[typeNo].y,			boss->charaSize[typeNo], boss->charaSize[typeNo] ) ) ) ||

							( ( Player->ability.chg != 0 ) && ( CollisionDetection(
							( int ) Player->shot[numNo][dirNo].x - S_SIZE,	( int ) Player->shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,					S_SIZE * 2,
							( int ) boss->chara[typeNo].x,					( int ) boss->chara[typeNo].y,					boss->charaSize[typeNo],	boss->charaSize[typeNo] ) ) ) ) {

							//ボスのダメージSEを鳴らす.
							CSoundManager::PlaySE( CSoundManager::SE_EDamage );

							//HPを減らす.
							boss->status[typeNo].hp -= Player->status.atk;
							if ( boss->status[typeNo].hp <= 0 ) {
								boss->chara[typeNo].State		= enCharaState::Dying;	//状態を死亡中にする.
								boss->chara[typeNo].ExpAnimCnt	= 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
								boss->chara[typeNo].chgCnt		= 0;
								boss->AttackNo[typeNo]			= enBossAttackNo::BWait;
								//本体がやられた場合両腕を死亡中にする.
								if ( typeNo == 0 ) {
									//ボスの入退出SEの再生.
									CSoundManager::PlaySE( CSoundManager::SE_BEarth );

									for ( int i = enBoss::RHand; i < boss->BOSS_TYPE; i++ ) {
										if ( ( boss->chara[i].State == enCharaState::Living ) || ( boss->chara[i].State == enCharaState::Invi ) ) {
											boss->chara[i].State = enCharaState::Dying;		//状態を死亡中にする.
											boss->chara[i].ExpAnimCnt = 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
											boss->chara[i].chgCnt = 0;
										}
									}
								}

								//追尾終了.
								Player->shot[numNo][dirNo].homingCnt = HOMINGMAX;
							}
							else {
								//状態を無敵にする.
								boss->chara[typeNo].State = enCharaState::Invi;
								boss->chara[typeNo].inviTime = INVITIME;
							}

							//自機の弾の着弾後の処理.
							if ( Player->ability.pene == 0 ) {	//貫通をもっていない.
								if ( Player->ability.beam != 0 ) {
									//ビームのヒット回数+2.
									Player->shot[numNo][dirNo].beamCnt -= 2;
									if ( Player->shot[numNo][dirNo].beamCnt == 0 ) {

										Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
										Player->shot[numNo][dirNo].beamCnt = BEAMMAX;		//ビームのヒット回数を初期化.
										Player->shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
										Player->shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
									}
								}
								else {
									Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
									Player->shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
									Player->shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
								}
							}
							break;	//当たればfor文を抜ける.
						}
					}
				}
			}
		}
	}
}

//プレイヤーの弾とステージギミックの当たり判定.
void PStoG_DC( PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss )
{
	for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
		for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
			//自機の弾が発射されている.
			if ( Player->shot[numNo][dirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GWait ) {
								//自機の弾とステージギミックの当たり判定.
								if ( Player->ability.chg == 0 ) {
									if ( CollisionDetection(
										( int ) Player->shot[numNo][dirNo].x, ( int ) Player->shot[numNo][dirNo].y, S_SIZE, S_SIZE,
										( int ) gimmick[typeNo][ptnNo][gNo].x, ( int ) gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) ) {

										//ステージギミックの生存確認.
										if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GLiving ) {
											//木なら.
											if ( typeNo == 0 ) {
												if ( boss->chara[enBoss::Main].State == enCharaState::Wait ){
													gimmick[typeNo][ptnNo][gNo].hp -= Player->status.atk;

													//葉っぱ出現.
													int DispNum = rand() % 2;
													int DispCnt = 0;
													for ( int LNo = 0; LNo < W_HITMAX; LNo++ ){
														if ( WoodLeaf[LNo].DispFlag == false ){
															WoodLeaf[LNo].DispFlag = true;
															WoodLeaf[LNo].x = ( int )( gimmick[typeNo][ptnNo][gNo].x + rand() % C_SIZE );
															WoodLeaf[LNo].y = ( int )( gimmick[typeNo][ptnNo][gNo].y + rand() % C_SIZE );
															WoodLeaf[LNo].FallPower = rand() % 3;
															WoodLeaf[LNo].Speed = rand() % 7 + 2;
															WoodLeaf[LNo].PtnX = rand() % 3;
															WoodLeaf[LNo].PtnY = rand() % 2;
															WoodLeaf[LNo].Ang = 0;

															DispCnt++;
														}
														if ( DispCnt >= DispNum ){
															break;
														}
													}

													if ( gimmick[typeNo][ptnNo][gNo].hp <= 0 ) {

														//木の破壊SEを鳴らす.
														CSoundManager::PlaySE( CSoundManager::SE_GTreeDest );

														gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GDead;	//状態を死亡にする.
														gimmick[typeNo][ptnNo][gNo].wSize = C_SIZE;
														gimmick[typeNo][ptnNo][gNo].hSize = C_SIZE;
													}
													else {

														//ギミックのダメージSEを鳴らす.
														CSoundManager::PlaySE( CSoundManager::SE_GDamage );

														gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GInvi;
														gimmick[typeNo][ptnNo][gNo].inviTime = INVITIME;
													}
												}
											}
											//自機の弾の着弾後の処理.
											if ( typeNo < 2 ) {
												if ( Player->ability.pene == 0 ) {	//貫通をもっていない.
													if ( Player->ability.beam != 0 ) {
														//ビームのヒット回数+2.
														Player->shot[numNo][dirNo].beamCnt -= 2;
														if ( Player->shot[numNo][dirNo].beamCnt == 0 ) {

															Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
															Player->shot[numNo][dirNo].beamCnt = BEAMMAX;		//ビームのヒット回数を初期化.
															Player->shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
															Player->shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
														}
													}
													else {
														Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
														Player->shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
														Player->shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
													}
												}
											}
										}
									}
								}
								else {
									if ( CollisionDetection(
										( int ) Player->shot[numNo][dirNo].x - S_SIZE, ( int ) Player->shot[numNo][dirNo].y - S_SIZE, S_SIZE * 2, S_SIZE * 2,
										( int ) gimmick[typeNo][ptnNo][gNo].x, ( int ) gimmick[typeNo][ptnNo][gNo].y, gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize ) ) {

										switch ( typeNo )
										{
										case 0:	//木.
											if ( boss->chara[enBoss::Main].State == enCharaState::Wait ){
												if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GDead ) {	//切り株.

													//ギミックのダメージSEを鳴らす.
													CSoundManager::PlaySE( CSoundManager::SE_GDamage );

													gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GWait;
													gimmick[typeNo][ptnNo][gNo].x = -WND_W;
													gimmick[typeNo][ptnNo][gNo].y = -WND_H;
													gimmick[typeNo][ptnNo][gNo].wSize = C_SIZE;
													gimmick[typeNo][ptnNo][gNo].hSize = C_SIZE + C_SIZE / 2;
												}
												else {	//木.

													//木の破壊SEを鳴らす.
													CSoundManager::PlaySE( CSoundManager::SE_GTreeDest );

													gimmick[typeNo][ptnNo][gNo].hp = 0;
													gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GDead;	//状態を死亡にする.
													gimmick[typeNo][ptnNo][gNo].wSize = C_SIZE;
													gimmick[typeNo][ptnNo][gNo].hSize = C_SIZE;

													//着弾後処理.
													Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
													Player->shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
													Player->shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.

													//葉っぱ出現.
													int DispNum = rand() % 3 + 1;
													int DispCnt = 0;
													for ( int LNo = 0; LNo < W_HITMAX; LNo++ ){
														if ( WoodLeaf[LNo].DispFlag == false ){
															WoodLeaf[LNo].DispFlag = true;
															WoodLeaf[LNo].x = ( int )( gimmick[typeNo][ptnNo][gNo].x + rand() % C_SIZE );
															WoodLeaf[LNo].y = ( int )( gimmick[typeNo][ptnNo][gNo].y + rand() % C_SIZE );
															WoodLeaf[LNo].FallPower = rand() % 3;
															WoodLeaf[LNo].Speed = rand() % 7 + 2;
															WoodLeaf[LNo].PtnX = rand() % 3;
															WoodLeaf[LNo].PtnY = rand() % 2;
															WoodLeaf[LNo].Ang = 0;

															DispCnt++;
														}
														if ( DispCnt >= DispNum ){
															break;
														}
													}
												}
											}
											break;
										case 1:	//岩.

											//岩の破壊SEを鳴らす.
											CSoundManager::PlaySE( CSoundManager::SE_GRockDest );

											gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GWait;
											gimmick[typeNo][ptnNo][gNo].x = -WND_W;
											gimmick[typeNo][ptnNo][gNo].y = -WND_H;

											//着弾後処理.
											Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
											Player->shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
											Player->shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
											break;
										default:
											if ( !( ( typeNo == 2 ) && ( ptnNo == 1 ) ) ){
												//ギミックのダメージSEを鳴らす.
												CSoundManager::PlaySE( CSoundManager::SE_GDamage );

												gimmick[typeNo][ptnNo][gNo].State = enGimmickState::GWait;
												gimmick[typeNo][ptnNo][gNo].x = -WND_W;
												gimmick[typeNo][ptnNo][gNo].y = -WND_H;
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//プレイヤーの弾と敵のバリアの当たり判定.
void PStoEB_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int numNo = 0; numNo < Player->status.num; numNo++ ) {
		for ( int dirNo = 0; dirNo < Player->status.dir; dirNo++ ) {
			//自機の弾が発射されている.
			if ( Player->shot[numNo][dirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							if ( ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) || ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) ){
								//バリアを持っている.
								if ( Enemy[typeNo][ptnNo][eNo].ability.barrUP != 0 ){
									for ( int bNo = 0; bNo < Enemy[typeNo][ptnNo][eNo].ability.barrUP * BARRMAX; bNo++ ) {
										//自機の弾と敵のバリアの当たり判定.
										if ( ( ( Player->ability.chg == 0 ) && ( CollisionDetection(
											 ( int )Player->shot[numNo][dirNo].x,				( int )Player->shot[numNo][dirNo].y,				S_SIZE,		S_SIZE,
											 ( int )Enemy[typeNo][ptnNo][eNo].barrier[bNo].x,	( int )Enemy[typeNo][ptnNo][eNo].barrier[bNo].y,	S_SIZE,		S_SIZE ) ) ) ||

											 ( ( Player->ability.chg != 0 ) && ( CollisionDetection(
											 ( int )Player->shot[numNo][dirNo].x - S_SIZE,		( int )Player->shot[numNo][dirNo].y - S_SIZE,		S_SIZE * 2, S_SIZE * 2,
											 ( int )Enemy[typeNo][ptnNo][eNo].barrier[bNo].x,	( int )Enemy[typeNo][ptnNo][eNo].barrier[bNo].y,	S_SIZE,		S_SIZE ) ) ) ) {

											//自機の弾の着弾後の処理.
											if ( Player->ability.pene == 0 ){	//貫通をもっていない.

												//バリアSEを鳴らす.
												CSoundManager::PlaySE( CSoundManager::SE_Barr );

												if ( Player->ability.beam != 0 ){
													//ビームのヒット回数+2.
													Player->shot[numNo][dirNo].beamCnt -= 2;
													if ( Player->shot[numNo][dirNo].beamCnt == 0 ){

														Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
														Player->shot[numNo][dirNo].beamCnt		= BEAMMAX;	//ビームのヒット回数を初期化.
														Player->shot[numNo][dirNo].x			= -WND_W;	//自機の弾x座標:画面の外へ.
														Player->shot[numNo][dirNo].y			= -WND_H;	//自機の弾y座標:画面の外へ.
													}
												}
												else{
													Player->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
													Player->shot[numNo][dirNo].x			= -WND_W;	//自機の弾x座標:画面の外へ.
													Player->shot[numNo][dirNo].y			= -WND_H;	//自機の弾y座標:画面の外へ.
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//敵の弾とプレイヤーの当たり判定.
void EStoP_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
		for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
			for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
				for ( int numNo = 0; numNo < Enemy[typeNo][ptnNo][eNo].status.num; numNo++ ) {
					for ( int dirNo = 0; dirNo < Enemy[typeNo][ptnNo][eNo].status.dir; dirNo++ ) {
						//敵の弾が発射されている.
						if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) {
							//プレイヤーの生存確認.
							if ( Player->chara.State == enCharaState::Living ) {
								//自機の弾と敵機の当たり判定.
								if ( ( ( typeNo == 1 ) && ( CollisionDetection(
									( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE / 2, ( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2, S_SIZE * 2, S_SIZE * 2,
									( int )Player->chara.x + PH_CORR,									( int )Player->chara.y + PH_CORR,									PH_SIZE,	PH_SIZE ) ) ) ||

									( ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
									( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x,			( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y,			S_SIZE,			S_SIZE,
									( int )Player->chara.x + PH_CORR,								( int )Player->chara.y + PH_CORR,								PH_SIZE,		PH_SIZE ) ) ) ||

									( ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
									( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE, ( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,		S_SIZE * 2,
									( int )Player->chara.x + PH_CORR,								( int )Player->chara.y + PH_CORR,								PH_SIZE,		PH_SIZE ) ) ) ){

									//プレイヤーのダメージSEを鳴らす.
									CSoundManager::PlaySE( CSoundManager::SE_PDamage );

									//HPを減らす.
									Player->status.hp -= Enemy[typeNo][ptnNo][eNo].status.atk;
									if ( Player->status.hp <= 0 ){
										Player->chara.State			= enCharaState::Dying;	//状態を死亡中にする.
										Player->chara.ExpAnimCnt	= 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
										Player->DeadNo				= typeNo;				//敵(種類)で死亡.
									}
									else{
										//状態を無敵にする.
										Player->chara.State = enCharaState::Invi;
										Player->chara.inviTime = PINVITIME;
									}

									//敵の弾の着弾後の処理.
									if ( Enemy[typeNo][ptnNo][eNo].ability.pene == 0 ){	//貫通をもっていない.
										if ( Enemy[typeNo][ptnNo][eNo].ability.beam != 0 ){
											//ビームのヒット回数+2.
											Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt -= 2;
											if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt == 0 ){

												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt = BEAMMAX;		//ビームのヒット回数を初期化.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
											}
										}
										else{
											Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
											Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
											Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
										}
									}

									break;	//当たればfor文を抜ける.
								}
							}
						}
					}
				}
			}
		}
	}
}

//敵の弾とステージギミックの当たり判定.
void EStoG_DC( ENEMY Enemy[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss )
{
	for ( int EtypeNo = 0; EtypeNo < E_TYPE; EtypeNo++ ) {
		for ( int EptnNo = 0; EptnNo < E_PTN; EptnNo++ ) {
			for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
				for ( int numNo = 0; numNo < Enemy[EtypeNo][EptnNo][eNo].status.num; numNo++ ) {
					for ( int dirNo = 0; dirNo < Enemy[EtypeNo][EptnNo][eNo].status.dir; dirNo++ ) {
						//自機の弾が発射されている.
						if ( Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) {
							for ( int GtypeNo = 0; GtypeNo < G_TYPE; GtypeNo++ ) {
								for ( int GptnNo = 0; GptnNo < G_PTN; GptnNo++ ) {
									for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
										if ( gimmick[GtypeNo][GptnNo][gNo].State != enGimmickState::GWait ) {
											//自機の弾とステージギミックの当たり判定.
											if ( Enemy[EtypeNo][EptnNo][eNo].ability.chg == 0 ) {
												if ( ( ( EtypeNo == 1 ) && ( CollisionDetection(
													( int )Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x - S_SIZE / 2,	( int )Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2,	S_SIZE * 2,								S_SIZE * 2,
													( int )gimmick[GtypeNo][GptnNo][gNo].x,									( int )gimmick[GtypeNo][GptnNo][gNo].y,									gimmick[GtypeNo][GptnNo][gNo].wSize,	gimmick[GtypeNo][GptnNo][gNo].hSize ) ) ) ||

													( ( EtypeNo != 1 ) && ( CollisionDetection(
													( int ) Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x,	( int ) Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y,	S_SIZE,									S_SIZE,
													( int ) gimmick[GtypeNo][GptnNo][gNo].x,					( int ) gimmick[GtypeNo][GptnNo][gNo].y,					gimmick[GtypeNo][GptnNo][gNo].wSize,	gimmick[GtypeNo][GptnNo][gNo].hSize ) ) ) ) {

													//ステージギミックの生存確認.
													if ( gimmick[GtypeNo][GptnNo][gNo].State == enGimmickState::GLiving ) {
														//木なら.
														if ( GtypeNo == 0 ) {
															if ( boss->chara[enBoss::Main].State == enCharaState::Wait ){
																gimmick[GtypeNo][GptnNo][gNo].hp -= Enemy[EtypeNo][EptnNo][eNo].status.atk;

																//葉っぱ出現.
																int DispNum = rand() % 2;
																int DispCnt = 0;
																for ( int LNo = 0; LNo < W_HITMAX; LNo++ ){
																	if ( WoodLeaf[LNo].DispFlag == false ){
																		WoodLeaf[LNo].DispFlag = true;
																		WoodLeaf[LNo].x = ( int )( gimmick[GtypeNo][GptnNo][gNo].x + rand() % C_SIZE );
																		WoodLeaf[LNo].y = ( int )( gimmick[GtypeNo][GptnNo][gNo].y + rand() % C_SIZE );
																		WoodLeaf[LNo].FallPower = rand() % 3;
																		WoodLeaf[LNo].Speed = rand() % 7 + 2;
																		WoodLeaf[LNo].PtnX = rand() % 3;
																		WoodLeaf[LNo].PtnY = rand() % 2;
																		WoodLeaf[LNo].Ang = 0;

																		DispCnt++;
																	}
																	if ( DispCnt >= DispNum ){
																		break;
																	}
																}

																if ( gimmick[GtypeNo][GptnNo][gNo].hp <= 0 ) {

																	//木破壊SEを鳴らす.
																	CSoundManager::PlaySE( CSoundManager::SE_GTreeDest );

																	gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GDead;	//状態を死亡にする.
																	gimmick[GtypeNo][GptnNo][gNo].wSize = C_SIZE;
																	gimmick[GtypeNo][GptnNo][gNo].hSize = C_SIZE;
																}
																else {

																	//ギミックのダメージSEを鳴らす.
																	CSoundManager::PlaySE( CSoundManager::SE_GDamage );

																	gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GInvi;
																	gimmick[GtypeNo][GptnNo][gNo].inviTime = INVITIME;
																}
															}
														}
														//自機の弾の着弾後の処理.
														if ( GtypeNo < 2 ) {
															if ( Enemy[EtypeNo][EptnNo][eNo].ability.pene == 0 ) {	//貫通をもっていない.
																if ( Enemy[EtypeNo][EptnNo][eNo].ability.beam != 0 ) {
																	//ビームのヒット回数+2.
																	Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].beamCnt -= 2;
																	if ( Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].beamCnt == 0 ) {

																		Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
																		Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].beamCnt = BEAMMAX;		//ビームのヒット回数を初期化.
																		Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
																		Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
																	}
																}
																else {
																	Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;		//発射ﾌﾗｸﾞを降ろす.
																	Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x = -WND_W;					//自機の弾x座標:画面の外へ.
																	Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y = -WND_H;					//自機の弾y座標:画面の外へ.
																}
															}
														}
													}
												}
											}
											else {
												if ( CollisionDetection(
													( int ) Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x - S_SIZE,	( int ) Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,								S_SIZE * 2,
													( int ) gimmick[GtypeNo][GptnNo][gNo].x,							( int ) gimmick[GtypeNo][GptnNo][gNo].y,							gimmick[GtypeNo][GptnNo][gNo].wSize,	gimmick[GtypeNo][GptnNo][gNo].hSize ) ) {

													switch ( GtypeNo )
													{
													case 0:	//木.
														if ( boss->chara[enBoss::Main].State == enCharaState::Wait ){
															if ( gimmick[GtypeNo][GptnNo][gNo].State == enGimmickState::GDead ) {	//切り株.

																//ギミックのダメージSEを鳴らす.
																CSoundManager::PlaySE( CSoundManager::SE_GDamage );

																gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GWait;
																gimmick[GtypeNo][GptnNo][gNo].x = -WND_W;
																gimmick[GtypeNo][GptnNo][gNo].y = -WND_H;
																gimmick[GtypeNo][GptnNo][gNo].wSize = C_SIZE;
																gimmick[GtypeNo][GptnNo][gNo].hSize = C_SIZE + C_SIZE / 2;
															}
															else {	//木.

																//木破壊SEを鳴らす.
																CSoundManager::PlaySE( CSoundManager::SE_GTreeDest );

																gimmick[GtypeNo][GptnNo][gNo].hp = 0;
																gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GDead;	//状態を死亡にする.
																gimmick[GtypeNo][GptnNo][gNo].wSize = C_SIZE;
																gimmick[GtypeNo][GptnNo][gNo].hSize = C_SIZE;

																//着弾後処理.
																Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
																Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x = -WND_W;	//自機の弾x座標:画面の外へ.
																Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y = -WND_H;	//自機の弾y座標:画面の外へ.

																//葉っぱ出現.
																int DispNum = rand() % 3 + 1;
																int DispCnt = 0;
																for ( int LNo = 0; LNo < W_HITMAX; LNo++ ){
																	if ( WoodLeaf[LNo].DispFlag == false ){
																		WoodLeaf[LNo].DispFlag = true;
																		WoodLeaf[LNo].x = ( int )( gimmick[GtypeNo][GptnNo][gNo].x + rand() % C_SIZE );
																		WoodLeaf[LNo].y = ( int )( gimmick[GtypeNo][GptnNo][gNo].y + rand() % C_SIZE );
																		WoodLeaf[LNo].FallPower = rand() % 3;
																		WoodLeaf[LNo].Speed = rand() % 7 + 2;
																		WoodLeaf[LNo].PtnX = rand() % 3;
																		WoodLeaf[LNo].PtnY = rand() % 2;
																		WoodLeaf[LNo].Ang = 0;

																		DispCnt++;
																	}
																	if ( DispCnt >= DispNum ){
																		break;
																	}
																}
															}
														}
														break;
													case 1:	//岩.

														//岩破壊SEを鳴らす.
														CSoundManager::PlaySE( CSoundManager::SE_GRockDest );

														gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GWait;
														gimmick[GtypeNo][GptnNo][gNo].x		= -WND_W;
														gimmick[GtypeNo][GptnNo][gNo].y		= -WND_H;

														//着弾後処理.
														Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
														Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].x			= -WND_W;	//自機の弾x座標:画面の外へ.
														Enemy[EtypeNo][EptnNo][eNo].shot[numNo][dirNo].y			= -WND_H;	//自機の弾y座標:画面の外へ.
														break;
													default:
														if ( !( ( GtypeNo == 2 ) && ( GptnNo == 1 ) ) ){

															//ギミックのダメージSEを鳴らす.
															CSoundManager::PlaySE( CSoundManager::SE_GDamage );

															gimmick[GtypeNo][GptnNo][gNo].State = enGimmickState::GWait;
															gimmick[GtypeNo][GptnNo][gNo].x = -WND_W;
															gimmick[GtypeNo][GptnNo][gNo].y = -WND_H;
															break;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//敵の弾とプレイヤーのバリアの当たり判定.
void EStoPB_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	//プレイヤーがバリアを持っている.
	if ( Player->ability.barrUP != 0 ){
		for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
			for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
				for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
					for ( int numNo = 0; numNo < Enemy[typeNo][ptnNo][eNo].status.num; numNo++ ) {
						for ( int dirNo = 0; dirNo < Enemy[typeNo][ptnNo][eNo].status.dir; dirNo++ ) {
							//敵の弾が発射されている.
							if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) {
								for ( int bNo = 0; bNo < Player->ability.barrUP * BARRMAX; bNo++ ) {
									//敵の弾とプレイヤーのバリアの当たり判定.
									if ( ( ( typeNo == 1 ) && ( CollisionDetection(
										( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE / 2, ( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2, S_SIZE * 2, S_SIZE * 2,
										( int )Player->barrier[bNo].x,										( int )Player->barrier[bNo].y,										S_SIZE,		S_SIZE ) ) ) ||

										( ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
										( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x,			( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y,			S_SIZE,		S_SIZE,
										( int )Player->barrier[bNo].x,									( int )Player->barrier[bNo].y,									S_SIZE,		S_SIZE ) ) ) ||

										( ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
										( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE, ( int )Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2, S_SIZE * 2,
										( int )Player->barrier[bNo].x,									( int )Player->barrier[bNo].y,									S_SIZE,		S_SIZE ) ) ) ){

										//敵の弾の着弾後の処理.
										if ( Enemy[typeNo][ptnNo][eNo].ability.pene == 0 ){	//貫通をもっていない.

											//バリアSEを鳴らす.
											CSoundManager::PlaySE( CSoundManager::SE_Barr );

											if ( Enemy[typeNo][ptnNo][eNo].ability.beam != 0 ){
												//ビームのヒット回数+2.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt -= 2;
												if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt == 0 ){

													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt = BEAMMAX;		//ビームのヒット回数を初期化.
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
												}
											}
											else{
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x = -WND_W;				//自機の弾x座標:画面の外へ.
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y = -WND_H;				//自機の弾y座標:画面の外へ.
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//ボスの弾とプレイヤーの当たり判定.
void BStoP_DC( PLAYER* Player, BOSS* boss )
{
	for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
		for ( int dirNo = 0; dirNo < boss->status[enBoss::Main].dir; dirNo++ ) {
			//敵の弾が発射されている.
			if ( boss->shot[numNo][dirNo].DispShotFlag == true ) {
				//プレイヤーの生存確認.
				if ( Player->chara.State == enCharaState::Living ) {
					//自機の弾と敵機の当たり判定.
					if ( CollisionDetection(
						( int ) boss->shot[numNo][dirNo].x, ( int ) boss->shot[numNo][dirNo].y,			S_SIZE,		S_SIZE,
						( int ) Player->chara.x + PH_CORR,			 ( int ) Player->chara.y + PH_CORR,	PH_SIZE,	PH_SIZE ) ) {

						//ダメージSEを鳴らす.
						CSoundManager::PlaySE( CSoundManager::SE_PDamage );

						//HPを減らす.
						Player->status.hp -= boss->status[enBoss::Main].atk;
						if ( Player->status.hp <= 0 ) {
							Player->chara.State			= enCharaState::Dying;	//状態を死亡中にする.
							Player->chara.ExpAnimCnt	= 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
							Player->DeadNo				= enDeedNo::Golem;		//ボスで死亡.
						}
						else {
							//状態を無敵にする.
							Player->chara.State = enCharaState::Invi;
							Player->chara.inviTime = PINVITIME;
						}

						//敵の弾の着弾後の処理.
						boss->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
						boss->shot[numNo][dirNo].x			  = -WND_W;	//自機の弾x座標:画面の外へ.
						boss->shot[numNo][dirNo].y			  = -WND_H;	//自機の弾y座標:画面の外へ.

						break;	//当たればfor文を抜ける.
					}
				}
			}
		}
	}
}

//ボスの弾とプレイヤーのバリアの当たり判定.
void BStoPB_DC( PLAYER* Player, BOSS* boss )
{
	//プレイヤーがバリアを持っている.
	if ( Player->ability.barrUP != 0 ) {
		for ( int numNo = 0; numNo < boss->BS_NUM; numNo++ ) {
			for ( int dirNo = 0; dirNo < boss->status[enBoss::Main].dir; dirNo++ ) {
				//敵の弾が発射されている.
				if ( boss->shot[numNo][dirNo].DispShotFlag == true ) {
					for ( int bNo = 0; bNo < Player->ability.barrUP * BARRMAX; bNo++ ) {
						//敵の弾とプレイヤーのバリアの当たり判定.
						if ( CollisionDetection(
							 ( int ) boss->shot[numNo][dirNo].x - S_SIZE,	( int ) boss->shot[numNo][dirNo].y - S_SIZE,	S_SIZE, S_SIZE,
							 ( int ) Player->barrier[bNo].x,				( int ) Player->barrier[bNo].y,					S_SIZE,	S_SIZE ) ) {

							//バリアSEを鳴らす.
							CSoundManager::PlaySE( CSoundManager::SE_Barr );

							//敵の弾の着弾後の処理.
							boss->shot[numNo][dirNo].DispShotFlag = false;	//発射ﾌﾗｸﾞを降ろす.
							boss->shot[numNo][dirNo].x			  = -WND_W;	//自機の弾x座標:画面の外へ.
							boss->shot[numNo][dirNo].y			  = -WND_H;	//自機の弾y座標:画面の外へ.
						}
					}
				}
			}
		}
	}
}

//プレイヤーの弾と敵の弾の当たり判定.
void PStoES_DC( PLAYER* Player, ENEMY Enemy[E_TYPE][E_PTN][E_MAX] )
{
	for ( int PnumNo = 0; PnumNo < Player->status.num; PnumNo++ ) {
		for ( int PdirNo = 0; PdirNo < Player->status.dir; PdirNo++ ) {
			//自機の弾が発射されている.
			if ( Player->shot[PnumNo][PdirNo].DispShotFlag == true ) {
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							for ( int EnumNo = 0; EnumNo < Enemy[typeNo][ptnNo][eNo].status.num; EnumNo++ ) {
								for ( int EdirNo = 0; EdirNo < Enemy[typeNo][ptnNo][eNo].status.dir; EdirNo++ ) {
									//敵の弾が発射されている.
									if ( Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].DispShotFlag == true ) {
										//自機の弾と敵機の当たり判定.
										if ( ( ( Player->ability.chg == 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x,							 ( int )Player->shot[PnumNo][PdirNo].y,								S_SIZE,		S_SIZE,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x,			 ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y,			S_SIZE,		S_SIZE ) ) ) ||

											 ( ( Player->ability.chg == 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x,							 ( int )Player->shot[PnumNo][PdirNo].y,								S_SIZE,		S_SIZE,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x - S_SIZE, ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y - S_SIZE,	S_SIZE * 2, S_SIZE * 2 ) ) ) ||
											
											 ( ( Player->ability.chg == 0 ) && ( typeNo == 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x,								 ( int )Player->shot[PnumNo][PdirNo].y,									S_SIZE,		S_SIZE,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x - S_SIZE / 2, ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y - S_SIZE / 2,	S_SIZE * 2, S_SIZE * 2 ) ) ) ||

											 ( ( Player->ability.chg != 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x - S_SIZE,					 ( int )Player->shot[PnumNo][PdirNo].y - S_SIZE,					S_SIZE * 2, S_SIZE * 2,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x,			 ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y,			S_SIZE,		S_SIZE ) ) ) ||

											 ( ( Player->ability.chg != 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) && ( typeNo != 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x - S_SIZE,					 ( int )Player->shot[PnumNo][PdirNo].y - S_SIZE,					S_SIZE * 2, S_SIZE * 2,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x - S_SIZE, ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y - S_SIZE,	S_SIZE * 2, S_SIZE * 2 ) ) ) ||
												 
											 ( ( Player->ability.chg != 0 ) && ( typeNo == 1 ) && ( CollisionDetection(
											   ( int )Player->shot[PnumNo][PdirNo].x - S_SIZE,						 ( int )Player->shot[PnumNo][PdirNo].y - S_SIZE,						S_SIZE * 2, S_SIZE * 2,
											   ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x - S_SIZE / 2, ( int )Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y - S_SIZE / 2,	S_SIZE * 2, S_SIZE * 2 ) ) ) )
										{
											//自機の弾の着弾後の処理.
											if ( !( ( Player->ability.pene != 0 ) ||
												    ( ( Player->ability.beam != 0 ) || ( Player->ability.chg != 0 ) ) && ( Enemy[typeNo][ptnNo][eNo].ability.pene == 0 ) ||
												    ( ( Player->ability.pene == 0 ) && ( Player->ability.beam == 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.chg == 0 ) &&
													  ( Enemy[typeNo][ptnNo][eNo].ability.beam == 0 ) && ( Enemy[typeNo][ptnNo][eNo].ability.beam == 0 ) && ( Player->ability.chg == 0 ) ) ) )
											{
												if ( Player->ability.beam != 0 ){
													//ビームのヒット回数+2.
													Player->shot[PnumNo][PdirNo].beamCnt -= 2;
													if ( Player->shot[PnumNo][PdirNo].beamCnt == 0 ){

														Player->shot[PnumNo][PdirNo].DispShotFlag	= false;	//発射ﾌﾗｸﾞを降ろす.
														Player->shot[PnumNo][PdirNo].beamCnt		= BEAMMAX;	//ビームのヒット回数を初期化.
														Player->shot[PnumNo][PdirNo].x				= -WND_W;	//自機の弾x座標:画面の外へ.
														Player->shot[PnumNo][PdirNo].y				= -WND_H;	//自機の弾y座標:画面の外へ.
													}
												}
												else{
													Player->shot[PnumNo][PdirNo].DispShotFlag		= false;	//発射ﾌﾗｸﾞを降ろす.
													Player->shot[PnumNo][PdirNo].x					= -WND_W;	//自機の弾x座標:画面の外へ.
													Player->shot[PnumNo][PdirNo].y					= -WND_H;	//自機の弾y座標:画面の外へ.
												}
											}

											//敵の弾の着弾後の処理.
											if ( !( ( ( ( Enemy[typeNo][ptnNo][eNo].ability.chg != 0 ) || ( Enemy[typeNo][ptnNo][eNo].ability.beam != 0 ) ) &&
													  ( ( Player->ability.chg == 0 ) && ( Player->ability.beam == 0 ) && ( Player->ability.pene == 0 ) ) ) ||
													( ( Enemy[typeNo][ptnNo][eNo].ability.pene != 0 ) && ( Player->ability.pene == 0 ) ) ) )
											{
												if ( Enemy[typeNo][ptnNo][eNo].ability.beam != 0 ){
													//ビームのヒット回数+2.
													Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].beamCnt -= 2;
													if ( Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].beamCnt == 0 ){

														Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].DispShotFlag	= false;	//発射ﾌﾗｸﾞを降ろす.
														Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].beamCnt		= BEAMMAX;	//ビームのヒット回数を初期化.
														Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x			= -WND_W;	//自機の弾x座標:画面の外へ.
														Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y			= -WND_H;	//自機の弾y座標:画面の外へ.
													}
												}
												else{
													Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].DispShotFlag		= false;	//発射ﾌﾗｸﾞを降ろす.
													Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].x				= -WND_W;	//自機の弾x座標:画面の外へ.
													Enemy[typeNo][ptnNo][eNo].shot[EnumNo][EdirNo].y				= -WND_H;	//自機の弾y座標:画面の外へ.
												}
											}
											break;	//当たればfor文を抜ける.
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//プレイヤーの弾とボスの弾の当たり判定.
void PStoBS_DC( PLAYER* Player, BOSS* boss )
{
	for ( int PnumNo = 0; PnumNo < Player->status.num; PnumNo++ ) {
		for ( int PdirNo = 0; PdirNo < Player->status.dir; PdirNo++ ) {
			//自機の弾が発射されている.
			if ( Player->shot[PnumNo][PdirNo].DispShotFlag == true ) {
				for ( int BnumNo = 0; BnumNo < boss->BS_NUM; BnumNo++ ) {
					for ( int BdirNo = 0; BdirNo < boss->status[enBoss::Main].dir; BdirNo++ ) {
						//敵の弾が発射されている.
						if ( boss->shot[BnumNo][BdirNo].DispShotFlag == true ) {
							//自機の弾と敵機の当たり判定.
							if ( ( ( Player->ability.chg == 0 ) && ( CollisionDetection(
								 ( int ) Player->shot[PnumNo][PdirNo].x,			( int ) Player->shot[PnumNo][PdirNo].y,			 S_SIZE,	 S_SIZE,
								 ( int ) boss->shot[BnumNo][BdirNo].x,				( int ) boss->shot[BnumNo][BdirNo].y,			 S_SIZE,	 S_SIZE ) ) ) ||

								 ( ( Player->ability.chg != 0 ) && ( CollisionDetection(
								 ( int ) Player->shot[PnumNo][PdirNo].x - S_SIZE,	( int ) Player->shot[PnumNo][PdirNo].y - S_SIZE, S_SIZE * 2, S_SIZE * 2,
								 ( int ) boss->shot[BnumNo][BdirNo].x,				( int ) boss->shot[BnumNo][BdirNo].y,			 S_SIZE,	 S_SIZE ) ) ) ) {

								//自機の弾の着弾後の処理.
								if ( Player->ability.pene == 0 ){
									if ( Player->ability.beam != 0 ) {
										//ビームのヒット回数+2.
										Player->shot[PnumNo][PdirNo].beamCnt -= 2;
										if ( Player->shot[PnumNo][PdirNo].beamCnt == 0 ) {

											Player->shot[PnumNo][PdirNo].DispShotFlag	= false;	//発射ﾌﾗｸﾞを降ろす.
											Player->shot[PnumNo][PdirNo].beamCnt		= BEAMMAX;	//ビームのヒット回数を初期化.
											Player->shot[PnumNo][PdirNo].x				= -WND_W;	//自機の弾x座標:画面の外へ.
											Player->shot[PnumNo][PdirNo].y				= -WND_H;	//自機の弾y座標:画面の外へ.
										}
									}
									else {
										Player->shot[PnumNo][PdirNo].DispShotFlag		= false;	//発射ﾌﾗｸﾞを降ろす.
										Player->shot[PnumNo][PdirNo].x					= -WND_W;	//自機の弾x座標:画面の外へ.
										Player->shot[PnumNo][PdirNo].y					= -WND_H;	//自機の弾y座標:画面の外へ.
									}
								}

								//敵の弾の着弾後の処理.
								boss->shot[BnumNo][BdirNo].DispShotFlag					= false;	//発射ﾌﾗｸﾞを降ろす.
								boss->shot[BnumNo][BdirNo].x							= -WND_W;	//自機の弾x座標:画面の外へ.
								boss->shot[BnumNo][BdirNo].y							= -WND_H;	//自機の弾y座標:画面の外へ.
								break;	//当たればfor文を抜ける.
							}
						}
					}
				}
			}
		}
	}
}
