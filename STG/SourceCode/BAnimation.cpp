#include "Main.h"
#include "CSoundManager.h"

//ボスの登場アニメーション.
void DispAnimation( BOSS* boss, SCROLL* scroll, PLAYER* player, CXInput* pXInput )
{
	//ボス出現アニメーションカウント+.
	boss->DispAnima.cnt++;
	if ( boss->DispAnima.cnt > boss->DispAnima.cntMax ) {
		if ( boss->DispAnima.No != boss->DispAnima.max ) {

			//振動開始.
			pXInput->SetVibration( 30000, 30000 );
			
			boss->DispAnima.No++;
			for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				boss->chara[typeNo].y -= 1.0;
			}

			//画面揺れ.
			scroll->BossDAng	 += 1.0;
			scroll->BossDispYCnt += 1;
			if ( scroll->BossDispYCnt >= 50 ) {
				scroll->BossDispYCnt = 0;
			}
		}
		else {

			//ボスの入退出SEの停止.
			CSoundManager::Stop( CSoundManager::SE_BEarth );

			//振動停止.
			pXInput->SetVibration( 0, 0 );

			for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				boss->chara[typeNo].State = enCharaState::Living;
			}
			//動作開始.
			player->KeyStop			= false;
			boss->DispFlag			= false;

			//初期化.
			boss->DispAnima.cnt		= 0;
			boss->DispAnima.No		= 0;
			scroll->BossDAng		= 0.0;
			scroll->BossDispYCnt	= 0;
		}
	}
	scroll->BossDispY = sin( scroll->BossDAng ) * ( 10 - scroll->BossDispYCnt / 5 );
	//scroll->BossDispY = sin( scroll->BossDAng ) * ( 10.0 - ( fmod( scroll->BossDAng, 10.0 ) ) );	//double型の％計算.
}

//ボスの死亡アニメーション.
bool DeadAnimation( BOSS* boss, SCROLL* scroll, PLAYER* player, CXInput* pXInput )
{
	//ボス出現アニメーションカウント+.
	boss->DispAnima.cnt++;
	if ( boss->DispAnima.cnt > boss->DispAnima.cntMax ) {
		if ( boss->DispAnima.No != boss->DispAnima.max ) {

			//振動開始.
			pXInput->SetVibration( 30000, 30000 );

			boss->DispAnima.No++;
			for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				boss->chara[typeNo].y += 1.0;
			}

			//画面揺れ.
			scroll->BossDAng += 1.0;
			scroll->BossDispYCnt += 1;
			if ( scroll->BossDispYCnt >= 50 ) {
				scroll->BossDispYCnt = 0;
			}
		}
		else {

			//ボスの入退出SEの停止.
			CSoundManager::Stop( CSoundManager::SE_BEarth );

			//振動停止.
			pXInput->SetVibration( 0, 0 );

			for ( int typeNo = 0; typeNo < boss->BOSS_TYPE; typeNo++ ) {
				boss->chara[typeNo].State = enCharaState::Living;
			}
			//動作開始.
			player->KeyStop			= false;
			boss->DispFlag			= false;

			//アニメーション全て終了.
			return true;
		}
	}
	scroll->BossDispY = sin( scroll->BossDAng ) * ( 10 - scroll->BossDispYCnt / 5 );

	//まだアニメーションの続きあり.
	return false;
}