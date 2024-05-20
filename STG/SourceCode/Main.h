#pragma once

#include <Windows.h>
#include <time.h>	//time()関数を使用するために必要.
#include <stdio.h>	//敵の操作で使用.
#include <math.h>	//敵の操作.
#include "CXInput.h"

//ﾗｲﾌﾞﾗﾘ読み込み.
#pragma comment(lib,"winmm.lib")	//音声再生、およびｽﾚｯﾄﾞで使用する.
#pragma comment(lib,"msimg32.lib")	//指定色を透過して描画で使用する.
#pragma warning(disable : 4996)		//ファイルの入出力で使用する.

//定数宣言.
#define APP_NAME	"コピペシューティング"	//ｱﾌﾟﾘｹｰｼｮﾝ名[Copy And Paste Shooting].
#define WND_TITLE	"コピペシューティング"	//ｳｨﾝﾄﾞｳﾀｲﾄﾙ.
#define FPS			50						//FPS.
#define WND_W		1280					//ｳｨﾝﾄﾞｳの幅.
#define WND_H		720						//ｳｨﾝﾄﾞｳの高さ.
#define GWND_W		720						//ゲーム画面の幅.
#define GWND_H		720						//ゲーム画面の高さ.
#define PI			3.1415926535			//円周率.

/*UI関係.*/
#define ABLT_W		256						//能力説明の幅.
#define ABLT_H		128						//能力説明の高さ.
#define	ABLT_SIZE	64						//能力の幅高さ.
#define ABLT_MAX	4						//能力の所持個数.
#define NUM_W		24						//数字の幅.
#define NUM_H		32						//数字の高さ.
#define PAUSE_W		384						//ポーズ画像の幅.
#define PAUSE_H		192						//ポーズ画像の高さ.

/*タイトル.*/
#define ENG_H		40						//英語の高さ(幅は個別に入力).
#define TITLE_H		128						//タイトルのスライムがいる地面の画像の高さ
#define LOGO_W		960						//ロゴの幅.
#define LOGO_H		400						//ロゴの高さ.
#define LEAFMAX		100						//画面に出せる葉っぱの数.

/*エンディング*/
#define STARMAX		20						//画面に出せる星の数.
#define DROPMAX		3						//画面に出せる雫の数.
#define ENGMAX		37						//スタッフロールの数.

/*ステージギミック.*/
#define W_HITMAX	100						//木のダメージ時の葉っぱの最大数.
#define P_MAX		5						//毒沼のぶくぶくアニメーションの最大数.
#define G_TYPE		4						//ステージギミックの種類.
#define G_PTN		2						//ステージギミックのデザインのパターン.
#define	G_MAX		50						//ステージに出せるステージギミックの量.

/*キャラ.*/
#define C_CORR		( WND_W - GWND_W ) / 2	//画面内のキャラ座標の補正値.
#define C_SIZE		64						//キャラクタの幅高さ.
#define BARRMAX		7						//バリアの連結個数.
#define CHARGEMAX	80						//溜めのMAXカウント.
#define HOMINGMAX	60						//追尾のMAXカウント.
#define INVITIME	10						//無敵時間.
#define ABLT_TYPE	11						//能力の種類.
#define DISP_STA	5						//表示ステータスの個数.
#define H_MAX		3						//ヒールアニメーションの最大数.

/*弾*/
#define S_DIRMAX	10						//弾数の最大数.
#define S_NUMMAX	5						//弾方向の最大数.
#define S_SIZE		16						//弾のサイズ.
#define S_TIME		15						//弾の待ち時間.
#define BEAMMAX		4						//ビームの最大ヒット数( ÷2 ).

/*プレイヤー.*/
#define P_HP_W		256						//HPバーの幅(旧).
#define P_HP_H		32						//HPバーの高さ(旧).
#define PHP_W		288						//プレイヤーのHPバー幅.
#define PHP_H		45						//プレイヤーのHPバー高さ.
#define PS_SPD		10						//プレイヤーの弾の初期速度.
#define PH_SIZE	C_SIZE / 4					//キャラの当たり判定サイズ.
#define PH_CORR ( C_SIZE / 8 ) * 3			//プレイヤー当たり判定のXY座標の補正値.
#define PINVITIME	INVITIME * 4			//無敵時間.

/*敵*/
#define E_TYPE		5						//敵の種類.
#define E_PTN		6						//敵のパターン.
#define E_MAX		10						//敵の出現数.
#define ES_SPD		10						//敵の弾の初期速度.

/*ボス*/
#define ROCK_NUM	10						//岩の出せる数.
#define ROCK_DIR	25						//岩の出せる方向.
#define BEDISPMAX	11						//ボスエリアの敵の出現データ数.
#define BECNTMAX	550						//ボスエリアの敵の表示カウント上限.
#define BLOW_H		40						//吹き飛ばし画像の高さ.

/*マップ*/
#define MAP_W		22						//マップの幅.
#define MAP_H		120						//マップの高さ.

//-----------------------------.
//	マクロ.
//-----------------------------.
#define RAD( p )	( p ) * PI / 180	//度数からラジアンに変換.
#define DO( p )		( p ) * 180 / PI	//ラジアンから度数に変換.

//-----------------------------.
//	列挙体宣言.
//-----------------------------.
//ｷｬﾗｸﾀ状態列挙体.
enum enCharaState
{
	Wait = 0,	//待機.
	Living,		//生存中.
	Dying,		//死亡中.
	Dead,		//死亡.
	Invi,		//無敵.

	Anima = -1,	//アニメーション状態(ボスの登場アニメーションで使用).
};
//ステージギミック状態列挙体.
enum enGimmickState
{
	GWait = 0,	//待機.
	GLiving,	//生存中.
	GDead,		//死亡.
	GInvi,		//無敵.
};
//ｼｰﾝ列挙体.
enum enScene
{
	Title,		//ﾀｲﾄﾙ.
	GameMain,	//ｹﾞｰﾑﾒｲﾝ.
	GameOver,	//ｹﾞｰﾑｵｰﾊﾞｰ.
	Ending,		//ｴﾝﾃﾞｨﾝｸﾞ.
};

//移動状態列挙体.
enum enEmRanAng {
	STOP,
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

//ボスの部位列挙体.
enum enBoss
{
	Main,	//本体.
	RHand,	//右手.
	LHand	//左手.
};

//ボスの攻撃パターン列挙隊.
enum enBossMoveNo
{
	EarlyForm1,	//パターン１.
	MidForm2,	//パターン２.
	LateForm3,	//パターン３.
	Milli		//パターン４.
};

//ボスの攻撃列挙体.
enum enBossAttackNo
{
	BPunchAng	= 1,	//手の角度を合わせる.
	BPunchInit,			//パンチ初期動作.
	BPunch,				//パンチ中.
	BPunchReturn,		//手回収.
	BPunchEnd,			//手の角度を戻す.

	BUpRockUp	= 10,	//突き上げ岩初期動作( 手UP   ).
	BUpRockDown,		//突き上げ岩初期動作( 手DOWN ).
	BUpRock,			//突き上げ岩中.
	BUpRockEnd,			//突き上げ岩壊す.

	BRockUp		= 20,	//岩出現初期動作( 手UP   ).
	BRockDown,			//岩出現初期動作( 手DOWN ).
	BRock,				//岩出現中.

	BUpHand = 30,		//手を上げる.
	BDownHand,			//手を下げる.

	BWait = -1,			//待機.
};

//フェード列挙体.
enum enFade
{
	in,		//フェードイン.
	out,	//フェードアウト.

	None = -1,	//無し.
};

//能力列挙体.
enum enAbility
{
	beam = 1,	//[特殊]ビーム.
	pene,		//[特殊]貫通.
	chg,		//[特殊]溜め.
	homing,		//[特殊]追尾.
	dirUP,		//[能力]弾方向＋.
	numUP,		//[能力]弾数＋.
	hpUP,		//[能力]ＨＰ＋.
	barrUP,		//[能力]バリア＋.
	atkUP,		//[能力]ダメージ＋.
	cspdUP,		//[能力]移動速度＋.
	sspdUP,		//[能力]弾速度＋
};

//スライム見た目列挙体.
enum enSlime
{
	SWolf,
	SGoblin,
	SBat,
	SBee,
	SPlant,

	SNormal = -1, 
};

//英語列挙体.
enum enENG_No
{
	Damage,
	CharaSpd,
	ShotSpd,
	ShotDir,
	ShotNum,
	Charge,
	Homing,
	Barrier,
	Pane,
	Beam,
	Heel,
	Hp,

	max,
};

//Keyの状態配列列挙体.
enum enPushKey
{
	Space,
	A,
	S,
	D,
	F,
	kUP,
	kDOWN,
	LT,
	RT,

	Max
};

//HPフレームのスライムNo.
enum enHPFrame
{
	HPHigh,		//HP多い.
	HPMiddle,	//HP真ん中.
	HPLow,		//HP少ない.
};

//単色画像の描画No.
enum enBltNo
{
	Bit,
	Trans,
	Blend,
};

//プレイヤーの死亡No.
enum enDeedNo
{
	Plant,
	Wolf,
	Goblin,
	Bat,
	Bee,
	Gimmick,
	Golem,

	DeedNone = 14
};

//動作列挙体.
enum enAction
{
	AcWait = 0,	//待機.
	MoveRight,	//右移動.
	MoveLeft,	//左移動.

	AcNone = -1,	//未設定.
};

//敵(花)の状態.
enum enPlantState
{
	StayUp,		//起床中.
	WokeUp,		//起きた.
	Sleeping,	//寝ている.
};
//-----------------------------.
//	構造体宣言.
//-----------------------------.
typedef struct _GameWindow
{
	HWND	hWnd;		//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	HDC		hScreenDC;	//ﾊﾞｯｸﾊﾞｯﾌｧ.
	SIZE	size;		//ｻｲｽﾞ.
	DWORD	dwFPS;		//FPS(Frame Per Second: ﾌﾚｰﾑ / 秒).
	BOOL	isWndClose;	//ｳｨﾝﾄﾞｳが閉じているか確認用.
} GameWindow;

//背景スクロール.
struct SCROLL
{
	double	y;				//スクロール.
	double	spd;			//スクロール速度.
	bool	stop;			//スクロールを停止.
	double	BossDAng;		//ボスの出現演出でのY座標揺れの角度.
	double	BossDispY;		//ボスの出現演出でのY座標揺れ.
	int		BossDispYCnt;	//ボスの出現演出でのY座標揺れカウント.
	bool	Start;			//ゲームスタート時のスライム移動.
};

//x,y座標.
struct XY
{
	double	x;
	double	y;
};

//ｷｬﾗｸﾀ構造体.
struct CHARA
{
	double	x;					//x座標.
	double	y;					//y座標.
	double	ang;				//キャラクタの向き.
	double	chgCnt;				//溜めカウント.
	int		healCnt;			//回復カウント.
	int		State;				//状態.
	int		ExpAnimCnt;			//爆発ｱﾆﾒｰｼｮﾝｶｳﾝﾀ.
	int		inviTime;			//無敵時間.
	int		abArea[ABLT_MAX];	//能力エリア.
	int		ShotDirAng;			//弾同士の間の間隔.
};

//アニメーション.
struct ANIMATION
{
	int		No;		//アニメーションNo.
	int		max;	//アニメーションの最大値.
	int		cnt;	//アニメーションカウント.
	int		cntMax;	//アニメーション切り替え数.
	bool	Flag;	//アニメーション切り替えフラグ.
};

//敵の表示データ.
struct DISPDATA
{
	int		type;
	int		ptn;
	int		eNo;
	int		DispNum;
};
//弾構造体.
struct SHOT
{
	double	x;				//x座標.
	double	y;				//y座標.
	double	ang;			//飛ぶ方向.
	bool	DispShotFlag;	//発射フラグ.
	int		sspd;			//弾速度.
	int		homingCnt;		//追尾カウント.
	int		beamCnt;		//ビームのヒット回数.
};

//ステータス構造体.
struct STATUS
{
	int		hp;		//HP.
	int		atk;	//ダメージ.
	double  cspd;	//移動速度.
	int		dir;	//弾方向.
	int		num;	//弾数.
};

//能力構造体.
struct ABILITY
{
	int		beam;	//[特殊]ビーム.
	int		pene;	//[特殊]貫通.
	int		chg;	//[特殊]溜め.
	int		homing;	//[特殊]追尾.
	int		dirUP;	//[能力]弾方向＋.
	int		numUP;	//[能力]弾数＋.
	int		hpUP;	//[能力]ＨＰ＋.
	int		barrUP;	//[能力]バリア＋.
	int		atkUP;	//[能力]ダメージ＋.
	int		cspdUP;	//[能力]移動速度＋.
	int		sspdUP;	//[能力]弾速度＋
};

//半円構造体.
struct HALF_ROTATION
{
	double	E_height;
	double	M_x;
	double	M_y;
};

//ボス岩構造体.
struct ROCK
{
	double	x;
	double	y;
	double	Rang;
	int		cnt;
	bool	DispFlag;
};

//ボス岩の数データ.
struct ROCKDATA
{
	int		Num;
	int		Dir;
	int		Ang;
	int		Spaa;
	int		Time;
	int		iAng;
};

//ボスの吹き飛ばし攻撃.
struct BLOWAWAY
{
	int		y;
	bool	DispFlag;
};

//ボスエリアの敵出現.
struct BEDISP
{
	int		DispCnt;
	int		CntData[BEDISPMAX];
	int		ENoData[BEDISPMAX];
	int		DispData[BEDISPMAX];
};

//バリア(回転運動).
struct BARRIER
{
	double	x;			//バリアx座標.
	double	y;			//バリアy座標.
	double	C_x;		//中心のx座標.
	double	C_y;		//中心のy座標.
	double	length_x;	//x座標の直径.
	double	length_y;	//y座標の直径.
	double	ang;		//方向.
};

//ターゲット構造体.
struct TARGET
{
	double	x;
	double	y;
	int		ty;
	int		ptn;
	int		eNo;
};

//能力削除アニメーション.
struct DGABI
{
	XY		xy;			//座標.
	int		AbiNo;		//能力No.
	bool	DispFlag;	//表示フラグ.
	bool	ExpaFlag;	//拡大フラグ.
	float	Expa;		//拡大率.
	int		Alpha;		//アルファ値.
};

//プレイヤーのダメージ印.
struct DAMAGE
{
	double	x;
	double	y;
	bool	DispFlag;
};

//タイトルの葉っぱ.
struct LEAF
{
	int		x;
	int		y;
	int		PtnX;
	int		PtnY;
	int		Speed;
	int		FallPower;
	double	Ang;
	bool	DispFlag;
};

//回復エフェクト.
struct HEELANIMA
{
	double		x;
	double		y;
	int			UpY;
	int			PtnX;
	int			PtnY;
	int			Alpha;
	bool		DispFlag;
	ANIMATION	animation;
};

//エンディングのスライム.
struct SLIME
{
	int			x;
	int			y;
	int			GroundPos;
	int			MoveSpeed;
	int			Action;
	bool		Jumping;	//ジャンプ中.
	float		JumpAcc;	//ジャンプ減速用.
	float		JumpPower;	//ジャンプ力(定数でも可).
	float		Gravity;	//重力(定数でも可).
	ANIMATION	animation;
};

//エンディングの星.
struct STAR
{
	int		x;
	int		y;
	int		Ptn;
	int		Alpha;
	int		AlSpd;
	bool	DispFlag;

	bool	Shooting;
	int		Spd;
};

//エンディングのスライムの雫.
struct DROP
{
	int		x;
	int		y;
	int		Ptn;
	bool	DispFlag;
};

//エンディングのスタッフロール.
struct ENENG
{
	float	x;
	float	y;
	int		StartX;
	int		StartY;
	int		wDest;
	int		hDest;
	bool	DispFlag;

	bool	HitFlag;
	float	CosDAng;
	float	width;
	float	Dang;
	int		Corr;
};

//タイトル.
struct TITLE
{
	ANIMATION		Panimation;			//スライムの触角アニメーション.
	float			UPCloudX1;			//上空のスクロール1.
	float			UPCloudX2;			//上空のスクロール2.
	float			DownCloudX1;		//下空のスクロール3.
	float			DownCloudX2;		//下空のスクロール4.

	LEAF			Leaf[LEAFMAX];		//タイトルの葉っぱ.
	int				LCosAng;			//タイトルの葉っぱのCOS用角度(風).
	int				LDispMAX;			//葉っぱの出現数.
	int				LTime;				//次の葉っぱの出現待ち時間.
	int				LTimeCnt;			//次の葉っぱの出現待ち時間カウント.

	int				Alpha;				//アルファ値.
};

//エンディング.
struct ENDING
{
	SLIME			Slime;
	STAR			Star[STARMAX];
	int				SDispMAX;
	int				STime;
	int				STimeCnt;

	DROP			Drop[DROPMAX];
	int				StartX[3];
	int				StartY[3];
	int				wDest [3];
	int				hDest [3];
	int				DTime;
	int				DTimeCnt;
	int				StopCnt;
	int				ClearY;

	ENENG			EnEng[ENGMAX];
	int				DispNo;
	int				DispTime;
	int				DispCnt;
	int				Space;
	bool			Start;

	int				Hit;
	int				BestHit;
};

//プレイヤー構造体.
struct PLAYER
{
	CHARA			chara;
	ANIMATION		animation;
	STATUS			status;
	STATUS			initial_status;
	ABILITY			ability;
	BARRIER			barrier[( ABLT_MAX - 1 ) * BARRMAX];
	SHOT			shot[S_NUMMAX][S_DIRMAX];
	TARGET			target;
	DGABI			DGAbi[ABLT_MAX];
	HEELANIMA		HeelAnima[H_MAX];
	DAMAGE			Damage[2];
	bool			AbGetFlag;
	int				GetAnimaCnt;
	int				GetAnimaX;
	int				GetAnimaY;
	int				SlimeNo;
	int				NewSlimeNo;
	bool			NumShotFlag[S_NUMMAX];
	bool			KeyStop;
	int				shotCnt;
	bool			PushKey[enPushKey::Max];
	int				chgAniCnt;
	bool			chgAniFlag;
	int				chgNumCnt;
	int				XInArea;
	int				HPFrameNo;
	int				DeadNo;
};

//敵構造体.
struct ENEMY
{
	CHARA			chara;
	ANIMATION		animetion;
	STATUS			status;
	STATUS			initial_status;
	ABILITY			ability;
	BARRIER			barrier[( ABLT_MAX - 1 ) * BARRMAX];
	SHOT			shot[S_NUMMAX][S_DIRMAX];
	TARGET			target;
	HALF_ROTATION	half_rot;
	HEELANIMA		HeelAnima[H_MAX];
	bool			NumShotFlag[S_NUMMAX];
	int				ran;		//敵の動き.
	int				attack;		//ボス敵の攻撃.
	int				EmRanCnt;	//ranｶｳﾝﾄ.
	int				EshotCnt;	//ｼｮｯﾄｶｳﾝﾄ.
	int				EmMoveCnt;	//移動ｶｳﾝﾄ.
	bool			appFlag;	//出現ﾌﾗｸﾞ.
	bool			BossFlag;	//ﾎﾞｽﾌﾗｸﾞ.
	double			LBE_ang;
	int				EGetAbNo;
};

//敵(花)の睡眠エフェクト.
struct SLEEP
{
	double			x;
	double			y;
	int				ptnNo;
	int				Alpha;
	bool			DispFlag;
};

//敵(花)の起床エフェクト.
struct WOKE
{
	double			x;
	double			y;
	int				Alpha;
	int				AnimaX;
	int				AnimaY;
	int				AnimaCnt;
	bool			AlphaFlag;
};

//敵(花).
struct PLANT
{
	SLEEP			Sleep[2];	// 2 : [ z Z ]の表示数.
	WOKE			Woke;
	int				State;

};

//ボス.
struct BOSS
{
	const static int BOSS_TYPE	= 3;	//0:本体,1:右腕,2:左腕.
	const static int BS_NUM		= 20;	//ボスの弾の弾数上限.
	const static int BS_DIR		= 25;	//ボスの弾の方向上限.

	CHARA			chara[BOSS_TYPE];
	XY				initial_xy[BOSS_TYPE];
	int				charaSize[BOSS_TYPE];
	int				AttackNo[BOSS_TYPE];
	double			PunchRAng[BOSS_TYPE];
	ANIMATION		animation[BOSS_TYPE];
	ANIMATION		DispAnima;
	STATUS			status[BOSS_TYPE];
	STATUS			initial_status[BOSS_TYPE];
	ABILITY			ability[BOSS_TYPE];
	SHOT			shot[BS_NUM][BS_DIR];
	bool			NumShotFlag[BS_NUM];
	ROCK			rock[BOSS_TYPE][ROCK_NUM][ROCK_DIR];
	ROCKDATA		rockData[BOSS_TYPE];
	double			RockRAngData[ROCK_NUM][ROCK_DIR];
	int				RockDispNoData[ROCK_NUM][ROCK_DIR];
	BEDISP			BEDisp;
	BLOWAWAY		BlowAway;
	int				shotCnt;
	int				PatternCnt;
	int				AttackCnt;
	int				AttackMove;
	int				AttackMovecnt;
	int				Blowcnt;
	bool			DispFlag;
};

//ステージギミック.
struct GIMMICK
{
	double	x;			//x座標.
	double	y;			//y座標.
	int		hp;			//体力.
	int		State;		//状態.
	int		hSize;		//高さサイズ.
	int		wSize;		//幅サイズ.
	int		inviTime;	//無敵時間.
	int		TreeAlpha;	//葉っぱのアルファ値.
	bool	RockFlag;	//岩の頭表示フラグ.
};

//ステージギミック(毒沼)のぶくぶくアニメーション.
struct POISON
{
	float	x;
	float	y;
	float	Spd;
	int		PtnNo;
	int		Alpha;
	int		AlSpd;
	bool	DispFlag;
};

//マップ.
struct MAP
{
	double	cnt;				//表示待ちカウント.
	int		dispCnt;			//データ取得場所カウント.
	int		Data[MAP_H][MAP_W];	//データを全て保存.
	int		LastNo;				//前回のMapNo.
};

//フェード.
struct FADE
{
	int		FadeState;
	int		FadeAlpha;
	enScene EndScene;
	int		EndAlpha;
	bool	InOutFlag;
};

//コントローラのスティックとトリガー.
struct XInStTrg
{
	int stL_Y;		//LスティックのX座標.
	int stL_X;		//LスティックのY座標.
	int stR_Y;		//RスティックのX座標.
	int stR_X;		//RスティックのY座標.
	int trgL;		//Lトリガーが押されたか.
	int trgR;		//Rトリガーが押されたか.
};
//-----------------------------.
//	関数のﾌﾟﾛﾄﾀｲﾌﾟ宣言.
//-----------------------------.
//画像の読み込み関数.
//戻り値の型　関数名( 第一引数、　第二引数 ).
bool LoadBmp( HBITMAP* phBmp, const char* fileName );
//単色画像を表示する関数.
void ColorBlt(
	int BltNo,	  HDC hdcDest,  int xDest, int yDest, int wDest, int hDest,
	HDC hdcColor, COLORREF RGB, int Alpha = 255 );
//画像を透過して表示する関数.
bool TransBlt(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc );
//拡大した画像を透過して表示する関数.
bool ExpaTransBlt(
	HDC hdcDest, int xDest, int yDest, int wDest,  int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc,  int weDest, int heDest, bool xyFlag = true );
//画像を半透明にして表示する関数.
bool AlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc,  int Alpha );
//回転した画像を透過して表示する関数.
bool TransPlgBlt(
	HDC hdcDest, LONG xDest, LONG yDest, LONG wDest, LONG hDest,
	HDC hdcSrc,  LONG xSrc,  LONG ySrc,  DOUBLE Ang, HDC hdcWork );
//回転した画像を透過して半透明にする関数.
bool TransPlgAlBlend(
	HDC hdcDest, LONG xDest, LONG yDest, LONG wDest, LONG hDest,
	HDC hdcSrc,  LONG xSrc,  LONG ySrc,  DOUBLE Ang, HDC hdcPlg, int Alpha, HDC hdcWork );
//画像を透過して半透明にする.
bool TransAlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc,  int Alpha, HDC hdcWork );
//拡大した画像を透過して半透明にする.
bool ExpaTransAlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc,  int Alpha, HDC hdcWork, int weDest, int heDest, bool xyFlag = true );

//矩形同士の衝突判定.
bool CollisionDetection(
	double Ax, double Ay, int Aw, int Ah,	//矩形Aのx,y座標と幅と高さ.
	double Bx, double By, int Bw, int Bh );//矩形Bのx,y座標と幅と高さ.

//マップ.
bool GetMap( MAP* map );
void CntMap( SCROLL* scroll, MAP* map, ENEMY enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], BOSS* boss, PLAYER* player );
void DispMap( MAP* map, ENEMY enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], BOSS* boss, SCROLL* scroll, PLAYER* player );
//ギミック.
void Gimmick_Ran( GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], SCROLL scroll, PLAYER* player );

//プレイヤーの能力の更新.
void P_UpdateAblt( PLAYER* playerData );
//敵の能力の更新.
void E_UpdateAblt( ENEMY enemey[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo );
//能力削除アニメーション.
void DeleteAblt( PLAYER* player );
//プレイヤーのバリア、回復の制御.
void P_Barr( PLAYER* Player );
//敵のバリア、回復の制御.
void E_Barr( ENEMY enemy[E_TYPE][E_PTN][E_MAX] );
//プレイヤーの回復.
void P_heal( PLAYER* Player );
//敵の回復.
void E_heal( ENEMY enemy[E_TYPE][E_PTN][E_MAX] );

//プレイヤーの移動.
void PlayerOp( PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], CXInput* pXInput, XInStTrg XInKey, SCROLL* scroll, enScene Scene, ENDING Ending, bool* pause, FADE* Fade );

//弾の発射.
void P_ShotFlag( PLAYER* playerData, enScene Scene, ENDING Ending );
void E_ShotFlag( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnN, int eNo, double Rang = 0, bool angSkip = false );
//弾の動作.
void P_ShotControl( PLAYER* playerData, BOSS* boss, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], enScene Scene );
void E_ShotControl( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, PLAYER playerData );
//弾関係の当たり判定.
void S_CollisionDetection( PLAYER* PlayerData, ENEMY EnemyData[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss );
//敵ステータス.
bool E_Status( ENEMY enemydata[E_TYPE][E_PTN][E_MAX] );
//アビリティ.
bool E_Ability( ENEMY enemydata[E_TYPE][E_PTN][E_MAX] );

//移動パターン.
void E_ran( SCROLL scroll, PLAYER p_k, ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int ty, int ptn, int eNo );
//エリアボス移動パターン.
void LBE_ran( SCROLL scroll, PLAYER p_k, ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int ty );

//ボスの攻撃動作.
void B_AttackControl( BOSS* boss, int typeNo, SCROLL* scroll, PLAYER* player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX] );
//ボスの攻撃パターン.
void B_AttackMove( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX] );

//ボスの出現アニメーション.
void DispAnimation( BOSS* boss, SCROLL* scroll, PLAYER* player, CXInput* pXInput );
//ボスの死亡アニメーション.
bool DeadAnimation( BOSS* boss, SCROLL* scroll, PLAYER* player, CXInput* pXInput );

//ボスエリアの敵の表示のデータ取得.
bool GetBEDispData( BOSS* boss );

//コントローラーまたはキーボードのどこかが押された.
bool GetAsyncAnyKey( PLAYER* Player, CXInput* pXInput, XInStTrg* XInKey );
