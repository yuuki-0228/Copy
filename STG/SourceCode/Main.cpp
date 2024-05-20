#include "Main.h"
#include "CSoundManager.h"
#include <time.h>	//time()関数を使用するために必要.

//ｽﾚｯﾄﾞ関数.
DWORD WINAPI ThreadFunc( LPVOID vdParam )
{
	//変数宣言.
	GameWindow* gameWnd = nullptr;	//ｹﾞｰﾑｳｨﾝﾄﾞｳ構造体.

	DWORD frames	  = 0;	//ﾌﾚｰﾑ数.
	DWORD beforeTime  = 0;	//前の時間.
	char  MsgFPS[128]  = "";
	char  MsgE_St[128] = "";

	CXInput*	pXInput;	//コントローラのインスタンスを生成.
	XInStTrg	XInKey;		//コントローラのスティック、トリガー.

	HDC		hMemDC		= nullptr;	//ﾒﾓﾘﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	HFONT	hFont		= nullptr;	//ﾌｫﾝﾄﾊﾝﾄﾞﾙ.
	HDC		hWorkDC		= nullptr;	//作業用DC.
	HBITMAP	hWorkBmp	= nullptr;	//作業用BITMAPハンドル.
	HDC		hColorDC	= nullptr;	//単色用DC.
	HBITMAP	hColorBmp	= nullptr;	//単色用BITMAPハンドル.
	HDC		hPlgDC		= nullptr;	//画像の回転用DC.
	HBITMAP hPlgBmp		= nullptr;	//画像の回転用BITMAPハンドル.
	HDC		hDeatDC		= nullptr;	//死亡シーン用DC.
	HBITMAP hDeatBmp	= nullptr;	//死亡シーン用BITMAPハンドル.
	HBITMAP hscroll		= nullptr;	//ゲーム背景(スクロール).
	HBITMAP hhp_Gmck	= nullptr;	//HPバー、数字.
	HBITMAP hPHp		= nullptr;	//プレイヤーのHPバー.
	HBITMAP hGmck		= nullptr;	//ステージギミック.
	HBITMAP hTree		= nullptr;	//ステージギミック(木).
	HBITMAP hTreeHit	= nullptr;	//ステージギミック(木)ダメージ.
	HBITMAP hability	= nullptr;	//能力.
	HBITMAP harea		= nullptr;	//能力エリア、説明背景.
	HBITMAP hLetter		= nullptr;	//文字.
	HBITMAP hChara		= nullptr;	//キャラクター.
	HBITMAP hPSlime		= nullptr;	//キャラクターの見た目.
	HBITMAP hCharaHit	= nullptr;	//キャラクターダメージ.
	HBITMAP hPSlimeHit	= nullptr;	//キャラクターの見た目変更後ダメージ.
	HBITMAP hCharaDeat	= nullptr;	//キャラクター死亡.
	HBITMAP hSlime		= nullptr;	//スライム.
	HBITMAP hShot		= nullptr;	//弾.
	HBITMAP hboss		= nullptr;	//ボス,プレイヤーの弾.
	HBITMAP hSleep		= nullptr;	//敵(花)睡眠中エフェクト.
	HBITMAP hMainLR		= nullptr;	//ゲームメインの左右.
	HBITMAP hPause		= nullptr;	//ポーズ画像.

	//----- プレイヤー -----.
	static PLAYER	Player;
	Player.PushKey[enPushKey::Space]	= false;
	Player.PushKey[enPushKey::RT]		= false;
	Player.PushKey[enPushKey::LT]		= false;

	//----- 敵 -----.
	static ENEMY	Enemy[E_TYPE][E_PTN][E_MAX];
	PLANT		Plant[E_PTN][E_MAX];
	//----- ボス -----.
	static BOSS	Boss;
	//----- マップ -----.
	MAP			map;
	map.LastNo = 0;
	//----- ステージギミック -----.
	GIMMICK		gimmick[G_TYPE][G_PTN][G_MAX];
	POISON		Poison[G_MAX][P_MAX];
	int			PTime[G_MAX];
	int			PTimeCnt[G_MAX];
	LEAF		WoodLeaf[W_HITMAX];
	//----- 英語の高さ -----.		
	const int ENG_W[enENG_No::max] = { 160, 208, 192, 176, 192, 160, 152, 160, 96, 104, 96, 56 };
	//----- ゲームオーバー -----.
	const int GO_DispX [14 + 1] = { 584, 573, 592, 608, 592, 559, 573, 595, 592, 576, 608, 607, 559, 573,		0 };
	const int GO_DispY [14 + 1] = { 411, 429, 408, 394, 416, 406, 418, 370, 370, 370, 370, 370, 372, 370,		0 };
	const int GO_StartX[14 + 1] = { 128, 0,   0,   256, 256, 0,   0,   224, 128, 256, 0  , 64,  0,   128,		0 };
	const int GO_StartY[14 + 1] = { 0,   192, 0,   80,  0,   0,   96,  256, 256, 224, 256, 256, 128, 224,		0 };
	const int GO_wDest [14 + 1] = { 128, 128, 128, 128, 128, 160, 128, 160, 96,  128, 64,  64,  160, 128,		0 };
	const int GO_hDest [14 + 1] = { 112, 64,  96,  64,  80,  128, 96,  32,  32,  32,  32,  32,  32,  32 ,		0 };
	ANIMATION BatAnima;

	//背景スクロール用.
	SCROLL  scroll;	//y座標.

	HBITMAP	hTitle		= nullptr;	//ﾀｲﾄﾙ画像.
	HBITMAP hsky		= nullptr;	//空.
	HBITMAP hcloud		= nullptr;	//雲.
	HBITMAP hGameOver	= nullptr;	//ゲームオーバー画像.
	HBITMAP hDead		= nullptr;	//ゲームオーバーの死亡グラフィックの画像.
	HBITMAP hDeadG		= nullptr;	//ゲームオーバーの死亡グラフィック(ギミック)の画像.
	HBITMAP hEnding		= nullptr;	//エンディング画像.
	HBITMAP hEnSlime	= nullptr;	//エンディングのスライム画像.
	HBITMAP hEnClear	= nullptr;	//エンディングのゲームクリア.
	HBITMAP	hEnGround	= nullptr;	//エンディングの地面画像.
	HBITMAP hEnEnemy	= nullptr;	//エンディングの敵の画像.
	HBITMAP hEnEng		= nullptr;	//エンディングの文字.
	HBITMAP hEnEneEng	= nullptr;	//エンディングの敵の文字.
	HBITMAP hEnHitNum	= nullptr;	//エンディングのヒット数.

	//エンディング.
	static ENDING	Ending;

	//最高Hit数を読み込む.
	FILE *fp;

	fp = fopen( "Data\\CSV\\BESTHIT.csv", "r" );	//ファイルを開く.
	if ( fp == NULL ) {
		//ファイルが開けない場合.
		Ending.BestHit = 0;
	}
	else{
		fscanf( fp, "%d", &Ending.BestHit );
	}

	//ファイルを閉じる.
	fclose( fp );


	//ｼｰﾝ.
	enScene	Scene	= enScene::Title;	//ﾀｲﾄﾙ.

	//ポーズ.
	bool	pause		= false;
	bool	Stop		= false;
	int		pauseArea	= 0;
	int		pauseAlpha	= 255;

	//フェード.
	FADE	Fade	= { enFade::None, 0, enScene::Title, 255, true };

	//he .
	static TITLE	Title;
	//初期化.
	Title.Alpha						= 255;	//ﾀｲﾄﾙのPressEnterKeyで使う.
	Title.UPCloudX1					= WND_W - 256;
	Title.UPCloudX2					= -446;
	Title.DownCloudX1				= WND_W - ( WND_W / 2 - 50 );
	Title.DownCloudX2				= -( WND_W - ( WND_W / 2 - 50 ) );
	Title.Panimation.cnt			= 0;
	Title.Panimation.cntMax			= 20;
	Title.Panimation.Flag			= false;
	Title.Panimation.No				= 0;
	Title.Panimation.max			= 3;
	Title.LCosAng					= 0;
	Title.LDispMAX					= 0;
	Title.LTime						= 0;
	Title.LTimeCnt					= 0;
	for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
		Title.Leaf[LNo].x			= -16;
		Title.Leaf[LNo].y			= 0;
		Title.Leaf[LNo].PtnX		= 0;
		Title.Leaf[LNo].PtnY		= 0;
		Title.Leaf[LNo].Speed		= 5;
		Title.Leaf[LNo].FallPower	= 1;
		Title.Leaf[LNo].Ang			= 0.0;
		Title.Leaf[LNo].DispFlag	= false;
	}

	//ｹﾞｰﾑｳｨﾝﾄﾞｳ構造体を取得.
	gameWnd = (GameWindow*)vdParam;	//先頭ｱﾄﾞﾚｽをｷｬｽﾄして移す.

	//時間の取得.
	beforeTime = timeGetTime();		//現在の時間を取得.

	//ブラシの作成.
	HBRUSH hGBrush	= CreateSolidBrush( RGB( 0x00, 0xFF, 0x00 ) );	//緑色ブラシ.
	HBRUSH hBBrush	= CreateSolidBrush( RGB( 0x00, 0x00, 0x00 ) );	//黒色ブラシ.
	HBRUSH hGrBrush = CreateSolidBrush( RGB( 0x22, 0x22, 0x22 ) );	//灰色ブラシ.
	HBRUSH hRBrush	= CreateSolidBrush( RGB( 0xFF, 0x00, 0x00 ) );	//赤色ブラシ.

//-------------------------------------------------------------------.
//	起動処理[WM_CREATE].
//-------------------------------------------------------------------.
	//ﾒﾓﾘDCの作成.
	hMemDC = CreateCompatibleDC( nullptr );

	//作業用DCの作成.
	hWorkBmp = CreateCompatibleBitmap(
		gameWnd->hScreenDC, WND_W, WND_H );				//作業用BITMAPの作成.
	hWorkDC = CreateCompatibleDC( gameWnd->hScreenDC );	//作業用DCの作成.
	SelectObject( hWorkDC, hWorkBmp );					//DCとBITMAPを紐付け.

	//単色用DCの作成.
	hColorBmp = CreateCompatibleBitmap(
		gameWnd->hScreenDC, WND_W, WND_H );				//単色用BITMAPの作成.
	hColorDC = CreateCompatibleDC( gameWnd->hScreenDC );//単色用BITMAPの作成.
	SelectObject( hColorDC, hColorBmp );				//DCとBITMAPを紐付け.

	//画像の回転用DCの作成.
	hPlgBmp = CreateCompatibleBitmap(
		gameWnd->hScreenDC, WND_W, WND_H );				//画像の回転用BITMAPの作成.
	hPlgDC = CreateCompatibleDC( gameWnd->hScreenDC );	//画像の回転用DCの作成.
	SelectObject( hPlgDC, hPlgBmp );					//DCとBITMAPを紐付け.
	//背景を緑色にしておく.
	SelectObject( hPlgDC, hGBrush );
	PatBlt(
		hDeatDC,		//単色用DCへ出力.
		0, 0,			//表示位置x,y.
		C_SIZE, C_SIZE,	//画像の幅,高さ.
		PATCOPY );

	//死亡シーン用DCの作成.
	hDeatBmp = CreateCompatibleBitmap(
		gameWnd->hScreenDC, WND_W, WND_H );				//画像の回転用BITMAPの作成.
	hDeatDC = CreateCompatibleDC( gameWnd->hScreenDC );	//画像の回転用DCの作成.
	SelectObject( hDeatDC, hDeatBmp );					//DCとBITMAPを紐付け.
	//背景を灰色にしておく.
	SelectObject( hDeatDC, hGrBrush );
	PatBlt(
		hDeatDC,				//単色用DCへ出力.
		0, 0,					//表示位置x,y.
		C_SIZE * 3, C_SIZE * 3,	//画像の幅,高さ.
		PATCOPY );

	//コントローラのインスタンスを生成.
	pXInput = new CXInput( 0 );

	//サウンド管理クラスのインスタンス生成.
	CSoundManager::GetInstance()->SetWnd( gameWnd->hWnd );
	if ( CSoundManager::GetInstance()->LoadData() == false ) return 0;

	//ﾌｫﾝﾄの作成.
	hFont = CreateFont(
		13,								//文字の高さ.
		0,								//文字の幅.
		0,								//角度.
		0,								//ﾍﾞｰｽﾗｲﾝ角度.
		FW_REGULAR,						//文字の太さ(FW_BOLD:太文字、FW_REGULAR:通常).
		FALSE,							//斜体(TRUE:有効).
		FALSE,							//下線(TRUE:有効).
		FALSE,							//打ち消し線(TRUE:有効).
		SHIFTJIS_CHARSET,				//文字ｾｯﾄ.
		OUT_DEFAULT_PRECIS,				//出力精度.
		CLIP_DEFAULT_PRECIS,			//ｸﾘｯﾋﾟﾝｸﾞ精度.
		DEFAULT_QUALITY,				//出力品質.
		VARIABLE_PITCH | FF_ROMAN,		//可変ﾋﾟｯﾁとﾌｫﾝﾄﾌｧﾐﾘ.
		nullptr/*"HG創英角ﾎﾟｯﾌﾟ体"*/ );	//書体(nullptr:現在使われている書体).
	SelectObject( gameWnd->hScreenDC, hFont );

	//BITMAPの読み込み.
	/*メイン*/
	if ( LoadBmp( &hscroll,		"Data\\BMP\\background\\background.bmp"						) == false ) return 0;		//スクロールの読み込み.
	if ( LoadBmp( &hhp_Gmck,	"Data\\BMP\\iroiro\\iroiro.bmp"								) == false ) return 0;		//HPバー、ギミック.
	if ( LoadBmp( &hPHp,		"Data\\BMP\\iroiro\\hp_1204.bmp"							) == false ) return 0;		//プレイヤーのHPバー.
	if ( LoadBmp( &hGmck,		"Data\\BMP\\iroiro\\syougaibutu.bmp"						) == false ) return 0;		//ステージギミック.
	if ( LoadBmp( &hTree,		"Data\\BMP\\iroiro\\syuusei_1130.bmp"						) == false ) return 0;		//ステージギミック(木).
	if ( LoadBmp( &hTreeHit,	"Data\\BMP\\iroiro\\tuika_0120.bmp"							) == false ) return 0;		//ステージギミック(木)ダメージ.
	if ( LoadBmp( &hability,	"Data\\BMP\\passibu_subete\\passibu_1120.bmp"				) == false ) return 0;		//能力の読み込み.
	if ( LoadBmp( &harea,		"Data\\BMP\\passibu_kouka_hyouji\\shuusei_nado_1231.bmp"	) == false ) return 0;		//能力エリア.
	if ( LoadBmp( &hLetter,		"Data\\BMP\\iroiro\\moji_1210.bmp"							) == false ) return 0;		//文字.
	if ( LoadBmp( &hChara,		"Data\\BMP\\kyara\\kyara_1130.bmp"							) == false ) return 0;		//ｷｬﾗｸﾀｰの読み込み.
	if ( LoadBmp( &hPSlime,		"Data\\BMP\\kyara\\sulim_matome_2_0119.bmp"					) == false ) return 0;		//ｷｬﾗｸﾀｰの読み込み.
	if ( LoadBmp( &hCharaHit,	"Data\\BMP\\kyara\\kyara_hit_1130.bmp"						) == false ) return 0;		//キャラクターダメージの読み込み.
	if ( LoadBmp( &hPSlimeHit,	"Data\\BMP\\kyara\\sulim_hit_matome_0120.bmp"				) == false ) return 0;		//キャラクターダメージの読み込み.
	if ( LoadBmp( &hCharaDeat,	"Data\\BMP\\kyara\\deth_1211.bmp"							) == false ) return 0;		//キャラクター死亡の読み込み.
	if ( LoadBmp( &hSlime,		"Data\\BMP\\iroiro\\shuusei_1225.bmp"						) == false ) return 0;		//スライムの読み込み.
	if ( LoadBmp( &hShot,		"Data\\BMP\\kyara\\tama_remake.bmp"							) == false ) return 0;		//弾の読み込み.
	if ( LoadBmp( &hboss,		"Data\\BMP\\kyara\\boss_jiki_tama.bmp"						) == false ) return 0;		//ボス,プレイヤーの弾の読み込み.
	if ( LoadBmp( &hSleep,		"Data\\BMP\\kyara\\tuika_2_0120.bmp"						) == false ) return 0;		//敵(花)の睡眠中エフェクト.
	if ( LoadBmp( &hMainLR,		"Data\\BMP\\scene\\gamescreen_right_left_1231.bmp"			) == false ) return 0;		//ゲームメインの左右.
	if ( LoadBmp( &hPause,		"Data\\BMP\\scene\\pouse.bmp"								) == false ) return 0;		//ポーズ画像.
	/*その他シーン*/
	if ( LoadBmp( &hTitle,		"Data\\BMP\\scene\\strat_1216.bmp"							) == false ) return 0;		//ﾀｲﾄﾙ画像の読み込み.
	if ( LoadBmp( &hsky,		"Data\\BMP\\scene\\sky_0109.bmp"							) == false ) return 0;		//空画像の読み込み.
	if ( LoadBmp( &hcloud,		"Data\\BMP\\scene\\gamestrat_shuusei_0109.bmp"				) == false ) return 0;		//雲画像の読み込み.
	if ( LoadBmp( &hGameOver,	"Data\\BMP\\scene\\gameover.bmp"							) == false ) return 0;		//ｹﾞｰﾑｵｰﾊﾞｰ画像の読み込み.
	if ( LoadBmp( &hDead,		"Data\\BMP\\scene\\gameover_nado_0106.bmp"					) == false ) return 0;		//ｹﾞｰﾑｵｰﾊﾞｰ画像の読み込み.
	if ( LoadBmp( &hDeadG,		"Data\\BMP\\scene\\gameover_tuika_0107.bmp"					) == false ) return 0;		//ｹﾞｰﾑｵｰﾊﾞｰ画像の読み込み.
	if ( LoadBmp( &hEnding,		"Data\\BMP\\scene\\haikei.bmp"								) == false ) return 0;		//ｴﾝﾃﾞｨﾝｸﾞ画像の読み込み.
	if ( LoadBmp( &hEnSlime,	"Data\\BMP\\scene\\gameclear_slimu.bmp"						) == false ) return 0;		//ｴﾝﾃﾞｨﾝｸﾞ画像の読み込み.
	if ( LoadBmp( &hEnClear,	"Data\\BMP\\scene\\gameclear_zairyou.bmp"					) == false ) return 0;		//ｴﾝﾃﾞｨﾝｸﾞ画像の読み込み.
	if ( LoadBmp( &hEnGround,	"Data\\BMP\\scene\\kusa_golem.bmp"							) == false ) return 0;		//ｴﾝﾃﾞｨﾝｸﾞ画像の読み込み.
	if ( LoadBmp( &hEnEnemy,	"Data\\BMP\\scene\\gameclear_kyara.bmp"						) == false ) return 0;		//ｴﾝﾃﾞｨﾝｸﾞ画像の読み込み.
	if ( LoadBmp( &hEnEng,		"Data\\BMP\\scene\\moji_nado.bmp"							) == false ) return 0;		//ｴﾝﾃﾞｨﾝｸﾞ画像の読み込み.
	if ( LoadBmp( &hEnEneEng,	"Data\\BMP\\scene\\teki_moji.bmp"							) == false ) return 0;		//ｴﾝﾃﾞｨﾝｸﾞ画像の読み込み.
	if ( LoadBmp( &hEnHitNum,	"Data\\BMP\\scene\\moji_hit.bmp"							) == false ) return 0;		//ｴﾝﾃﾞｨﾝｸﾞ画像の読み込み.

	//タイトルBGMの再生.
	CSoundManager::GetInstance()->PlayLoop( CSoundManager::BGM_Title );

	//乱数の初期化(種の設定).
	srand( ( unsigned int ) time( nullptr ) );

	//ｳｨﾝﾄﾞｳが閉じられるまで無限ﾙｰﾌﾟ.
	while( gameWnd->isWndClose == FALSE )
	{
		DWORD nowTime		= 0;	//現在の時間.
		DWORD progressTime	= 0;	//経過の時間.
		DWORD idealtime		= 0;	//理想の時間.

		nowTime = timeGetTime();	//現在の時間を取得.

		progressTime = nowTime - beforeTime;	//経過時間.

		//理想時間の算出.
		// 現在のﾌﾚｰﾑ数 × ( 1000 ÷ FPS値 ).
		idealtime = (DWORD) ( frames * ( 1000.0f / gameWnd->dwFPS ) );

		//コントローラー.
		pXInput->Update();
		XInKey.stL_X = pXInput->GetLThumbX();								//LスティックのX座標を更新.
		XInKey.stL_Y = pXInput->GetLThumbY();								//LスティックのY座標を更新.
		XInKey.stR_X = pXInput->GetRThumbX();								//RスティックのX座標を更新.
		XInKey.stR_Y = pXInput->GetRThumbY();								//RスティックのY座標を更新.
		XInKey.trgL = pXInput->GetLTrigger();								//Lトリガーの状態を更新.
		XInKey.trgR = pXInput->GetRTrigger();								//Rトリガーの状態を更新.

		//葉っぱ用COS角度(風の制御).
		Title.LCosAng++;
		if ( Title.LCosAng >= 360 ){
			Title.LCosAng = 0;
		}

//-------------------------------------------------------------------.
//	ｹﾞｰﾑ処理[WM_TIMER].
//-------------------------------------------------------------------.
		//ｼｰﾝごとの処理.
		if ( ( pause == false ) && ( Stop == false ) ) {
			switch ( Scene ) {
			case enScene::Title:	//ﾀｲﾄﾙ.
			{
				//BGM再生( ループ ).
				CSoundManager::PlayLoop( CSoundManager::BGM_Title );

				//上背景ｽｸﾛｰﾙ処理.
				if ( Title.UPCloudX1 < WND_W ) {
					Title.UPCloudX1 += 0.9f;		//ｽｸﾛｰﾙ値を減らす.
				}
				else {
					Title.UPCloudX1 = -WND_W;		//初期値に戻す.
				}
				if ( Title.UPCloudX2 < WND_W ) {
					Title.UPCloudX2 += 0.9f;		//ｽｸﾛｰﾙ値を減らす.
				}
				else {
					Title.UPCloudX2 = -WND_W;		//初期値に戻す.
				}

				//下背景ｽｸﾛｰﾙ処理.
				if ( Title.DownCloudX1 < WND_W ) {
					Title.DownCloudX1 += 0.2f;		//ｽｸﾛｰﾙ値を減らす.0.2
				}
				else {
					Title.DownCloudX1 = -WND_W;		//初期値に戻す.
				}
				if ( Title.DownCloudX2 < WND_W ) {
					Title.DownCloudX2 += 0.20f;		//ｽｸﾛｰﾙ値を減らす.0.2
				}
				else {
					Title.DownCloudX2 = -WND_W;		//初期値に戻す.
				}

				//PressEnterKeyを点滅させる.
				if ( Title.Alpha <= 255 ) {		// 0(不透明) ~ 255(透明).
					Title.Alpha += 3;			//透明にしていく.
					if ( Title.Alpha >= 255 ) {	//透明になったら.
						Title.Alpha = 510;		// 255 => 511 にする(透明).
					}
				}
				else {							// 256(不透明) ~ 511(透明).
					Title.Alpha -= 3;			//不透明にしていく.
					if ( Title.Alpha <= 255 ) {	//不透明になったら.
						Title.Alpha = 0;		// 256 => 0 にする(不透明).
					}
				}

				//スライムの触覚のアニメーション.
				if ( Title.Panimation.Flag == false ){
					Title.Panimation.cnt++;
					if ( Title.Panimation.cnt > Title.Panimation.cntMax ) {
						Title.Panimation.cnt = 0;
						Title.Panimation.No++;
						if ( Title.Panimation.No >= Title.Panimation.max ) {
							Title.Panimation.No -= 2;
							Title.Panimation.Flag = true;
						}
					}
				}
				else{
					Title.Panimation.cnt++;
					if ( Title.Panimation.cnt > Title.Panimation.cntMax ) {
						Title.Panimation.cnt = 0;
						Title.Panimation.No--;
						if ( Title.Panimation.No <= 0 ) {
							Title.Panimation.Flag = false;
						}
					}
				}

				//葉っぱの出現.
				if ( Title.LTimeCnt >= Title.LTime ) {
					//次の出現タイムを決める.
					Title.LTimeCnt = 0;
					Title.LTime = rand() % 10 + 5;

					//出現数を決める.
					Title.LDispMAX = rand() % 5 + 2;
					int DispCnt = 0;

					//葉っぱの出現出現.
					for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
						if ( Title.Leaf[LNo].DispFlag == false ) {
							//出現.
							Title.Leaf[LNo].DispFlag	= true;
							Title.Leaf[LNo].x			= -16;
							Title.Leaf[LNo].y			= rand() % WND_H - WND_H / 3;
							Title.Leaf[LNo].FallPower	= rand() % 2 + 1;
							Title.Leaf[LNo].Speed		= rand() % 7 + 2;
							Title.Leaf[LNo].PtnX		= rand() % 3;
							Title.Leaf[LNo].PtnY		= rand() % 2;
							Title.Leaf[LNo].Ang			= 0;

							DispCnt++;	//出現数カウント.

							//全て出したら.
							if ( DispCnt >= Title.LDispMAX ) {
								Title.LDispMAX = 0;
								break;
							}
						}
					}
				}
				else {
					//葉っぱの待ち時間カウント＋.
					Title.LTimeCnt++;
				}

				//空を舞う葉っぱの動作.
				for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
					if ( Title.Leaf[LNo].DispFlag == true ) {
						//回転させる.
						Title.Leaf[LNo].Ang += 0.02;
						if ( Title.Leaf[LNo].Ang >= 360 ){
							Title.Leaf[LNo].Ang = 0;
						}

						//風で飛ばす.
						Title.Leaf[LNo].x += Title.Leaf[LNo].Speed + static_cast< int >( cos( RAD( Title.LCosAng ) - 10 ) * Title.Leaf[LNo].Speed / 2 );
						Title.Leaf[LNo].y += Title.Leaf[LNo].FallPower;		//落下.

						//画面外に出た時.
						if ( Title.Leaf[LNo].x > WND_W ) {
							Title.Leaf[LNo].DispFlag = false;
						}
					}
				}

				if ( GetAsyncAnyKey( &Player, pXInput, &XInKey ) ) {
					if ( Player.PushKey[enPushKey::Space] == false ) {
						//決定SEを鳴らす.
						CSoundManager::PlaySE( CSoundManager::SE_Decision );
						
						//初期化処理.
						{
							//-----自機の初期化-----.
							//初期配置.
							Player.KeyStop				= false;
							Player.chara.x				= ( GWND_W / 2 ) - ( C_SIZE / 2 );	//ｾﾝﾀﾘﾝｸﾞ(横を中央に揃える).
							Player.chara.y				= WND_H;							//補正値:16上に配置.
							Player.chara.ang			= 0;								//角度.
							Player.chara.State			= enCharaState::Living;				//生存中.
							Player.chara.ExpAnimCnt		= 0;								//爆発カウント.
							Player.chara.chgCnt			= 0;								//溜めカウント.
							Player.chara.ShotDirAng		= 20;								//弾同士の間の間隔.
							Player.chara.healCnt		= 0;								//回復待ち時間カウント.
							Player.XInArea				= 0;								//コントローラの削除場所.
							Player.HPFrameNo			= enHPFrame::HPHigh;				//HPのフレームの種類.
							for ( int KeyNo = 0; KeyNo < enPushKey::Max; KeyNo++ ) {
								Player.PushKey[KeyNo] = false;
							}
							//アニメーション.
							Player.animation.No			= 0;
							Player.animation.max		= 2;
							Player.animation.cnt		= 0;
							Player.animation.cntMax		= 25;
							Player.animation.Flag		= false;
							//ターゲット初期化.
							Player.target.x				= GWND_W / 2 - C_SIZE / 2;
							Player.target.y				= -C_SIZE;
							//初期ステータス.
							Player.initial_status.hp = 15;
							Player.initial_status.atk	= 1;
							Player.initial_status.dir	= 1;
							Player.initial_status.num	= 1;
							Player.initial_status.cspd	= 5;
							Player.status				= Player.initial_status;	//能力を取得.
							//能力エリアの初期化.
							for ( int i = 0; i < ABLT_MAX; i++ ) {
								Player.chara.abArea[i] = 0;
							}
#ifdef _DEBUG				/*** ▼ ********* 能力エリア *********** ▼ ***/
							Player.chara.abArea[0] = pene;
							Player.chara.abArea[1] = atkUP;
							Player.chara.abArea[2] = atkUP;
							Player.chara.abArea[3] = numUP;
#endif						/*** ▲ ********* 能力エリア *********** ▲ ***/
							P_UpdateAblt( &Player );	//能力の取得状態の更新.
							//能力入手削除アニメーション.
							for ( int abltNo = 0; abltNo < ABLT_MAX; abltNo++ ) {
								Player.DGAbi[abltNo].xy.x		= 0;
								Player.DGAbi[abltNo].xy.y		= 0;
								Player.DGAbi[abltNo].AbiNo		= 0;
								Player.DGAbi[abltNo].DispFlag	= false;
								Player.DGAbi[abltNo].ExpaFlag	= false;
								Player.DGAbi[abltNo].Expa		= 0.0f;
								Player.DGAbi[abltNo].Alpha		= 255;
							}

							//バリアの初期化.
							for ( int bNo = 0; bNo < ABLT_MAX * BARRMAX; bNo++ ) {
								Player.barrier[bNo].x			= WND_W;
								Player.barrier[bNo].y			= WND_H;
								Player.barrier[bNo].C_x			= 0;
								Player.barrier[bNo].C_y			= 0;
								Player.barrier[bNo].length_x	= C_SIZE;
								Player.barrier[bNo].length_y	= C_SIZE;
								Player.barrier[bNo].ang			= 15 * bNo + 13 * ( bNo / BARRMAX );
							}

							//回復エフェクトの初期化.
							for ( int hNo = 0; hNo < H_MAX; hNo++ ){
								Player.HeelAnima[hNo].x					= 0;
								Player.HeelAnima[hNo].y					= 0;
								Player.HeelAnima[hNo].UpY				= 0;
								Player.HeelAnima[hNo].PtnX				= 0;
								Player.HeelAnima[hNo].PtnY				= 0;
								Player.HeelAnima[hNo].Alpha				= 255;
								Player.HeelAnima[hNo].DispFlag			= false;
								Player.HeelAnima[hNo].animation.cnt		= 0;
								Player.HeelAnima[hNo].animation.cntMax	= 5;
								Player.HeelAnima[hNo].animation.No		= 0;
								Player.HeelAnima[hNo].animation.max		= 2;
								Player.HeelAnima[hNo].animation.Flag	= false;
							}

							//-----自機の弾の初期化-----.
							for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
								for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
									Player.shotCnt							= 0;
									Player.shot[numNo][dirNo].x				= -WND_W;
									Player.shot[numNo][dirNo].y				= -WND_H;
									Player.shot[numNo][dirNo].DispShotFlag	= false;
									Player.shot[numNo][dirNo].homingCnt		= 0;
									Player.shot[numNo][dirNo].beamCnt		= BEAMMAX;
									Player.shot[numNo][dirNo].sspd			= PS_SPD;
								}
								Player.NumShotFlag[numNo]					= false;
							}
							Player.chgAniCnt	= 0;
							Player.chgAniFlag	= false;
							Player.chgNumCnt	= 0;
							Player.DeadNo		= enDeedNo::DeedNone;
							Player.SlimeNo		= enSlime::SNormal;
							Player.NewSlimeNo	= 0;
							for ( int i = 0; i < 2; i++ ){
								Player.Damage[i].x			= 0;
								Player.Damage[i].y			= 0;
								Player.Damage[i].DispFlag	= false;
							}
							Player.AbGetFlag	= false;
							Player.GetAnimaCnt	= 0;
							Player.GetAnimaX	= 0;
							Player.GetAnimaY	= 0;

							//-----敵機の初期化-----.
							for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
								for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
									for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
										//初期配置.
										Enemy[typeNo][ptnNo][eNo].chara.x			= WND_W;
										Enemy[typeNo][ptnNo][eNo].chara.y			= WND_H;
										Enemy[typeNo][ptnNo][eNo].chara.ang			= 0;
										Enemy[typeNo][ptnNo][eNo].chara.State		= enCharaState::Wait;	//生存中.
										Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt	= 0;
										Enemy[typeNo][ptnNo][eNo].chara.chgCnt		= 0;
										Enemy[typeNo][ptnNo][eNo].chara.ShotDirAng	= 20;
										Enemy[typeNo][ptnNo][eNo].chara.healCnt		= 0;
										Enemy[typeNo][ptnNo][eNo].EGetAbNo			= 0;
										//アニメーションの初期化.
										Enemy[typeNo][ptnNo][eNo].animetion.No		= 0;
										Enemy[typeNo][ptnNo][eNo].animetion.max		= 2;
										Enemy[typeNo][ptnNo][eNo].animetion.cnt		= 0;
										Enemy[typeNo][ptnNo][eNo].animetion.Flag	= false;
										//アニメーションカウントの上限設定.
										int AnimeMAX[E_TYPE]						= { 25, 25, 25, 20, 10 };	//ハチ:10, コウモリ:20, 地上にいる敵:25.
										Enemy[typeNo][ptnNo][eNo].animetion.cntMax	= AnimeMAX[typeNo];
										//ターゲット初期化.
										Enemy[typeNo][ptnNo][eNo].target.x			= GWND_W / 2 - C_SIZE / 2;
										Enemy[typeNo][ptnNo][eNo].target.y			= -C_SIZE;
										//ステータスの取得.
										E_Status( Enemy );
										Enemy[typeNo][ptnNo][eNo].status			= Enemy[typeNo][ptnNo][eNo].initial_status;	//能力を取得.
										//能力の取得.
										E_Ability( Enemy );
										E_UpdateAblt( Enemy, typeNo, ptnNo, eNo );
										//バリアの初期化.
										for ( int bNo = 0; bNo < ( ABLT_MAX - 1 ) * BARRMAX; bNo++ ) {
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].x		= WND_W;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].y		= WND_H;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].C_x		= 0;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].C_y		= 0;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].length_x = C_SIZE;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].length_y = C_SIZE;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].ang		= 15 * bNo + 13 * ( bNo / BARRMAX );
										}
										//回復エフェクトの初期化.
										for ( int hNo = 0; hNo < H_MAX; hNo++ ){
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].x					= 0;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].y					= 0;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].UpY				= 0;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].PtnX				= 0;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].PtnY				= 0;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].Alpha				= 255;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].DispFlag			= false;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cnt		= 0;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cntMax	= 5;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No		= 0;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.max		= 2;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.Flag		= false;
										}
										//半回転初期化.
										Enemy[typeNo][ptnNo][eNo].half_rot.E_height = 90;
										Enemy[typeNo][ptnNo][eNo].half_rot.M_x		= 0;
										Enemy[typeNo][ptnNo][eNo].half_rot.M_y		= -C_SIZE;
										//敵の初期化.
										Enemy[typeNo][ptnNo][eNo].ran				= enEmRanAng::STOP;
										Enemy[typeNo][ptnNo][eNo].EmRanCnt			= 0;
										Enemy[typeNo][ptnNo][eNo].EshotCnt			= 0;
										Enemy[typeNo][ptnNo][eNo].EmMoveCnt			= 0;
										Enemy[typeNo][ptnNo][eNo].appFlag			= false;
										Enemy[typeNo][ptnNo][eNo].BossFlag			= false;
										Enemy[typeNo][ptnNo][eNo].LBE_ang			= 0;
										Enemy[typeNo][ptnNo][eNo].attack			= 0;
										//敵の弾.
										for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
											for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x				= WND_W;
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y				= WND_H;
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag	= false;
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt		= 0;
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt		= BEAMMAX;
												if ( typeNo == 0 ) {		//食虫植物.
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd = PS_SPD / 2;
												}
												else if ( typeNo == 4 ) {	//ハチ.
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd = PS_SPD - 3;
												}
												else {
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd = PS_SPD;
												}
											}
											Enemy[typeNo][ptnNo][eNo].NumShotFlag[numNo]					= false;
										}
									}
								}
							}
							//敵(花)の初期化.
							for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
								for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
									for ( int sNo = 0; sNo < 2; sNo++ ) {
										Plant[ptnNo][eNo].Sleep[sNo].x			= 0.0;
										Plant[ptnNo][eNo].Sleep[sNo].y			= 0.0;
										Plant[ptnNo][eNo].Sleep[sNo].ptnNo		= 2;
										Plant[ptnNo][eNo].Sleep[sNo].Alpha		= 255;
										Plant[ptnNo][eNo].Sleep[sNo].DispFlag	= false;

										Plant[ptnNo][eNo].Woke.x				= 0.0;
										Plant[ptnNo][eNo].Woke.y				= 0.0;
										Plant[ptnNo][eNo].Woke.AnimaX			= 0;
										Plant[ptnNo][eNo].Woke.AnimaY			= 0;
										Plant[ptnNo][eNo].Woke.AnimaCnt			= 0;
										Plant[ptnNo][eNo].Woke.Alpha			= 255;
										Plant[ptnNo][eNo].Woke.AlphaFlag		= false;

										Plant[ptnNo][eNo].State					= enPlantState::Sleeping;
									}
								}
							}

							//ボス.
							Boss.DispFlag			= false;
							Boss.charaSize[0]		= C_SIZE * 2;
							Boss.charaSize[1]		= C_SIZE * 1;
							Boss.charaSize[2]		= C_SIZE * 1;
							//初期座標.
							Boss.initial_xy[0].x	= GWND_W / 2 - Boss.charaSize[0] / 2;
							Boss.initial_xy[0].y	= C_SIZE;
							Boss.initial_xy[1].x	= GWND_W / 2 + Boss.charaSize[0] / 2;
							Boss.initial_xy[1].y	= Boss.charaSize[1] + C_SIZE;
							Boss.initial_xy[2].x	= GWND_W / 2 - Boss.charaSize[0];
							Boss.initial_xy[2].y	= Boss.charaSize[2] + C_SIZE;
							//HP.
							Boss.initial_status[enBoss::Main].hp	= 150;
							Boss.initial_status[enBoss::RHand].hp	= 50;
							Boss.initial_status[enBoss::LHand].hp	= 50;
							//初期化処理.
							Boss.AttackMove							= enBossMoveNo::EarlyForm1;
							Boss.AttackMovecnt						= 0;
							Boss.AttackCnt							= 0;
							Boss.Blowcnt							= 0;
							Boss.PatternCnt							= 1;							
							for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ) {
								//初期設定.
								Boss.chara[typeNo].x				= WND_W;
								Boss.chara[typeNo].y				= WND_H;
								Boss.chara[typeNo].ang				= RAD( 0 );
								Boss.chara[typeNo].State			= enCharaState::Wait;
								Boss.chara[typeNo].ExpAnimCnt		= 0;
								Boss.chara[typeNo].chgCnt			= 0;
								Boss.chara[typeNo].ShotDirAng		= 20;
								Boss.chara[typeNo].healCnt			= 0;
								//アニメーション.
								Boss.animation[typeNo].No			= 0;
								Boss.animation[typeNo].max			= 5;
								Boss.animation[typeNo].cnt			= 0;
								Boss.animation[typeNo].cntMax		= 5;
								Boss.animation[typeNo].Flag			= false;
								Boss.DispAnima.No					= 0;
								Boss.DispAnima.max					= C_SIZE * 2;
								Boss.DispAnima.cnt					= 0;
								Boss.DispAnima.cntMax				= 10;
								Boss.DispAnima.Flag					= false;
								//攻撃状態.
								Boss.AttackNo[typeNo]				= enBossAttackNo::BWait;
								//初期ステータス.
								Boss.initial_status[typeNo].atk		= 2;
								Boss.initial_status[typeNo].dir		= 1;
								Boss.initial_status[typeNo].num		= 1;
								Boss.initial_status[typeNo].cspd	= 5;
								Boss.status[typeNo]					= Boss.initial_status[typeNo];	//能力を取得.
								//能力エリアの初期化.
								for ( int abltNo = 0; abltNo < ABLT_MAX; abltNo++ ) {
									Boss.chara[typeNo].abArea[abltNo] = 0;
								}
								//-----ボスの弾の初期化-----.
								for ( int numNo = 0; numNo < Boss.BS_NUM; numNo++ ) {
									for ( int dirNo = 0; dirNo < Boss.BS_DIR; dirNo++ ) {
										Boss.shotCnt							= 0;
										Boss.shot[numNo][dirNo].x				= WND_W;
										Boss.shot[numNo][dirNo].y				= WND_H;
										Boss.shot[numNo][dirNo].DispShotFlag	= false;
										Boss.shot[numNo][dirNo].sspd			= PS_SPD;
										Boss.shot[numNo][dirNo].homingCnt		= 0;
										Boss.shot[numNo][dirNo].beamCnt			= BEAMMAX;
									}
									Boss.NumShotFlag[numNo] = false;
								}
								//-----突き上げ岩の初期化-----.
								for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
									for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
										Boss.rock[typeNo][numNo][dirNo].x			= WND_W;
										Boss.rock[typeNo][numNo][dirNo].y			= WND_H;
										Boss.rock[typeNo][numNo][dirNo].Rang		= 0;
										Boss.rock[typeNo][numNo][dirNo].cnt			= 0;
										Boss.rock[typeNo][numNo][dirNo].DispFlag	= false;

										//ステージギミック(岩)の出現角度.
										Boss.RockRAngData[numNo][dirNo]				= 0;
										Boss.RockDispNoData[numNo][dirNo]			= 0;
									}
								}
								//岩の数、方向初期化、弾同士の間隔.
								Boss.rockData[typeNo].Num	= 1;
								Boss.rockData[typeNo].Dir	= 1;
								Boss.rockData[typeNo].Ang	= 20;
								Boss.rockData[typeNo].Spaa	= 8;
								Boss.rockData[typeNo].Time	= 100;
								Boss.rockData[typeNo].iAng	= 90;
							}
							//ボスエリアの敵の表示データを取得.
							GetBEDispData( &Boss );
							//ボスの吹き飛ばしエフェクト.
							Boss.BlowAway.y			= 0;
							Boss.BlowAway.DispFlag	= false;

							//-----ステージギミック-----.
							for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
								for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
									for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
										gimmick[typeNo][ptnNo][gNo].x			= WND_W;
										gimmick[typeNo][ptnNo][gNo].y			= WND_H;
										gimmick[typeNo][ptnNo][gNo].hp			= 50;
										gimmick[typeNo][ptnNo][gNo].inviTime	= 0;
										gimmick[typeNo][ptnNo][gNo].State		= enGimmickState::GWait;
										gimmick[typeNo][ptnNo][gNo].TreeAlpha	= 255;
										gimmick[typeNo][ptnNo][gNo].RockFlag	= false;
										if ( typeNo == 0 ) {
											gimmick[typeNo][ptnNo][gNo].hSize	= C_SIZE + C_SIZE / 2;
											gimmick[typeNo][ptnNo][gNo].wSize	= C_SIZE;
										}
										else {
											gimmick[typeNo][ptnNo][gNo].hSize	= C_SIZE;
											gimmick[typeNo][ptnNo][gNo].wSize	= C_SIZE;
										}
									}
								}
							}
							for ( int LNo = 0; LNo < W_HITMAX; LNo++ ){
								WoodLeaf[LNo].x			= -16;
								WoodLeaf[LNo].y			= 0;
								WoodLeaf[LNo].PtnX		= 0;
								WoodLeaf[LNo].PtnY		= 0;
								WoodLeaf[LNo].Speed		= 5;
								WoodLeaf[LNo].FallPower	= 1;
								WoodLeaf[LNo].Ang		= 0.0;
								WoodLeaf[LNo].DispFlag	= false;
							}
							//ステージギミック(毒沼)のアニメーション.
							for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
								for ( int pNo = 0; pNo < P_MAX; pNo++ ) {
									Poison[gNo][pNo].x			= 0;
									Poison[gNo][pNo].y			= 0;
									Poison[gNo][pNo].Spd		= 0.0f;
									Poison[gNo][pNo].PtnNo		= 0;
									Poison[gNo][pNo].Alpha		= 255;
									Poison[gNo][pNo].AlSpd		= 0;
									Poison[gNo][pNo].DispFlag	= false;
								}
								PTime[gNo]		= 0;
								PTimeCnt[gNo]	= 0;
							}

							//-----マップの初期化-----.
							map.cnt			= 0;
							map.dispCnt		= 0;
							for ( int hNo = 0; hNo < MAP_H; hNo++ ) {
								for ( int wNo = 0; wNo < MAP_W; wNo++ ) {
									map.Data[hNo][wNo] = 0;
								}
							}
#ifdef _DEBUG				/*** ▼ ********* 開始位置[ ボス直前:100 ] *********** ▼ ***/
							map.dispCnt = 0;

#endif						/*** ▲ ********* 開始位置[ ボス直前:100 ] *********** ▲ ***/
							GetMap( &map );
							DispMap( &map, Enemy, Plant, gimmick, &Boss, &scroll, &Player );

							//----- ゲームオーバーアニメーション -----.
							BatAnima.cnt	= 0;
							BatAnima.cntMax = 10;
							BatAnima.No		= 0;
							BatAnima.max	= 2;
							BatAnima.Flag	= false;

							//-----背景ｽｸﾛｰﾙ-----.
							//初期配置.
							scroll.y			= WND_H;
							scroll.spd			= 1;
							scroll.stop			= true;
							scroll.BossDAng		= 0;
							scroll.BossDispY	= 0;
							scroll.BossDispYCnt = 0;
							scroll.Start		= true;

							//-----エンディング-----.
							Ending.Slime.GroundPos			= WND_H - ( C_SIZE * 2 + C_SIZE / 2 );
							Ending.Slime.x					= WND_W - C_SIZE * 4;
							Ending.Slime.y					= Ending.Slime.GroundPos;
							Ending.Slime.MoveSpeed			= 4;
							Ending.Slime.Action				= enAction::MoveLeft;
							Ending.Slime.Jumping			= false;
							Ending.Slime.JumpAcc			= 0.0f;
							Ending.Slime.JumpPower			= 24.0f;
							Ending.Slime.Gravity			= 0.8f;
							Ending.Slime.animation.cnt		= 0;
							Ending.Slime.animation.cntMax	= 25;
							Ending.Slime.animation.No		= 0;
							Ending.Slime.animation.max		= 2;
							Ending.Slime.animation.Flag		= false;
							for ( int sNo = 0; sNo < STARMAX; sNo++ ){
								Ending.Star[sNo].x			= 0;
								Ending.Star[sNo].y			= 0;
								Ending.Star[sNo].Ptn		= 0;
								Ending.Star[sNo].AlSpd		= 1;
								Ending.Star[sNo].Alpha		= 0;
								Ending.Star[sNo].DispFlag	= false;
								Ending.Star[sNo].Shooting	= false;
								Ending.Star[sNo].Spd		= 0;
							}
							Ending.SDispMAX					= 0;	//葉っぱの出現数.
							Ending.STime					= 0;	//次の葉っぱの出現待ち時間.
							Ending.STimeCnt					= 0;	//次の葉っぱの出現待ち時間カウント.
							for ( int dNo = 0; dNo < DROPMAX; dNo++ ){
								Ending.Drop[dNo].x			= 0;
								Ending.Drop[dNo].y			= 0;
								Ending.Drop[dNo].Ptn		= 0;
								Ending.Drop[dNo].DispFlag	= false;
							}
							Ending.StartX[0]				= 0;
							Ending.StartY[0]				= C_SIZE * 4;
							Ending.wDest [0]				= 32;
							Ending.hDest [0]				= 32;
							Ending.StartX[1]				= 32;
							Ending.StartY[1]				= C_SIZE * 4;
							Ending.wDest [1]				= 16;
							Ending.hDest [1]				= 16;
							Ending.StartX[2]				= 32;
							Ending.StartY[2]				= C_SIZE * 4 + 16;
							Ending.wDest [2]				= 16;
							Ending.hDest [2]				= 16;
							Ending.DTime					= 0;
							Ending.DTimeCnt					= 0;
							Ending.StopCnt					= 0;
							Ending.ClearY					= 230;

							//Hit数が最大Hit数より高い場合上書きする.
							if ( Ending.Hit > Ending.BestHit ){
								//最高Hit数を上書き.
								Ending.BestHit = Ending.Hit;
							}
							Ending.Hit						= 0;

							// -1 ： 空白.
							const int StartX[ENGMAX] = {   0, 224,   0, -1, 256, 256, -1,   0, 224,   0, -1, 448,   0,   0,  96,  96,   0,   0, -1,   0,   0,   0, 192, 320, 320,   0, -1,   0,   0, 288,   0, -1, 0,			    -1,   0, -1,   0 };
							const int StartY[ENGMAX] = {  96, 128, 160, -1,  96, 160, -1, 128, 128, 160, -1,  64,  32,   0,  32,   0,  64,  96, -1,  64, 192, 256, 256, 192, 224, 224, -1,  32, 336, 296, 296, -1, WND_H + TITLE_H, -1, 368, -1,   0 };
							const int wDest [ENGMAX] = { 256, 256, 256, -1, 200, 320, -1, 224, 256, 256, -1, 128,  96,  96,  64,  64, 128, 128, -1, 448, 320, 192, 256, 200, 160, 320, -1, 500, 288, 288, 288, -1, LOGO_W,		    -1, 256, -1, 576 };
							const int hDest [ENGMAX] = {  32,  32,  32, 32,  32,  32, 32,  32,  32,  32, 32,  32,  32,  32,  32,  32,  32,  32, 32,  32,  32,  40,  32,  32,  32,  32, 32,  32,  32,  32,  40, 32, LOGO_H,		    64, 144, 32,  32 };
							const int Corr	[ENGMAX] = { 110, 110, 110, -1,  85, 145, -1,  95, 110, 110, -1,  48,  36,  36,  20,  20,  48,  48, -1, 205, 145,  75, 110,  85,  63, 142, -1, 235, 130, 130, 122, -1, 0,			    -1,   0, -1, 270 };

							for ( int eNo = 0; eNo < ENGMAX; eNo++ ) {
								Ending.EnEng[eNo].StartX	= StartX[eNo];
								Ending.EnEng[eNo].StartY	= StartY[eNo];
								Ending.EnEng[eNo].wDest		= wDest [eNo];
								Ending.EnEng[eNo].hDest		= hDest [eNo];
								Ending.EnEng[eNo].x			= ( float ) ( WND_W / 2 - Ending.EnEng[eNo].wDest / 2 );
								Ending.EnEng[eNo].y			= ( float ) ( WND_H );
								Ending.EnEng[eNo].DispFlag	= false;

								Ending.EnEng[eNo].HitFlag	= false;
								Ending.EnEng[eNo].CosDAng	= 0.0f;
								Ending.EnEng[eNo].width		= 20.0f;
								Ending.EnEng[eNo].Dang		= 0.0f;
								Ending.EnEng[eNo].Corr		= Corr[eNo];
							}
							Ending.DispNo					= 0;
							Ending.DispTime					= 0;
							Ending.DispCnt					= 0;
							Ending.Space					= 16;
							Ending.Start					= false;

							//ゲームメインBGMの再生.
							CSoundManager::Stop( CSoundManager::BGM_Title );
							CSoundManager::PlayLoop( CSoundManager::BGM_Main );

							//フェードイン(ゲームメインへ).
							Stop			= true;
							Fade.FadeState	= enFade::in;
							Fade.EndScene	= enScene::GameMain;
						}
					}
				}
				else {
					Player.PushKey[enPushKey::Space] = false;
				}
				if ( GetAsyncKeyState( VK_ESCAPE ) & 0x8000 || pXInput->IsKeyDown( XINPUT_GAMEPAD_START ) ) {
					//ｳｨﾝﾄﾞｳを閉じることをWindowsに知らせる.
					PostMessage( gameWnd->hWnd, WM_CLOSE, 0, 0 );
				}
				break;
			}
			case enScene::GameMain:	//ｹﾞｰﾑﾒｲﾝ.
			{
				//BGM再生( ループ ).
				if ( Boss.chara[enBoss::Main].State == enCharaState::Wait ){
					CSoundManager::PlayLoop( CSoundManager::BGM_Main );
				}
				else{
					CSoundManager::PlayLoop( CSoundManager::BGM_Boss );
				}

				//HPバーの見た目Noを指定.
				if ( Player.status.hp <= Player.initial_status.hp / 3 ){
					Player.HPFrameNo = enHPFrame::HPLow;
					Player.Damage[0].DispFlag = true;
					Player.Damage[1].DispFlag = true;
				}
				else if ( Player.status.hp <= Player.initial_status.hp / 3 * 2 ){
					Player.HPFrameNo = enHPFrame::HPMiddle;
					Player.Damage[0].DispFlag = true;
					Player.Damage[1].DispFlag = false;
				}
				else{
					Player.HPFrameNo = enHPFrame::HPHigh;
					Player.Damage[0].DispFlag = false;
					Player.Damage[1].DispFlag = false;
				}

				//プレイヤーの操作.
				if ( Player.KeyStop != true ) {
					PlayerOp( &Player, gimmick, pXInput, XInKey, &scroll, Scene, Ending, &pause, &Fade );
				}
				
				//プレイヤー弾の動作.
				P_ShotControl( &Player, &Boss, Enemy, Scene );

				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							//敵の弾の動作.
							E_ShotControl( Enemy, typeNo, ptnNo, eNo, Player );
						}
					}
				}

				//ボスの動作.
				if ( ( Boss.chara[enBoss::Main].State == enCharaState::Living ) ||
					 ( Boss.chara[enBoss::Main].State == enCharaState::Invi ) ) {
					B_AttackMove( &Boss, Player, gimmick, Enemy );
				}
				for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ) {
					B_AttackControl( &Boss, typeNo, &scroll, &Player, gimmick, Enemy );
				}

				//弾の当たり判定.
				S_CollisionDetection( &Player, Enemy, gimmick, WoodLeaf, &Boss );

				//自機の生存確認.
				if ( Player.chara.State == enCharaState::Living ) {
					for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
						for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
							for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
								//敵機の生存確認.
								if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) {
									//自機と敵機の当たり判定.
									if ( CollisionDetection(
										Player.chara.x + PH_CORR,			Player.chara.y + PH_CORR,			PH_SIZE,	PH_SIZE,
										Enemy[typeNo][ptnNo][eNo].chara.x,	Enemy[typeNo][ptnNo][eNo].chara.y,	C_SIZE,		C_SIZE ) )
									{
										//プレイヤーのダメージSEを鳴らす.
										CSoundManager::PlaySE( CSoundManager::SE_PDamage );

										//HPを減らす.
										Player.status.hp -= Enemy[typeNo][ptnNo][eNo].status.atk;
										if ( Player.status.hp <= 0 ) {
											Player.chara.State		= enCharaState::Dying;	//状態を死亡中にする.
											Player.chara.ExpAnimCnt = 0;					//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀを0にする.
											Player.DeadNo			= typeNo;				//敵(種類)で死亡.
										}
										else {
											//状態を無敵にする.
											Player.chara.State = enCharaState::Invi;
											Player.chara.inviTime = INVITIME * 2;
										}

										break;	//敵機と当たればfor文を抜ける.
									}
								}
							}
						}
					}
				}
				//自機の死亡中.
				else if ( Player.chara.State == enCharaState::Dying ) {
					//死亡アニメーション用座標ずらし.
					Player.chara.x--;
					Player.chara.y += 2;
					Player.chara.ExpAnimCnt++;

					if ( Player.chara.ExpAnimCnt > 15 ) {

						//フェードイン(ゲームオーバーへ).
						Stop				= true;
						Fade.FadeState		= enFade::in;
						Fade.EndScene		= enScene::GameOver;

						Player.chara.State	= enCharaState::Living;	//状態を生存中にする.

						//初期位置に戻す.
						Player.chara.x		= -WND_W;
						Player.chara.y		= -WND_H;

						Title.Alpha			= 255;

						//ゲームオーバーBGMの再生.
						CSoundManager::Stop		( CSoundManager::BGM_Main );
						CSoundManager::Stop		( CSoundManager::BGM_Boss );
						CSoundManager::PlayLoop	( CSoundManager::BGM_GameOver );
					}
				}

				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							//敵機が死亡中.
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dying ) {

								//敵機の爆発処理.
								Enemy[typeNo][ptnNo][eNo].chara.x--;
								Enemy[typeNo][ptnNo][eNo].chara.y += 2;
								Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt++;
								if ( Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt > 15 )//爆発画像が0～15の16ｺﾏ.
								{
									//死亡アニメーション用座標ずらし.
									Enemy[typeNo][ptnNo][eNo].chara.x += 15;
									Enemy[typeNo][ptnNo][eNo].chara.y -= 30;
									//状態を死亡にする.
									Enemy[typeNo][ptnNo][eNo].chara.State	= enCharaState::Dead;
									Enemy[typeNo][ptnNo][eNo].ran			= enEmRanAng::STOP;
								
									//能力をセット.
									if ( ( Enemy[typeNo][ptnNo][eNo].chara.abArea[0] <= enAbility::homing ) && ( Player.chara.abArea[0] == 0 ) ) {
										Enemy[typeNo][ptnNo][eNo].EGetAbNo = Enemy[typeNo][ptnNo][eNo].chara.abArea[0];
									}
									else {
										int getAbl = 0;	//ランダムで取得できる能力を選ぶ.
										//取得能力が決まるまでループ.
										do {
											//ランダムで選ぶ.
											getAbl = rand() % 10;

											//ランダムで選んだ場所の能力が取得できるか.
											if		( getAbl == 0 )																			{ Enemy[typeNo][ptnNo][eNo].EGetAbNo = Enemy[typeNo][ptnNo][eNo].chara.abArea[3]; }
											else if ( ( getAbl > 0 ) && ( getAbl <= 2 ) )													{ Enemy[typeNo][ptnNo][eNo].EGetAbNo = Enemy[typeNo][ptnNo][eNo].chara.abArea[2]; }
											else if ( ( getAbl > 2 ) && ( getAbl <= 5 ) )													{ Enemy[typeNo][ptnNo][eNo].EGetAbNo = Enemy[typeNo][ptnNo][eNo].chara.abArea[1]; }
											else if ( ( getAbl > 5 ) && ( Enemy[typeNo][ptnNo][eNo].chara.abArea[0] > enAbility::homing ) ) { Enemy[typeNo][ptnNo][eNo].EGetAbNo = Enemy[typeNo][ptnNo][eNo].chara.abArea[0]; }
										} while ( Enemy[typeNo][ptnNo][eNo].EGetAbNo == 0 );
									}
								}
							}
						}
					}
				}

				for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ) {
					//ボスが死亡中.
					if ( Boss.chara[typeNo].State == enCharaState::Dying ) {
						if ( typeNo == enBoss::Main ){
							//操作を不可能に.
							Player.KeyStop = true;
							//死亡アニメーションを行い、アニメーション終了通知を受け取った時.
							if ( DeadAnimation( &Boss, &scroll, &Player, pXInput ) == true ){
								//画面外に.
								for ( int type = 0; type < Boss.BOSS_TYPE; type++ ){
									Boss.chara[type].x = -WND_W;
									Boss.chara[type].y = -WND_H;
								}

								//フェードイン(エンディングへ).
								Stop				= true;
								Fade.FadeState		= enFade::in;
								Fade.EndScene		= enScene::Ending;

								Title.Alpha			= 255;

								//操作可能に.
								Player.KeyStop		= false;

								//エンディングBGMの再生.
								CSoundManager::Stop	   ( CSoundManager::BGM_Boss	);
								CSoundManager::PlayLoop( CSoundManager::BGM_Ending	);
							}
						}
						else{
							if ( Boss.chara[enBoss::Main].State != enCharaState::Dying ){
								//死亡アニメーション用座標ずらし.
								Boss.chara[typeNo].x--;
								Boss.chara[typeNo].y += 2;
								Boss.chara[typeNo].ExpAnimCnt++;
								if ( Boss.chara[typeNo].ExpAnimCnt > 15 )//爆発画像が0～15の16ｺﾏ.
								{
									if ( Boss.chara[0].ExpAnimCnt <= 15 ) {
										//状態を死亡にする( 左右の手のみ ).
										Boss.chara[typeNo].State = enCharaState::Dead;
									}
								}
							}
						}
					}
				}
				if ( Boss.BlowAway.DispFlag == true ) {
					//吹き飛ばしエフェクトの移動.
					Boss.BlowAway.y += 20;

					//画面外に出た.
					if ( Boss.BlowAway.y > GWND_H ) {
						Boss.BlowAway.DispFlag = false;
					}
				}

				//敵の能力を取得.
				if ( ( Player.chara.abArea[0] == 0 ) || ( Player.chara.abArea[1] == 0 ) ||
					 ( Player.chara.abArea[2] == 0 ) || ( Player.chara.abArea[3] == 0 ) )
				{
					//自機の生存確認.
					if ( Player.chara.State == enCharaState::Living ) {
						for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
							for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
								for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
									//敵機の死亡確認.
									if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dead ) {
										//自機と敵機の当たり判定.
										if ( CollisionDetection(
											Player.chara.x,						Player.chara.y,						C_SIZE, C_SIZE,
											Enemy[typeNo][ptnNo][eNo].chara.x,	Enemy[typeNo][ptnNo][eNo].chara.y,	C_SIZE, C_SIZE ) )
										{
											//能力の取得.
											if ( ( Player.chara.abArea[0] == 0 ) || ( Player.chara.abArea[1] == 0 ) ||
												 ( Player.chara.abArea[2] == 0 ) || ( Player.chara.abArea[3] == 0 ) )
											{
												if ( ( Enemy[typeNo][ptnNo][eNo].EGetAbNo <= enAbility::homing ) && ( Player.chara.abArea[0] == 0 ) ) {
													//能力取得SEを鳴らす.
													CSoundManager::PlaySE( CSoundManager::SE_AGet );

													//空から能力移行.
													Player.DGAbi[0].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 0;
													Player.DGAbi[0].xy.y		= WND_H - ABLT_SIZE - 6;
													Player.DGAbi[0].DispFlag	= true;
													Player.DGAbi[0].ExpaFlag	= false;
													Player.DGAbi[0].AbiNo		= -1;
													Player.DGAbi[0].Expa		= 0;
													Player.DGAbi[0].Alpha		= 200;

													//見た目の変更.
													switch ( Enemy[typeNo][ptnNo][eNo].EGetAbNo )
													{
													case enAbility::beam:
														if ( Player.SlimeNo != enSlime::SBat ){
															Player.NewSlimeNo	= enSlime::SBat;
															Player.AbGetFlag	= true;
														}
														break;
													case enAbility::pene:
														if ( Player.SlimeNo != enSlime::SGoblin ){
															Player.NewSlimeNo	= enSlime::SGoblin;
															Player.AbGetFlag	= true;
														}
														break;
													case enAbility::chg:
														if ( Player.SlimeNo != enSlime::SPlant ){
															Player.NewSlimeNo	= enSlime::SPlant;
															Player.AbGetFlag	= true;
														}
														break;
													case enAbility::homing:
														if ( Player.SlimeNo != enSlime::SBee ){
															Player.NewSlimeNo	= enSlime::SBee;
															Player.AbGetFlag	= true;
														}
														break;
													default:
														break;
													}

													//能力取得.
													Player.chara.abArea[0] = Enemy[typeNo][ptnNo][eNo].EGetAbNo;
													Enemy[typeNo][ptnNo][eNo].EGetAbNo = 0;

													//敵機の処理.
													Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Wait;
													Enemy[typeNo][ptnNo][eNo].chara.x = GWND_W * 2;
													Enemy[typeNo][ptnNo][eNo].appFlag = false;
												}
												else if ( Enemy[typeNo][ptnNo][eNo].EGetAbNo > enAbility::homing ) {
													if( ( Player.chara.abArea[1] == 0 ) || ( Player.chara.abArea[2] == 0 ) || ( Player.chara.abArea[3] == 0 ) ) {
														for( int ablNo = 1; ablNo < ABLT_MAX; ablNo++ ) {
															if( Player.chara.abArea[ablNo] == 0 ) {
																//能力取得SEを鳴らす.
																CSoundManager::PlaySE( CSoundManager::SE_AGet );

																//空から能力に移行.
																Player.DGAbi[ablNo].xy.x		= 5 + ( ABLT_SIZE + 5 ) * ablNo;
																Player.DGAbi[ablNo].xy.y		= WND_H - ABLT_SIZE - 6;
																Player.DGAbi[ablNo].DispFlag	= true;
																Player.DGAbi[ablNo].ExpaFlag	= false;
																Player.DGAbi[ablNo].AbiNo		= Player.chara.abArea[ablNo];
																Player.DGAbi[ablNo].Expa		= 0;
																Player.DGAbi[ablNo].Alpha		= 200;

																//見た目の変更.
																if ( Player.chara.abArea[0] == 0 ){
																	switch ( typeNo )
																	{
																	case 0:	//Plant,
																		if ( Player.SlimeNo != enSlime::SPlant ){
																			Player.NewSlimeNo	= enSlime::SPlant;
																			Player.AbGetFlag	= true;
																		}
																		break;
																	case 1:	//Wolf
																		if ( Player.SlimeNo != enSlime::SWolf ){
																			Player.NewSlimeNo	= enSlime::SWolf;
																			Player.AbGetFlag	= true;
																		}
																		break;
																	case 2:	//Goblin
																		if ( Player.SlimeNo != enSlime::SGoblin ){
																			Player.NewSlimeNo	= enSlime::SGoblin;
																			Player.AbGetFlag	= true;
																		}
																		break;
																	case 3:	//Bat
																		if ( Player.SlimeNo != enSlime::SBat ){
																			Player.NewSlimeNo	= enSlime::SBat;
																			Player.AbGetFlag	= true;
																		}
																		break;
																	case 4: //Bee
																		if ( Player.SlimeNo != enSlime::SBee ){
																			Player.NewSlimeNo	= enSlime::SBee;
																			Player.AbGetFlag	= true;
																		}
																		break;
																	default:
																		break;
																	}
																}

																//能力取得.
																Player.chara.abArea[ablNo] = Enemy[typeNo][ptnNo][eNo].EGetAbNo;
																Enemy[typeNo][ptnNo][eNo].EGetAbNo = 0;
																break;
															}
														}

														//敵機の処理.
														Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Wait;
														Enemy[typeNo][ptnNo][eNo].chara.x = GWND_W * 2;
														Enemy[typeNo][ptnNo][eNo].appFlag = false;
													}
												}
												//能力の取得状態の取得.
												P_UpdateAblt( &Player );
											}
											break;	//敵機と当たればfor文を抜ける.
										}
									}
								}
							}
						}
					}
				}

				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN - 1; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							//敵移動.
							E_ran( scroll, Player, Enemy, Plant, &Boss, gimmick, typeNo, ptnNo, eNo );
						}
					}
					//少し強い敵の移動.
					LBE_ran( scroll, Player, Enemy, Plant, &Boss, gimmick, typeNo );
				}

				//敵(花)の状態アニメーション.
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ){
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[0][ptnNo][eNo].chara.State == enCharaState::Living ) ||
							 ( Enemy[0][ptnNo][eNo].chara.State == enCharaState::Invi ) )
						{
							for ( int pNo = 0; pNo < 2; pNo++ ){
								if ( Plant[ptnNo][eNo].Sleep[pNo].DispFlag == true ){

									//スクロールによるY移動.
									Plant[ptnNo][eNo].Sleep[pNo].y += scroll.spd;

									if ( Plant[ptnNo][eNo].State == enPlantState::Sleeping ){

										//右上に移動させていく.
										Plant[ptnNo][eNo].Sleep[pNo].x += 0.5;
										Plant[ptnNo][eNo].Sleep[pNo].y -= 0.5;

										//透明にしていく.
										Plant[ptnNo][eNo].Sleep[pNo].Alpha -= 3;
										//透明度によるzZの変化.
										if ( Plant[ptnNo][eNo].Sleep[pNo].Alpha <= 85 ){
											Plant[ptnNo][eNo].Sleep[pNo].ptnNo	= 0;

											//新しいzZの出現.
											for ( int pNo = 0; pNo < 2; pNo++ ){
												if ( Plant[ptnNo][eNo].Sleep[pNo].DispFlag == false ){
													Plant[ptnNo][eNo].Sleep[pNo].DispFlag	= true;
													Plant[ptnNo][eNo].Sleep[pNo].x			= Enemy[0][ptnNo][eNo].chara.x + C_SIZE / 2 - 16 / 2;
													Plant[ptnNo][eNo].Sleep[pNo].y			= Enemy[0][ptnNo][eNo].chara.y - 16 / 2;
												}
											}
										}
										else if ( Plant[ptnNo][eNo].Sleep[pNo].Alpha <= 170 ){
											Plant[ptnNo][eNo].Sleep[pNo].ptnNo = 1;
										}

										//完全に透明になった.
										if ( Plant[ptnNo][eNo].Sleep[pNo].Alpha <= 0 ){
											Plant[ptnNo][eNo].Sleep[pNo].Alpha		= 255;
											Plant[ptnNo][eNo].Sleep[pNo].ptnNo		= 2;
											Plant[ptnNo][eNo].Sleep[pNo].DispFlag	= false;
										}
									}
								}
							}
							if ( Plant[ptnNo][eNo].State == enPlantState::WokeUp ){
								//座標を揃える.
								Plant[ptnNo][eNo].Woke.x = Enemy[0][ptnNo][eNo].chara.x + C_SIZE / 2 - 16 / 2;
								Plant[ptnNo][eNo].Woke.y = Enemy[0][ptnNo][eNo].chara.y - 16 / 2;
								if ( Plant[ptnNo][eNo].Woke.AlphaFlag == false ){
									//アニメーション用座標ずらし.
									Plant[ptnNo][eNo].Woke.AnimaX += 1;
									Plant[ptnNo][eNo].Woke.AnimaY += 2;
									Plant[ptnNo][eNo].Woke.AnimaCnt++;
									if ( Plant[ptnNo][eNo].Woke.AnimaCnt > 15 ){
										//アニメーション用座標ずらしの初期化.
										Plant[ptnNo][eNo].Woke.AnimaX = 0;
										Plant[ptnNo][eNo].Woke.AnimaY = 0;
										Plant[ptnNo][eNo].Woke.AnimaCnt = 0;

										Plant[ptnNo][eNo].Woke.AlphaFlag = true;
									}
								}
								else{
									//透明にしていく.
									Plant[ptnNo][eNo].Woke.Alpha -= 3;
									if ( Plant[ptnNo][eNo].Woke.Alpha <= 0 ){
										//起床状態にする.
										Plant[ptnNo][eNo].Woke.Alpha	= 255;
										Plant[ptnNo][eNo].State			= enPlantState::StayUp;
									}
								}
							}
						}
					}
				}

				//プレイヤーの能力取得アニメーション.
				if ( Player.AbGetFlag == true ){
					//アニメーション用座標ずらし.
					Player.GetAnimaX += 1;
					Player.GetAnimaY += 2;
					Player.GetAnimaCnt++;
					if ( Player.GetAnimaCnt > 15 ) {
						//アニメーション用座標ずらしの初期化.
						Player.GetAnimaX = 0;
						Player.GetAnimaY = 0;

						Player.GetAnimaCnt	= 0;
						Player.AbGetFlag	= false;
						Player.SlimeNo		= Player.NewSlimeNo;
						Player.NewSlimeNo	= 0;
					}
				}

				//プレイヤーのダメージ画像.
				if ( Player.Damage[0].DispFlag == true ){
					Player.Damage[0].x = Player.chara.x + C_SIZE - 16 - 20;
					Player.Damage[0].y = Player.chara.y + 20;
				}
				if ( Player.Damage[1].DispFlag == true ){
					Player.Damage[1].x = Player.chara.x + 10;
					Player.Damage[1].y = Player.chara.y + C_SIZE - 16 - 10;
				}

				//ステージギミック(毒沼)のアニメーションの表示.
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					if ( gimmick[2][1][gNo].State != enGimmickState::GWait ) {
						if ( PTimeCnt[gNo] >= PTime[gNo] ) {
							int DispNum		= rand() % 4 + 1;
							PTimeCnt[gNo]	= 0;
							PTime[gNo]		= rand() % 31 + 50;

							for ( int pNo = 0; pNo < DispNum; pNo++ ) {
								//初期設定.
								Poison[gNo][pNo].DispFlag	= true;
								Poison[gNo][pNo].x			= ( float ) ( rand() % ( C_SIZE - 18 ) + gimmick[2][1][gNo].x );
								Poison[gNo][pNo].y			= ( float ) ( rand() % ( C_SIZE - 18 ) + gimmick[2][1][gNo].y );
								Poison[gNo][pNo].PtnNo		= rand() % 3;
								int TmpSpd = rand() % 10 + 1;
								Poison[gNo][pNo].Spd = ( float ) ( TmpSpd ) / 10;
								Poison[gNo][pNo].AlSpd		= rand() % 6 + 10;
							}
						}
					}
				}

				//ステージギミック(毒沼)のアニメーションの動作.
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					for ( int pNo = 0; pNo < P_MAX; pNo++ ) {
						if ( Poison[gNo][pNo].DispFlag == true ) {
							if ( gimmick[2][1][gNo].State != enGimmickState::GWait ) {
								//透明になりながら上にあげていく.
								Poison[gNo][pNo].y		-= Poison[gNo][pNo].Spd;
								Poison[gNo][pNo].Alpha	-= Poison[gNo][pNo].AlSpd;

								//完全に透明になった.
								if ( Poison[gNo][pNo].Alpha < 0 ) {
									Poison[gNo][pNo].Alpha		= 255;
									Poison[gNo][pNo].DispFlag	= false;
								}
							}
							else {
								//初期化.
								PTime[gNo]					= 0;
								PTimeCnt[gNo]				= 0;
								Poison[gNo][pNo].Alpha		= 255;
								Poison[gNo][pNo].DispFlag	= false;
							}
						}
					}
				}

				//ステージギミック(毒沼)のカウント.
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					if ( gimmick[2][1][gNo].State != enGimmickState::GWait ) {
						PTimeCnt[gNo]++;
					}
				}

				//能力削除アニメーション.
				DeleteAblt( &Player );
				//バリアの制御.
				P_Barr( &Player );
				E_Barr( Enemy );
				//回復の制御.
				P_heal( &Player );
				E_heal( Enemy );

				//ギミック.
				Gimmick_Ran( gimmick, scroll, &Player );

				if ( Boss.chara[enBoss::Main].State != enCharaState::Dying ){
					//敵のカウント+.
					for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
						for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
							for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
								if ( Enemy[typeNo][ptnNo][eNo].appFlag == true || Enemy[typeNo][ptnNo][eNo].BossFlag == true ) {
									if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living || Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) {
										//敵カウント.
										Enemy[typeNo][ptnNo][eNo].EmRanCnt++;
										Enemy[typeNo][ptnNo][eNo].EshotCnt++;
									}
									//アニメーション.
									if ( typeNo != 0 ) {
										Enemy[typeNo][ptnNo][eNo].animetion.cnt++;
										if ( Enemy[typeNo][ptnNo][eNo].animetion.cnt >= Enemy[typeNo][ptnNo][eNo].animetion.cntMax ) {
											Enemy[typeNo][ptnNo][eNo].animetion.cnt = 0;
											Enemy[typeNo][ptnNo][eNo].animetion.No++;
											if ( Enemy[typeNo][ptnNo][eNo].animetion.No >= Enemy[typeNo][ptnNo][eNo].animetion.max ) {
												Enemy[typeNo][ptnNo][eNo].animetion.No = 0;
											}
										}
									}
									//無敵時間の減少.
									if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) {
										Enemy[typeNo][ptnNo][eNo].chara.inviTime--;
										if ( Enemy[typeNo][ptnNo][eNo].chara.inviTime == 0 ) {
											Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Living;
										}
									}
									//回復エフェクトアニメーション.
									for ( int hNo = 0; hNo < H_MAX; hNo++ ){
										if ( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].DispFlag == true ){
											//回復エフェクトアニメーション.
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cnt++;
											if ( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cnt >= Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cntMax ){
												//座標を揃える.
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].x = rand() % ( C_SIZE - C_SIZE / 2 ) + Enemy[typeNo][ptnNo][eNo].chara.x;
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].y = rand() % ( C_SIZE / 3 ) + ( Enemy[typeNo][ptnNo][eNo].chara.y + ( C_SIZE - C_SIZE / 2 ) );

												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cnt = 0;
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No++;
												if ( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No >= Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.max ){
													Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No = 0;
												}
											}
											//薄くなりながら上に上げる.
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].UpY	+= 2;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].Alpha	-= 5;
											//アルファ値が0以下になったら回復エフェクトを消す.
											if ( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].Alpha <= 0 ){
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cnt	= 0;
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No	= 0;
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].Alpha			= 255;
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].UpY			= 0;
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].DispFlag		= false;
											}
										}
									}
								}
							}
						}
					}

					//プレイヤーのカウント+.
					//ショットカウントを増やす.
					Player.shotCnt++;
					//アニメーション.
					Player.animation.cnt++;
					if ( Player.animation.cnt >= Player.animation.cntMax ) {
						Player.animation.cnt = 0;
						Player.animation.No++;
						if ( Player.animation.No >= Player.animation.max ) {
							Player.animation.No = 0;
						}
					}
					//回復エフェクトアニメーション.
					for ( int hNo = 0; hNo < H_MAX; hNo++ ){
						if ( Player.HeelAnima[hNo].DispFlag == true ){
							//回復エフェクトアニメーション.
							Player.HeelAnima[hNo].animation.cnt++;
							if ( Player.HeelAnima[hNo].animation.cnt >= Player.HeelAnima[hNo].animation.cntMax ){
								//座標を揃える.
								Player.HeelAnima[hNo].x = rand() % ( C_SIZE - C_SIZE / 2 ) + Player.chara.x;
								Player.HeelAnima[hNo].y = rand() % ( C_SIZE / 3 ) + ( Player.chara.y + ( C_SIZE - C_SIZE / 2 ) );

								Player.HeelAnima[hNo].animation.cnt = 0;
								Player.HeelAnima[hNo].animation.No++;
								if ( Player.HeelAnima[hNo].animation.No >= Player.HeelAnima[hNo].animation.max ){
									Player.HeelAnima[hNo].animation.No = 0;
								}
							}
							//薄くなりながら上に上げる.
							Player.HeelAnima[hNo].UpY += 2;
							Player.HeelAnima[hNo].Alpha -= 5;
							//アルファ値が0以下になったら回復エフェクトを消す.
							if ( Player.HeelAnima[hNo].Alpha <= 0 ){
								Player.HeelAnima[hNo].animation.cnt = 0;
								Player.HeelAnima[hNo].animation.No = 0;
								Player.HeelAnima[hNo].Alpha = 255;
								Player.HeelAnima[hNo].UpY = 0;
								Player.HeelAnima[hNo].DispFlag = false;
							}
						}
					}
					//無敵時間の減少.
					if ( Player.chara.State == enCharaState::Invi ) {
						//コントローラ振動.
						if ( Player.chara.inviTime >= INVITIME - 1 ) {
							pXInput->SetVibration( 20000, 20000 );
						}
						else {
							pXInput->SetVibration( 0, 0 );
						}

						Player.chara.inviTime--;
						if ( Player.chara.inviTime == 0 ) {
							pXInput->SetVibration( 0, 0 );

							Player.chara.State = enCharaState::Living;
						}
					}

					//ボスのカウント+.
					if ( Boss.AttackNo[0] == enBossAttackNo::BWait ) {
						if ( Boss.animation[0].Flag == false ) {
							Boss.animation[0].cnt++;
							if ( Boss.animation[0].cnt > Boss.animation[0].cntMax ) {
								Boss.animation[0].cnt = 0;
								Boss.animation[0].No++;
								if ( Boss.animation[0].No > Boss.animation[0].max ) {
									Boss.animation[0].No = Boss.animation[0].max;
									Boss.animation[0].Flag = true;
								}
							}
						}
						else {
							Boss.animation[0].cnt++;
							if ( Boss.animation[0].cnt > Boss.animation[0].cntMax ) {
								Boss.animation[0].cnt = 0;
								Boss.animation[0].No--;
								if ( Boss.animation[0].No < 0 ) {
									Boss.animation[0].No = 0;
									Boss.animation[0].Flag = false;
								}
							}
						}
					}
					if ( ( Boss.chara[enBoss::Main].State == enCharaState::Living ) ||
						( Boss.chara[enBoss::Main].State == enCharaState::Invi ) )
					{
						Boss.AttackCnt++;
						Boss.AttackMovecnt++;
					}
					for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ) {
						//無敵時間の減少.
						if ( Boss.chara[typeNo].State == enCharaState::Invi ) {
							Boss.chara[typeNo].inviTime--;
							if ( Boss.chara[typeNo].inviTime == 0 ) {
								Boss.chara[typeNo].State = enCharaState::Living;
							}
						}
					}

					//ステージギミックのカウント+
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( gimmick[0][ptnNo][gNo].State == enGimmickState::GInvi ) {
								gimmick[0][ptnNo][gNo].inviTime--;
								if ( gimmick[0][ptnNo][gNo].inviTime == 0 ) {
									gimmick[0][ptnNo][gNo].State = enGimmickState::GLiving;
								}
							}
						}
					}
				}

				//ボスの出現演出.
				if ( Boss.DispFlag == true ) {
					DispAnimation( &Boss, &scroll, &Player, pXInput );
				}

				//空を舞う葉っぱの動作.
				for ( int LNo = 0; LNo < W_HITMAX; LNo++ ) {
					if ( WoodLeaf[LNo].DispFlag == true ) {
						//回転させる.
						WoodLeaf[LNo].Ang += 0.02;
						if ( WoodLeaf[LNo].Ang >= 360 ){
							WoodLeaf[LNo].Ang = 0;
						}

						//風で飛ばす.
						WoodLeaf[LNo].x += WoodLeaf[LNo].Speed + static_cast< int >( cos( RAD( Title.LCosAng ) - 10 ) * WoodLeaf[LNo].Speed / 2 );
						WoodLeaf[LNo].y += WoodLeaf[LNo].FallPower;		//落下.

						//画面外に出た時.
						if ( WoodLeaf[LNo].x > C_CORR + GWND_W ) {
							WoodLeaf[LNo].DispFlag = false;
						}
					}
				}

				//背景ｽｸﾛｰﾙ処理.
				if ( scroll.stop == false ) {
					if ( scroll.y > -WND_H ) {
						scroll.y -= scroll.spd;	//ｽｸﾛｰﾙ値を減らす.
					}
					else {
						scroll.y = WND_H;		//初期値に戻す.
					}

					//マップカウント.
					CntMap( &scroll, &map, Enemy, Plant, gimmick, &Boss, &Player );
				}

//#ifdef _DEBUG	/* デバック操作 */
				//スクロールストップ.
				if ( GetAsyncKeyState( VK_F1 ) & 0x0001 ){
					if ( scroll.stop == false ) { scroll.stop = true;  }
					else						{ scroll.stop = false; }
				}
				//ゲームオーバー画面へ.
				if ( GetAsyncKeyState( VK_F3 ) & 0x8000 ) {
					bool Flag = false;

					if ( GetAsyncKeyState( '1' ) & 0x8000 ){
						Player.DeadNo	= enDeedNo::Plant;
						Flag			= true;
					}
					if ( GetAsyncKeyState( '2' ) & 0x8000 ){
						Player.DeadNo	= enDeedNo::Wolf;
						Flag			= true;
					}
					if ( GetAsyncKeyState( '3' ) & 0x8000 ){
						Player.DeadNo	= enDeedNo::Goblin;
						Flag			= true;
					}
					if ( GetAsyncKeyState( '4' ) & 0x8000 ){
						Player.DeadNo	= enDeedNo::Bat;
						Flag			= true;
					}
					if ( GetAsyncKeyState( '5' ) & 0x8000 ){
						Player.DeadNo	= enDeedNo::Bee;
						Flag			= true;
					}
					if ( GetAsyncKeyState( '6' ) & 0x8000 ){
						Player.DeadNo	= enDeedNo::Gimmick;
						Flag			= true;
					}
					if ( GetAsyncKeyState( '7' ) & 0x8000 ){
						Player.DeadNo	= enDeedNo::Golem;
						Flag			= true;
					}

					if ( Flag == true ){
						CSoundManager::Stop( CSoundManager::BGM_Main );
						CSoundManager::Stop( CSoundManager::BGM_Boss );
						Player.chara.State = enCharaState::Dying;
					}
				}

				//エンディングへ.
				if ( GetAsyncKeyState( VK_F4 ) & 0x8000 ) {
					scroll.stop = true;
					CSoundManager::Stop( CSoundManager::BGM_Main );
					CSoundManager::Stop( CSoundManager::BGM_Boss );
					Boss.chara[enBoss::Main].State = enCharaState::Dying;
				}

				//スクロールを飛ばす.
				if ( GetAsyncKeyState( VK_F5 ) & 0x8000 ) {
					map.dispCnt = 0;
				}
				if ( GetAsyncKeyState( VK_F6 ) & 0x8000 ) {
					map.dispCnt = 15;
				}
				if ( GetAsyncKeyState( VK_F7 ) & 0x8000 ) {
					map.dispCnt = 30;
				}
				if ( GetAsyncKeyState( VK_F8 ) & 0x8000 ) {
					map.dispCnt = 45;
				}
				if ( GetAsyncKeyState( VK_F9 ) & 0x8000 ) {
					map.dispCnt = 60;
				}
				if ( GetAsyncKeyState( VK_F10 ) & 0x8000 ) {
					map.dispCnt = 75;
				}
				if ( GetAsyncKeyState( VK_F11 ) & 0x8000 ) {
					map.dispCnt = 89;
				}
//#endif
				break;
			}
			case enScene::GameOver:	//ｹﾞｰﾑｵｰﾊﾞｰ.
			{
				//BGM再生( ループ ).
				CSoundManager::PlayLoop( CSoundManager::BGM_GameOver );

				//PressEnterKeyを点滅させる.
				if ( Title.Alpha <= 255 ) {			// 0(不透明) ~ 255(透明).
					Title.Alpha += 2;				//透明にしていく.
					if ( Title.Alpha >= 255 ) {		//透明になったら.
						Title.Alpha = 511;			// 255 => 511 にする(透明).
					}
				}
				else {								// 256(不透明) ~ 511(透明).
					Title.Alpha -= 2;				//不透明にしていく.
					if ( Title.Alpha <= 256 ) {		//不透明になったら.
						Title.Alpha = 0;			// 256 => 0 にする(不透明).
					}
				}

				if ( ( Player.DeadNo == enDeedNo::Bat ) || ( Player.DeadNo == enDeedNo::Gimmick ) ) {
					BatAnima.cnt++;
					if ( BatAnima.cnt >= BatAnima.cntMax ) {
						BatAnima.cnt = 0;
						BatAnima.No++;
						if ( BatAnima.No >= BatAnima.max ) {
							BatAnima.No = 0;
						}
					}
				}

				if ( GetAsyncAnyKey( &Player, pXInput ,&XInKey ) ) {
					if ( Player.PushKey[enPushKey::Space] == false ) {
						Player.PushKey[enPushKey::Space] = true;

						//決定SEを鳴らす.
						CSoundManager::PlaySE( CSoundManager::SE_Decision );

						//次にﾀｲﾄﾙに入ってきても大丈夫なように初期化.
						Title.Alpha				= 255;	//ﾀｲﾄﾙのPressEnterKeyで使う.
						Title.UPCloudX1			= WND_W - 256;
						Title.UPCloudX2			= -446;
						Title.DownCloudX1		= WND_W - ( WND_W / 2 - 50 );
						Title.DownCloudX2		= -( WND_W - ( WND_W / 2 - 50 ) );
						Title.Panimation.cnt	= 0;
						Title.Panimation.cntMax = 20;

						Title.Panimation.Flag	= false;
						Title.Panimation.No		= 0;
						Title.Panimation.max	= 3;
						Title.LCosAng			= 0;
						Title.LDispMAX			= 0;
						Title.LTime				= 0;
						Title.LTimeCnt			= 0;
						for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
							Title.Leaf[LNo].x			= -16;
							Title.Leaf[LNo].y			= 0;
							Title.Leaf[LNo].PtnX		= 0;
							Title.Leaf[LNo].PtnY		= 0;
							Title.Leaf[LNo].Speed		= 5;
							Title.Leaf[LNo].FallPower	= 1;
							Title.Leaf[LNo].Ang			= 0.0;
							Title.Leaf[LNo].DispFlag	= false;
						}
						//フェードイン(タイトルへ).
						Stop			= true;
						Fade.FadeState	= enFade::in;
						Fade.EndScene	= enScene::Title;

						//タイトルBGMの再生.
						CSoundManager::Stop( CSoundManager::BGM_GameOver );
						CSoundManager::PlayLoop( CSoundManager::BGM_Title );
					}
				}
				else {
					Player.PushKey[enPushKey::Space] = false;
				}
				break;
			}
			case enScene::Ending:	//ｴﾝﾃﾞｨﾝｸﾞ.
			{
				//BGM再生( ループ ).
				CSoundManager::PlayLoop( CSoundManager::BGM_Ending );

				//能力エリアリセット.
				Player.chara.abArea[0] = enAbility::dirUP;
				Player.chara.abArea[1] = enAbility::numUP;
				Player.chara.abArea[2] = enAbility::numUP;
				Player.chara.abArea[3] = enAbility::numUP;
				//能力の取得状態の取得.
				P_UpdateAblt( &Player );

				//毎フレーム最初は待機に戻す.
				//Ending.Slime.Action = enAction::AcWait;

				//ジャンプ.
				if ( GetAsyncKeyState( VK_UP ) & 0x8000 || XInKey.trgL >= CXInput::INPUT_TRIGGER_MAX ) {
					if ( Ending.EnEng[ENGMAX - 1].y > 300.0f ) {
						if ( Ending.Slime.Jumping == false ) {

							//ジャンプSEの再生.
							CSoundManager::PlaySE( CSoundManager::SE_Jump );

							Ending.Slime.Jumping	= true;	//ジャンプ中にする.
							Ending.Slime.JumpAcc	= 0.0f;	//値を初期化.
						}
					}
				}
				if ( GetAsyncKeyState( VK_LEFT ) & 0x8000 || XInKey.stL_X <= CXInput::INPUT_THUMB_MIN ) {
					if ( Ending.EnEng[ENGMAX - 1].y > 300.0f ) {
						Ending.Slime.Action	 = enAction::MoveLeft;
						Ending.Slime.x		-= Ending.Slime.MoveSpeed;
					}
				}
				if ( GetAsyncKeyState( VK_RIGHT ) & 0x8000 || XInKey.stL_X >= CXInput::INPUT_THUMB_MAX ) {
					if ( Ending.EnEng[ENGMAX - 1].y > 300.0f ) {
						Ending.Slime.Action  = enAction::MoveRight;
						Ending.Slime.x		+= Ending.Slime.MoveSpeed;
					}
				}
				if ( GetAsyncKeyState( VK_SPACE ) & 0x8000 || XInKey.trgR >= CXInput::INPUT_TRIGGER_MAX ) {
					if ( Ending.EnEng[ENGMAX - 1].y > 300.0f ) {
						P_ShotFlag( &Player, Scene, Ending );
					}
				}
				else if ( Player.ability.chg != 0 ) {
					if ( ( Player.chara.chgCnt >= CHARGEMAX ) ||
						 ( ( Player.chgNumCnt > 0 ) && ( Player.shotCnt > S_TIME ) ) ) {

						//発射準備アニメーションCNT初期化.
						Player.chgAniCnt	= 0;
						Player.chgAniFlag	= false;

						// chgCnt = -1：「 -1 」で弾の発射.
						Player.chara.chgCnt = -1;
						Player.chgNumCnt	-= 1;
						P_ShotFlag( &Player, Scene, Ending );
					}
					else {
						if ( Player.chara.chgCnt > 0 ) {
							//演出用で[4][9]を使用.
							Player.shot[4][9].x = ( Player.chara.x + C_SIZE / 2 );
							Player.shot[4][9].y = ( Player.chara.y );

							Player.chara.chgCnt -= 3;
							if ( Player.chara.chgCnt <= 0 ) {
								Player.chara.chgCnt = 0;
							}
						}
					}
				}

				//ジャンプ中処理.
				if ( Ending.Slime.Jumping == true )
				{
					//公式: y = Vot - 1/2gt^2.
					Ending.Slime.y -= static_cast< int >( Ending.Slime.JumpPower - Ending.Slime.JumpAcc );
					Ending.Slime.JumpAcc += Ending.Slime.Gravity;		//重力加速.

					//雫の出現.
					if ( Ending.DTimeCnt >= Ending.DTime ) {
						//次の出現タイムを決める.
						Ending.DTimeCnt = 0;
						Ending.DTime = rand() % 3 + 1;

						//雫の出現.
						for ( int dNo = 0; dNo < DROPMAX; dNo++ ) {
							if ( Ending.Drop[dNo].DispFlag == false ) {
								//出現.
								Ending.Drop[dNo].DispFlag = true;
								Ending.Drop[dNo].Ptn = rand() % 3;
								Ending.Drop[dNo].x = rand() % ( C_SIZE * 2 - Ending.wDest[Ending.Drop[dNo].Ptn] ) + Ending.Slime.x;
								Ending.Drop[dNo].y = Ending.Slime.y + C_SIZE * 2;

								break;
							}
						}
					}
					else {
						Ending.DTimeCnt++;
					}

					//地面より下になった.
					if ( Ending.Slime.y >= Ending.Slime.GroundPos )
					{
						//ジャンプSEの再生.
						CSoundManager::PlaySE( CSoundManager::SE_Landing );

						Ending.Slime.y			= Ending.Slime.GroundPos;	//地面にそろえる.
						Ending.Slime.Jumping	= false;					//ジャンプ中解除.
					}
				}

				//画面外処理.
				if ( Ending.Slime.x < 0 ) {
					Ending.Slime.x = 0;
				}
				if ( Ending.Slime.x > WND_W - C_SIZE * 2 ) {
					Ending.Slime.x = WND_W - C_SIZE * 2;
				}

				//アニメーション制御.
				Ending.Slime.animation.cnt++;
				if ( Ending.Slime.animation.cnt >= Ending.Slime.animation.cntMax ) {
					Ending.Slime.animation.cnt = 0;
					Ending.Slime.animation.No++;
					if ( Ending.Slime.animation.No >= Ending.Slime.animation.max ) {
						Ending.Slime.animation.No = 0;
					}
				}

				//星の出現.
				if ( Ending.STimeCnt >= Ending.STime ) {
					//次の出現タイムを決める.
					Ending.STimeCnt = 0;
					Ending.STime = rand() % 10 + 5;

					//出現数を決める.
					Ending.SDispMAX = rand() % 5 + 1;
					int DispCnt = 0;

					//星の出現.
					for ( int sNo = 0; sNo < STARMAX; sNo++ ) {
						if ( Ending.Star[sNo].DispFlag == false ) {
							//出現.
							Ending.Star[sNo].DispFlag	= true;
							Ending.Star[sNo].x			= rand() % ( WND_W - 16 );
							Ending.Star[sNo].y			= rand() % ( WND_H - C_SIZE * 4 );
							Ending.Star[sNo].AlSpd		= rand() % 5 + 1;
							Ending.Star[sNo].Ptn		= rand() % 3;
							int Check = rand() % 10;
							if ( Check == 0 ){
								Ending.Star[sNo].Shooting = true;
								Ending.Star[sNo].Spd = rand() % 4 + 2;
							}

							DispCnt++;	//出現数カウント.

							//全て出したら.
							if ( DispCnt >= Ending.SDispMAX ) {
								Ending.SDispMAX = 0;
								break;
							}
						}
					}
				}
				else {
					//葉っぱの待ち時間カウント＋.
					Ending.STimeCnt++;
				}

				//星の動作.
				for ( int sNo = 0; sNo < STARMAX; sNo++ ) {
					if ( Ending.Star[sNo].DispFlag == true ) {
						//流れ星なら.
						if ( Ending.Star[sNo].Shooting == true ){
							Ending.Star[sNo].x -= Ending.Star[sNo].Spd;
							Ending.Star[sNo].y += Ending.Star[sNo].Spd;
						}
						//星を出現させ消滅させる.
						if ( Ending.Star[sNo].Alpha <= 255 ) {												// 0(透明) ~ 255(不透明).
							if ( Ending.Star[sNo].Shooting == true ){
								Ending.Star[sNo].Alpha += Ending.Star[sNo].AlSpd * Ending.Star[sNo].Spd;	//不透明にしていく.
							}
							else{
								Ending.Star[sNo].Alpha += Ending.Star[sNo].AlSpd;							//不透明にしていく.
							}
							if ( Ending.Star[sNo].Alpha >= 255 ) {											//不透明になったら.
								Ending.Star[sNo].Alpha = 510;												// 255 => 511 にする(不透明).
							}
						}
						else {																				// 256(透明) ~ 511(不透明).
							if ( Ending.Star[sNo].Shooting == true ){
								Ending.Star[sNo].Alpha -= Ending.Star[sNo].AlSpd * Ending.Star[sNo].Spd;	//不透明にしていく.
							}
							else{
								Ending.Star[sNo].Alpha -= Ending.Star[sNo].AlSpd;							//不透明にしていく.
							}
							if ( Ending.Star[sNo].Alpha <= 255 ) {											//透明になったら.
								Ending.Star[sNo].Alpha		= 0;											// 256 => 0 にする(透明).
								Ending.Star[sNo].Spd		= 0;
								Ending.Star[sNo].Shooting	= false;
								Ending.Star[sNo].DispFlag	= false;
							}
						}
					}
				}

				//雫の動作.
				for ( int dNo = 0; dNo < DROPMAX; dNo++ ) {
					if ( Ending.Drop[dNo].DispFlag == true ) {
						//下に落としていく.
						Ending.Drop[dNo].y += 8;

						//地面に付いた.
						if ( Ending.Drop[dNo].y > WND_H ) {
							Ending.Drop[dNo].DispFlag = false;
						}
					}
				}

				//プレイヤーを座標を揃える.
				Player.chara.x = Ending.Slime.x + C_SIZE / 2;
				Player.chara.y = Ending.Slime.y + C_SIZE / 2;

				//プレイヤー弾の動作.
				P_ShotControl( &Player, &Boss, Enemy, Scene );

				//ショットカウントを増やす.
				Player.shotCnt++;

				//少し待機した後"GAME CLEAR"を上にあげていく.
				if ( Ending.StopCnt <= 50 ) {
					Ending.StopCnt++;
				}
				else {
					Ending.ClearY--;
					if ( Ending.ClearY < -126 ) {
						Ending.Start = true;
					}
				}

				//スタッフロールの表示.
				if ( Ending.Start == true ) {
					if ( Ending.DispCnt >= Ending.DispTime ) {
						if ( Ending.EnEng[Ending.DispNo].StartX != -1 ) {
							Ending.EnEng[Ending.DispNo].DispFlag = true;
						}
						Ending.DispTime = ( Ending.EnEng[Ending.DispNo].hDest + Ending.Space ) * 2;
						Ending.DispCnt = 0;
						Ending.DispNo++;
						if ( Ending.DispNo >= ENGMAX ) {
							Ending.DispNo = ENGMAX - 1;
						}
					}
				}

				//スタッフロールの移動.
				for ( int eNo = 0; eNo < ENGMAX; eNo++ ) {
					if ( Ending.Start == true ) {
						if ( Ending.EnEng[eNo].DispFlag == true ) {
							if ( ( Ending.EnEng[ENGMAX - 1].y >= 300.0f ) || ( eNo != ENGMAX - 1 ) ) {
								Ending.EnEng[eNo].y -= 0.5f;
							}
							if ( Ending.EnEng[eNo].y < -Ending.EnEng[eNo].hDest ) {
								Ending.EnEng[eNo].DispFlag = false;
							}
						}
					}
				}

				//スクロール待ち時間カウント＋.
				if ( Ending.Start == true ) {
					Ending.DispCnt++;
				}

				//タイトルに戻る.
				if ( ( ( Ending.EnEng[ENGMAX - 3].DispFlag == false ) && ( Ending.EnEng[ENGMAX - 1].y <= 300.0f ) ) || ( GetAsyncKeyState( VK_F2 ) & 0x8000 ) ) {
					//次にﾀｲﾄﾙに入ってきても大丈夫なように初期化.
					Title.Alpha				= 255;	//ﾀｲﾄﾙのPressEnterKeyで使う.
					Title.UPCloudX1			= WND_W - 256;
					Title.UPCloudX2			= -446;
					Title.DownCloudX1		= WND_W - ( WND_W / 2 - 50 );
					Title.DownCloudX2		= -( WND_W - ( WND_W / 2 - 50 ) );
					Title.Panimation.cnt	= 0;
					Title.Panimation.cntMax = 20;

					Title.Panimation.Flag	= false;
					Title.Panimation.No		= 0;
					Title.Panimation.max	= 3;
					Title.LCosAng			= 0;
					Title.LDispMAX			= 0;
					Title.LTime				= 0;
					Title.LTimeCnt			= 0;
					for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
						Title.Leaf[LNo].x			= -16;
						Title.Leaf[LNo].y			= 0;
						Title.Leaf[LNo].PtnX		= 0;
						Title.Leaf[LNo].PtnY		= 0;
						Title.Leaf[LNo].Speed		= 5;
						Title.Leaf[LNo].FallPower	= 1;
						Title.Leaf[LNo].Ang			= 0.0;
						Title.Leaf[LNo].DispFlag	= false;
					}

					//フェードイン(タイトルへ).
					Stop			= true;
					Fade.FadeState	= enFade::in;
					Fade.EndScene	= enScene::Title;

					//Hit数が最大Hit数より高い場合保存する.
					if ( Ending.Hit > Ending.BestHit ){

						FILE *fp = fopen( "Data\\CSV\\BESTHIT.csv", "w" );

						if ( fp == NULL ){		//エラーが起きたらNULLを返す.
							return 0;
						}
						fprintf( fp, "%d", Ending.Hit );

						fclose( fp );			//ファイルを閉じる.
					}

					//タイトルBGMの再生.
					CSoundManager::Stop( CSoundManager::BGM_Ending );
					CSoundManager::PlayLoop( CSoundManager::BGM_Title );
				}

				//弾とスタッフロールの当たり判定.
				for ( int numNo = 0; numNo < Player.status.num; numNo++ ) {
					for ( int dirNo = 0; dirNo < Player.status.dir; dirNo++ ) {
						//自機の弾が発射されている.
						if ( Player.shot[numNo][dirNo].DispShotFlag == true ) {
							for ( int eNo = 0; eNo < ENGMAX; eNo++ ) {
								//スタッフロールが出ている.
								if ( ( Ending.EnEng[eNo].HitFlag == false ) && ( Ending.EnEng[eNo].DispFlag == true ) && ( eNo != 32 ) && ( eNo != 34 ) ) {
									//自機の弾とスタッフロールの当たり判定.
									if ( ( ( Player.ability.chg == 0 ) && ( CollisionDetection(
										 ( int ) Player.shot[numNo][dirNo].x,			( int ) Player.shot[numNo][dirNo].y,			S_SIZE,						S_SIZE,
										 ( int ) Ending.EnEng[eNo].x,					( int ) Ending.EnEng[eNo].y,					Ending.EnEng[eNo].wDest,	Ending.EnEng[eNo].hDest ) ) ) ||

										 ( ( Player.ability.chg != 0 ) && ( CollisionDetection(
										 ( int ) Player.shot[numNo][dirNo].x - S_SIZE,	( int ) Player.shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,					S_SIZE * 2,
											( int ) Ending.EnEng[eNo].x,				( int ) Ending.EnEng[eNo].y,					Ending.EnEng[eNo].wDest,	Ending.EnEng[eNo].hDest ) ) ) ) {

										//敵のダメージSEを鳴らす.
										CSoundManager::PlaySE( CSoundManager::SE_EDamage );

										//スタッフロールのHP減少.
										Ending.EnEng[eNo].HitFlag = true;

										//1000以上になると表示がバグるので999で止める.
										if ( Ending.Hit < 1000 ){
											//最高Hit数を超えた時にSEを鳴らす.
											if ( ( Ending.Hit <= Ending.BestHit ) && ( Ending.Hit + 1 > Ending.BestHit ) ){
												//最高Hit数更新SEを鳴らす.
												CSoundManager::PlaySE( CSoundManager::SE_Best );
											}

											//Hit数をカウント.
											Ending.Hit++;
											if ( Ending.Hit >= 1000 ){
												Ending.Hit = 999;
											}
										}

										//自機の弾の着弾後の処理.
										if ( Player.ability.pene == 0 ) {	//貫通をもっていない.
											if ( Player.ability.beam != 0 ) {
												//ビームのヒット回数+2.
												Player.shot[numNo][dirNo].beamCnt -= 2;
												if ( Player.shot[numNo][dirNo].beamCnt == 0 ) {

													Player.shot[numNo][dirNo].DispShotFlag	= false;	//発射ﾌﾗｸﾞを降ろす.
													Player.shot[numNo][dirNo].beamCnt		= BEAMMAX;	//ビームのヒット回数を初期化.
													Player.shot[numNo][dirNo].x				= -WND_W;	//自機の弾x座標:画面の外へ.
													Player.shot[numNo][dirNo].y				= -WND_H;	//自機の弾y座標:画面の外へ.
												}
											}
											else {
												Player.shot[numNo][dirNo].DispShotFlag		= false;	//発射ﾌﾗｸﾞを降ろす.
												Player.shot[numNo][dirNo].x					= -WND_W;	//自機の弾x座標:画面の外へ.
												Player.shot[numNo][dirNo].y					= -WND_H;	//自機の弾y座標:画面の外へ.
											}
										}
									}
								}
							}
						}
					}
				}

				//弾が当たったスタッフロールの動作.
				for ( int eNo = 0; eNo < ENGMAX; eNo++ ) {
					if ( Ending.EnEng[eNo].HitFlag == true ){
						Ending.EnEng[eNo].CosDAng++;
						if ( Ending.EnEng[eNo].CosDAng >= 360 ) {
							Ending.EnEng[eNo].CosDAng = 0;
						}
						Ending.EnEng[eNo].Dang = cosf( Ending.EnEng[eNo].CosDAng ) * Ending.EnEng[eNo].width;
						if ( cos( Ending.EnEng[eNo].CosDAng ) <= 0 ) {
							Ending.EnEng[eNo].width--;
							if ( Ending.EnEng[eNo].width <= 0 ) {
								Ending.EnEng[eNo].HitFlag	= false;
								Ending.EnEng[eNo].CosDAng	= 0.0f;
								Ending.EnEng[eNo].Dang		= 0.0f;
								Ending.EnEng[eNo].width		= 20.0f;
							}
						}
					}
				}

				break;
			}
			default:	//上記以外の場合.
				//基本ここには来ないはず.
				break;
			}
		}
		//フェードイン.
		if ( Fade.FadeState == enFade::in ) {
			if ( Fade.FadeAlpha < Fade.EndAlpha ){
				Fade.FadeAlpha += 5;
				if ( Fade.FadeAlpha >= Fade.EndAlpha ){
					Fade.FadeAlpha = Fade.EndAlpha;
					if ( Fade.InOutFlag == true ) {
						Fade.FadeState	= enFade::out;
						Scene			= Fade.EndScene;

						//次にﾀｲﾄﾙに入ってきても大丈夫なように初期化.
						Title.Alpha		= 255;
					}
				}
			}
		}
		//フェードアウト.
		else if ( Fade.FadeState == enFade::out ) {
			Fade.FadeAlpha -= 5;
			if ( Fade.FadeAlpha < 0 ){
				Fade.FadeAlpha	= 0;
				Fade.FadeState	= enFade::None;
				pauseArea		= 0;
				pauseAlpha		= 255;
				pause			= false;
				Stop			= false;
			}
		}
		//ポーズ.
		if ( pause == true ){

			//BGM再生( ループ ).
			if ( Boss.chara[enBoss::Main].State == enCharaState::Wait ) {
				CSoundManager::PlayLoop( CSoundManager::BGM_Main );
			}
			else {
				CSoundManager::PlayLoop( CSoundManager::BGM_Boss );
			}

			//カーソルの点滅.
			if ( pauseAlpha <= 255 ) {		// 0(不透明) ~ 255(透明).
				pauseAlpha += 3;			//透明にしていく.
				if ( pauseAlpha >= 255 ) {	//透明になったら.
					pauseAlpha = 510;		// 255 => 511 にする(透明).
				}
			}
			else {							// 256(不透明) ~ 511(透明).
				pauseAlpha -= 3;			//不透明にしていく.
				if ( pauseAlpha <= 255 ) {	//不透明になったら.
					pauseAlpha = 0;			// 256 => 0 にする(不透明).
				}
			}

			//↑ or LSU.
			if ( GetAsyncKeyState( VK_UP ) & 0x8000 || XInKey.stL_Y >= CXInput::INPUT_THUMB_MAX ) {
				if ( Player.PushKey[enPushKey::kUP] == false ){
					Player.PushKey[enPushKey::kUP] = true;
					pauseArea--;
					if ( pauseArea < 0 ){
						pauseArea = 1;
					}
				}
			}
			else { Player.PushKey[enPushKey::kUP] = false; }
			//↓ LSD.
			if ( GetAsyncKeyState( VK_DOWN ) & 0x8000 || XInKey.stL_Y <= CXInput::INPUT_THUMB_MIN ) {
				if ( Player.PushKey[enPushKey::kDOWN] == false ){
					Player.PushKey[enPushKey::kDOWN] = true;
					pauseArea++;
					if ( pauseArea > 1 ){
						pauseArea = 0;
					}
				}
			}
			else { Player.PushKey[enPushKey::kDOWN] = false; }
			//スペースキー or RT.
			if ( GetAsyncKeyState( VK_SPACE ) & 0x8000 || XInKey.trgR >= CXInput::INPUT_TRIGGER_MAX ){
				Player.PushKey[enPushKey::Space] = true;
			}
			else if ( Player.PushKey[enPushKey::Space] == true ) {

				Player.PushKey[enPushKey::Space] = false;

				if ( pauseArea == 0 ){
					//決定SEを鳴らす.
					CSoundManager::PlaySE( CSoundManager::SE_Decision );

					//フェードアウト.
					pause = false;
					Fade.InOutFlag = true;
					Fade.FadeState = enFade::out;
					Fade.EndAlpha = 255;
				}
				else{
					//決定SEを鳴らす.
					CSoundManager::PlaySE( CSoundManager::SE_Decision );

					//次にﾀｲﾄﾙに入ってきても大丈夫なように初期化.
					Title.Alpha				= 255;	//ﾀｲﾄﾙのPressEnterKeyで使う.
					Title.UPCloudX1			= WND_W - 256;
					Title.UPCloudX2			= -446;
					Title.DownCloudX1		= WND_W - ( WND_W / 2 - 50 );
					Title.DownCloudX2		= -( WND_W - ( WND_W / 2 - 50 ) );
					Title.Panimation.cnt	= 0;
					Title.Panimation.cntMax = 20;

					Title.Panimation.Flag	= false;
					Title.Panimation.No		= 0;
					Title.Panimation.max	= 3;
					Title.LCosAng			= 0;
					Title.LDispMAX			= 0;
					Title.LTime				= 0;
					Title.LTimeCnt			= 0;
					for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
						Title.Leaf[LNo].x			= -16;
						Title.Leaf[LNo].y			= 0;
						Title.Leaf[LNo].PtnX		= 0;
						Title.Leaf[LNo].PtnY		= 0;
						Title.Leaf[LNo].Speed		= 5;
						Title.Leaf[LNo].FallPower	= 1;
						Title.Leaf[LNo].Ang			= 0.0;
						Title.Leaf[LNo].DispFlag	= false;
					}
					//フェードイン(タイトルへ).
					pause			= false;
					Stop			= true;
					Fade.InOutFlag	= true;
					Fade.EndAlpha	= 255;
					Fade.FadeState	= enFade::in;
					Fade.EndScene	= enScene::Title;

					//タイトルBGMの再生.
					CSoundManager::Stop( CSoundManager::BGM_Main );
					CSoundManager::Stop( CSoundManager::BGM_Boss );
					CSoundManager::PlayLoop( CSoundManager::BGM_Title );
				}
			}
		}

//-------------------------------------------------------------------.
//	描画処理[WM_PAINT].
//-------------------------------------------------------------------.
		//ｼｰﾝごとの処理.
		switch( Scene ){
		case enScene::Title:	//ﾀｲﾄﾙ.
		{
			//ﾀｲﾄﾙ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hsky );
			//ﾀｲﾄﾙ画像の表示.
			BitBlt(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				0, 0,							//表示位置x,y座標.
				WND_W, WND_H,					//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.
				0, 0,							//元画像のx,y座標.
				SRCCOPY );						//ｺﾋﾟｰする.

			//ﾀｲﾄﾙ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hcloud );
			//上の雲1の表示.
			TransBlt(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				( int ) ( Title.UPCloudX1 ), 0, //表示位置x,y座標.
				256, 128,						//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.	
				0, 256 * 2 );					//元画像のx,y座標.
			//ﾀｲﾄﾙ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hcloud );
			//上の雲2の表示.
			TransBlt(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				( int ) ( Title.UPCloudX2 ), 0,	//表示位置x,y座標.
				446, 128,						//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.	
				256, 256 * 2 );					//元画像のx,y座標.

			//ﾀｲﾄﾙ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hcloud );
			//下の雲1の表示.
			TransBlt(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				( int ) ( Title.DownCloudX1 ),	//表示位置x座標.
				WND_H - ( 256 + 45 ),			//表示位置y座標.
				WND_W, 256,						//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.	
				0, 256 );						//元画像のx,y座標.
			//ﾀｲﾄﾙ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hcloud );
			//下の雲2の表示.
			TransBlt(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				( int ) ( Title.DownCloudX2 ),	//表示位置x座標.
				WND_H - ( 256 + 45 ),			//表示位置y座標.
				WND_W, 256,						//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.	
				0, 0 );							//元画像のx,y座標.

			//ﾀｲﾄﾙ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hTitle );
			//空を舞う葉っぱ描画.
			for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
				if ( Title.Leaf[LNo].DispFlag == true ) {
					TransPlgBlt(
						gameWnd->hScreenDC,									//デバイスコンテキスト.
						Title.Leaf[LNo].x,									//表示位置x座標.
						Title.Leaf[LNo].y,									//表示位置y座標.
						16, 16,												//画像の幅,高さ.
						hMemDC,												//メモリDC.
						LOGO_W + 25 + 16 * Title.Leaf[LNo].PtnX,			//元画像のx座標.
						WND_H + TITLE_H + 80 + 16 * Title.Leaf[LNo].PtnY, 	//元画像のy座標.
						Title.Leaf[LNo].Ang,								//回転角度.
						hPlgDC );											//回転用DC.
				}
			}

			//スライム画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hSlime );
			//スライムの表示.
			TransAlBlend(
				gameWnd->hScreenDC,				//デバイスコンテキスト.
				WND_W - 128 * 2, WND_H - 115,	//表示位置x,y座標.
				128, 64,						//画像の幅,高さ.
				hMemDC,							//メモリDC.
				128 * Title.Panimation.No,		//元画像のx座標.
				192,							//元画像のy座標.
				200,							//アルファ値.
				hWorkDC );						//作業用DC.

			//スライム画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hSlime );
			//地面の表示.
			TransBlt(
				gameWnd->hScreenDC,				//デバイスコンテキスト.
				0, WND_H - 96,					//表示位置x,y座標.
				WND_W, 96,						//画像の幅,高さ.
				hMemDC,							//メモリDC.
				0, 0 );							//元画像のx,y座標.

			//ﾀｲﾄﾙ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hTitle );
			//ロゴの表示.
			TransBlt(
				gameWnd->hScreenDC,				//デバイスコンテキスト.
				145, 131,						//表示位置x,y座標.
				LOGO_W, LOGO_H,					//画像の幅,高さ.
				hMemDC,							//メモリDC.
				0, WND_H + TITLE_H );			//元画像のx,y座標.

			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hcloud );
			//"GAME START"画像の表示.
			TransBlt(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				509, 550,						//表示位置x,y座標.
				256, 64,						//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.
				256 + 448, 256 * 2 );			//元画像のx,y座標.

			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hcloud );
			//"PRESS ANY BUTTON"画像の表示.
			TransAlBlend(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				544, 595,						//表示位置x,y座標.
				200, 16,						//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.
				256 + 448, 256 * 2 + 64,		//元画像のx,y座標.
				Title.Alpha,					//アルファ値.
				hWorkDC );

			break;
		}
		case enScene::GameMain:	//ｹﾞｰﾑﾒｲﾝ.
		{
			//ゲーム背景(スクロール)の画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hscroll );
			//ゲーム背景(スクロール)の表示.
			BitBlt(
				gameWnd->hScreenDC,					//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				( WND_W / 2 ) - ( GWND_W / 2 ),		//表示位置x座標.
				0,									//表示位置y座標.
				GWND_W, GWND_H,						//画像の幅,高さ.
				hMemDC,								//ﾒﾓﾘDC.
				0, static_cast< int >( scroll.y ),	//元画像のx,y座標.
				SRCCOPY );							//ｺﾋﾟｰする.
			//ゲーム背景(サブスクロール)の表示.
			if ( scroll.y < 0 ) {
				BitBlt(
					gameWnd->hScreenDC,									//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					( WND_W / 2 ) - ( GWND_W / 2 ),						//表示位置x座標.
					0,													//表示位置y座標.
					GWND_W, static_cast< int >( -scroll.y ),			//画像の幅,高さ.
					hMemDC,												//ﾒﾓﾘDC.
					0,													//元画像のx座標.
					static_cast< int >( GWND_H * 2 + scroll.y ),		//元画像のy座標.
					SRCCOPY );											//ｺﾋﾟｰする.
			}

			//ボスの突き上げ岩.
			for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ) {
				for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
					for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
						if ( Boss.rock[typeNo][numNo][dirNo].DispFlag == true ) {
							//キャラクタの画像をメモリDCへコピー.
							SelectObject( hMemDC, hboss );
							//ボスの突き上げ岩の表示.
							TransBlt(
								gameWnd->hScreenDC,																//デバイスコンテキスト.
								static_cast< int >( C_CORR + Boss.rock[typeNo][numNo][dirNo].x ),				//表示位置x座標.
								static_cast< int >( Boss.rock[typeNo][numNo][dirNo].y ),						//表示位置y座標.
								S_SIZE * 2, ( S_SIZE / 2 ) * ( Boss.rock[typeNo][numNo][dirNo].cnt / 10 + 1 ),	//画像の幅,高さ.
								hMemDC,																			//メモリDC.
								S_SIZE * 3,																		//元画像のx座標
								S_SIZE * 8 );																	//元画像のy座標.
						}
					}
				}
			}

			//ステージギミック.
			for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
					for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
						if ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GWait ) {
							if ( typeNo == 0 ) {	//木.
								if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GLiving ) {		//木.
									//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hTree );
									//ステージギミックの表示.
									TransBlt(
										gameWnd->hScreenDC,														//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//表示位置x座標.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//表示位置y座標.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//画像の幅,高さ.
										hMemDC,																	//ﾒﾓﾘDC.
										0, C_SIZE + gimmick[typeNo][ptnNo][gNo].hSize * ptnNo );				//元画像のx,y座標.
								}
								else if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GInvi ) {	//木(ダメージ).
									//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hTreeHit );
									//ステージギミックの表示.
									TransBlt(
										gameWnd->hScreenDC,														//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//表示位置x座標.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//表示位置y座標.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//画像の幅,高さ.
										hMemDC,																	//ﾒﾓﾘDC.
										C_SIZE * 4 + gimmick[typeNo][ptnNo][gNo].wSize * ptnNo, BLOW_H );			//元画像のx,y座標.
								}
								else if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GDead ) {	//切り株.
									//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hTree );
									//ステージギミックの表示.
									TransBlt(
										gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),					//表示位置x座標.
										static_cast< int >( C_SIZE + gimmick[typeNo][ptnNo][gNo].y - C_SIZE / 2 - 3 ),	//表示位置y座標.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//画像の幅,高さ.
										hMemDC,																			//ﾒﾓﾘDC.
										C_SIZE * 3,																		//元画像のx座標.
										gimmick[typeNo][ptnNo][gNo].hSize * ptnNo );									//元画像のy座標.
								}
							}
							else if ( typeNo == 1 ) {	//岩.
								//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
								SelectObject( hMemDC, hGmck );
								if ( gimmick[typeNo][ptnNo][gNo].RockFlag == false ) {
									//ステージギミックの表示.
									TransBlt(
										gameWnd->hScreenDC,														//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//表示位置x座標.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//表示位置y座標.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//画像の幅,高さ.
										hMemDC,																	//ﾒﾓﾘDC.
										C_SIZE * 2 + gimmick[typeNo][ptnNo][gNo].hSize * ptnNo, 0 );			//元画像のx,y座標.
								}
								else {
									for ( int bty = 1; bty < Boss.BOSS_TYPE; bty++ ) {
										if ( Boss.AttackNo[bty] == enBossAttackNo::BRock ) {
											//ステージギミックの表示.
											TransBlt(
												gameWnd->hScreenDC,															//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
												static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),				//表示位置x座標.
												static_cast< int >( gimmick[typeNo][ptnNo][gNo].y + ( gimmick[typeNo][ptnNo][gNo].hSize - ( gimmick[typeNo][ptnNo][gNo].hSize * 100 / Boss.animation[bty].cntMax * Boss.animation[bty].cnt / 100 ) ) ),						//表示位置y座標.
												gimmick[typeNo][ptnNo][gNo].wSize,
												( gimmick[typeNo][ptnNo][gNo].hSize * 100 / Boss.animation[bty].cntMax ) * Boss.animation[bty].cnt / 100,				//画像の幅,高さ.
												hMemDC,																		//ﾒﾓﾘDC.
												C_SIZE * 2 + gimmick[typeNo][ptnNo][gNo].hSize * ptnNo, 0 ); 				//元画像のx,y座標.
										}
									}
								}
							}
							else if ( typeNo == 2 ) {	//茨.
								if( ptnNo == 0 ){	//岩のとげ.
									//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hPSlime );
									//ステージギミックの表示.
									TransBlt(
										gameWnd->hScreenDC,														//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//表示位置x座標.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//表示位置y座標.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//画像の幅,高さ.
										hMemDC,																	//ﾒﾓﾘDC.
										C_SIZE * 2, C_SIZE * 2 );												//元画像のx,y座標.
								}
								else{				//毒の沼.
									//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hSlime );
									//ステージギミックの表示.
									TransBlt(
										gameWnd->hScreenDC,														//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//表示位置x座標.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//表示位置y座標.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//画像の幅,高さ.
										hMemDC,																	//ﾒﾓﾘDC.
										C_SIZE * 11, 192 );														//元画像のx,y座標.

									//毒沼のぶくぶくアニメーション.
									for ( int pNo = 0; pNo < P_MAX; pNo++ ) {
										if ( Poison[gNo][pNo].DispFlag == true ) {
											//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
											SelectObject( hMemDC, hTreeHit );
											//ぶくぶくの表示.
											TransAlBlend(
												gameWnd->hScreenDC,												//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
												static_cast< int >( C_CORR + Poison[gNo][pNo].x ),				//表示位置x座標.
												static_cast< int >( Poison[gNo][pNo].y ),						//表示位置y座標.
												18, 18,															//画像の幅,高さ.
												hMemDC,															//ﾒﾓﾘDC.
												447 + 18 * ( Poison[gNo][pNo].PtnNo / 2 ),						//元画像のx座標.
												BLOW_H + 18 * ( Poison[gNo][pNo].PtnNo % 2 ),					//元画像のy座標.
												Poison[gNo][pNo].Alpha,
												hWorkDC );
										}
									}
								}
							}
							else if ( typeNo == 3 ) {	//倒木.
								//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
								SelectObject( hMemDC, harea );
								//ステージギミックの表示.
								TransBlt(
									gameWnd->hScreenDC,															//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
									static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),				//表示位置x座標.
									static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),						//表示位置y座標.
									gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,		//画像の幅,高さ.
									hMemDC,																		//ﾒﾓﾘDC.
									ABLT_W + C_SIZE * ptnNo, ( PHP_H * 2 + 6 ) * 3 + C_SIZE * 2 );				//元画像のx,y座標.
							}
						}
					}
				}
			}

			//敵(死亡).
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[typeNo][ptnNo][eNo].appFlag == true ) || ( Enemy[typeNo][ptnNo][eNo].BossFlag == true ) ) {
							//敵機が死亡.
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dead )
							{
								if ( typeNo != 0 ) {	//花以外.
									//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hCharaDeat );
									//敵機の表示.
									TransBlt(
										gameWnd->hScreenDC,													//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),	//表示位置x座標.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),			//表示位置y座標.
										C_SIZE, C_SIZE,														//画像の幅,高さ.
										hMemDC,																//ﾒﾓﾘDC.
										C_SIZE * ( ( typeNo + 1 ) % 3 ),									//元画像のx座標.
										C_SIZE * ( ( typeNo + 1 ) / 3 ) );									//元画像のy座標.
								}
								else {	//花.
									//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hCharaDeat );
									//敵機の表示.
									TransBlt(
										gameWnd->hScreenDC,													//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),	//表示位置x座標.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),			//表示位置y座標.
										C_SIZE, C_SIZE,														//画像の幅,高さ.
										hMemDC,																//ﾒﾓﾘDC.
										C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No / 3 ), 0 );		//元画像のx,y座標.
								}
							}
						}
					}
				}
			}

			//ボスの吹き飛ばしエフェクト.
			if ( Boss.BlowAway.DispFlag == true ) {
				//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hTreeHit );
				//敵機の表示.
				TransAlBlend(
					gameWnd->hScreenDC,													//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + 0 ),									//表示位置x座標.
					static_cast< int >( Boss.BlowAway.y ),								//表示位置y座標.
					GWND_W, BLOW_H,														//画像の幅,高さ.
					hMemDC,																//ﾒﾓﾘDC.
					0, 0,																//元画像のx,y座標.
					200,																//アルファ値.
					hWorkDC );															//作業用DC.
			}

			//自機が生存中.
			if ( Player.chara.State == enCharaState::Living )
			{
				if ( Player.SlimeNo == enSlime::SNormal ){
					if ( Player.AbGetFlag == true ){
						//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
						SelectObject( hMemDC, hEnEng );
						//自機の表示.
						ExpaTransAlBlend(
							gameWnd->hScreenDC,													//デバイスコンテキスト.
							static_cast< int >( C_CORR + Player.chara.x - Player.GetAnimaX ),	//表示位置x座標( 補正値:13 ).
							static_cast< int >( Player.chara.y + Player.GetAnimaY ),			//表示位置y座標( 補正値:13 ).
							C_SIZE, C_SIZE,														//画像の幅,高さ.
							hMemDC,																//メモリDC.
							288 + C_SIZE * Player.animation.No,									//元画像のx座標.
							336,																//元画像のy座標.
							200,																//アルファ値.
							hWorkDC,															//作業用DC.
							C_SIZE + Player.GetAnimaCnt * 2,									//サイズ変更後の幅.
							C_SIZE - Player.GetAnimaCnt * 2,									//サイズ変更後の高さ.
							false );															//中央拡大をしない(右下拡大).
					}
					else{
						//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
						SelectObject( hMemDC, hEnEng );
						//自機の表示.
						TransPlgAlBlend(
							gameWnd->hScreenDC,													//デバイスコンテキスト.
							static_cast< int >( C_CORR + Player.chara.x - 13 ),					//表示位置x座標( 補正値:13 ).
							static_cast< int >( Player.chara.y - 13 ),							//表示位置y座標( 補正値:13 ).
							C_SIZE, C_SIZE,														//画像の幅,高さ.
							hMemDC,																//メモリDC.
							288 + C_SIZE * Player.animation.No,									//元画像のx座標.
							336,																//元画像のy座標.
							Player.chara.ang,													//画像角度.
							hPlgDC,																//回転用DC.
							200,																//アルファ値.
							hWorkDC );															//作業用DC.
					}
				}
				else{
					if ( Player.AbGetFlag == true ){
						//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
						SelectObject( hMemDC, hPSlime );
						//自機の表示.
						ExpaTransAlBlend(
							gameWnd->hScreenDC,													//デバイスコンテキスト.
							static_cast< int >( C_CORR + Player.chara.x - Player.GetAnimaX ),	//表示位置x座標( 補正値:13 ).
							static_cast< int >( Player.chara.y + Player.GetAnimaY ),			//表示位置y座標( 補正値:13 ).
							C_SIZE, C_SIZE,														//画像の幅,高さ.
							hMemDC,																//メモリDC.
							C_SIZE * ( ( Player.SlimeNo * 2 ) % 4 + Player.animation.No ),		//元画像のx座標.
							C_SIZE * ( ( Player.SlimeNo * 2 ) / 4 ),							//元画像のy座標.
							200,																//アルファ値.
							hWorkDC,															//作業用DC.
							C_SIZE + Player.GetAnimaCnt * 2,									//サイズ変更後の幅.
							C_SIZE - Player.GetAnimaCnt * 2,									//サイズ変更後の高さ.
							false );															//中央拡大をしない(右下拡大).
					}
					else{
						//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
						SelectObject( hMemDC, hPSlime );
						//自機の表示.
						TransPlgAlBlend(
							gameWnd->hScreenDC,													//デバイスコンテキスト.
							static_cast< int >( C_CORR + Player.chara.x - 13 ),					//表示位置x座標( 補正値:13 ).
							static_cast< int >( Player.chara.y - 13 ),							//表示位置y座標( 補正値:13 ).
							C_SIZE, C_SIZE,														//画像の幅,高さ.
							hMemDC,																//メモリDC.
							C_SIZE * ( ( Player.SlimeNo * 2 ) % 4 + Player.animation.No ),		//元画像のx座標.
							C_SIZE * ( ( Player.SlimeNo * 2 ) / 4 ),							//元画像のy座標.
							Player.chara.ang,													//画像角度.
							hPlgDC,																//回転用DC.
							235,																//アルファ値.
							hWorkDC );															//作業用DC.
					}
				}
			}
			//自機が無敵.
			else if ( Player.chara.State == enCharaState::Invi )
			{
				if ( Player.SlimeNo == enSlime::SNormal ){
					//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, hEnEng );
					//自機の表示.
					TransPlgAlBlend(
						gameWnd->hScreenDC,														//デバイスコンテキスト.
						static_cast< int >( C_CORR + Player.chara.x - 13 ),						//表示位置x座標( 補正値:13 ).
						static_cast< int >( Player.chara.y - 13 ),								//表示位置y座標( 補正値:13 ).
						C_SIZE, C_SIZE,															//画像の幅,高さ.
						hMemDC,																	//メモリDC.
						416 + C_SIZE * Player.animation.No,										//元画像のx座標.
						336,																	//元画像のy座標.
						Player.chara.ang,														//画像角度.
						hPlgDC,																	//回転用DC.
						200,																	//アルファ値.
						hWorkDC );																//作業用DC.
				}
				else{
					//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, hPSlimeHit );
					//自機の表示.
					TransPlgAlBlend(
						gameWnd->hScreenDC,														//デバイスコンテキスト.
						static_cast< int >( C_CORR + Player.chara.x - 13 ),						//表示位置x座標( 補正値:13 ).
						static_cast< int >( Player.chara.y - 13 ),								//表示位置y座標( 補正値:13 ).
						C_SIZE, C_SIZE,															//画像の幅,高さ.
						hMemDC,																	//メモリDC.
						C_SIZE * ( ( Player.SlimeNo * 2 ) % 4 + Player.animation.No ),			//元画像のx座標.
						C_SIZE * ( ( Player.SlimeNo * 2 ) / 4 ),								//元画像のy座標.
						Player.chara.ang,														//画像角度.
						hPlgDC,																	//回転用DC.
						235,																	//アルファ値.
						hWorkDC );																//作業用DC.
				}
			}
			//自機が死亡中.
			else if ( Player.chara.State == enCharaState::Dying )
			{
				if ( Player.SlimeNo == enSlime::SNormal ){
					//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, hSlime );
					//自機の表示.
					ExpaTransAlBlend(
						gameWnd->hScreenDC,														//デバイスコンテキスト.
						static_cast< int >( C_CORR + Player.chara.x ),							//表示位置x座標( 補正値:13 ).
						static_cast< int >( Player.chara.y ),									//表示位置y座標( 補正値:13 ).
						C_SIZE, C_SIZE,															//画像の幅,高さ.
						hMemDC,																	//メモリDC.
						128 * 3 + C_SIZE * ( Player.animation.No * 2 ),							//元画像のx座標.
						192,																	//元画像のy座標.
						200,																	//アルファ値.
						hWorkDC,																//作業用DC.
						C_SIZE + Player.chara.ExpAnimCnt * 2,									//サイズ変更後の幅.
						C_SIZE - Player.chara.ExpAnimCnt * 2,									//サイズ変更後の高さ.
						false );																//中央拡大をしない(右下拡大).
				}
				else{
					//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, hPSlime );
					//自機の表示.
					ExpaTransAlBlend(
						gameWnd->hScreenDC,														//デバイスコンテキスト.
						static_cast< int >( C_CORR + Player.chara.x ),							//表示位置x座標( 補正値:13 ).
						static_cast< int >( Player.chara.y ),									//表示位置y座標( 補正値:13 ).
						C_SIZE, C_SIZE,															//画像の幅,高さ.
						hMemDC,																	//メモリDC.
						C_SIZE * ( ( Player.SlimeNo * 2 ) % 4 + Player.animation.No ),			//元画像のx座標.
						C_SIZE * ( ( Player.SlimeNo * 2 ) / 4 ),								//元画像のy座標.
						235,																	//アルファ値.
						hWorkDC,																//作業用DC.
						C_SIZE + Player.chara.ExpAnimCnt * 2,									//サイズ変更後の幅.
						C_SIZE - Player.chara.ExpAnimCnt * 2,									//サイズ変更後の高さ.
						false );																//中央拡大をしない(右下拡大).
				}
			}

			//プレイヤーのダメージ画像.
			for( int i = 0; i < 2; i++ ){
				if ( Player.Damage[i].DispFlag == true ){
					//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, hPSlime );
					//自機の表示.
					TransBlt(
						gameWnd->hScreenDC,										//デバイスコンテキスト.
						static_cast< int >( C_CORR + Player.Damage[i].x ),		//表示位置x座標( 補正値:13 ).
						static_cast< int >( Player.Damage[i].y ),				//表示位置y座標( 補正値:13 ).
						23, 23,													//画像の幅,高さ.
						hMemDC,													//メモリDC.
						C_SIZE * 3,												//元画像のx座標.
						C_SIZE * 2 + 16 * 2 );									//元画像のy座標.
				}
			}

			//プレイヤーの回復エフェクト.
			for ( int hNo = 0; hNo < H_MAX; hNo++ ){
				if ( Player.HeelAnima[hNo].DispFlag == true ){
					//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, harea );
					//自機の表示.
					TransAlBlend(
						gameWnd->hScreenDC,																								//デバイスコンテキスト.
						static_cast< int >( C_CORR + Player.HeelAnima[hNo].x ),															//表示位置x座標( 補正値:13 ).
						static_cast< int >( Player.HeelAnima[hNo].y - Player.HeelAnima[hNo].UpY ),										//表示位置y座標( 補正値:13 ).
						C_SIZE / 2, C_SIZE / 2,																							//画像の幅,高さ.
						hMemDC,																											//メモリDC.
						ABLT_W + C_SIZE * 3 + C_SIZE / 2 * Player.HeelAnima[hNo].PtnX,													//元画像のx座標.
						( PHP_H * 2 + 5 ) * 3 + C_SIZE * Player.HeelAnima[hNo].PtnY + C_SIZE / 2 * Player.HeelAnima[hNo].animation.No,	//元画像のy座標.
						Player.HeelAnima[hNo].Alpha,																					//アルファ値.
						hWorkDC );																										//作業用DC.
				}
			}

			//ボス本体.
			if ( Boss.chara[enBoss::Main].State == enCharaState::Anima ) {	//登場アニメーション.
				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ本体の表示.
				TransBlt(
					gameWnd->hScreenDC,															//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x ),					//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::Main].y ),							//表示位置y座標.
					Boss.charaSize[enBoss::Main],												//画像の幅.
					( Boss.charaSize[enBoss::Main] / Boss.DispAnima.max ) * Boss.DispAnima.No,	//画像の高さ.
					hMemDC,																		//ﾒﾓﾘDC.
					0, 0 );																		//元画像のx,y座標.
			}
			else if ( Boss.chara[enBoss::Main].State == enCharaState::Living ) {	//生存中.
				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ本体の表示.
				TransBlt(
					gameWnd->hScreenDC,											//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x ),	//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::Main].y ),			//表示位置y座標.
					Boss.charaSize[enBoss::Main], Boss.charaSize[enBoss::Main],	//画像の幅,高さ.
					hMemDC,														//ﾒﾓﾘDC.
					0, 0 );														//元画像のx,y座標.
			}
			else if ( Boss.chara[enBoss::Main].State == enCharaState::Invi ) {	//無敵時間.
				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, harea );
				//ﾎﾞｽ本体の表示.
				TransBlt(
					gameWnd->hScreenDC,												//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x ),		//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::Main].y ),				//表示位置y座標.
					Boss.charaSize[enBoss::Main], Boss.charaSize[enBoss::Main],		//画像の幅,高さ.
					hMemDC,															//ﾒﾓﾘDC.
					ABLT_W + 1, ( PHP_H * 2 + 6 ) * 3 );							//元画像のx,y座標.
			}
			else if ( Boss.chara[enBoss::Main].State == enCharaState::Dying ) {	//死亡中.
				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ本体の表示.
				TransBlt(
					gameWnd->hScreenDC,																					//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x ),											//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::Main].y ),													//表示位置y座標.
					Boss.charaSize[enBoss::Main],																		//画像の幅.
					( Boss.charaSize[enBoss::Main] / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ),	//画像の高さ.
					hMemDC,																								//ﾒﾓﾘDC.
					0, 0 );																								//元画像のx,y座標.
			}

			//ボス右手.
			if ( Boss.chara[enBoss::RHand].State == enCharaState::Anima ) {	//登場アニメーション.
				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hTree );
				//ﾎﾞｽ右手の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::RHand].x - 20 ),								//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::RHand].y - 5 - Boss.animation[enBoss::Main].No ),		//表示位置y座標.
					Boss.charaSize[enBoss::RHand],																	//画像の幅.
					( ( Boss.charaSize[enBoss::RHand] * 100 / Boss.DispAnima.max ) * Boss.DispAnima.No ) / 100,		//画像の高さ.
					hMemDC,																							//ﾒﾓﾘDC.
					0, 0,																							//元画像のx,y座標.
					Boss.chara[enBoss::RHand].ang,																	//画像角度.
					hPlgDC );																						//画像回転用DC.

				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ右肩の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																												//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x + Boss.charaSize[0] - 20 ),												//表示位置x.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::RHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),//表示位置y.
					C_SIZE / 2 + S_SIZE,																											//画像の幅.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * Boss.DispAnima.No ) / 100,													//画像の高さ.
					hMemDC,																															//ﾒﾓﾘDC.
					0, C_SIZE * 2 + C_SIZE / 2,																										//元画像のx,y.
					Boss.chara[enBoss::RHand].ang,																									//角度.
					hPlgDC );																														//画像回転用DC.
			}
			else if ( Boss.chara[enBoss::RHand].State == enCharaState::Living ) {	//生存中.
				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hTree );
				//ﾎﾞｽ右手の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																		//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::RHand].x - 20 ),						//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::RHand].y - 5 - Boss.animation[enBoss::Main].No ),//表示位置y座標.
					Boss.charaSize[enBoss::RHand], Boss.charaSize[enBoss::RHand],							//画像の幅,高さ.
					hMemDC,																					//ﾒﾓﾘDC.
					0, 0,																					//元画像のx,y座標.
					Boss.chara[enBoss::RHand].ang,															//角度.
					hPlgDC );																				//画像回転用DC.

				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ右肩の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																										//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x + Boss.charaSize[enBoss::Main] - 20 ),							//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::RHand] - 20 + ( Boss.animation[0].No / 2 ) ),	//表示位置y座標.
					C_SIZE / 2 + S_SIZE, C_SIZE / 2,																						//画像の幅,高さ.
					hMemDC,																													//ﾒﾓﾘDC.
					0, C_SIZE * 2 + C_SIZE / 2,																								//元画像のx,y座標.
					Boss.chara[enBoss::RHand].ang,																							//角度.
					hPlgDC );																												//画像回転用DC.
			}
			else if ( Boss.chara[enBoss::RHand].State == enCharaState::Invi ) {	//無敵時間.
				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, harea );
				//ﾎﾞｽ右手の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																										//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::RHand].x - 20 ),														//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::RHand].y - 5 - Boss.animation[enBoss::Main].No ),								//表示位置y座標.
					Boss.charaSize[enBoss::RHand], Boss.charaSize[enBoss::RHand],															//画像の幅,高さ.
					hMemDC,																													//ﾒﾓﾘDC.
					ABLT_W + C_SIZE * 2, ( PHP_H * 2 + 5 ) * 3 + C_SIZE + 3,																	//元画像のx,y座標.
					Boss.chara[enBoss::RHand].ang,																							//角度.
					hPlgDC );																												//画像回転用DC.

				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, harea );
				//ﾎﾞｽ右肩の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																										//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x + Boss.charaSize[enBoss::Main] - 20 ),							//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::RHand] - 20 + ( Boss.animation[0].No / 2 ) ),	//表示位置y座標.
					C_SIZE / 2 + S_SIZE, C_SIZE / 2,																						//画像の幅,高さ.
					hMemDC,																													//ﾒﾓﾘDC.
					ABLT_W + C_SIZE * 2, ( PHP_H * 2 + 5 ) * 3 + C_SIZE * 2 + 3,																//元画像のx,y座標.
					Boss.chara[enBoss::RHand].ang,																							//角度.
					hPlgDC );																												//画像回転用DC.
			}
			else if ( Boss.chara[enBoss::RHand].State == enCharaState::Dying ) {	//死亡中.

				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hTree );
				//ﾎﾞｽ右手の表示.
				ExpaTransBlt(
					gameWnd->hScreenDC,																																				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::RHand].x - 20 ),																								//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::RHand].y - 5 - Boss.animation[enBoss::Main].No ),																		//表示位置y座標.
					Boss.charaSize[enBoss::RHand],																																	//画像の幅.
					( ( Boss.charaSize[enBoss::RHand] * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100,											//画像の高さ.
					hMemDC,																																							//ﾒﾓﾘDC.
					0, 0,																																							//元画像のx,y座標.
					Boss.charaSize[enBoss::RHand] + Boss.chara[enBoss::RHand].ExpAnimCnt * 2,																						//サイズ変更後の幅.
					( ( Boss.charaSize[enBoss::RHand] * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100 - Boss.chara[enBoss::RHand].ExpAnimCnt * 2,	//サイズ変更後の高さ.
					false );																																						//中央拡大をしない(右下拡大).

				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ右肩の表示.
				ExpaTransBlt(
					gameWnd->hScreenDC,																																				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x + Boss.charaSize[enBoss::Main] - 20 ),																	//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::RHand] - 20 + ( Boss.animation[0].No / 2 ) ),											//表示位置y座標.
					C_SIZE / 2 + S_SIZE,																																			//画像の幅.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100,															//画像の高さ.
					hMemDC,																																							//ﾒﾓﾘDC.
					0, C_SIZE * 2 + C_SIZE / 2,																																		//元画像のx,y座標.
					C_SIZE / 2 + S_SIZE + Boss.chara[enBoss::RHand].ExpAnimCnt * 2,																									//サイズ変更後の幅.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100 - Boss.chara[enBoss::RHand].ExpAnimCnt * 2,				//サイズ変更後の高さ.
					false );																																						//中央拡大をしない(右下拡大).
			}

			//ボスの左手.
			if ( Boss.chara[enBoss::LHand].State == enCharaState::Anima ) {	//登場アニメーション.
				//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ左手の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::LHand].x - 5 ),									//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::LHand].y - 5 - Boss.animation[enBoss::Main].No ),		//表示位置y座標.
					Boss.charaSize[enBoss::LHand],																	//画像の幅.
					( ( Boss.charaSize[enBoss::LHand] * 100 / Boss.DispAnima.max ) * Boss.DispAnima.No ) / 100,		//画像の高さ.
					hMemDC,																							//ﾒﾓﾘDC.
					Boss.charaSize[enBoss::Main], 0,																//元画像のx,y座標.
					Boss.chara[enBoss::LHand].ang,																	//画像角度.
					hPlgDC );																						//画像回転用DC.

				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ左肩の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																													//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x - Boss.charaSize[enBoss::LHand] / 2 ),										//表示位置x座標.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::LHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),	//表示位置y座標.
					C_SIZE / 2,																															//画像の幅.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * Boss.DispAnima.No ) / 100,														//画像の高さ.
					hMemDC,																																//ﾒﾓﾘDC.
					0, C_SIZE * 2,																														//元画像のx,y座標.
					Boss.chara[enBoss::LHand].ang,																										//角度.
					hPlgDC );																															//画像回転DC.
			}
			else if ( Boss.chara[enBoss::LHand].State == enCharaState::Living ) {	//生存中.
				//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ左手の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																		//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::LHand].x - 5 ),							//表示位置x.
					static_cast< int >( Boss.chara[enBoss::LHand].y - 5 - Boss.animation[enBoss::Main].No ),//表示位置y.
					Boss.charaSize[enBoss::LHand], Boss.charaSize[enBoss::LHand],							//画像の幅,高さ.
					hMemDC,																					//ﾒﾓﾘDC.
					Boss.charaSize[enBoss::Main], 0,														//元画像のx,y.
					Boss.chara[enBoss::LHand].ang,															//角度.
					hPlgDC );																				//作業用DC.

				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ左肩の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																												//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x - Boss.charaSize[enBoss::LHand] / 2 ),									//表示位置x.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::LHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),//表示位置y.
					C_SIZE / 2, C_SIZE / 2,																											//画像の幅,高さ.
					hMemDC,																															//ﾒﾓﾘDC.
					0, C_SIZE * 2,																													//元画像のx,y.
					Boss.chara[enBoss::LHand].ang,																									//角度.
					hPlgDC );																														//作業用DC.
			}
			else if ( Boss.chara[enBoss::LHand].State == enCharaState::Invi ) {	//無敵時間.
				//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, harea );
				//ﾎﾞｽ左手の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																												//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::LHand].x - 5 ),																	//表示位置x.
					static_cast< int >( Boss.chara[enBoss::LHand].y - 5 - Boss.animation[enBoss::Main].No ),										//表示位置y.
					Boss.charaSize[enBoss::LHand], Boss.charaSize[enBoss::LHand],																	//画像の幅,高さ.
					hMemDC,																															//ﾒﾓﾘDC.
					ABLT_W + C_SIZE * 2, ( PHP_H * 2 + 5 ) * 3 + 3,																						//元画像のx,y.
					Boss.chara[enBoss::LHand].ang,																									//角度.
					hPlgDC );																														//作業用DC.

				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, harea );
				//ﾎﾞｽ左肩の表示.
				TransPlgBlt(
					gameWnd->hScreenDC,																												//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x - Boss.charaSize[enBoss::LHand] / 2 ),									//表示位置x.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::LHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),//表示位置y.
					C_SIZE / 2, C_SIZE / 2,																											//画像の幅,高さ.
					hMemDC,																															//ﾒﾓﾘDC.
					ABLT_W + C_SIZE * 2, ( PHP_H * 2 + 5 ) * 3 + C_SIZE * 2 + C_SIZE / 2 + 3,															//元画像のx,y.
					Boss.chara[enBoss::LHand].ang,																									//角度.
					hPlgDC );																														//作業用DC.
			}
			else if ( Boss.chara[enBoss::LHand].State == enCharaState::Dying ) {	//死亡中.
				//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ左手の表示.
				ExpaTransBlt(
					gameWnd->hScreenDC,																												//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::LHand].x - 5 ),																	//表示位置x.
					static_cast< int >( Boss.chara[enBoss::LHand].y - 5 - Boss.animation[enBoss::Main].No ),										//表示位置y.
					Boss.charaSize[enBoss::LHand],																									//画像の幅.
					( ( Boss.charaSize[enBoss::LHand] * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100,			//画像の高さ.
					hMemDC,																															//ﾒﾓﾘDC.
					Boss.charaSize[enBoss::Main], 0,																								//元画像のx,y.
					Boss.charaSize[enBoss::LHand] + Boss.chara[enBoss::LHand].ExpAnimCnt * 2,
					( ( Boss.charaSize[enBoss::LHand] * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100 - Boss.chara[enBoss::LHand].ExpAnimCnt * 2,
					false );

				//ボスの画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hboss );
				//ﾎﾞｽ左肩の表示.
				ExpaTransBlt(
					gameWnd->hScreenDC,																												//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x - Boss.charaSize[enBoss::LHand] / 2 ),									//表示位置x.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::LHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),//表示位置y.
					C_SIZE / 2,																														//画像の幅.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100,							//画像の高さ.
					hMemDC,																															//ﾒﾓﾘDC.
					0, C_SIZE * 2,																													//元画像のx,y.
					C_SIZE / 2 + Boss.chara[enBoss::LHand].ExpAnimCnt * 2,
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100 - Boss.chara[enBoss::LHand].ExpAnimCnt * 2,
					false );
			}

			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[typeNo][ptnNo][eNo].appFlag == true ) || ( Enemy[typeNo][ptnNo][eNo].BossFlag == true ) ) {
							//敵機が生存中.
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living )
							{
								if ( typeNo != 0 ) {	//花以外.
									//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hChara );
									//敵機(花以外)の表示.
									TransBlt(
										gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//表示位置x座標.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//表示位置y座標.
										C_SIZE, C_SIZE,																	//画像の幅,高さ.
										hMemDC,																			//ﾒﾓﾘDC.
										C_SIZE * ( ( typeNo - 1 ) % 2 * 2 + Enemy[typeNo][ptnNo][eNo].animetion.No ),	//元画像のx座標( %2 : 0, 1, 0, 1, 0, ... ).
										C_SIZE * 2 + C_SIZE * ( ( typeNo - 1 ) / 2 ) );									//元画像のy座標( /2 : 0, 0, 1, 1, 2, ... ).
								}
								else {	//花.
									//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hChara );
									//敵機(花)の表示.
									TransBlt(
										gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//表示位置x座標.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//表示位置y座標.
										C_SIZE, C_SIZE,																	//画像の幅,高さ.
										hMemDC,																			//ﾒﾓﾘDC.
										C_SIZE + C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No % 3 ),				//元画像のx座標( %3 : 0, 1, 2, 0, 1, 2, ... ).
										C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No / 3 ) );						//元画像のy座標( /3 : 0, 0, 0, 1, 1, 1, ... ).
								}
							}
							//敵機が無敵.
							else if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) {
								if ( typeNo != 0 ) {
									//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hCharaHit );
									//敵機の表示.
									TransBlt(
										gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//表示位置x座標.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//表示位置y座標.
										C_SIZE, C_SIZE,										 							//画像の幅,高さ.
										hMemDC,																			//ﾒﾓﾘDC.
										C_SIZE * ( ( typeNo - 1 ) % 2 * 2 + Enemy[typeNo][ptnNo][eNo].animetion.No ),	//元画像のx座標( %2 : 0, 1, 0, 1, 0, 1, ... ).
										C_SIZE * 2 + C_SIZE * ( ( typeNo - 1 ) / 2 ) );									//元画像のy座標( /2 : 0, 0, 1, 1, 2, 2, ... ).
								}
								else {
									//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hCharaHit );
									//敵機の表示.
									TransBlt(
										gameWnd->hScreenDC,														//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),		//表示位置x座標.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),				//表示位置y座標.
										C_SIZE, C_SIZE,															//画像の幅,高さ.
										hMemDC,																	//ﾒﾓﾘDC.
										C_SIZE + C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No % 3 ),		//元画像のx座標( %3 : 0, 1, 2, 0, 1, 2, ... ).
										C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No / 3 ) );				//元画像のy座標( /3 : 0, 0, 0, 1, 1, 1, ... ).
								}
							}
							//敵機が死亡中.
							else if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dying )
							{
								if ( typeNo != 0 ) {
									//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hChara );
									//敵機(花以外)の表示.
									ExpaTransBlt(
										gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//表示位置x座標.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//表示位置y座標.
										C_SIZE, C_SIZE,																	//画像の幅,高さ.
										hMemDC,																			//ﾒﾓﾘDC.
										C_SIZE * ( ( typeNo - 1 ) % 2 * 2 + Enemy[typeNo][ptnNo][eNo].animetion.No ),	//元画像のx座標.
										C_SIZE * 2 + C_SIZE * ( ( typeNo - 1 ) / 2 ),									//元画像のy座標.
										C_SIZE + Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt * 2,						//サイズ変更後の幅.
										C_SIZE - Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt * 2,						//サイズ変更後の高さ.
										false );																		//中央拡大をしない(右下拡大).
								}
								else {
									//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hChara );
									//敵機(花)の表示.
									ExpaTransBlt(
										gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//表示位置x座標.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//表示位置y座標.
										C_SIZE, C_SIZE,																	//画像の幅,高さ.
										hMemDC,																			//ﾒﾓﾘDC.
										C_SIZE + C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No % 3 ),				//元画像のx座標.
										C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No / 3 ),						//元画像のy座標.
										C_SIZE + Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt * 2,						//サイズ変更後の幅.
										C_SIZE - Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt * 2,						//サイズ変更後の高さ.
										false );																		//中央拡大をしない(右下拡大).
								}
							}
						}
						//敵の回復エフェクト.
						for ( int hNo = 0; hNo < H_MAX; hNo++ ){
							if ( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].DispFlag == true ){
								//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
								SelectObject( hMemDC, harea );
								//自機の表示.
								TransAlBlend(
									gameWnd->hScreenDC,																																		//デバイスコンテキスト.
									static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].x ),																				//表示位置x座標( 補正値:13 ).
									static_cast< int >( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].y - Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].UpY ),										//表示位置y座標( 補正値:13 ).
									C_SIZE / 2, C_SIZE / 2,																																	//画像の幅,高さ.
									hMemDC,																																					//メモリDC.
									ABLT_W + C_SIZE * 3 + C_SIZE / 2 * Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].PtnX,																		//元画像のx座標.
									( PHP_H * 2 + 5 ) * 3 + C_SIZE * Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].PtnY + C_SIZE / 2 * Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No,	//元画像のy座標.
									Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].Alpha,																											//アルファ値.
									hWorkDC );																																				//作業用DC.
							}
						}
					}
				}
			}

			for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
				for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
					if ( ( Player.shot[numNo][dirNo].DispShotFlag == true ) || ( Player.chara.chgCnt != 0 ) ) {
						if ( Player.ability.beam != 0 ) {
							//キャラクタの画像をメモリDCへコピー.
							SelectObject( hMemDC, hboss );
							//プレイヤーのビーム弾の表示.
							TransPlgBlt(
								gameWnd->hScreenDC,																													//デバイスコンテキスト.
								static_cast< int >( C_CORR + Player.shot[numNo][dirNo].x - S_SIZE - ( 8 - 7 * ( BEAMMAX - Player.shot[numNo][dirNo].beamCnt ) ) ),	//表示位置x.
								static_cast< int >( Player.shot[numNo][dirNo].y - S_SIZE ),																			//表示位置y.
								S_SIZE, S_SIZE * Player.shot[numNo][dirNo].beamCnt,																					//画像の幅,高さ.
								hMemDC,																																//メモリDC.
								S_SIZE * 7,																															//元画像のx座標
								S_SIZE * 8 + S_SIZE * ( BEAMMAX - Player.shot[numNo][dirNo].beamCnt ),																//元画像のy座標.
								RAD( 90 ) - Player.shot[numNo][dirNo].ang,
								hPlgDC );
						}
						else if ( Player.ability.chg != 0 ) {
							if ( ( Player.shot[numNo][dirNo].DispShotFlag == false ) && ( Player.chara.chgCnt != 0 ) ) {	//溜め状態.
								//キャラクタの画像をメモリDCへコピー.
								SelectObject( hMemDC, hboss );
								//プレイヤーの溜め弾の表示.
								ExpaTransAlBlend(
									gameWnd->hScreenDC,											//デバイスコンテキスト.
									static_cast< int >( C_CORR + Player.shot[numNo][dirNo].x ),	//表示位置x.
									static_cast< int >( Player.shot[numNo][dirNo].y ),			//表示位置y.
									S_SIZE * 2, S_SIZE * 2,										//画像の幅,高さ.
									hMemDC,														//メモリDC.
									S_SIZE * 5, S_SIZE * 10,									//元画像のy座標
									static_cast< int >( 50 + Player.chara.chgCnt + Player.chgAniCnt ),
									hWorkDC,
									static_cast< int >( S_SIZE * ( 2 + Player.chara.chgCnt / 75 ) ),	//幅の拡大倍率.
									static_cast< int >( S_SIZE *( 2 + Player.chara.chgCnt / 75 ) ) );	//高さの拡大倍率.	
							}
							else {	//発射後.
								//キャラクタの画像をメモリDCへコピー.
								SelectObject( hMemDC, hboss );
								//プレイヤーの溜め弾の表示.
								ExpaTransBlt(
									gameWnd->hScreenDC,											//デバイスコンテキスト.
									static_cast< int >( C_CORR + Player.shot[numNo][dirNo].x ),	//表示位置x.
									static_cast< int >( Player.shot[numNo][dirNo].y ),			//表示位置y.
									S_SIZE * 2, S_SIZE * 2,										//画像の幅,高さ.
									hMemDC,														//メモリDC.
									S_SIZE * 5, S_SIZE * 10,									//元画像のy座標
									static_cast< int >( S_SIZE * ( 2 + Player.chara.chgCnt / 75 ) ),	//幅の拡大倍率.
									static_cast< int >( S_SIZE *( 2 + Player.chara.chgCnt / 75 ) ) );	//高さの拡大倍率.	
							}
						}
						else if ( Player.ability.pene != 0 ) {
							//キャラクタの画像をメモリDCへコピー.
							SelectObject( hMemDC, hboss );
							//プレイヤーの貫通弾の表示.
							TransBlt(
								gameWnd->hScreenDC,											//デバイスコンテキスト.
								static_cast< int >( C_CORR + Player.shot[numNo][dirNo].x ),	//表示位置x.
								static_cast< int >( Player.shot[numNo][dirNo].y ),			//表示位置y.
								S_SIZE, S_SIZE,												//画像の幅,高さ.
								hMemDC,														//メモリDC.
								S_SIZE * 3, S_SIZE * 11 );									//元画像のy座標
						}
						else {
							//キャラクタの画像をメモリDCへコピー.
							SelectObject( hMemDC, hShot );
							//プレイヤーのノーマル弾の表示.
							TransBlt(
								gameWnd->hScreenDC,											//デバイスコンテキスト.
								static_cast< int >( C_CORR +  Player.shot[numNo][dirNo].x ),//表示位置x.
								static_cast< int >( Player.shot[numNo][dirNo].y ),			//表示位置y.
								S_SIZE, S_SIZE,												//画像の幅,高さ.
								hMemDC,														//メモリDC.
								0, 0 );														//画像のy座標.
						}	
					}
				}
			}

			//敵の弾.
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
							for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
								if ( ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) || ( Enemy[typeNo][ptnNo][eNo].chara.chgCnt != 0 ) ) {
									switch ( typeNo )
									{
									case 0:	//溜め.
										if ( ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == false ) && ( Enemy[typeNo][ptnNo][eNo].chara.chgCnt != 0 ) ) {	//溜め状態.
											//キャラクタの画像をメモリDCへコピー.
											SelectObject( hMemDC, hShot );
											ExpaTransAlBlend(
												gameWnd->hScreenDC,																//デバイスコンテキスト.
												static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//表示位置x.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),			//表示位置y.
												S_SIZE, S_SIZE,																	//画像の幅,高さ.
												hMemDC,																			//メモリDC.
												0, S_SIZE * 4,
												static_cast< int >( 50 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt ),
												hWorkDC,
												static_cast< int >( S_SIZE * ( 2 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt / 75 ) ),
												static_cast< int >( S_SIZE * ( 2 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt / 75 ) ) );
										}
										else {	//発射後.
											//キャラクタの画像をメモリDCへコピー.
											SelectObject( hMemDC, hShot );
											ExpaTransBlt(
												gameWnd->hScreenDC,																//デバイスコンテキスト.
												static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//表示位置x.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),			//表示位置y.
												S_SIZE, S_SIZE,																	//画像の幅,高さ.
												hMemDC,																			//メモリDC.
												0, S_SIZE * 4,
												static_cast< int >( S_SIZE * ( 2 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt / 75 ) ),
												static_cast< int >( S_SIZE * ( 2 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt / 75 ) ) );
										}
										break;
									case 1:	//普通.
										//キャラクタの画像をメモリDCへコピー.
										SelectObject( hMemDC, hShot );
										TransPlgBlt(
											gameWnd->hScreenDC,																			//デバイスコンテキスト.
											static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE * 2 ),	//表示位置x.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE * 2 ),			//表示位置y.
											S_SIZE * 4, 50 - 10 * Enemy[typeNo][ptnNo][eNo].animetion.No,								//画像の幅,高さ.
											hMemDC,																						//メモリDC.
											S_SIZE * 2, 50 * Enemy[typeNo][ptnNo][eNo].animetion.No,									//元画像のx,y座標.
											RAD( 90 ) - Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang,								//角度.
											hPlgDC );																					//作業DC.
										break;
									case 2:	//貫通.
										//キャラクタの画像をメモリDCへコピー.
										SelectObject( hMemDC, hSlime );
										TransPlgBlt(
											gameWnd->hScreenDC,																	//デバイスコンテキスト.
											static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - 13 ),	//表示位置x.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2 ),	//表示位置y.
											S_SIZE, S_SIZE * 3,																	//画像の幅,高さ.
											hMemDC,																				//メモリDC.
											C_SIZE * 13, 192,
											RAD( 90 ) - Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang,
											hPlgDC );
										break;
									case 3:	//ビーム.
										//キャラクタの画像をメモリDCへコピー.
										SelectObject( hMemDC, hShot );
										TransPlgBlt(
											gameWnd->hScreenDC,																																							//デバイスコンテキスト.
											static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE - ( 8 - 7 * ( BEAMMAX - Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt ) ) ),	//表示位置x.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE ),																								//表示位置y.
											S_SIZE, S_SIZE * Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt,																										//画像の幅,高さ.
											hMemDC,																																										//メモリDC.
											S_SIZE, 0,
											RAD( 90 ) - Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang,
											hPlgDC );
										break;
									case 4: //追尾.
										//キャラクタの画像をメモリDCへコピー.
										SelectObject( hMemDC, hSlime );
										TransPlgBlt(
											gameWnd->hScreenDC,																	//デバイスコンテキスト.
											static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - 5 ),	//表示位置x.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),				//表示位置y.
											S_SIZE + 5, S_SIZE + S_SIZE / 2,													//画像の幅,高さ.
											hMemDC,																				//メモリDC.
											C_SIZE * 13 + S_SIZE, 192,
											RAD( 90 ) - Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang,
											hPlgDC );
										break;
									}
								}
							}
						}
					}
				}
			}

			//ボスの弾.
			for ( int numNo = 0; numNo < Boss.BS_NUM; numNo++ ) {
				for ( int dirNo = 0; dirNo < Boss.BS_DIR; dirNo++ ) {
					if ( Boss.shot[numNo][dirNo].DispShotFlag == true ) {
						//ボスの弾の画像をメモリDCへコピー.
						SelectObject( hMemDC, hSlime );
						//ボスの弾の表示.
						TransPlgBlt(
							gameWnd->hScreenDC,								//デバイスコンテキスト.
							( int ) ( C_CORR + Boss.shot[numNo][dirNo].x - 5 ),	//表示位置x.
							( int ) Boss.shot[numNo][dirNo].y,				//表示位置y.
							S_SIZE + 5, S_SIZE + 5,							//画像の幅,高さ.
							hMemDC,											//メモリDC.
							C_SIZE * 13 + S_SIZE, 192 + S_SIZE + S_SIZE / 2,//元画像のx,y座標.
							RAD( 90 ) - Boss.shot[numNo][dirNo].ang,
							hPlgDC );
					}
				}
			}

			//プレイヤーのバリアの表示.
			if ( Player.ability.barrUP != 0 ) {
				for ( int bNo = 0; bNo < Player.ability.barrUP * BARRMAX; bNo++ ) {
					//プレイヤーのバリアの画像をメモリDCへコピー.
					SelectObject( hMemDC, hboss );
					//プレイヤーのバリアの表示.
					TransBlt(
						gameWnd->hScreenDC,										//デバイスコンテキスト.
						static_cast< int >( C_CORR + Player.barrier[bNo].x ),	//表示位置x.
						static_cast< int >( Player.barrier[bNo].y ),			//表示位置y.
						S_SIZE, S_SIZE,											//画像の幅,高さ.
						hMemDC,													//メモリDC.
						S_SIZE * 4,												//元画像のx座標
						S_SIZE * 11 );											//元画像のy座標.
				}
			}

			//敵のバリアの表示.
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( Enemy[typeNo][ptnNo][eNo].ability.barrUP != 0 ) {
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) {
								for ( int bNo = 0; bNo < Enemy[typeNo][ptnNo][eNo].ability.barrUP * BARRMAX; bNo++ ) {
									//敵のバリアの画像をメモリDCへコピー.
									SelectObject( hMemDC, hTree );
									//敵のバリアの表示.
									TransBlt(
										gameWnd->hScreenDC,														//デバイスコンテキスト.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].barrier[bNo].x ),//表示位置x.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].barrier[bNo].y ),			//表示位置y.
										S_SIZE, S_SIZE,															//画像の幅,高さ.
										hMemDC,																	//メモリDC.
										C_SIZE * 3 + S_SIZE,													//元画像のx座標
										C_SIZE * 2 + S_SIZE );													//元画像のy座標.
								}
							}
						}
					}
				}
			}

			//ﾀｲﾄﾙ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hPSlime );
			//空を舞う葉っぱ描画.
			for ( int LNo = 0; LNo < W_HITMAX; LNo++ ) {
				if ( WoodLeaf[LNo].DispFlag == true ) {
					TransPlgBlt(
						gameWnd->hScreenDC,									//デバイスコンテキスト.
						C_CORR + WoodLeaf[LNo].x,							//表示位置x座標.
						WoodLeaf[LNo].y,									//表示位置y座標.
						16, 16,												//画像の幅,高さ.
						hMemDC,												//メモリDC.
						C_SIZE * 3 + 16 * WoodLeaf[LNo].PtnX,				//元画像のx座標.
						C_SIZE * 2 + 16 * WoodLeaf[LNo].PtnY, 				//元画像のy座標.
						WoodLeaf[LNo].Ang,									//回転角度.
						hPlgDC );											//回転用DC.
				}
			}

			//ステージギミック(木)の葉っぱ.
			for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					if ( gimmick[0][ptnNo][gNo].State != enGimmickState::GWait ) {
						if ( gimmick[0][ptnNo][gNo].State == enGimmickState::GLiving ) {	//木.
							//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
							SelectObject( hMemDC, hTree );
							//ステージギミックの表示.
							TransAlBlend(
								gameWnd->hScreenDC,														//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
								static_cast< int >( C_CORR + gimmick[0][ptnNo][gNo].x - C_SIZE / 2 ),	//表示位置x.
								static_cast< int >( gimmick[0][ptnNo][gNo].y - C_SIZE - C_SIZE / 2 + 5),//表示位置y.
								C_SIZE * 2, C_SIZE * 2,													//画像の幅,高さ.
								hMemDC,																	//ﾒﾓﾘDC.
								C_SIZE, ( C_SIZE * 2 ) * ptnNo,											//元画像のx,y座標.
								gimmick[0][ptnNo][gNo].TreeAlpha,
								hWorkDC );
						}
						if ( gimmick[0][ptnNo][gNo].State == enGimmickState::GInvi ) {	//木.
							//ステージギミックの画像をﾒﾓﾘDCへｺﾋﾟｰ.
							SelectObject( hMemDC, hTreeHit );
							//ステージギミックの表示.
							TransBlt(
								gameWnd->hScreenDC,															//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
								static_cast< int >( C_CORR + gimmick[0][ptnNo][gNo].x - C_SIZE / 2 ),		//表示位置x.
								static_cast< int >( gimmick[0][ptnNo][gNo].y - C_SIZE - C_SIZE / 2 + 5 ),	//表示位置y.
								C_SIZE * 2, C_SIZE * 2,														//画像の幅,高さ.
								hMemDC,																		//ﾒﾓﾘDC.
								( C_SIZE * 2 ) * ptnNo, BLOW_H );												//元画像のx,y座標.
						}
					}
				}
			}

			//敵のHP表示.
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) ||
							 ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) ){
							//HPバー、ギミックの画像をメモリDCへコピー.
							SelectObject( hMemDC, hhp_Gmck );
							//敵のHPバー(背景)の表示.
							ExpaTransBlt(
								gameWnd->hScreenDC,														//デバイスコンテキスト.
								static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x - 10 ),	//表示位置x(補正値：10).
								static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y - 10 ),			//表示位置y(補正値：10).
								P_HP_W, P_HP_H,															//画像の幅,高さ.
								hMemDC,																	//メモリDC.
								0, C_SIZE * 2,															//元画像のx,y座標.
								P_HP_W / 3, P_HP_H / 3,													//縮小後の幅,高さ.
								false );																//表示位置x,yを移動させない.

							//HPバー、ギミックの画像をメモリDCへコピー.
							SelectObject( hMemDC, hhp_Gmck );
							//敵のHPバー(メーター)の表示.
							ExpaTransBlt(
								gameWnd->hScreenDC,														//デバイスコンテキスト.
								static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x - 10 ),	//表示位置x(補正値：10).
								static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y - 10 ),			//表示位置y(補正値：10).
								P_HP_W, P_HP_H,															//画像の幅,高さ.
								hMemDC,																	//メモリDC.
								0, C_SIZE * 2 + P_HP_H,													//元画像のx,y座標.
								( ( P_HP_W * 100 / Enemy[typeNo][ptnNo][eNo].initial_status.hp ) * Enemy[typeNo][ptnNo][eNo].status.hp / 100 ) / 3,	//縮小後の幅.
								P_HP_H / 3,																											//縮小後の高さ.
								false );																											//表示位置x,yを移動させない.
						}
					}
				}
			}

			//敵の能力の表示.
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) ||
							 ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) )
						{
							for ( int i = 0; i < ABLT_MAX; i++ ) {
								if ( Enemy[typeNo][ptnNo][eNo].chara.abArea[i] != 0 ) {
									//能力エリアの画像をメモリDCへコピー.
									SelectObject( hMemDC, hability );
									//能力エリアの表示.
									ExpaTransBlt(
										gameWnd->hScreenDC,																				//デバイスコンテキスト.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x + ( ABLT_SIZE / 3 ) * i - 10 ),	//表示位置x(補正値：10).
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y - ABLT_SIZE / 2 + 3 ),					//表示位置y(補正値： 5).
										ABLT_SIZE, ABLT_SIZE,																			//画像の幅,高さ.
										hMemDC,																							//メモリDC.
										ABLT_SIZE * ( ( Enemy[typeNo][ptnNo][eNo].chara.abArea[i] + 1 ) % 4 ),							//元画像のx座標.
										ABLT_SIZE * ( ( Enemy[typeNo][ptnNo][eNo].chara.abArea[i] + 1 ) / 4 ),							//元画像のy座標.
										ABLT_SIZE / 3, ABLT_SIZE / 3,																	//拡大縮小後の幅,高さ.
										false );																						//表示位置x,yを移動させない.
								}
							}
						}
						else if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dead ) {
							//敵の能力の表示.
							if ( Enemy[typeNo][ptnNo][eNo].EGetAbNo != 0 ) {
								//能力エリアの画像をメモリDCへコピー.
								SelectObject( hMemDC, hability );
								//能力エリアの表示.
								ExpaTransBlt(
									gameWnd->hScreenDC,														//デバイスコンテキスト.
									static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x + 20 ),	//表示位置x(補正値：10).
									static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y - 10 ),			//表示位置y(補正値： 5).
									ABLT_SIZE, ABLT_SIZE,													//画像の幅,高さ.
									hMemDC,																	//メモリDC.
									ABLT_SIZE * ( ( Enemy[typeNo][ptnNo][eNo].EGetAbNo + 1 ) % 4 ),			//元画像のx座標.
									ABLT_SIZE * ( ( Enemy[typeNo][ptnNo][eNo].EGetAbNo + 1 ) / 4 ),			//元画像のy座標.
									ABLT_SIZE / 3, ABLT_SIZE / 3,											//拡大縮小後の幅,高さ.
									false );																//表示位置x,yを移動させない.
							}
						}

					}
				}
			}

			//敵(花)の状態にアニメーション.
			for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
				for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
					if ( ( Enemy[0][ptnNo][eNo].chara.State == enCharaState::Living ) ||
						 ( Enemy[0][ptnNo][eNo].chara.State == enCharaState::Invi ) )
					{
						if ( Plant[ptnNo][eNo].State == enPlantState::Sleeping ){
							for ( int pNo = 0; pNo < 2; pNo++ ){
								if ( Plant[ptnNo][eNo].Sleep[pNo].DispFlag == true ){
									//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
									SelectObject( hMemDC, hSleep );
									//zZの表示.
									TransAlBlend(
										gameWnd->hScreenDC,												//デバイスコンテキスト.
										static_cast< int >( C_CORR + Plant[ptnNo][eNo].Sleep[pNo].x ),	//表示位置x座標.
										static_cast< int >( Plant[ptnNo][eNo].Sleep[pNo].y ),			//表示位置y座標.
										16, 16,															//画像の幅,高さ.
										hMemDC,															//メモリDC.
										16 * ( Plant[ptnNo][eNo].Sleep[pNo].ptnNo % 2 ),				//元画像のx座標.
										16 * ( Plant[ptnNo][eNo].Sleep[pNo].ptnNo / 2 ),				//元画像のy座標.
										Plant[ptnNo][eNo].Sleep[pNo].Alpha,								//アルファ値.
										hWorkDC );														//作業用DC.
								}
							}
						}
						else if ( Plant[ptnNo][eNo].State == enPlantState::WokeUp ){
							//ｷｬﾗｸﾀの画像をﾒﾓﾘDCへｺﾋﾟｰ.
							SelectObject( hMemDC, hSleep );
							//自機の表示.
							ExpaTransAlBlend(
								gameWnd->hScreenDC,																			//デバイスコンテキスト.
								static_cast< int >( C_CORR + Plant[ptnNo][eNo].Woke.x - Plant[ptnNo][eNo].Woke.AnimaX ),	//表示位置x座標( 補正値:13 ).
								static_cast< int >( Plant[ptnNo][eNo].Woke.y + Plant[ptnNo][eNo].Woke.AnimaY ),				//表示位置y座標( 補正値:13 ).
								16, 16,																						//画像の幅,高さ.
								hMemDC,																						//メモリDC.
								16, 16,																						//元画像のx,y座標.
								Plant[ptnNo][eNo].Woke.Alpha,																//アルファ値.
								hWorkDC,																					//作業用DC.
								16 * 2 + Plant[ptnNo][eNo].Woke.AnimaCnt * 2,													//サイズ変更後の幅.
								16 * 2 - Plant[ptnNo][eNo].Woke.AnimaCnt * 2,													//サイズ変更後の高さ.
								false );																					//中央拡大をしない(右下拡大).
						}
					}
				}
			}

			for ( int i = 0; i < Boss.BOSS_TYPE; i++ ) {
				if ( ( Boss.chara[i].State == enCharaState::Living ) || ( Boss.chara[i].State == enCharaState::Invi ) ) {
					//HPバー、ギミックの画像をメモリDCへコピー.
					SelectObject( hMemDC, hhp_Gmck );
					//ボスのHPバー(背景)の表示.
					ExpaTransBlt(
						gameWnd->hScreenDC,										//デバイスコンテキスト.
						static_cast< int >( C_CORR + Boss.chara[i].x ),			//表示位置x(補正値：25).
						static_cast< int >( Boss.chara[i].y - ABLT_SIZE / 4 ),	//表示位置y(補正値：25).
						P_HP_W, P_HP_H,											//画像の幅,高さ.
						hMemDC,													//メモリDC.
						0, C_SIZE * 2,											//元画像のx,y座標
						P_HP_W / ( P_HP_W / Boss.charaSize[i] ),
						P_HP_H / ( P_HP_W / Boss.charaSize[i] ),
						false );
					//HPバー、ギミックの画像をメモリDCへコピー.
					SelectObject( hMemDC, hhp_Gmck );
					//ボスのHPバー(メーター)の表示.
					ExpaTransBlt(
						gameWnd->hScreenDC,											//デバイスコンテキスト.
						static_cast< int >( C_CORR + Boss.chara[i].x ),				//表示位置x(補正値：25).
						static_cast< int >( Boss.chara[i].y - ABLT_SIZE / 4 ),		//表示位置y(補正値：25).
						P_HP_W, P_HP_H,												//画像の幅,高さ.
						hMemDC,														//メモリDC.
						0, C_SIZE * 2 + P_HP_H,										//元画像のx,y座標.
						( P_HP_W / ( P_HP_W / Boss.charaSize[i] ) * 100 / Boss.initial_status[i].hp ) * Boss.status[i].hp / 100,
						P_HP_H / ( P_HP_W / Boss.charaSize[i] ),
						false );
				}
			}

			//背景の画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hMainLR );
			//左背景の表示.
			BitBlt(
				gameWnd->hScreenDC,					//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				0, 0,								//表示位置x,y座標.
				280,								//画像の幅.
				WND_H,								//画像の高さ.
				hMemDC,								//ﾒﾓﾘDC.
				0, 0,								//元画像のx,y座標.
				SRCCOPY );							//ｺﾋﾟｰする.

			//背景の画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hMainLR );
			//右背景の表示.
			BitBlt(
				gameWnd->hScreenDC,					//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				WND_W - 280,						//表示位置x座標.
				0,									//表示位置y座標.
				280,								//画像の幅.
				WND_H,								//画像の高さ.
				hMemDC,								//ﾒﾓﾘDC.
				280, 0,								//元画像のx,y座標.
				SRCCOPY );							//ｺﾋﾟｰする.

			//HPバーの画像をメモリDCへコピー.
			SelectObject( hMemDC, hPHp );
			//プレイヤーのHPバー(背景)の表示.
			TransBlt(
				gameWnd->hScreenDC,		//デバイスコンテキスト.
				-4, 3 + PHP_H,			//表示位置x,y(補正値：10).
				PHP_W, PHP_H,			//画像の幅,高さ.
				hMemDC,					//メモリDC.
				0, PHP_H * 3 + 10 );	//元画像のx,y座標
			//HPバーの画像をメモリDCへコピー.
			SelectObject( hMemDC, hPHp );
			//プレイヤーのHPバー(メーター)の表示.
			TransBlt(
				gameWnd->hScreenDC,														//デバイスコンテキスト.
				-4, 3 + PHP_H,															//表示位置x,y(補正値：10).
				( PHP_W * 100 / Player.initial_status.hp ) * Player.status.hp / 100,	//画像の幅.
				PHP_H,																	//画像の高さ.
				hMemDC,																	//メモリDC.
				0, PHP_H * 2 + 10 );													//元画像のx,y座標.
			//スライム画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hSlime );
			//プレイヤーのHPバー(フレーム)の表示.
			TransBlt(
				gameWnd->hScreenDC,				//デバイスコンテキスト.
				-4, -2,							//表示位置x,y座標.
				PHP_W, PHP_H * 2 + 5,			//画像の幅,高さ.
				hMemDC,							//メモリDC.
				PHP_W * Player.HPFrameNo, 96 );	//元画像のx,y座標.

			//文字の画像をメモリDCへコピー.
			SelectObject( hMemDC, hLetter );
			//プレイヤーのステータス表示.
			for ( int i = 0; i < DISP_STA; i++ ) {
				if ( i == 0 ) {
					//"DAMAGE"の表示.
					TransBlt(
						gameWnd->hScreenDC,							//デバイスコンテキスト.
						4, 135 + ( ENG_H * 2 + ENG_H / 2 ) * i,	//表示位置x,y.
						ENG_W[i], ENG_H,							//画像の幅,高さ.
						hMemDC,										//メモリDC.
						0, NUM_H );									//元画像のx,y座標.
				}
				else {
					//ステータスの表示.
					TransBlt(
						gameWnd->hScreenDC,							//デバイスコンテキスト.
						6, 135 + ( ENG_H * 2 + ENG_H / 2 ) * i,		//表示位置x,y.
						ENG_W[i], ENG_H,							//画像の幅,高さ.
						hMemDC,										//メモリDC.
						0, NUM_H + ENG_H * ( 1 + i ) );				//元画像のx,y座標.
				}
				//":"の表示.
				TransBlt(
					gameWnd->hScreenDC,								//デバイスコンテキスト.
					211, 145 + ( ENG_H * 2 + ENG_H / 2 ) * i,		//表示位置x,y.
					16, 24,											//画像の幅,高さ.
					hMemDC,											//メモリDC.
					NUM_W * 10, NUM_H + ENG_H );					//元画像のx,y座標.

				//位ごとに一時保管( [0]:1の位, [1]:10の位 ).
				int Dig[2] = {};

				//位分け.
				switch ( i )
				{
				case enENG_No::Damage:
					Dig[0] = Player.status.atk / 10;
					Dig[1] = Player.status.atk % 10;
					break;
				case enENG_No::CharaSpd:
					Dig[0] = static_cast< int >( Player.status.cspd ) / 10;
					Dig[1] = static_cast< int >( Player.status.cspd ) % 10;
					break;
				case enENG_No::ShotSpd:
					Dig[0] = Player.shot[S_NUMMAX - 1][S_DIRMAX - 1].sspd / 10;
					Dig[1] = Player.shot[S_NUMMAX - 1][S_DIRMAX - 1].sspd % 10;
					break;
				case enENG_No::ShotDir:
					Dig[0] = Player.status.dir / 10;
					Dig[1] = Player.status.dir % 10;
					break;
				case enENG_No::ShotNum:
					Dig[0] = Player.status.num / 10;
					Dig[1] = Player.status.num % 10;
					break;
				default:
					Dig[0] = 10;	//何も表示しない.
					Dig[1] = 10;	//何も表示しない.
					break;
				}

				//数字の表示.
				for ( int j = 0; j < 2; j++ ) {
					TransBlt(
						gameWnd->hScreenDC,							//デバイスコンテキスト.
						224 + NUM_W * j,							//表示位置x.
						141 + ( ENG_H * 2 + ENG_H / 2 ) * i,		//表示位置y.
						NUM_W, NUM_H,								//画像の幅,高さ.
						hMemDC,										//メモリDC.
						NUM_W * Dig[j], 0 );						//元画像のx,y座標.
				}
			}

			//コントローラーの選択エリア表示.
			if ( pXInput->IsConnect() == true ) {
				ColorBlt(
					enBltNo::Bit,
					gameWnd->hScreenDC,
					3 + ( ABLT_SIZE + 5 ) * Player.XInArea,
					WND_H - ABLT_SIZE - 8,
					ABLT_SIZE + 4, ABLT_SIZE + 4,
					hColorDC,
					RGB( 0xFF, 0x30, 0x00 ) );
			}

			for ( int i = 0; i < ABLT_MAX; i++ ){
				if ( Player.chara.abArea[i] == 0 ){
					if ( i == 0 ){
						//能力エリアの画像をメモリDCへコピー.
						SelectObject( hMemDC, hability );
						//能力エリアの表示.
						TransBlt(
							gameWnd->hScreenDC,				//デバイスコンテキスト.
							5 + ( ABLT_SIZE + 5 ) * i,		//表示位置x(補正値：10).
							WND_H - ABLT_SIZE - 6,			//表示位置y(補正値： 5).
							ABLT_SIZE, ABLT_SIZE,			//画像の幅,高さ.
							hMemDC,							//メモリDC.
							0, 0 ); 						//元画像のx,y座標.
					}
					else {
						//能力エリアの画像をメモリDCへコピー.
						SelectObject( hMemDC, hability );
						//能力エリアの表示.
						TransBlt(
							gameWnd->hScreenDC,				//デバイスコンテキスト.
							5 + ( ABLT_SIZE + 5 ) * i,		//表示位置x(補正値：10).
							WND_H - ABLT_SIZE - 6,			//表示位置y(補正値： 5).
							ABLT_SIZE, ABLT_SIZE,			//画像の幅,高さ.
							hMemDC,							//メモリDC.
							C_SIZE, 0 ); 					//元画像のx,y座標.
					}
				}
				else{
					//能力エリアの画像をメモリDCへコピー.
					SelectObject( hMemDC, hability );
					//能力エリアの表示.
					TransBlt(
						gameWnd->hScreenDC,										//デバイスコンテキスト.
						5 + ( ABLT_SIZE + 5 ) * i,								//表示位置x(補正値：10).
						WND_H - ABLT_SIZE - 6,									//表示位置y(補正値： 5).
						ABLT_SIZE, ABLT_SIZE,									//画像の幅,高さ.
						hMemDC,													//メモリDC.
						ABLT_SIZE * ( ( Player.chara.abArea[i] + 1 ) % 4 ),		//元画像のx座標.
						ABLT_SIZE * ( ( Player.chara.abArea[i] + 1 ) / 4 ) );	//元画像のy座標.
				}
			}

			//削除能力アニメーション.
			for ( int i = 0; i < ABLT_MAX; i++ ) {
				if ( Player.DGAbi[i].DispFlag == true ) {
					//能力エリアの画像をメモリDCへコピー.
					SelectObject( hMemDC, hability );
					//能力エリアの表示.
					ExpaTransAlBlend(
						gameWnd->hScreenDC,										//デバイスコンテキスト.
						static_cast< int >( Player.DGAbi[i].xy.x ),				//表示位置x(補正値：10).
						static_cast< int >( Player.DGAbi[i].xy.y ),				//表示位置y(補正値： 5).
						ABLT_SIZE, ABLT_SIZE,									//画像の幅,高さ.
						hMemDC,													//メモリDC.
						ABLT_SIZE * ( ( Player.DGAbi[i].AbiNo + 1 ) % 4 ),		//元画像のx座標.
						ABLT_SIZE * ( ( Player.DGAbi[i].AbiNo + 1 ) / 4 ),		//元画像のy座標.
						Player.DGAbi[i].Alpha,
						hWorkDC,
						static_cast< int >( ABLT_SIZE * ( 1 + Player.DGAbi[i].Expa ) ),
						static_cast< int >( ABLT_SIZE * ( 1 + Player.DGAbi[i].Expa ) ),
						false );
				}
			}

			//コントローラーの選択エリア表示.
			if ( pXInput->IsConnect() == true ) {
				ColorBlt(
					enBltNo::Bit,
					gameWnd->hScreenDC,
					WND_W - ( 12 + ABLT_W ) - 3,
					113 + ( ABLT_H + 24 ) * Player.XInArea,
					ABLT_W + 6, ABLT_H + 6,
					hColorDC,
					RGB( 0xFF, 0x30, 0x00 ) );
			}

			//能力説明.
			for ( int i = 0; i < ABLT_MAX; i++ ) {
				//能力説明の画像をメモリDCへコピー.
				SelectObject( hMemDC, harea );
				//能力エリア背景の表示.
				TransBlt(
					gameWnd->hScreenDC,					//デバイスコンテキスト.
					WND_W - ( 12 + ABLT_W ),			//表示位置x(補正値：15).
					116 + ( ABLT_H + 24 ) * i,			//表示位置y(補正値：10).
					ABLT_W, ABLT_H,						//画像の幅,高さ.
					hMemDC,								//メモリDC.
					0, ABLT_H * i ); 					//元画像のx,y座標.
				//アイコン.
				if ( Player.chara.abArea[i] == 0 ){
					if ( i == 0 ){	//赤空.
						//能力エリアの画像をメモリDCへコピー.
						SelectObject( hMemDC, hability );
						//能力エリアの表示.
						TransBlt(
							gameWnd->hScreenDC,				//デバイスコンテキスト.
							WND_W - ( 12 + ABLT_W ),		//表示位置x(補正値：15).
							116 + ( ABLT_H + 24 ) * i,		//表示位置y(補正値：10).
							ABLT_SIZE, ABLT_SIZE,			//画像の幅,高さ.
							hMemDC,							//メモリDC.
							0, 0 ); 						//元画像のx座標.
					}
					else {	//青空.
						//能力エリアの画像をメモリDCへコピー.
						SelectObject( hMemDC, hability );
						//能力エリアの表示.
						TransBlt(
							gameWnd->hScreenDC,				//デバイスコンテキスト.
							WND_W - ( 12 + ABLT_W ),		//表示位置x(補正値：15).
							116 + ( ABLT_H + 24 ) * i,		//表示位置y(補正値：10).
							ABLT_SIZE, ABLT_SIZE,			//画像の幅,高さ.
							hMemDC,							//メモリDC.
							C_SIZE, 0 ); 					//元画像のx座標.
					}
				}
				else{	//アイコン.
					//能力エリアの画像をメモリDCへコピー.
					SelectObject( hMemDC, hability );
					//能力エリアの表示.
					TransBlt(
						gameWnd->hScreenDC,										//デバイスコンテキスト.
						WND_W - ( 12 + ABLT_W ),								//表示位置x(補正値：15).
						116 + ( ABLT_H + 24 ) * i,								//表示位置y(補正値：10).
						ABLT_SIZE, ABLT_SIZE,									//画像の幅,高さ.
						hMemDC,													//メモリDC.
						ABLT_SIZE * ( ( Player.chara.abArea[i] + 1 ) % 4 ),		//元画像のx座標.
						ABLT_SIZE * ( ( Player.chara.abArea[i] + 1 ) / 4 ) );	//元画像のy座標.
				}
				//説明の画像をメモリDCへコピー.
				SelectObject( hMemDC, hLetter );
				switch ( Player.chara.abArea[i] )
				{
				case enAbility::beam:
					//"BEAM"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::Beam], ENG_H,			//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 7,								//元画像のx座標.
						NUM_H + ENG_H * 7 );					//元画像のy座標.
					break;
				case enAbility::pene:
					//"PENE"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::Pane], ENG_H,			//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 7,								//元画像のx座標.
						NUM_H + ENG_H * 6 );					//元画像のy座標.
					break;
				case enAbility::chg:
					//"CHARGE"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::Charge], ENG_H,			//画像の幅,高さ.
						hMemDC,									//メモリDC.
						0, NUM_H + ENG_H * 6 );					//元画像のx,y座標.
					break;
				case enAbility::homing:
					//"HOMING"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::Homing], ENG_H,			//画像の幅,高さ.
						hMemDC,									//メモリDC.
						0, NUM_H + ENG_H * 7 );					//元画像のx,y座標.
					break;
				case enAbility::dirUP:
					//"SHOTDIR"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::ShotDir], ENG_H,		//画像の幅,高さ.
						hMemDC,									//メモリDC.
						0, NUM_H + ENG_H * 4 );					//元画像のx,y座標.
					//"+"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 170 ,					//表示位置x(補正値：15).
						199 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						24, 24,									//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 9,								//元画像のx座標.
						NUM_H + ENG_H * 1 );					//元画像のy座標.
					//"2"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 193,					//表示位置x(補正値：15).
						195 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						NUM_W, NUM_H,							//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 2, 0 );							//元画像のx,y座標.
					break;
				case enAbility::numUP:
					//"SHOTNUM"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::ShotNum], ENG_H,		//画像の幅,高さ.
						hMemDC,									//メモリDC.
						0, NUM_H + ENG_H * 5 );					//元画像のx,y座標.
					//"+"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 188,					//表示位置x(補正値：15).
						199 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						24, 24,									//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 9,								//元画像のx座標.
						NUM_H + ENG_H * 1 );					//元画像のy座標.
					//"1"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 210,					//表示位置x(補正値：15).
						195 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						NUM_W, NUM_H,							//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 1, 0 );							//元画像のx,y座標.
					break;
				case enAbility::hpUP:
					//"HEEL"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::Heel], ENG_H,			//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 7,								//元画像のx座標.
						NUM_H + ENG_H * 8 );					//元画像のy座標.
					//"+"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 95,					//表示位置x(補正値：15).
						199 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						24, 24,									//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 9,								//元画像のx座標.
						NUM_H + ENG_H * 1 );					//元画像のy座標.
					//"1"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 120,					//表示位置x(補正値：15).
						195 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						NUM_W, NUM_H,							//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 1, 0 );							//元画像のx,y座標.
					break;
				case enAbility::barrUP:
					//"BARRIER"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::Barrier], ENG_H,		//画像の幅,高さ.
						hMemDC,									//メモリDC.
						0, NUM_H + ENG_H * 8 );					//元画像のx,y座標.
					//"+"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 160,					//表示位置x(補正値：15).
						199 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						24, 24,									//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 9,								//元画像のx座標.
						NUM_H + ENG_H * 1 );					//元画像のy座標.
					//"1"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 185,					//表示位置x(補正値：15).
						195 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						NUM_W, NUM_H,							//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 1, 0 );							//元画像のx,y座標.
					break;
				case enAbility::atkUP:
					//"DAMAGE"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::Damage], ENG_H,			//画像の幅,高さ.
						hMemDC,									//メモリDC.
						0, NUM_H );								//元画像のx,y座標.
					//"+"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 160,					//表示位置x(補正値：15).
						199 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						24, 24,									//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 9,								//元画像のx座標.
						NUM_H + ENG_H * 1 );					//元画像のy座標.
					//"2"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 185,					//表示位置x(補正値：15).
						195 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						NUM_W, NUM_H,							//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 2, 0 );							//元画像のx,y座標.
					break;
				case enAbility::cspdUP:
					//"CHARASPD"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 10,					//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::CharaSpd], ENG_H,		//画像の幅,高さ.
						hMemDC,									//メモリDC.
						0, NUM_H + ENG_H * 2 );					//元画像のx,y座標.
					//"+"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 199,					//表示位置x(補正値：15).
						199 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						24, 24,									//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 9,								//元画像のx座標.
						NUM_H + ENG_H * 1 );					//元画像のy座標.
					//"1"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 217,					//表示位置x(補正値：15).
						195 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						NUM_W, NUM_H,							//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 1, 0 );							//元画像のx,y座標.
					break;
				case enAbility::sspdUP:
					//"SHOTSPD"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W - 5,						//表示位置x(補正値：15).
						191 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						ENG_W[enENG_No::ShotSpd], ENG_H,		//画像の幅,高さ.
						hMemDC,									//メモリDC.
						0, NUM_H + ENG_H * 3 );					//元画像のx,y座標.
					//"+"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 185,					//表示位置x(補正値：15).
						199 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						24, 24,									//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 9,								//元画像のx座標.
						NUM_H + ENG_H * 1 );					//元画像のy座標.
					//"1"の表示.
					TransBlt(
						gameWnd->hScreenDC,						//デバイスコンテキスト.
						WND_W - ABLT_W + 210,					//表示位置x(補正値：15).
						195 + ( ABLT_H + 24 ) * i,				//表示位置y(補正値：10).
						NUM_W, NUM_H,							//画像の幅,高さ.
						hMemDC,									//メモリDC.
						NUM_W * 1, 0 );							//元画像のx,y座標.
					break;
				default:
					break;
				}
			}

#ifdef _DEBUG	/** ▼ ********************** 当たり判定エリア描画 ************************** ▼ **/
			const int	Alpha = 100;		//当たり判定のアルファ値.
			const bool	AreaDisp = false;	//当たり判定表示.

			if ( AreaDisp == true ) {
				//単色用DCに赤色のブラシをセット.
				SelectObject( hColorDC, hRBrush );
				//赤色の画像を用意.
				PatBlt(
					hColorDC,		//単色用DCへ出力.
					0, 0,			//表示位置x,y.
					WND_W, WND_H,	//画像の幅,高さ.
					PATCOPY );

				//画面上当たり判定.
				AlBlend(
					gameWnd->hScreenDC,								//デバイスコンテキスト.
					280, GWND_H / 4 + S_SIZE - 2,					//表示位置x,y.
					GWND_W, 10,										//画像の幅,高さ.
					hColorDC,										//メモリDC.
					0, 0,
					Alpha );

				
				for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ){
					//ボスの表示.
					AlBlend(
						gameWnd->hScreenDC,	//デバイスコンテキスト.
						static_cast< int >( C_CORR + Boss.chara[typeNo].x ),		//表示位置x.
						static_cast< int >( Boss.chara[typeNo].y ),					//表示位置y.
						Boss.charaSize[typeNo], Boss.charaSize[typeNo],				//画像の幅,高さ.
						hColorDC,													//メモリDC.
						0, 0,
						Alpha );

					//ボスの突き上げ岩の表示.
					for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
						for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
							if ( Boss.rock[typeNo][numNo][dirNo].DispFlag == true ) {
								//ボスの突き上げ岩の表示.
								AlBlend(
									gameWnd->hScreenDC,	//デバイスコンテキスト.
									static_cast< int >( C_CORR + Boss.rock[typeNo][numNo][dirNo].x ),					//表示位置x.
									static_cast< int >( Boss.rock[typeNo][numNo][dirNo].y ),							//表示位置y.
									S_SIZE * 2, ( S_SIZE / 2 ) * ( Boss.rock[typeNo][numNo][dirNo].cnt / 10 + 1 ),		//画像の幅,高さ.
									hColorDC,																			//メモリDC.
									0, 0,
									Alpha );
							}
						}
					}
				}
				//ボスの弾の表示.
				for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
					for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
						if ( Boss.shot[numNo][dirNo].DispShotFlag == true ) {
							//ボスの突き上げ岩の表示.
							AlBlend(
								gameWnd->hScreenDC,	//デバイスコンテキスト.
								static_cast< int >( C_CORR + Boss.shot[numNo][dirNo].x ),							//表示位置x.
								static_cast< int >( Boss.shot[numNo][dirNo].y ),									//表示位置y.
								S_SIZE, S_SIZE,																		//画像の幅,高さ.
								hColorDC,																			//メモリDC.
								0, 0,
								Alpha );
						}
					}
				}

				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GWait ) {
								if ( typeNo == 0 ) {	//木.
									if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GLiving ) {		//木.
										//ステージギミックの表示.
										AlBlend(
											gameWnd->hScreenDC,														//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
											static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//表示位置x.
											static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//表示位置y.
											gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//画像の幅,高さ.
											hColorDC,																//ﾒﾓﾘDC.
											0, 0,																	//元画像のx,y座標.
											Alpha );
									}
									else if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GDead ) {	//壊れれいる木.
										//ステージギミックの表示.
										AlBlend(
											gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
											static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),					//表示位置x.
											static_cast< int >( C_SIZE + gimmick[typeNo][ptnNo][gNo].y - C_SIZE / 2 - 3 ),	//表示位置y.
											gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//画像の幅,高さ.
											hColorDC,																		//ﾒﾓﾘDC.
											0, 0,
											Alpha );;
									}
								}
								else if ( typeNo == 1 ) {	//岩.
									//ステージギミックの表示.
									AlBlend(
										gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + gimmick[typeNo][ptnNo][gNo].x ),	//表示位置x.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),							//表示位置y.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//画像の幅,高さ.
										hColorDC,																		//ﾒﾓﾘDC.
										0, 0,
										Alpha );
								}
								else if ( typeNo == 2 ) {	//茨.
									//ステージギミックの表示.
									AlBlend(
										gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + gimmick[typeNo][ptnNo][gNo].x ),	//表示位置x.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),							//表示位置y.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//画像の幅,高さ.
										hColorDC,																		//ﾒﾓﾘDC.
										0, 0,																			//元画像のx,y座標.
										Alpha );
								}
								else if ( typeNo == 3 ) {	//倒木.
									//ステージギミックの表示.
									AlBlend(
										gameWnd->hScreenDC,																//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + gimmick[typeNo][ptnNo][gNo].x ),	//表示位置x.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),							//表示位置y.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//画像の幅,高さ.
										hColorDC,																		//ﾒﾓﾘDC.
										0, 0,																			//元画像のx,y座標.
										Alpha );
								}
							}
						}
					}
				}

				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							if ( ( Enemy[typeNo][ptnNo][eNo].appFlag == true ) || ( Enemy[typeNo][ptnNo][eNo].BossFlag == true ) ) {
								//敵機が生存中.
								if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living )
								{
									//敵機の表示.
									AlBlend(
										gameWnd->hScreenDC,																	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].chara.x ),	//表示位置x.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),							//表示位置y.
										C_SIZE, C_SIZE,																		//画像の幅,高さ.
										hColorDC,																			//ﾒﾓﾘDC.
										0, 0,
										Alpha );
								}
								else if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dead )
								{
									//敵機の表示.
									AlBlend(
										gameWnd->hScreenDC,																	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].chara.x ),	//表示位置x.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),							//表示位置y.
										C_SIZE, C_SIZE,																		//画像の幅,高さ.
										hColorDC,																			//ﾒﾓﾘDC.
										0, 0,																				//元画像のx,y.
										Alpha );																			//アルファ値.								
								}
							}
						}
					}
				}

				for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
					for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
						if ( Player.shot[numNo][dirNo].DispShotFlag == true ) {
							if ( Player.ability.beam == 1 ) {
								//プレイヤーのビーム弾の表示.
								AlBlend(
									gameWnd->hScreenDC,															//デバイスコンテキスト.
									static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.shot[numNo][dirNo].x ),	//表示位置x.
									static_cast< int >( Player.shot[numNo][dirNo].y ),							//表示位置y.
									S_SIZE, S_SIZE,																//画像の幅,高さ.
									hColorDC,																	//メモリDC.
									0, 0,																		//元画像のx,y座標.
									Alpha );
							}
							else if ( Player.ability.chg == 1 ) {
								//プレイヤーの溜め弾の表示.
								AlBlend(
									gameWnd->hScreenDC,	//デバイスコンテキスト.
									static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.shot[numNo][dirNo].x - S_SIZE ),//表示位置x.
									static_cast< int >( Player.shot[numNo][dirNo].y - S_SIZE ),							//表示位置y.
									S_SIZE * 2, S_SIZE * 2,																//画像の幅,高さ.
									hColorDC,																			//メモリDC.
									0, 0,																				//元画像のy座標
									Alpha );
							}
							else {
								//プレイヤーのノーマル弾の表示.
								AlBlend(
									gameWnd->hScreenDC,	//デバイスコンテキスト.
									static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.shot[numNo][dirNo].x ),	//表示位置x.
									static_cast< int >( Player.shot[numNo][dirNo].y ),							//表示位置y.
									S_SIZE, S_SIZE,																//画像の幅,高さ.
									hColorDC,																	//メモリDC.
									0, 0,																		//元画像のx,y.
									Alpha );
							}
						}
					}
				}

				//敵の弾.
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
								for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
									if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) {
										switch ( typeNo )
										{
										case 0:	//溜め.
											AlBlend(
												gameWnd->hScreenDC,																						//デバイスコンテキスト.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE ),	//表示位置x.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE ),							//表示位置y.
												S_SIZE * 2, S_SIZE * 2,																					//画像の幅,高さ.
												hColorDC,																								//メモリDC.
												0, 0,																									//元画像のx,y.
												Alpha );
											break;
										case 1:	//普通.
											AlBlend(
												gameWnd->hScreenDC,																							//デバイスコンテキスト.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE / 2 ),	//表示位置x.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2 ),							//表示位置y.
												S_SIZE * 2, S_SIZE * 2,																						//画像の幅,高さ.
												hColorDC,																									//メモリDC.
												0, 0,																										//元画像のx,y.
												Alpha );
											break;
										case 2:	//貫通.
											AlBlend(
												gameWnd->hScreenDC,																				//デバイスコンテキスト.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//表示位置x.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),							//表示位置y.
												S_SIZE, S_SIZE,																					//画像の幅,高さ.
												hColorDC,																						//メモリDC.
												0, 0,																							//元画像のx,y.
												Alpha );
											break;
										case 3:	//ビーム.
											AlBlend(
												gameWnd->hScreenDC,																				//デバイスコンテキスト.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//表示位置x.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),							//表示位置y.
												S_SIZE, S_SIZE,																					//画像の幅,高さ.
												hColorDC,																						//メモリDC.
												0, 0,																							//元画像のx,y.
												Alpha );
											break;
										case 4: //追尾.
											AlBlend(
												gameWnd->hScreenDC,																				//デバイスコンテキスト.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//表示位置x.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),							//表示位置y.
												S_SIZE, S_SIZE,																					//画像の幅,高さ.
												hColorDC,																						//メモリDC.
												0, 0,																							//元画像のx,y.
												Alpha );
											break;
										}
									}
								}
							}
						}
					}
				}

				//自機が生存中.
				if ( Player.chara.State == enCharaState::Living )
				{
					//自機の表示.
					AlBlend(
						gameWnd->hScreenDC,														//デバイスコンテキスト.
						static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.chara.x + PH_CORR ),//表示位置x( 補正値:13 ).
						static_cast< int >( Player.chara.y + PH_CORR ),							//表示位置y( 補正値:13 ).
						PH_SIZE, PH_SIZE,														//画像の幅,高さ.
						hColorDC,																//メモリDC.
						0, 0,
						Alpha );
				}

				//敵のバリアの表示.
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							if ( Enemy[typeNo][ptnNo][eNo].ability.barrUP != 0 ) {
								if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) {
									for ( int bNo = 0; bNo < Enemy[typeNo][ptnNo][eNo].ability.barrUP * BARRMAX; bNo++ ) {
										//プレイヤーのノーマル弾の表示.
										AlBlend(
											gameWnd->hScreenDC,	//デバイスコンテキスト.
											static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].barrier[bNo].x ),//表示位置x.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].barrier[bNo].y ),							//表示位置y.
											S_SIZE, S_SIZE,																			//画像の幅,高さ.
											hColorDC,																				//メモリDC.
											0, 0,
											Alpha );
									}
								}
							}
						}
					}
				}
				//プレイヤーのバリアの表示.
				if ( Player.ability.barrUP != 0 ) {
					for ( int bNo = 0; bNo < Player.ability.barrUP * BARRMAX; bNo++ ) {
						//プレイヤーのノーマル弾の表示.
						AlBlend(
							gameWnd->hScreenDC,	//デバイスコンテキスト.
							static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.barrier[bNo].x ),	//表示位置x.
							static_cast< int >( Player.barrier[bNo].y ),							//表示位置y.
							S_SIZE, S_SIZE,															//画像の幅,高さ.
							hColorDC,																//メモリDC.
							0, 0,
							Alpha );
					}
				}
			}
#endif			/** ▲ ********************** 当たり判定エリア描画 ************************** ▲ **/

			//ボス出現アニメーション(画面揺れ).
			if ( ( Boss.DispFlag == true ) || ( Boss.chara[enBoss::Main].State == enCharaState::Dying ) ||
				 ( Boss.AttackNo[enBoss::RHand] == enBossAttackNo::BDownHand ) ) {
				BitBlt(
					gameWnd->hScreenDC,
					( WND_W / 2 ) - ( GWND_W / 2 ),
					static_cast< int >( scroll.BossDispY ),
					GWND_W, GWND_H,					//画像の幅,高さ.
					gameWnd->hScreenDC,				//ﾒﾓﾘDC.
					( WND_W / 2 ) - ( GWND_W / 2 ),	//元画像のx座標.
					0,								//元画像のy座標.
					SRCCOPY );						//ｺﾋﾟｰする.
			}

			break;
		}
		case enScene::GameOver:	//ｹﾞｰﾑｵｰﾊﾞｰ.
		{
			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hGameOver );
			//ｹﾞｰﾑｵｰﾊﾞｰ画像の表示.
			BitBlt(
				gameWnd->hScreenDC,	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				0, 0,				//表示位置x,y座標.
				WND_W, WND_H,		//画像の幅,高さ.
				hMemDC,				//ﾒﾓﾘDC.
				0, 0,				//元画像のx,y座標.
				SRCCOPY );			//ｺﾋﾟｰする.

			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hcloud );
			//"GAME START"画像の表示.
			TransBlt(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				509, 550,						//表示位置x,y座標.
				256, 64,						//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.
				256 + 448, 256 * 2 );			//元画像のx,y座標.

			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hcloud );
			//"PRESS ANY BUTTON"画像の表示.
			TransAlBlend(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				544, 595,						//表示位置x,y座標.
				200, 16,						//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.
				256 + 448, 256 * 2 + 64,		//元画像のx,y座標.
				Title.Alpha,					//アルファ値.
				hWorkDC );

			if ( Player.DeadNo == enDeedNo::Gimmick ) {
				//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hDeadG );
			}
			else {
				//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hDead );
			}
			//死亡キャラ画像の表示.
			TransBlt(
				gameWnd->hScreenDC,			//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				GO_DispX[Player.DeadNo],	//表示位置x座標.
				GO_DispY[Player.DeadNo],	//表示位置y座標.
				GO_wDest[Player.DeadNo],	//画像の幅.
				GO_hDest[Player.DeadNo],	//画像の高さ.
				hMemDC,						//ﾒﾓﾘDC.
				GO_StartX[Player.DeadNo],	//元画像のx座標.
				GO_StartY[Player.DeadNo] );	//元画像のy座標.

			if ( Player.DeadNo != enDeedNo::DeedNone ) {
				if ( Player.DeadNo == enDeedNo::Gimmick ) {
					//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, hDeadG );
				}
				else {
					//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, hDead );
				}

				//死亡キャラ画像の表示.
				TransBlt(
					gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					GO_DispX[7 + Player.DeadNo],	//表示位置x座標.
					GO_DispY[7 + Player.DeadNo],	//表示位置y座標.
					GO_wDest[7 + Player.DeadNo],	//画像の幅.
					GO_hDest[7 + Player.DeadNo],	//画像の高さ.
					hMemDC,							//ﾒﾓﾘDC.
					GO_StartX[7 + Player.DeadNo],	//元画像のx座標.
					GO_StartY[7 + Player.DeadNo] );	//元画像のy座標.
			}

			if ( Player.DeadNo == enDeedNo::Bat ) {
				SelectObject( hMemDC, hDead );
				//混乱画像の表示.
				TransBlt(
					gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					620 + 4 * BatAnima.No, 447,		//表示位置z,y座標.
					40, 32,							//画像の幅,高さ.
					hMemDC,							//ﾒﾓﾘDC.
					128 + 40 * BatAnima.No, 112 );	//元画像のx,y座標.
			}
			else if ( Player.DeadNo == enDeedNo::Gimmick ) {
				SelectObject( hMemDC, hDeadG );
				//混乱画像の表示.
				TransBlt(
					gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					623 + 6 * BatAnima.No,			//表示位置x座標.
					437 + 3 * BatAnima.No,			//表示位置y座標.
					32, 32,							//画像の幅,高さ.
					hMemDC,							//ﾒﾓﾘDC.
					32 * BatAnima.No, 160 );		//元画像のx,y座標.
			}
			break;
		}
		case enScene::Ending:	//ｴﾝﾃﾞｨﾝｸﾞ.
		{
			//ｴﾝﾃﾞｨﾝｸﾞ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hEnding );
			//ｴﾝﾃﾞｨﾝｸﾞ画像の表示.
			BitBlt(
				gameWnd->hScreenDC,				//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				0, 0,							//表示位置x,y座標.
				WND_W, WND_H,					//画像の幅,高さ.
				hMemDC,							//ﾒﾓﾘDC.
				0, 0,							//元画像のx,y座標.
				SRCCOPY );						//ｺﾋﾟｰする.

			for ( int sNo = 0; sNo < STARMAX; sNo++ ){
				if ( Ending.Star[sNo].DispFlag == true ){
					//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, hEnSlime );
					//星の表示.
					TransAlBlend(
						gameWnd->hScreenDC,								//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
						Ending.Star[sNo].x,								//表示位置x座標.
						Ending.Star[sNo].y,								//表示位置y座標.
						16, 16,											//画像の幅,高さ.
						hMemDC,											//ﾒﾓﾘDC.
						47 + 16 * ( Ending.Star[sNo].Ptn / 3 ),			//元画像のx座標.
						C_SIZE * 4 + 16 * ( Ending.Star[sNo].Ptn % 3 ),	//元画像のy座標.
						Ending.Star[sNo].Alpha,							//アルファ値.
						hWorkDC );
				}
			}

			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hEnClear );
			//地面画像の表示.
			TransBlt(
				gameWnd->hScreenDC,							//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				0, 464,										//表示位置x,y座標.
				WND_W, 256,									//画像の幅,高さ.
				hMemDC,										//ﾒﾓﾘDC.
				0, 0 );										//元画像のy座標.

			for ( int dNo = 0; dNo < DROPMAX; dNo++ ){
				if ( Ending.Drop[dNo].DispFlag == true ){
					//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
					SelectObject( hMemDC, hEnSlime );
					//雫の表示.
					TransBlt(
						gameWnd->hScreenDC,								//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
						Ending.Drop[dNo].x,								//表示位置x座標.
						Ending.Drop[dNo].y,								//表示位置y座標.
						Ending.wDest[Ending.Drop[dNo].Ptn],				//画像の幅.
						Ending.hDest[Ending.Drop[dNo].Ptn],				//画像の高さ.
						hMemDC,											//ﾒﾓﾘDC.
						Ending.StartX[Ending.Drop[dNo].Ptn],			//元画像のx座標.
						Ending.StartY[Ending.Drop[dNo].Ptn] );			//元画像のy座標.
				}
			}

			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hEnSlime );
			//スライム画像の表示.
			TransAlBlend(
				gameWnd->hScreenDC,							//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				Ending.Slime.x,								//表示位置x座標.
				Ending.Slime.y,								//表示位置y座標.
				C_SIZE * 2, C_SIZE * 2,						//画像の幅,高さ.
				hMemDC,										//ﾒﾓﾘDC.
				C_SIZE * 2 * Ending.Slime.Action,			//元画像のx座標.
				C_SIZE * 2 * Ending.Slime.animation.No,		//元画像のy座標.
				200,										//アルファ値.
				hWorkDC );									//作業用DC.


			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hEnGround );
			//地面画像の表示.
			TransBlt(
				gameWnd->hScreenDC,							//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				0, WND_H - 64,								//表示位置x,y座標.
				WND_W, 64,									//画像の幅,高さ.
				hMemDC,										//ﾒﾓﾘDC.
				0, 0 );										//元画像のy座標.

			//位ごとに一時保管( [0]:1の位, [1]:10の位, [3]:100の位 ).
			int Dig[3] = {};

			//Hit数の桁分け.
			Dig[2] = Ending.Hit / 100;
			Dig[1] = ( Ending.Hit % 100 ) / 10;
			Dig[0] = Ending.Hit % 10;

			//Hit数の表示.
			for ( int nNo = 0; nNo < 3; nNo++ ){
				//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hEnHitNum );
				//数字の表示.
				TransBlt(
					gameWnd->hScreenDC,					//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					WND_W - NUM_W * ( nNo + 1 ) - 5,	//表示位置x座標( 5 : 補正値 ).
					WND_H - NUM_H - 5,					//表示位置y座標( 5 : 補正値 ).
					NUM_W, NUM_H,						//画像の幅,高さ.
					hMemDC,								//ﾒﾓﾘDC.
					NUM_W * Dig[nNo], 0 );				//元画像のx,y座標.
			}
			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hEnHitNum );
			//"HIT:"の表示.
			TransBlt(
				gameWnd->hScreenDC,						//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				WND_W - NUM_W * 3 - 75 - 5,				//表示位置x座標( 5 : 補正値 ).
				WND_H - NUM_H - 5,						//表示位置y座標( 5 : 補正値 ).
				75, NUM_H,								//画像の幅,高さ.
				hMemDC,									//ﾒﾓﾘDC.
				0, NUM_H );								//元画像のx,y座標.

			//最高Hit数の桁分け.
			Dig[0] = Ending.BestHit / 100;
			Dig[1] = ( Ending.BestHit % 100 ) / 10;
			Dig[2] = Ending.BestHit % 10;

			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hEnHitNum );
			//"BEST"の表示.
			TransBlt(
				gameWnd->hScreenDC,						//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				5,										//表示位置x座標( 5 : 補正値 ).
				WND_H - NUM_H - 5,						//表示位置y座標( 5 : 補正値 ).
				85, NUM_H,								//画像の幅,高さ.
				hMemDC,									//ﾒﾓﾘDC.
				75, NUM_H );							//元画像のx,y座標.

			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hEnHitNum );
			//"HIT:"の表示.
			TransBlt(
				gameWnd->hScreenDC,						//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				85 + 5,									//表示位置x座標( 5 : 補正値 ).
				WND_H - NUM_H - 5,						//表示位置y座標( 5 : 補正値 ).
				75, NUM_H,								//画像の幅,高さ.
				hMemDC,									//ﾒﾓﾘDC.
				0, NUM_H );								//元画像のx,y座標.

			//最高Hit数の表示.
			for ( int nNo = 0; nNo < 3; nNo++ ){
				//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
				SelectObject( hMemDC, hEnHitNum );
				//数字の表示.
				TransBlt(
					gameWnd->hScreenDC,					//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
					85 + 75 + NUM_W * nNo + 5,			//表示位置x座標( 5 : 補正値 ).
					WND_H - NUM_H - 5,					//表示位置y座標( 5 : 補正値 ).
					NUM_W, NUM_H,						//画像の幅,高さ.
					hMemDC,								//ﾒﾓﾘDC.
					NUM_W * Dig[nNo], 0 );				//元画像のx,y座標.
			}

			//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
			SelectObject( hMemDC, hEnClear );
			//"GAME CLEAR"の表示.
			TransBlt(
				gameWnd->hScreenDC,						//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				320, Ending.ClearY,						//表示位置x,y座標.
				640, 126,								//画像の幅,高さ.
				hMemDC,									//ﾒﾓﾘDC.
				0, 256 );								//元画像のx,y座標.

			//スタッフロールの画像をメモリDCへコピー.
			for ( int eNo = 0; eNo < ENGMAX; eNo++ ) {
				if ( Ending.EnEng[eNo].DispFlag == true ) {
					if ( ( 12 <= eNo ) && ( eNo <= 17 ) ) {
						//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
						SelectObject( hMemDC, hEnEneEng );
					}
					else if ( eNo == 32 ) {
						//ﾀｲﾄﾙ画像をﾒﾓﾘDCへｺﾋﾟｰ.
						SelectObject( hMemDC, hTitle );
					}
					else {
						//ｹﾞｰﾑｵｰﾊﾞｰ画像をﾒﾓﾘDCへｺﾋﾟｰ.
						SelectObject( hMemDC, hEnEng );
					}

					if ( ( eNo == 32 ) || ( eNo == 34 ) ) {	//ロゴ.
						//スタッフロールの表示.
						TransBlt(
							gameWnd->hScreenDC,											//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
							( int ) ( Ending.EnEng[eNo].x ),							//表示位置x座標.
							( int ) ( Ending.EnEng[eNo].y ),							//表示位置y座標.
							Ending.EnEng[eNo].wDest,									//画像の幅.
							Ending.EnEng[eNo].hDest,									//画像の高さ.
							hMemDC,														//ﾒﾓﾘDC.
							Ending.EnEng[eNo].StartX,									//元画像のx座標.
							Ending.EnEng[eNo].StartY );									//元画像のy座標.
					}
					else {									//文字.
						//スタッフロールの表示.
						TransPlgBlt(
							gameWnd->hScreenDC,											//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
							( int ) ( Ending.EnEng[eNo].x ),							//表示位置x座標.
							( int ) ( Ending.EnEng[eNo].y - Ending.EnEng[eNo].Corr ),	//表示位置y座標.
							Ending.EnEng[eNo].wDest,									//画像の幅.
							Ending.EnEng[eNo].hDest,									//画像の高さ.
							hMemDC,														//ﾒﾓﾘDC.
							Ending.EnEng[eNo].StartX,									//元画像のx座標.
							Ending.EnEng[eNo].StartY,									//元画像のy座標.
							RAD( Ending.EnEng[eNo].Dang ),
							hPlgDC );
					}
				}
			}

			for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
				for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
					if ( ( Player.shot[numNo][dirNo].DispShotFlag == true ) || ( Player.chara.chgCnt != 0 ) ) {
						if ( Player.ability.beam != 0 ) {
							//キャラクタの画像をメモリDCへコピー.
							SelectObject( hMemDC, hboss );
							//プレイヤーのビーム弾の表示.
							TransPlgBlt(
								gameWnd->hScreenDC,																													//デバイスコンテキスト.
								static_cast< int >( Player.shot[numNo][dirNo].x - S_SIZE - ( 8 - 7 * ( BEAMMAX - Player.shot[numNo][dirNo].beamCnt ) ) ),			//表示位置x.
								static_cast< int >( Player.shot[numNo][dirNo].y - S_SIZE ),																			//表示位置y.
								S_SIZE, S_SIZE * Player.shot[numNo][dirNo].beamCnt,																					//画像の幅,高さ.
								hMemDC,																																//メモリDC.
								S_SIZE * 7,																															//元画像のx座標
								S_SIZE * 8 + S_SIZE * ( BEAMMAX - Player.shot[numNo][dirNo].beamCnt ),																//元画像のy座標.
								RAD( 90 ) - Player.shot[numNo][dirNo].ang,
								hPlgDC );
						}
						else if ( Player.ability.chg != 0 ) {
							if ( ( Player.shot[numNo][dirNo].DispShotFlag == false ) && ( Player.chara.chgCnt != 0 ) ) {	//溜め状態.
								//キャラクタの画像をメモリDCへコピー.
								SelectObject( hMemDC, hboss );
								//プレイヤーの溜め弾の表示.
								ExpaTransAlBlend(
									gameWnd->hScreenDC,													//デバイスコンテキスト.
									static_cast< int >( Player.shot[numNo][dirNo].x + 5 ),				//表示位置x.
									static_cast< int >( Player.shot[numNo][dirNo].y - 5 ),				//表示位置y.
									S_SIZE * 2, S_SIZE * 2,												//画像の幅,高さ.
									hMemDC,																//メモリDC.
									S_SIZE * 5, S_SIZE * 10,											//元画像のy座標
									static_cast< int >( 50 + Player.chara.chgCnt + Player.chgAniCnt ),
									hWorkDC,
									static_cast< int >( S_SIZE * ( 2 + Player.chara.chgCnt / 75 ) ),	//幅の拡大倍率.
									static_cast< int >( S_SIZE *( 2 + Player.chara.chgCnt / 75 ) ) );	//高さの拡大倍率.	
							}
							else {	//発射後.
								//キャラクタの画像をメモリDCへコピー.
								SelectObject( hMemDC, hboss );
								//プレイヤーの溜め弾の表示.
								ExpaTransBlt(
									gameWnd->hScreenDC,											//デバイスコンテキスト.
									static_cast< int >( Player.shot[numNo][dirNo].x ),			//表示位置x.
									static_cast< int >( Player.shot[numNo][dirNo].y ),			//表示位置y.
									S_SIZE * 2, S_SIZE * 2,										//画像の幅,高さ.
									hMemDC,														//メモリDC.
									S_SIZE * 5, S_SIZE * 10,									//元画像のy座標
									static_cast< int >( S_SIZE * ( 2 + Player.chara.chgCnt / 75 ) ),	//幅の拡大倍率.
									static_cast< int >( S_SIZE *( 2 + Player.chara.chgCnt / 75 ) ) );	//高さの拡大倍率.	
							}
						}
						else if ( Player.ability.pene != 0 ) {
							//キャラクタの画像をメモリDCへコピー.
							SelectObject( hMemDC, hboss );
							//プレイヤーの貫通弾の表示.
							TransBlt(
								gameWnd->hScreenDC,											//デバイスコンテキスト.
								static_cast< int >( Player.shot[numNo][dirNo].x ),			//表示位置x.
								static_cast< int >( Player.shot[numNo][dirNo].y ),			//表示位置y.
								S_SIZE, S_SIZE,												//画像の幅,高さ.
								hMemDC,														//メモリDC.
								S_SIZE * 3, S_SIZE * 11 );									//元画像のy座標
						}
						else {
							//キャラクタの画像をメモリDCへコピー.
							SelectObject( hMemDC, hShot );
							//プレイヤーのノーマル弾の表示.
							TransBlt(
								gameWnd->hScreenDC,											//デバイスコンテキスト.
								static_cast< int >( Player.shot[numNo][dirNo].x ),			//表示位置x.
								static_cast< int >( Player.shot[numNo][dirNo].y ),			//表示位置y.
								S_SIZE, S_SIZE,												//画像の幅,高さ.
								hMemDC,														//メモリDC.
								0, 0 );														//画像のy座標.
						}
					}
				}
			}

			break;
		}
		default:	//上記以外の場合.
			//基本ここには来ないはず.
			break;
		}
		if ( Fade.FadeState != enFade::None ){	//フェード.
			//単色用DCに黒色のブラシをセット.
			SelectObject( hColorDC, hBBrush );
			//黒色の画像を用意.
			PatBlt(
				hColorDC,		//単色用DCへ出力.
				0, 0,			//表示位置x,y.
				WND_W, WND_H,	//画像の幅,高さ.
				PATCOPY );
			//フェードの画像をﾒﾓﾘDCへｺﾋﾟｰ.
			AlBlend(
				gameWnd->hScreenDC,	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				0, 0,				//表示位置x,y.
				WND_W, WND_H,		//画像の幅,高さ.
				hColorDC,			//ﾒﾓﾘDC.
				0, 0,				//元画像のx,y座標.
				Fade.FadeAlpha );	//アルファ値.
		}
		//ポーズ.
		if ( pause == true ){
			//キャラクタの画像をメモリDCへコピー.
			SelectObject( hMemDC, hPause );
			BitBlt(
				gameWnd->hScreenDC,					//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				WND_W / 2 - PAUSE_W / 2,			//表示位置x座標.
				WND_H / 2 - PAUSE_H / 2,			//表示位置y座標.
				PAUSE_W, PAUSE_H,					//画像の幅,高さ.
				hMemDC,								//ﾒﾓﾘDC.
				0, 0,								//元画像のx,y座標.
				SRCCOPY );							//ｺﾋﾟｰする.

			//キャラクタの画像をメモリDCへコピー.
			SelectObject( hMemDC, hPSlime );
			//選択矢印の画像をﾒﾓﾘDCへｺﾋﾟｰ.
			TransAlBlend(
				gameWnd->hScreenDC,																		//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
				WND_W / 2 - PAUSE_W / 2 + S_SIZE * 3,													//表示位置x.
				WND_H / 2 - PAUSE_H / 2 + S_SIZE * 5 + ( S_SIZE * 2 + S_SIZE / 2 + 2 ) * pauseArea,		//表示位置y.
				23, 23,																					//画像の幅,高さ.
				hMemDC,																					//ﾒﾓﾘDC.
				C_SIZE * 3 + 23,																		//元画像のx座標.
				C_SIZE * 2 + 16 * 2,																	//元画像のy座標.
				pauseAlpha, 																					//アルファ値.
				hWorkDC );
		}

#ifdef _DEBUG	/** ▼ **********************出現中の変数データ表示************************ ▼ **/
		//FPSの表示.
		TextOut( gameWnd->hScreenDC,
			WND_W - 100,
			0,
			MsgFPS, lstrlen( MsgFPS ) );

		//出現状態.
		if ( Scene == enScene::GameMain ){
			const static int	FSize	= 13;		//フォントサイズ.
			const static bool	MasDisp = false;	//メッセージ表示フラグ.

			if ( MasDisp == true ) {
				int cnt = 0;

				SetTextColor( gameWnd->hScreenDC, RGB( 155, 0, 0 ) );
				wsprintf( MsgE_St, "ボス.PatternCnt = %d", Boss.PatternCnt );
				TextOut( gameWnd->hScreenDC,
					0,
					FSize * cnt,
					MsgE_St, lstrlen( MsgE_St ) );
				cnt++;

				SetTextColor( gameWnd->hScreenDC, RGB( 155, 0, 0 ) );
				wsprintf( MsgE_St, "ボス.AttackCnt = %d", Boss.AttackCnt );
				TextOut( gameWnd->hScreenDC,
					0,
					FSize * cnt,
					MsgE_St, lstrlen( MsgE_St ) );
				cnt++;

				SetTextColor( gameWnd->hScreenDC, RGB( 155, 0, 0 ) );
				wsprintf( MsgE_St, "ボス.AttackMovecnt = %d", Boss.AttackMovecnt );
				TextOut( gameWnd->hScreenDC,
					0,
					FSize * cnt,
					MsgE_St, lstrlen( MsgE_St ) );
				cnt++;

				SetTextColor( gameWnd->hScreenDC, RGB( 155, 0, 0 ) );
				wsprintf( MsgE_St, "ボス.Blowcnt = %d", Boss.Blowcnt );
				TextOut( gameWnd->hScreenDC,
					0,
					FSize * cnt,
					MsgE_St, lstrlen( MsgE_St ) );
				cnt++;

				for ( int type = 0; type < E_TYPE; type++ ) {
					for ( int ptn = 0; ptn < E_PTN; ptn++ ) {
						for ( int eno = 0; eno < E_MAX; eno++ ) {
							if ( Enemy[type][ptn][eno].chara.State != enCharaState::Wait ) {
								SetTextColor( gameWnd->hScreenDC, RGB( 155, 0, 0 ) );
								wsprintf( MsgE_St, "　 敵[%d][%d][%d].State = %d, HP = %d, Ran = %d, x = %3d, y = %3d", type, ptn, eno, Enemy[type][ptn][eno].chara.State, Enemy[type][ptn][eno].status.hp, Enemy[type][ptn][eno].ran, static_cast< int >( Enemy[type][ptn][eno].chara.x ), static_cast< int >( Enemy[type][ptn][eno].chara.y ) );
								TextOut( gameWnd->hScreenDC,
									0,
									FSize * cnt,
									MsgE_St, lstrlen( MsgE_St ) );
								cnt++;
							}
						}
					}
				}
				for ( int type = 0; type < Boss.BOSS_TYPE; type++ ) {
					if ( Boss.chara[type].State != enCharaState::Wait ) {
						SetTextColor( gameWnd->hScreenDC, RGB( 155, 155, 155 ) );
						wsprintf( MsgE_St, "ボス[%d].State = %d, HP = %d, x = %3d, y = %3d", type, Boss.chara[type].State, Boss.status[type].hp, static_cast< int >( Boss.chara[type].x ), static_cast< int >( Boss.chara[type].y ) );
						TextOut( gameWnd->hScreenDC,
							0,
							FSize * cnt,
							MsgE_St, lstrlen( MsgE_St ) );
						cnt++;
					}
				}
				for ( int type = 0; type < G_TYPE; type++ ) {
					for ( int ptn = 0; ptn < G_PTN; ptn++ ) {
						for ( int gno = 0; gno < G_MAX; gno++ ) {
							if ( gimmick[type][ptn][gno].State != enGimmickState::GWait ) {
								SetTextColor( gameWnd->hScreenDC, RGB( 0, 155, 0 ) );
								wsprintf( MsgE_St, "ギミック[%d][%d][%d].State = %d, HP = %d, x = %3d, y = %3d", type, ptn, gno, gimmick[type][ptn][gno].State, gimmick[type][ptn][gno].hp, static_cast< int >( gimmick[type][ptn][gno].x ), static_cast< int >( gimmick[type][ptn][gno].y ) );
								TextOut( gameWnd->hScreenDC,
									0,
									FSize * cnt,
									MsgE_St, lstrlen( MsgE_St ) );
								cnt++;
							}
						}
					}
				}
				for ( int num = 0; num < S_NUMMAX; num++ ) {
					for ( int dir = 0; dir < S_DIRMAX; dir++ ) {
						if ( Player.shot[num][dir].DispShotFlag == true ) {
							SetTextColor( gameWnd->hScreenDC, RGB( 0, 0, 155 ) );
							wsprintf( MsgE_St, "自弾.[%d][%d].x = %3d, y = %3d, Ang = %d, ", num, dir, static_cast< int >( Player.shot[num][dir].x ), static_cast< int >( Player.shot[num][dir].y ), static_cast< int >( DO( Player.shot[num][dir].ang ) ) );
							TextOut( gameWnd->hScreenDC,
								0,
								FSize * cnt,
								MsgE_St, lstrlen( MsgE_St ) );
							cnt++;
						}
					}
				}
				for ( int type = 0; type < E_TYPE; type++ ) {
					for ( int ptn = 0; ptn < E_PTN; ptn++ ) {
						for ( int eno = 0; eno < E_MAX; eno++ ) {
							for ( int num = 0; num < S_NUMMAX; num++ ) {
								for ( int dir = 0; dir < S_DIRMAX; dir++ ) {
									if ( Enemy[type][ptn][eno].shot[num][dir].DispShotFlag == true ) {
										SetTextColor( gameWnd->hScreenDC, RGB( 255, 0, 0 ) );
										wsprintf( MsgE_St, "敵弾[%d][%d][%d].[%d][%d].x = %3d, y = %3d, Ang = %d", num, dir, type, ptn, eno, static_cast< int >( Enemy[type][ptn][eno].shot[num][dir].x ), static_cast< int >( Enemy[type][ptn][eno].shot[num][dir].y ), static_cast< int >( DO( Enemy[type][ptn][eno].shot[num][dir].ang ) ) );
										TextOut( gameWnd->hScreenDC,
											0,
											FSize * cnt,
											MsgE_St, lstrlen( MsgE_St ) );
										cnt++;
									}
								}
							}
						}
					}
				}
				for ( int type = enBoss::RHand; type < Boss.BOSS_TYPE; type++ ) {
					for ( int num = 0; num < ROCK_NUM; num++ ) {
						for ( int dir = 0; dir < ROCK_DIR; dir++ ) {
							if ( Boss.rock[type][num][dir].DispFlag == true ) {
								SetTextColor( gameWnd->hScreenDC, RGB( 55, 55, 55 ) );
								wsprintf( MsgE_St, "ボス岩[%d][%d][%d].x = %3d, y = %3d, Ang = %d", type, num, dir, static_cast< int >( Boss.rock[type][num][dir].x ), static_cast< int >( Boss.rock[type][num][dir].y ), static_cast< int >( DO( Boss.rock[type][num][dir].Rang ) ) );
								TextOut( gameWnd->hScreenDC,
									0,
									FSize * cnt,
									MsgE_St, lstrlen( MsgE_St ) );
								cnt++;
							}
						}
					}
				}
				for ( int num = 0; num < Boss.BS_NUM; num++ ) {
					for ( int dir = 0; dir < Boss.BS_DIR; dir++ ) {
						if ( Boss.shot[num][dir].DispShotFlag == true ) {
							SetTextColor( gameWnd->hScreenDC, RGB( 0, 0, 0 ) );
							wsprintf( MsgE_St, "ボス弾.[%d][%d].x = %3d, y = %3d, Ang = %d", num, dir, static_cast< int >( Boss.shot[num][dir].x ), static_cast< int >( Boss.shot[num][dir].y ), static_cast< int >( DO( Boss.shot[num][dir].ang ) ) );
							TextOut( gameWnd->hScreenDC,
								0,
								FSize * cnt,
								MsgE_St, lstrlen( MsgE_St ) );
							cnt++;
						}
					}
				}
			}
		}
#endif			/** ▲ ************************ 出現中の変数データ表示 *********************** ▲ **/

		//描画の時間待ち.
		if( idealtime > progressTime ){
			Sleep( idealtime - progressTime );	//時間待ち.
		}

		//再描画.
		InvalidateRect( gameWnd->hWnd, nullptr, FALSE );

		//1秒経過した？.
		if( progressTime >= 1000 )
		{
			SetTextColor( gameWnd->hScreenDC, RGB( 0, 0, 0 ) );
			wsprintf( MsgFPS, "FPS[%03d]", frames );
			beforeTime = nowTime;	//現在の時間に更新.
			frames = 0;	//ﾌﾚｰﾑ数を初期化.
		}
		frames++;	//ﾌﾚｰﾑ数をUP.
	}

//-------------------------------------------------------------------.
//	解放処理[WM_DESTROY].
//-------------------------------------------------------------------.
	//BITMAPの解放.
	DeleteObject( hEnHitNum		);
	DeleteObject( hEnEneEng		);
	DeleteObject( hEnEng		);
	DeleteObject( hEnEnemy		);
	DeleteObject( hEnGround		);
	DeleteObject( hEnClear		);
	DeleteObject( hEnSlime		);
	DeleteObject( hEnding		);
	DeleteObject( hDeadG		);
	DeleteObject( hDead			);
	DeleteObject( hGameOver		);
	DeleteObject( hcloud		);
	DeleteObject( hsky			);
	DeleteObject( hTitle		);
	DeleteObject( hPause		);
	DeleteObject( hMainLR		);
	DeleteObject( hSleep		);
	DeleteObject( hboss			);
	DeleteObject( hShot			);
	DeleteObject( hSlime		);
	DeleteObject( hCharaDeat	);
	DeleteObject( hPSlimeHit	);
	DeleteObject( hCharaHit		);
	DeleteObject( hPSlime		);
	DeleteObject( hChara		);
	DeleteObject( hLetter		);
	DeleteObject( harea			);
	DeleteObject( hability		);
	DeleteObject( hTreeHit		);
	DeleteObject( hTree			);
	DeleteObject( hGmck			);
	DeleteObject( hPHp			);
	DeleteObject( hhp_Gmck		);
	DeleteObject( hscroll		);

	//死亡シーン用DCの解放.
	DeleteObject( hDeatBmp );
	DeleteDC( hDeatDC );
	//画像の回転用DCの解放.
	DeleteObject( hPlgBmp );
	DeleteDC( hPlgDC );
	//単色用DCの解放.
	DeleteObject( hColorBmp );
	DeleteDC( hColorDC );
	//作業用DCの解放.
	DeleteObject( hWorkBmp );
	DeleteDC( hWorkDC );

	//ブラシの解放.
	DeleteObject( hGBrush  );
	DeleteObject( hGrBrush );
	DeleteObject( hBBrush  );
	DeleteObject( hRBrush  );

	//ﾌｫﾝﾄの解放.
	DeleteObject( hFont );

	//ﾒﾓﾘDCの解放.
	DeleteDC( hMemDC );

	//振動の停止.
	pXInput->SetVibration( 0, 0 );

	//ｽﾚｯﾄﾞ終了通知.
	ExitThread( 0 );

	return 1;
}

//ｳｨﾝﾄﾞｳ関数.
LRESULT CALLBACK WindowProc(
	HWND hWnd,		//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	UINT uMsg,		//ﾒｯｾｰｼﾞ.
	WPARAM wParam,	//ﾒｯｾｰｼﾞに対する情報.
	LPARAM lParam)	//ﾒｯｾｰｼﾞに対する情報.
{
	HDC hDC;		//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	PAINTSTRUCT ps;	//描画構造体.

	//---------------------------------------------.
	//	ｽﾚｯﾄﾞ関連.
	//---------------------------------------------.
	static GameWindow gameWnd;	//ｹﾞｰﾑｳｨﾝﾄﾞｳ構造体.
	static HANDLE hThread;		//ｽﾚｯﾄﾞﾊﾝﾄﾞﾙ.
	DWORD dwID;					//ｽﾚｯﾄﾞID.
	HBITMAP hBmp;				//ﾋﾞｯﾄﾏｯﾌﾟﾊﾝﾄﾞﾙ.

	//ﾒｯｾｰｼﾞ別の処理.
	switch (uMsg) {
	case WM_DESTROY:	//ｳｨﾝﾄﾞｳが破棄されたとき.
		//ｱﾌﾟﾘｹｰｼｮﾝの終了をWindowsに通知する.
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:		//ｳｨﾝﾄﾞｳが生成されるとき.
		//乱数の初期化(種の設定).
		srand( (unsigned int)time( nullptr ) );
		
		//-----------------------------------------------.
		//	ｽﾚｯﾄﾞの設定.
		//-----------------------------------------------.
		//ｽﾚｯﾄﾞに渡す値を設定.
		gameWnd.hWnd = hWnd;	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
		gameWnd.size.cx = WND_W;//ｳｨﾝﾄﾞｳ幅.
		gameWnd.size.cy = WND_H;//ｳｨﾝﾄﾞｳ高さ.
		gameWnd.dwFPS = FPS;	//FPS値.

		//HDCの作成.
		hDC = GetDC( hWnd );	//DC(ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ)を取得.

		//ﾋﾞｯﾄﾏｯﾌﾟﾊﾝﾄﾞﾙを作成.
		hBmp = CreateCompatibleBitmap(
			hDC,
			gameWnd.size.cx,
			gameWnd.size.cy );

		//DCと互換性のあるDCを作成.
		gameWnd.hScreenDC = CreateCompatibleDC( hDC );

		//hScreenDCをﾋﾞｯﾄﾏｯﾌﾟ用として定義.
		SelectObject( gameWnd.hScreenDC, hBmp );

		//不要になったDCを解放.
		ReleaseDC( hWnd, hDC );

		//ｽﾚｯﾄﾞの作成と実行.
		hThread = CreateThread(
			nullptr,			//ﾊﾝﾄﾞﾙを他のﾌﾟﾛｾｽと共有する場合は指定.
			0,					//ｽﾀｯｸｻｲｽﾞ(ﾃﾞﾌｫﾙﾄ:0).
			ThreadFunc,			//ｽﾚｯﾄﾞ関数名.
			(LPVOID)&gameWnd,	//ｽﾚｯﾄﾞに渡す構造体.
			0,					//0:作成と同時に実行.
			&dwID );			//(out)ｽﾚｯﾄﾞID.

		//マウスカーソルを消す.
		ShowCursor( FALSE );

		return 0;

	case WM_KEYDOWN:	//ｷｰが押されたとき.
		////ｷ-別の処理.
		//switch( wParam ){
		//case VK_ESCAPE:	//Escapeｷｰ.
		//	//フェードイン.
		//	pause			= true;
		//	Fade.InOutFlag	= false;
		//	Fade.FadeState	= enFade::in;
		//	Fade.EndAlpha	= 127;

		//	//マウスカーソルを消す.
		//	ShowCursor( TRUE );

		//	if( MessageBox( nullptr,
		//		"ゲームを終了しますか？", "ポーズ中",
		//		MB_YESNO ) == IDYES )
		//	{
		//		//ｳｨﾝﾄﾞｳを閉じることをWindowsに知らせる.
		//		PostMessage( gameWnd.hWnd, WM_CLOSE, 0, 0 );
		//	}
		//	else { 
		//		//フェードアウト.
		//		Fade.InOutFlag	= true;
		//		Fade.FadeState	= enFade::out;
		//		Fade.EndAlpha	= 255;

		//		//マウスカーソルを消す.
		//		ShowCursor( FALSE );
		//	}
		//	break;
		//}

		return 0;

	case WM_CLOSE://ｳｨﾝﾄﾞｳを閉じる.
		//ｳｨﾝﾄﾞｳ閉じるﾌﾗｸﾞ有効.
		//※ｽﾚｯﾄﾞ側で確認するためのﾌﾗｸﾞ.
		gameWnd.isWndClose = TRUE;

		DWORD result;
		while( 1 ){
			//ｽﾚｯﾄﾞが終わったかﾁｪｯｸ.
			GetExitCodeThread( hThread, &result );
			//終わったらﾊﾝﾄﾞﾙを閉じる.
			if( STILL_ACTIVE != result )
			{
				//CloseHandleで閉じる.
				CloseHandle( hThread );
				//ﾙｰﾌﾟを抜ける.
				break;
			}
		}
		//ｳｨﾝﾄﾞｳを破棄する.
		DestroyWindow( hWnd );
		return 0;

	case WM_PAINT:		//ｳｨﾝﾄﾞｳが更新されたとき.
		//描画開始.
		hDC = BeginPaint( hWnd, &ps );

		BitBlt(
			hDC,				//表示DC.
			0, 0,				//表示先x,y座標.
			gameWnd.size.cx,	//表示する幅.
			gameWnd.size.cy,	//表示する高さ.
			gameWnd.hScreenDC,	//ﾊﾞｯｸﾊﾞｯﾌｧ(ｽﾚｯﾄﾞ内で作成).
			0, 0,				//元画像のx,y.
			SRCCOPY );			//ｺﾋﾟｰする.

		//描画終了.
		EndPaint( hWnd, &ps );
		return 0;
	}

	//ﾒｲﾝに返す情報.
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//ﾒｲﾝ関数.
int WINAPI WinMain(
	HINSTANCE hInstance,		//ｲﾝｽﾀﾝｽ番号(ｳｨﾝﾄﾞｳの番号).
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASS wc;	//ｳｨﾝﾄﾞｳｸﾗｽ構造体.
	HWND hWnd;		//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	MSG msg;		//ﾒｯｾｰｼﾞ.

	//--------------------------------------.
	//	複数CPUに対応させる ▼.
	//--------------------------------------.
	DWORD ProcessAffinitMask;	//CPUを指定.
	HANDLE hProcess;			//ﾌﾟﾛｾｽﾊﾝﾄﾞﾙ.
	DWORD errnum;				//ｴﾗｰNo.
	BOOL ret;					//関数復帰値.

	//自身のﾌﾟﾛｾｽのﾊﾝﾄﾞﾙを取得.
	hProcess = GetCurrentProcess();

	//自身を動作させるCPUを指定する.
	ProcessAffinitMask = 1;	//CPU 1を使う(2進数で指定).
	ret = SetProcessAffinityMask(
		hProcess,
		ProcessAffinitMask );
	if( ret == FALSE )
	{
		errnum = GetLastError();	//ｴﾗｰ情報を取得.
		return 0;
	}
	//--------------------------------------.
	//	複数CPUに対応させる ▲.
	//--------------------------------------.

	//--------------------------------------.
	//	ｳｨﾝﾄﾞｳｸﾗｽの登録.
	//--------------------------------------.
	//ｽﾀｲﾙの登録.
	//	CS_HREDRAW:水平方向の再描画.
	//	CS_VREDRAW:垂直方向の再描画.
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//ｳｨﾝﾄﾞｳ関数の登録.
	//	DefWindowProc:ﾃﾞﾌｫﾙﾄｳｨﾝﾄﾞｳ関数(後で自分で作る).
	wc.lpfnWndProc = WindowProc;
	//使わない(0固定).
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	//ｲﾝｽﾀﾝｽ番号の登録.
	wc.hInstance = hInstance;
	//ｱｲｺﾝの登録.
	//	nullptr:ﾃﾞﾌｫﾙﾄ.
	wc.hIcon = LoadIcon( hInstance, "ICON" );
	//ｶｰｿﾙの登録.
	//	nullptr:ﾃﾞﾌｫﾙﾄ.
	wc.hCursor = nullptr;
	//ｳｨﾝﾄﾞｳの背景色.
	//	LTGRAY_BRUSH:明るい灰色.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	//ﾒﾆｭｰの登録.
	//	nullptr:ﾃﾞﾌｫﾙﾄ.
	wc.lpszMenuName = nullptr;
	//ｱﾌﾟﾘｹｰｼｮﾝ名.
	wc.lpszClassName = APP_NAME;

	//ｳｨﾝﾄﾞｳをWindowsに登録.
	if (RegisterClass(&wc) == 0)
	{
		//ｴﾗｰﾒｯｾｰｼﾞ表示.
		MessageBox(nullptr,
			"ｳｨﾝﾄﾞｳｸﾗｽ登録失敗",
			"ｴﾗｰﾒｯｾｰｼﾞ",
			MB_YESNOCANCEL);
		//MB_OK:OKﾎﾞﾀﾝのみ.
		//MB_YESNO:はい・いいえ.
		//MB_OKCANCEL:OK・ｷｬﾝｾﾙ.
		//MB_YESNOCANCEL:はい・いいえ・ｷｬﾝｾﾙ.
		return 0;
	}

	//------------------------------------------------.
	//	ｳｨﾝﾄﾞｳ領域の調整.
	//------------------------------------------------.
	RECT rect;			//矩形構造体.
	DWORD dwStyle;		//ｳｨﾝﾄﾞｳｽﾀｲﾙ.
	rect.top = 0;		//上.
	rect.left = 0;		//左.
	rect.right = WND_W;	//右.
	rect.bottom = WND_H;//下.
	dwStyle = WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX;	//ｳｨﾝﾄﾞｳ種別.
	//	WS_SYSMENU		： 全てのボタン.
	//	WS_MINIMIZEBOX	： 最小化ボタン.
	//	WS_MAXIMIZEBOX	： 最大化ボタン.
	//	(WS_MIN + WS_MAX)で両方無効化 可.

	//フルスクリーンかどうか.
	bool FullScreenFlag = false;
	//フルスクリーンにするかのメッセージボックス.
	if ( MessageBox( nullptr,
			"フルスクリーンで起動しますか？",
			"確認", MB_YESNO ) == IDYES )
	{
		FullScreenFlag	= true;		//フルスクリーン.
		dwStyle			= WS_POPUP;	//枠無しウィンドウ.
	}

	if( AdjustWindowRect(
		&rect,	//(in)画面ｻｲｽﾞが入った矩形構造体.(out)計算結果.
		dwStyle,//ｳｨﾝﾄﾞｳ種別.
		FALSE ) == 0 )//ﾒﾆｭｰを持つかどうか指定.
	{
		MessageBox( nullptr,
			"ｳｨﾝﾄﾞｳ領域の調整に失敗",
			"ｴﾗｰﾒｯｾｰｼﾞ", MB_OK );
		return 0;
	}

	//------------------------------------------------.
	//	ｳｨﾝﾄﾞｳの作成.
	//------------------------------------------------.
	//ウィンドウの表示位置.
	int OffSet_x = 64, OffSet_y = 64;
	//フルスクリーンなら左上に変更.
	if ( FullScreenFlag == true ) {
		OffSet_x = 0;
		OffSet_y = 0;
	}

	hWnd = CreateWindow(
		APP_NAME,				//ｱﾌﾟﾘｹｰｼｮﾝ名.
		WND_TITLE,				//ｳｨﾝﾄﾞｳﾀｲﾄﾙ.
		dwStyle,				//普通のｳｨﾝﾄﾞｳ.
		OffSet_x, OffSet_y,		//ｳｨﾝﾄﾞｳの表示位置(x,y).
		rect.right - rect.left,	//ｳｨﾝﾄﾞｳの幅.
		rect.bottom - rect.top,	//ｳｨﾝﾄﾞｳの幅、高さ.
		nullptr,				//親ｳｨﾝﾄﾞｳのﾊﾝﾄﾞﾙ.
		nullptr,				//ﾒﾆｭｰ設定.
		hInstance,				//ｲﾝｽﾀﾝｽ番号.
		nullptr);				//ｳｨﾝﾄﾞｳ作成時に発生するｲﾍﾞﾝﾄに渡すﾃﾞｰﾀ.

	if (hWnd == nullptr) {
		MessageBox(nullptr,
			"ｳｨﾝﾄﾞｳ作成失敗",
			"ｴﾗｰﾒｯｾｰｼﾞ", MB_OK);
		return 0;
	}

	//フルスクリーンなら解像度を変更する.
	if ( FullScreenFlag == true ) {
		DEVMODE devMode;

		devMode.dmSize			= sizeof( DEVMODE );
		devMode.dmFields		= DM_PELSWIDTH | DM_PELSHEIGHT;
		devMode.dmPelsWidth		= WND_W;						//解像度の幅.
		devMode.dmPelsHeight	= WND_H;						//解像度の高さ.
		ChangeDisplaySettings( &devMode, CDS_FULLSCREEN );		//フルスクリーンにする.
	}

	//ｳｨﾝﾄﾞｳの表示.
	ShowWindow( hWnd, SW_SHOW );
	//SW_SHOW:表示する.
	//SW_HIDE:隠す(非表示).


	//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
	while( GetMessage( &msg, nullptr, 0, 0 ) > 0 ){
		//ｳｨﾝﾄﾞｳ関数にﾒｯｾｰｼﾞを送る.
		DispatchMessage( &msg );
	}

	return 0;
}


//画像の読み込み関数.
//戻り値の型　関数名( 第一引数、　第二引数 ).
bool LoadBmp( HBITMAP* phBmp, const char* fileName )
{
	*phBmp = (HBITMAP) LoadImage(
		nullptr,					//ｲﾝｽﾀﾝｽ.
		fileName,					//ﾌｧｲﾙ名(ﾊﾟｽ含む).
		IMAGE_BITMAP,				//ﾋﾞｯﾄﾏｯﾌﾟ.
		0, 0,						//読み込む座標.
		LR_LOADFROMFILE );			//ﾌｧｲﾙから読み込む.

	if( *phBmp == nullptr ){
		MessageBox( nullptr, fileName, "画像読み込み失敗", MB_OK );
		return false;
	}

	return true;
}

//単色画像を表示する関数.
void ColorBlt(
	int BltNo,		HDC hdcDest,  int xDest, int yDest, int wDest, int hDest,
	HDC hdcColor,	COLORREF RGB, int Alpha )
{
	//ブラシの作成.
	HBRUSH hBrush = CreateSolidBrush( RGB );
	SelectObject( hdcColor, hBrush );

	//単色画像作成.
	PatBlt(
		hdcColor,		//単色用DCへ出力.
		0, 0,			//表示位置x,y.
		wDest, hDest,	//画像の幅,高さ.
		PATCOPY );

	switch ( BltNo )
	{
	case enBltNo::Bit:
		BitBlt(
			hdcDest,							//表示先(実はﾃﾞｨｽﾌﾟﾚｲ).
			xDest, yDest,						//表示位置x,y座標.
			wDest, hDest,						//表示高さ.
			hdcColor,							//画像が設定されたﾒﾓﾘDC.
			0, 0,								//元画像の取り出し開始x,y座標.
			SRCCOPY );
		break;
	case enBltNo::Trans:
		TransBlt(
			hdcDest,							//表示先(実はﾃﾞｨｽﾌﾟﾚｲ).
			xDest, yDest,						//表示位置x,y座標.
			wDest, hDest,						//表示高さ.
			hdcColor,							//画像が設定されたﾒﾓﾘDC.
			0, 0 );								//元画像の取り出し開始x,y座標.
		break;
	case enBltNo::Blend:
		AlBlend(
			hdcDest,							//表示先(実はﾃﾞｨｽﾌﾟﾚｲ).
			xDest, yDest,						//表示位置x,y座標.
			wDest, hDest,						//表示高さ.
			hdcColor,							//画像が設定されたﾒﾓﾘDC.
			0, 0,								//元画像の取り出し開始x,y座標.
			Alpha );							//アルファ値.
		break;
	default:
		break;
	}

	//ブラシの解放.
	DeleteObject( hBrush );
}

//画像を透過して表示する関数.
bool TransBlt(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc )
{
	if( TransparentBlt(
		hdcDest,							//表示先(実はﾃﾞｨｽﾌﾟﾚｲ).
		xDest, yDest,						//表示位置x,y座標.
		wDest, hDest,						//表示高さ.
		hdcSrc,								//画像が設定されたﾒﾓﾘDC.
		xSrc, ySrc,							//元画像の取り出し開始x,y座標.
		wDest, hDest,						//元画像の取り出し幅,高さ.
		RGB( 0x00, 0xFF, 0x00 ) ) )			//透過色(緑を指定).
	{
		return true;
	}
	return false;
}

//拡大した画像を透過して表示する関数.
bool ExpaTransBlt(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int weDest, int heDest, bool xyFlag )
{
	if ( xyFlag == true ){
		xDest -= weDest / 2;
		yDest -= heDest / 2;
	}

	if ( TransparentBlt(
		hdcDest,					//表示先(実はﾃﾞｨｽﾌﾟﾚｲ).
		xDest, yDest,				//表示位置x,y座標.
		weDest, heDest,				//表示幅,高さ.
		hdcSrc,						//画像が設定されたﾒﾓﾘDC.
		xSrc, ySrc,					//元画像の取り出し開始x,y座標.
		wDest, hDest,				//元画像の取り出し幅,高さ.
		RGB( 0x00, 0xFF, 0x00 ) ) )	//透過色(緑を指定).
	{
		return true;
	}
	return false;
}

//画像を半透明にして表示する関数.
bool AlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int Alpha )
{
	//BLENDFUNCTION構造体.
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, (BYTE)Alpha, 0 };

	//半透明にする画像の表示.
	if( AlphaBlend(
		hdcDest,		//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
		xDest, yDest,	//表示位置x,y座標.
		wDest, hDest,	//表示幅,高さ.
		hdcSrc,			//読み込み元DC.
		xSrc, ySrc,		//読み込み元画像x,y座標.
		wDest, hDest,	//読み込み元画像幅,高さ.
		blendfunc ) )	//ﾌﾞﾚﾝﾄﾞﾌｧﾝｸｼｮﾝ.
	{
		return true;
	}
	return false;
}

//回転した画像を透過して表示する関数.
bool TransPlgBlt(
	HDC hdcDest, LONG xDest, LONG yDest, LONG wDest, LONG hDest,
	HDC hdcSrc,	 LONG xSrc,  LONG ySrc,  DOUBLE Ang, HDC hdcPlg )
{
 	DOUBLE	whDest = ( wDest / 2 );										//画像の幅の半分.
	DOUBLE	hhDest = ( hDest / 2 );										//画像の高さの半分.
	DOUBLE	radius = sqrt( ( whDest * whDest ) + ( hhDest * hhDest ) );	//回転半径.
	DOUBLE	theta = atan2( whDest, hhDest );							//三角形の角度 θ角度.
	POINT	po[3];														//矩形の回転頂点.

	//矩形の左上の頂点.
	Ang -= ( theta + PI / 2 );	//-( θ + 90 )度.
	po[0].x = ( LONG )( radius + cos( Ang ) * radius ); 
	po[0].y = ( LONG )( radius + sin( Ang ) * radius );
	//矩形の右上の頂点.
	Ang += theta * 2;			//+( θ * 2 )度.
	po[1].x = ( LONG )( radius + cos( Ang ) * radius );
	po[1].y = ( LONG )( radius + sin( Ang ) * radius );
	//矩形の左下の頂点.
	Ang += PI;				//+( 180 )度.
	po[2].x = ( LONG )( radius + cos( Ang ) * radius );
	po[2].y = ( LONG )( radius + sin( Ang ) * radius );

	//作業DCの背景を緑で塗る.
	PatBlt(
		hdcPlg,																//単色用DCへ出力.
		0, 0,																//表示位置x,y.
		static_cast< int >( radius * 2 ), static_cast< int >( radius * 2 ),	//画像の幅,高さ.
		PATCOPY );
	//作業DCに回転した画像を出力.
	PlgBlt(
		hdcPlg,					//デバイスコンテキスト.
		po,						//矩形の回転頂点.
		hdcSrc,					//メモリDC.
		xSrc, ySrc,				//元画像のx,y座標.
		wDest, hDest,			//画像の幅,高さ.
		NULL,
		0, 0 );
	//作業DCの画像を透過してスクリーンに出力.
	TransBlt(
		hdcDest,							//デバイスコンテキスト.
		xDest, yDest,						//表示位置x,y.
		static_cast< int >( radius * 2 ),	//画像の幅.
		static_cast< int >( radius * 2 ),	//画像の高さ.
		hdcPlg,								//メモリDC.
		0, 0 );								//元画像のx,y座標.

	return true;
}

//回転した画像を透過して半透明にする関数.
bool TransPlgAlBlend(
	HDC hdcDest, LONG xDest, LONG yDest, LONG wDest, LONG hDest,
	HDC hdcSrc,  LONG xSrc,  LONG ySrc,  DOUBLE Ang, HDC hdcPlg, int Alpha, HDC hdcWork )
{
	//BLENDFUNCTION構造体.
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, ( BYTE )Alpha, 0 };

	DOUBLE	whDest = ( wDest / 2 );										//画像の幅の半分.
	DOUBLE	hhDest = ( hDest / 2 );										//画像の高さの半分.
	DOUBLE	radius = sqrt( ( whDest * whDest ) + ( hhDest * hhDest ) );	//回転半径.
	DOUBLE	theta = atan2( whDest, hhDest );							//三角形の角度 θ角度.
	POINT	po[3];														//矩形の回転頂点.

	//矩形の左上の頂点.
	Ang -= ( theta + PI / 2 );	//-( θ + 90 )度.
	po[0].x = ( LONG )( radius + cos( Ang ) * radius );
	po[0].y = ( LONG )( radius + sin( Ang ) * radius );
	//矩形の右上の頂点.
	Ang += theta * 2;			//+( θ * 2 )度.
	po[1].x = ( LONG )( radius + cos( Ang ) * radius );
	po[1].y = ( LONG )( radius + sin( Ang ) * radius );
	//矩形の左下の頂点.
	Ang += PI;				//+( 180 )度.
	po[2].x = ( LONG )( radius + cos( Ang ) * radius );
	po[2].y = ( LONG )( radius + sin( Ang ) * radius );

	//作業DCの背景を緑で塗る.
	PatBlt(
		hdcPlg,																//単色用DCへ出力.
		0, 0,																//表示位置x,y.
		static_cast< int >( radius * 2 ), static_cast< int >( radius * 2 ),	//画像の幅,高さ.
		PATCOPY );
	//作業DCに回転した画像を出力.
	PlgBlt(
		hdcPlg,					//デバイスコンテキスト.
		po,						//矩形の回転頂点.
		hdcSrc,					//メモリDC.
		xSrc, ySrc,				//元画像のx,y座標.
		wDest, hDest,			//画像の幅,高さ.
		NULL,
		0, 0 );
	//表示位置のスクリーンの画像を出力.
	BitBlt(
		hdcWork,		//作業DCへ出力.
		0, 0,
		static_cast< int >( radius * 2 ),
		static_cast< int >( radius * 2 ),
		hdcDest,		//ディスプレイから切り取り.
		xDest, yDest,
		SRCCOPY );
	//作業DCの画像を透過してスクリーンに出力.
	TransBlt(
		hdcWork,							//デバイスコンテキスト.
		0, 0,								//表示位置x,y.
		static_cast< int >( radius * 2 ),	//画像の幅.
		static_cast< int >( radius * 2 ),	//画像の高さ.
		hdcPlg,								//メモリDC.
		0, 0 );								//元画像のx,y座標.
	//作業用DCに用意した物を半透明にして表示.
	AlphaBlend(
		hdcDest,		//ディスプレイへ出力.
		xDest, yDest,
		static_cast< int >( radius * 2 ),
		static_cast< int >( radius * 2 ),
		hdcWork,
		0, 0,
		static_cast< int >( radius * 2 ),
		static_cast< int >( radius * 2 ),
		blendfunc );
	return true;
}

//画像を透過して半透明にする.
bool TransAlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int Alpha, HDC hdcWork )
{
	//BLENDFUNCTION構造体.
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, ( BYTE ) Alpha, 0 };

	BitBlt(
		hdcWork,		//作業DCへ出力.
		0, 0,
		wDest, hDest,
		hdcDest,		//ディスプレイから切り取り.
		xDest, yDest,
		SRCCOPY );
	TransparentBlt(
		hdcWork,		//作業DCへ出力.
		0, 0,
		wDest, hDest,
		hdcSrc,
		xSrc, ySrc,
		wDest, hDest,
		RGB( 0x00, 0xFF, 0x00 ) );
	AlphaBlend(
		hdcDest,		//ディスプレイへ出力.
		xDest, yDest,
		wDest, hDest,
		hdcWork,
		0, 0,
		wDest, hDest,
		blendfunc );
	return true;
}

//拡大した画像を透過して半透明にする.
bool ExpaTransAlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int Alpha, HDC hdcWork, int weDest, int heDest, bool xyFlag )
{
	//BLENDFUNCTION構造体.
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, ( BYTE ) Alpha, 0 };

	if ( xyFlag == true ) {
		xDest -= weDest / 2;
		yDest -= heDest / 2;
	}

	BitBlt(
		hdcWork,		//作業DCへ出力.
		0, 0,
		weDest, heDest,
		hdcDest,		//ディスプレイから切り取り.
		xDest, yDest,
		SRCCOPY );
	TransparentBlt(
		hdcWork,		//作業DCへ出力.
		0, 0,
		weDest, heDest,
		hdcSrc,
		xSrc, ySrc,
		wDest, hDest,
		RGB( 0x00, 0xFF, 0x00 ) );
	AlphaBlend(
		hdcDest,		//ディスプレイへ出力.
		xDest, yDest,
		weDest, heDest,
		hdcWork,
		0, 0,
		weDest, heDest,
		blendfunc );
	return true;
}

//矩形同士の衝突判定.
bool CollisionDetection(
	double Ax, double Ay, int Aw, int Ah, //矩形Aのx,y座標と幅と高さ.
	double Bx, double By, int Bw, int Bh )//矩形Bのx,y座標と幅と高さ.
{
	//(Ax<=Bx+Bw)&&(Ax+Aw>=Bx)&&(Ay<=By+Bh)&&(Ay+Ah>=By).
	if( ( Ax < Bx + Bw ) && ( Ax + Aw > Bx ) &&
		( Ay < By + Bh ) && ( Ay + Ah > By ) )
	{
		//命中した時.
		return true;
	}
	return false;	//外れている.
}

//コントローラーまたはキーボードのどこかが押された.
bool GetAsyncAnyKey( PLAYER* Player, CXInput* pXInput, XInStTrg* XInKey )
{
	//キーボード.
	//for ( int vKeyNo = 8; vKeyNo <= 228; vKeyNo++ ){
	//	if ( GetAsyncKeyState( vKeyNo ) & 0x8000 ){
	//		return true;
	//	}
	//}
	//キーボードの使用する全てのボタン.
	int vKeyNo[9] = { 'A', 'S', 'D', 'F', VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN };
	for ( int KeyNo = 0; KeyNo < 9; KeyNo++ ) {
		if ( GetAsyncKeyState( vKeyNo[KeyNo] ) & 0x8000 ){
			return true;
		}
	}

	//コントローラーが接続されている.
	if ( pXInput->IsConnect() == true ) {

		//コントローラーの全てのボタン.
		int XINPUTNo[14] = { XINPUT_GAMEPAD_DPAD_UP,		XINPUT_GAMEPAD_DPAD_DOWN,	XINPUT_GAMEPAD_DPAD_LEFT,	XINPUT_GAMEPAD_DPAD_RIGHT,
							 XINPUT_GAMEPAD_BACK ,			XINPUT_GAMEPAD_LEFT_THUMB,	XINPUT_GAMEPAD_RIGHT_THUMB,	XINPUT_GAMEPAD_LEFT_SHOULDER,
							 XINPUT_GAMEPAD_RIGHT_SHOULDER,	XINPUT_GAMEPAD_A,			XINPUT_GAMEPAD_B,			XINPUT_GAMEPAD_X,				XINPUT_GAMEPAD_Y
		};
		for ( int PadNo = 0; PadNo < 14; PadNo++ ) {
			if ( pXInput->IsKeyDown( XINPUTNo[PadNo] ) ) {
				return true;
			}
		}

		//コントローラの左右のトリガー.
		if ( ( XInKey->trgR >= CXInput::INPUT_TRIGGER_MAX ) && ( Player->PushKey[enPushKey::RT] == false ) ) {
			Player->PushKey[enPushKey::RT] = true;
			return true;
		}
		else if ( ( XInKey->trgR != CXInput::INPUT_TRIGGER_MAX ) && ( Player->PushKey[enPushKey::RT] == true ) ) {
			Player->PushKey[enPushKey::RT] = false;
		}
		if ( ( XInKey->trgL >= CXInput::INPUT_TRIGGER_MAX ) && ( Player->PushKey[enPushKey::LT] == false ) ) {
			Player->PushKey[enPushKey::LT] = true;
			return true;
		}
		else if ( ( XInKey->trgR != CXInput::INPUT_TRIGGER_MAX ) && ( Player->PushKey[enPushKey::RT] == true ) ) {
			Player->PushKey[enPushKey::LT] = false;
		}
	}
	return false;
}