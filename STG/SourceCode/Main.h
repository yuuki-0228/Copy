#pragma once

#include <Windows.h>
#include <time.h>	//time()�֐����g�p���邽�߂ɕK�v.
#include <stdio.h>	//�G�̑���Ŏg�p.
#include <math.h>	//�G�̑���.
#include "CXInput.h"

//ײ���ؓǂݍ���.
#pragma comment(lib,"winmm.lib")	//�����Đ��A����ѽگ�ނŎg�p����.
#pragma comment(lib,"msimg32.lib")	//�w��F�𓧉߂��ĕ`��Ŏg�p����.
#pragma warning(disable : 4996)		//�t�@�C���̓��o�͂Ŏg�p����.

//�萔�錾.
#define APP_NAME	"�R�s�y�V���[�e�B���O"	//���ع���ݖ�[Copy And Paste Shooting].
#define WND_TITLE	"�R�s�y�V���[�e�B���O"	//����޳����.
#define FPS			50						//FPS.
#define WND_W		1280					//����޳�̕�.
#define WND_H		720						//����޳�̍���.
#define GWND_W		720						//�Q�[����ʂ̕�.
#define GWND_H		720						//�Q�[����ʂ̍���.
#define PI			3.1415926535			//�~����.

/*UI�֌W.*/
#define ABLT_W		256						//�\�͐����̕�.
#define ABLT_H		128						//�\�͐����̍���.
#define	ABLT_SIZE	64						//�\�͂̕�����.
#define ABLT_MAX	4						//�\�͂̏�����.
#define NUM_W		24						//�����̕�.
#define NUM_H		32						//�����̍���.
#define PAUSE_W		384						//�|�[�Y�摜�̕�.
#define PAUSE_H		192						//�|�[�Y�摜�̍���.

/*�^�C�g��.*/
#define ENG_H		40						//�p��̍���(���͌ʂɓ���).
#define TITLE_H		128						//�^�C�g���̃X���C��������n�ʂ̉摜�̍���
#define LOGO_W		960						//���S�̕�.
#define LOGO_H		400						//���S�̍���.
#define LEAFMAX		100						//��ʂɏo����t���ς̐�.

/*�G���f�B���O*/
#define STARMAX		20						//��ʂɏo���鐯�̐�.
#define DROPMAX		3						//��ʂɏo���鎴�̐�.
#define ENGMAX		37						//�X�^�b�t���[���̐�.

/*�X�e�[�W�M�~�b�N.*/
#define W_HITMAX	100						//�؂̃_���[�W���̗t���ς̍ő吔.
#define P_MAX		5						//�ŏ��̂Ԃ��Ԃ��A�j���[�V�����̍ő吔.
#define G_TYPE		4						//�X�e�[�W�M�~�b�N�̎��.
#define G_PTN		2						//�X�e�[�W�M�~�b�N�̃f�U�C���̃p�^�[��.
#define	G_MAX		50						//�X�e�[�W�ɏo����X�e�[�W�M�~�b�N�̗�.

/*�L����.*/
#define C_CORR		( WND_W - GWND_W ) / 2	//��ʓ��̃L�������W�̕␳�l.
#define C_SIZE		64						//�L�����N�^�̕�����.
#define BARRMAX		7						//�o���A�̘A����.
#define CHARGEMAX	80						//���߂�MAX�J�E���g.
#define HOMINGMAX	60						//�ǔ���MAX�J�E���g.
#define INVITIME	10						//���G����.
#define ABLT_TYPE	11						//�\�͂̎��.
#define DISP_STA	5						//�\���X�e�[�^�X�̌�.
#define H_MAX		3						//�q�[���A�j���[�V�����̍ő吔.

/*�e*/
#define S_DIRMAX	10						//�e���̍ő吔.
#define S_NUMMAX	5						//�e�����̍ő吔.
#define S_SIZE		16						//�e�̃T�C�Y.
#define S_TIME		15						//�e�̑҂�����.
#define BEAMMAX		4						//�r�[���̍ő�q�b�g��( ��2 ).

/*�v���C���[.*/
#define P_HP_W		256						//HP�o�[�̕�(��).
#define P_HP_H		32						//HP�o�[�̍���(��).
#define PHP_W		288						//�v���C���[��HP�o�[��.
#define PHP_H		45						//�v���C���[��HP�o�[����.
#define PS_SPD		10						//�v���C���[�̒e�̏������x.
#define PH_SIZE	C_SIZE / 4					//�L�����̓����蔻��T�C�Y.
#define PH_CORR ( C_SIZE / 8 ) * 3			//�v���C���[�����蔻���XY���W�̕␳�l.
#define PINVITIME	INVITIME * 4			//���G����.

/*�G*/
#define E_TYPE		5						//�G�̎��.
#define E_PTN		6						//�G�̃p�^�[��.
#define E_MAX		10						//�G�̏o����.
#define ES_SPD		10						//�G�̒e�̏������x.

/*�{�X*/
#define ROCK_NUM	10						//��̏o���鐔.
#define ROCK_DIR	25						//��̏o�������.
#define BEDISPMAX	11						//�{�X�G���A�̓G�̏o���f�[�^��.
#define BECNTMAX	550						//�{�X�G���A�̓G�̕\���J�E���g���.
#define BLOW_H		40						//������΂��摜�̍���.

/*�}�b�v*/
#define MAP_W		22						//�}�b�v�̕�.
#define MAP_H		120						//�}�b�v�̍���.

//-----------------------------.
//	�}�N��.
//-----------------------------.
#define RAD( p )	( p ) * PI / 180	//�x�����烉�W�A���ɕϊ�.
#define DO( p )		( p ) * 180 / PI	//���W�A������x���ɕϊ�.

//-----------------------------.
//	�񋓑̐錾.
//-----------------------------.
//��׸���ԗ񋓑�.
enum enCharaState
{
	Wait = 0,	//�ҋ@.
	Living,		//������.
	Dying,		//���S��.
	Dead,		//���S.
	Invi,		//���G.

	Anima = -1,	//�A�j���[�V�������(�{�X�̓o��A�j���[�V�����Ŏg�p).
};
//�X�e�[�W�M�~�b�N��ԗ񋓑�.
enum enGimmickState
{
	GWait = 0,	//�ҋ@.
	GLiving,	//������.
	GDead,		//���S.
	GInvi,		//���G.
};
//��ݗ񋓑�.
enum enScene
{
	Title,		//����.
	GameMain,	//�ް�Ҳ�.
	GameOver,	//�ްѵ��ް.
	Ending,		//���ިݸ�.
};

//�ړ���ԗ񋓑�.
enum enEmRanAng {
	STOP,
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

//�{�X�̕��ʗ񋓑�.
enum enBoss
{
	Main,	//�{��.
	RHand,	//�E��.
	LHand	//����.
};

//�{�X�̍U���p�^�[���񋓑�.
enum enBossMoveNo
{
	EarlyForm1,	//�p�^�[���P.
	MidForm2,	//�p�^�[���Q.
	LateForm3,	//�p�^�[���R.
	Milli		//�p�^�[���S.
};

//�{�X�̍U���񋓑�.
enum enBossAttackNo
{
	BPunchAng	= 1,	//��̊p�x�����킹��.
	BPunchInit,			//�p���`��������.
	BPunch,				//�p���`��.
	BPunchReturn,		//����.
	BPunchEnd,			//��̊p�x��߂�.

	BUpRockUp	= 10,	//�˂��グ�⏉������( ��UP   ).
	BUpRockDown,		//�˂��グ�⏉������( ��DOWN ).
	BUpRock,			//�˂��グ�⒆.
	BUpRockEnd,			//�˂��グ���.

	BRockUp		= 20,	//��o����������( ��UP   ).
	BRockDown,			//��o����������( ��DOWN ).
	BRock,				//��o����.

	BUpHand = 30,		//����グ��.
	BDownHand,			//���������.

	BWait = -1,			//�ҋ@.
};

//�t�F�[�h�񋓑�.
enum enFade
{
	in,		//�t�F�[�h�C��.
	out,	//�t�F�[�h�A�E�g.

	None = -1,	//����.
};

//�\�͗񋓑�.
enum enAbility
{
	beam = 1,	//[����]�r�[��.
	pene,		//[����]�ђ�.
	chg,		//[����]����.
	homing,		//[����]�ǔ�.
	dirUP,		//[�\��]�e�����{.
	numUP,		//[�\��]�e���{.
	hpUP,		//[�\��]�g�o�{.
	barrUP,		//[�\��]�o���A�{.
	atkUP,		//[�\��]�_���[�W�{.
	cspdUP,		//[�\��]�ړ����x�{.
	sspdUP,		//[�\��]�e���x�{
};

//�X���C�������ڗ񋓑�.
enum enSlime
{
	SWolf,
	SGoblin,
	SBat,
	SBee,
	SPlant,

	SNormal = -1, 
};

//�p��񋓑�.
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

//Key�̏�Ԕz��񋓑�.
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

//HP�t���[���̃X���C��No.
enum enHPFrame
{
	HPHigh,		//HP����.
	HPMiddle,	//HP�^��.
	HPLow,		//HP���Ȃ�.
};

//�P�F�摜�̕`��No.
enum enBltNo
{
	Bit,
	Trans,
	Blend,
};

//�v���C���[�̎��SNo.
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

//����񋓑�.
enum enAction
{
	AcWait = 0,	//�ҋ@.
	MoveRight,	//�E�ړ�.
	MoveLeft,	//���ړ�.

	AcNone = -1,	//���ݒ�.
};

//�G(��)�̏��.
enum enPlantState
{
	StayUp,		//�N����.
	WokeUp,		//�N����.
	Sleeping,	//�Q�Ă���.
};
//-----------------------------.
//	�\���̐錾.
//-----------------------------.
typedef struct _GameWindow
{
	HWND	hWnd;		//����޳�����.
	HDC		hScreenDC;	//�ޯ��ޯ̧.
	SIZE	size;		//����.
	DWORD	dwFPS;		//FPS(Frame Per Second: �ڰ� / �b).
	BOOL	isWndClose;	//����޳�����Ă��邩�m�F�p.
} GameWindow;

//�w�i�X�N���[��.
struct SCROLL
{
	double	y;				//�X�N���[��.
	double	spd;			//�X�N���[�����x.
	bool	stop;			//�X�N���[�����~.
	double	BossDAng;		//�{�X�̏o�����o�ł�Y���W�h��̊p�x.
	double	BossDispY;		//�{�X�̏o�����o�ł�Y���W�h��.
	int		BossDispYCnt;	//�{�X�̏o�����o�ł�Y���W�h��J�E���g.
	bool	Start;			//�Q�[���X�^�[�g���̃X���C���ړ�.
};

//x,y���W.
struct XY
{
	double	x;
	double	y;
};

//��׸��\����.
struct CHARA
{
	double	x;					//x���W.
	double	y;					//y���W.
	double	ang;				//�L�����N�^�̌���.
	double	chgCnt;				//���߃J�E���g.
	int		healCnt;			//�񕜃J�E���g.
	int		State;				//���.
	int		ExpAnimCnt;			//������Ұ��ݶ���.
	int		inviTime;			//���G����.
	int		abArea[ABLT_MAX];	//�\�̓G���A.
	int		ShotDirAng;			//�e���m�̊Ԃ̊Ԋu.
};

//�A�j���[�V����.
struct ANIMATION
{
	int		No;		//�A�j���[�V����No.
	int		max;	//�A�j���[�V�����̍ő�l.
	int		cnt;	//�A�j���[�V�����J�E���g.
	int		cntMax;	//�A�j���[�V�����؂�ւ���.
	bool	Flag;	//�A�j���[�V�����؂�ւ��t���O.
};

//�G�̕\���f�[�^.
struct DISPDATA
{
	int		type;
	int		ptn;
	int		eNo;
	int		DispNum;
};
//�e�\����.
struct SHOT
{
	double	x;				//x���W.
	double	y;				//y���W.
	double	ang;			//��ԕ���.
	bool	DispShotFlag;	//���˃t���O.
	int		sspd;			//�e���x.
	int		homingCnt;		//�ǔ��J�E���g.
	int		beamCnt;		//�r�[���̃q�b�g��.
};

//�X�e�[�^�X�\����.
struct STATUS
{
	int		hp;		//HP.
	int		atk;	//�_���[�W.
	double  cspd;	//�ړ����x.
	int		dir;	//�e����.
	int		num;	//�e��.
};

//�\�͍\����.
struct ABILITY
{
	int		beam;	//[����]�r�[��.
	int		pene;	//[����]�ђ�.
	int		chg;	//[����]����.
	int		homing;	//[����]�ǔ�.
	int		dirUP;	//[�\��]�e�����{.
	int		numUP;	//[�\��]�e���{.
	int		hpUP;	//[�\��]�g�o�{.
	int		barrUP;	//[�\��]�o���A�{.
	int		atkUP;	//[�\��]�_���[�W�{.
	int		cspdUP;	//[�\��]�ړ����x�{.
	int		sspdUP;	//[�\��]�e���x�{
};

//���~�\����.
struct HALF_ROTATION
{
	double	E_height;
	double	M_x;
	double	M_y;
};

//�{�X��\����.
struct ROCK
{
	double	x;
	double	y;
	double	Rang;
	int		cnt;
	bool	DispFlag;
};

//�{�X��̐��f�[�^.
struct ROCKDATA
{
	int		Num;
	int		Dir;
	int		Ang;
	int		Spaa;
	int		Time;
	int		iAng;
};

//�{�X�̐�����΂��U��.
struct BLOWAWAY
{
	int		y;
	bool	DispFlag;
};

//�{�X�G���A�̓G�o��.
struct BEDISP
{
	int		DispCnt;
	int		CntData[BEDISPMAX];
	int		ENoData[BEDISPMAX];
	int		DispData[BEDISPMAX];
};

//�o���A(��]�^��).
struct BARRIER
{
	double	x;			//�o���Ax���W.
	double	y;			//�o���Ay���W.
	double	C_x;		//���S��x���W.
	double	C_y;		//���S��y���W.
	double	length_x;	//x���W�̒��a.
	double	length_y;	//y���W�̒��a.
	double	ang;		//����.
};

//�^�[�Q�b�g�\����.
struct TARGET
{
	double	x;
	double	y;
	int		ty;
	int		ptn;
	int		eNo;
};

//�\�͍폜�A�j���[�V����.
struct DGABI
{
	XY		xy;			//���W.
	int		AbiNo;		//�\��No.
	bool	DispFlag;	//�\���t���O.
	bool	ExpaFlag;	//�g��t���O.
	float	Expa;		//�g�嗦.
	int		Alpha;		//�A���t�@�l.
};

//�v���C���[�̃_���[�W��.
struct DAMAGE
{
	double	x;
	double	y;
	bool	DispFlag;
};

//�^�C�g���̗t����.
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

//�񕜃G�t�F�N�g.
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

//�G���f�B���O�̃X���C��.
struct SLIME
{
	int			x;
	int			y;
	int			GroundPos;
	int			MoveSpeed;
	int			Action;
	bool		Jumping;	//�W�����v��.
	float		JumpAcc;	//�W�����v�����p.
	float		JumpPower;	//�W�����v��(�萔�ł���).
	float		Gravity;	//�d��(�萔�ł���).
	ANIMATION	animation;
};

//�G���f�B���O�̐�.
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

//�G���f�B���O�̃X���C���̎�.
struct DROP
{
	int		x;
	int		y;
	int		Ptn;
	bool	DispFlag;
};

//�G���f�B���O�̃X�^�b�t���[��.
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

//�^�C�g��.
struct TITLE
{
	ANIMATION		Panimation;			//�X���C���̐G�p�A�j���[�V����.
	float			UPCloudX1;			//���̃X�N���[��1.
	float			UPCloudX2;			//���̃X�N���[��2.
	float			DownCloudX1;		//����̃X�N���[��3.
	float			DownCloudX2;		//����̃X�N���[��4.

	LEAF			Leaf[LEAFMAX];		//�^�C�g���̗t����.
	int				LCosAng;			//�^�C�g���̗t���ς�COS�p�p�x(��).
	int				LDispMAX;			//�t���ς̏o����.
	int				LTime;				//���̗t���ς̏o���҂�����.
	int				LTimeCnt;			//���̗t���ς̏o���҂����ԃJ�E���g.

	int				Alpha;				//�A���t�@�l.
};

//�G���f�B���O.
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

//�v���C���[�\����.
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

//�G�\����.
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
	int				ran;		//�G�̓���.
	int				attack;		//�{�X�G�̍U��.
	int				EmRanCnt;	//ran����.
	int				EshotCnt;	//���Ķ���.
	int				EmMoveCnt;	//�ړ�����.
	bool			appFlag;	//�o���׸�.
	bool			BossFlag;	//�޽�׸�.
	double			LBE_ang;
	int				EGetAbNo;
};

//�G(��)�̐����G�t�F�N�g.
struct SLEEP
{
	double			x;
	double			y;
	int				ptnNo;
	int				Alpha;
	bool			DispFlag;
};

//�G(��)�̋N���G�t�F�N�g.
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

//�G(��).
struct PLANT
{
	SLEEP			Sleep[2];	// 2 : [ z Z ]�̕\����.
	WOKE			Woke;
	int				State;

};

//�{�X.
struct BOSS
{
	const static int BOSS_TYPE	= 3;	//0:�{��,1:�E�r,2:���r.
	const static int BS_NUM		= 20;	//�{�X�̒e�̒e�����.
	const static int BS_DIR		= 25;	//�{�X�̒e�̕������.

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

//�X�e�[�W�M�~�b�N.
struct GIMMICK
{
	double	x;			//x���W.
	double	y;			//y���W.
	int		hp;			//�̗�.
	int		State;		//���.
	int		hSize;		//�����T�C�Y.
	int		wSize;		//���T�C�Y.
	int		inviTime;	//���G����.
	int		TreeAlpha;	//�t���ς̃A���t�@�l.
	bool	RockFlag;	//��̓��\���t���O.
};

//�X�e�[�W�M�~�b�N(�ŏ�)�̂Ԃ��Ԃ��A�j���[�V����.
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

//�}�b�v.
struct MAP
{
	double	cnt;				//�\���҂��J�E���g.
	int		dispCnt;			//�f�[�^�擾�ꏊ�J�E���g.
	int		Data[MAP_H][MAP_W];	//�f�[�^��S�ĕۑ�.
	int		LastNo;				//�O���MapNo.
};

//�t�F�[�h.
struct FADE
{
	int		FadeState;
	int		FadeAlpha;
	enScene EndScene;
	int		EndAlpha;
	bool	InOutFlag;
};

//�R���g���[���̃X�e�B�b�N�ƃg���K�[.
struct XInStTrg
{
	int stL_Y;		//L�X�e�B�b�N��X���W.
	int stL_X;		//L�X�e�B�b�N��Y���W.
	int stR_Y;		//R�X�e�B�b�N��X���W.
	int stR_X;		//R�X�e�B�b�N��Y���W.
	int trgL;		//L�g���K�[�������ꂽ��.
	int trgR;		//R�g���K�[�������ꂽ��.
};
//-----------------------------.
//	�֐����������ߐ錾.
//-----------------------------.
//�摜�̓ǂݍ��݊֐�.
//�߂�l�̌^�@�֐���( �������A�@������ ).
bool LoadBmp( HBITMAP* phBmp, const char* fileName );
//�P�F�摜��\������֐�.
void ColorBlt(
	int BltNo,	  HDC hdcDest,  int xDest, int yDest, int wDest, int hDest,
	HDC hdcColor, COLORREF RGB, int Alpha = 255 );
//�摜�𓧉߂��ĕ\������֐�.
bool TransBlt(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc );
//�g�債���摜�𓧉߂��ĕ\������֐�.
bool ExpaTransBlt(
	HDC hdcDest, int xDest, int yDest, int wDest,  int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc,  int weDest, int heDest, bool xyFlag = true );
//�摜�𔼓����ɂ��ĕ\������֐�.
bool AlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc,  int Alpha );
//��]�����摜�𓧉߂��ĕ\������֐�.
bool TransPlgBlt(
	HDC hdcDest, LONG xDest, LONG yDest, LONG wDest, LONG hDest,
	HDC hdcSrc,  LONG xSrc,  LONG ySrc,  DOUBLE Ang, HDC hdcWork );
//��]�����摜�𓧉߂��Ĕ������ɂ���֐�.
bool TransPlgAlBlend(
	HDC hdcDest, LONG xDest, LONG yDest, LONG wDest, LONG hDest,
	HDC hdcSrc,  LONG xSrc,  LONG ySrc,  DOUBLE Ang, HDC hdcPlg, int Alpha, HDC hdcWork );
//�摜�𓧉߂��Ĕ������ɂ���.
bool TransAlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc,  int Alpha, HDC hdcWork );
//�g�債���摜�𓧉߂��Ĕ������ɂ���.
bool ExpaTransAlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc,  int Alpha, HDC hdcWork, int weDest, int heDest, bool xyFlag = true );

//��`���m�̏Փ˔���.
bool CollisionDetection(
	double Ax, double Ay, int Aw, int Ah,	//��`A��x,y���W�ƕ��ƍ���.
	double Bx, double By, int Bw, int Bh );//��`B��x,y���W�ƕ��ƍ���.

//�}�b�v.
bool GetMap( MAP* map );
void CntMap( SCROLL* scroll, MAP* map, ENEMY enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], BOSS* boss, PLAYER* player );
void DispMap( MAP* map, ENEMY enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], BOSS* boss, SCROLL* scroll, PLAYER* player );
//�M�~�b�N.
void Gimmick_Ran( GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], SCROLL scroll, PLAYER* player );

//�v���C���[�̔\�͂̍X�V.
void P_UpdateAblt( PLAYER* playerData );
//�G�̔\�͂̍X�V.
void E_UpdateAblt( ENEMY enemey[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo );
//�\�͍폜�A�j���[�V����.
void DeleteAblt( PLAYER* player );
//�v���C���[�̃o���A�A�񕜂̐���.
void P_Barr( PLAYER* Player );
//�G�̃o���A�A�񕜂̐���.
void E_Barr( ENEMY enemy[E_TYPE][E_PTN][E_MAX] );
//�v���C���[�̉�.
void P_heal( PLAYER* Player );
//�G�̉�.
void E_heal( ENEMY enemy[E_TYPE][E_PTN][E_MAX] );

//�v���C���[�̈ړ�.
void PlayerOp( PLAYER* Player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], CXInput* pXInput, XInStTrg XInKey, SCROLL* scroll, enScene Scene, ENDING Ending, bool* pause, FADE* Fade );

//�e�̔���.
void P_ShotFlag( PLAYER* playerData, enScene Scene, ENDING Ending );
void E_ShotFlag( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnN, int eNo, double Rang = 0, bool angSkip = false );
//�e�̓���.
void P_ShotControl( PLAYER* playerData, BOSS* boss, ENEMY enemyData[E_TYPE][E_PTN][E_MAX], enScene Scene );
void E_ShotControl( ENEMY enemyData[E_TYPE][E_PTN][E_MAX], int typeNo, int ptnNo, int eNo, PLAYER playerData );
//�e�֌W�̓����蔻��.
void S_CollisionDetection( PLAYER* PlayerData, ENEMY EnemyData[E_TYPE][E_PTN][E_MAX], GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], LEAF WoodLeaf[W_HITMAX], BOSS* boss );
//�G�X�e�[�^�X.
bool E_Status( ENEMY enemydata[E_TYPE][E_PTN][E_MAX] );
//�A�r���e�B.
bool E_Ability( ENEMY enemydata[E_TYPE][E_PTN][E_MAX] );

//�ړ��p�^�[��.
void E_ran( SCROLL scroll, PLAYER p_k, ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int ty, int ptn, int eNo );
//�G���A�{�X�ړ��p�^�[��.
void LBE_ran( SCROLL scroll, PLAYER p_k, ENEMY Enemy[E_TYPE][E_PTN][E_MAX], PLANT Plant[E_PTN][E_MAX], BOSS* boss, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], int ty );

//�{�X�̍U������.
void B_AttackControl( BOSS* boss, int typeNo, SCROLL* scroll, PLAYER* player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX] );
//�{�X�̍U���p�^�[��.
void B_AttackMove( BOSS* boss, PLAYER player, GIMMICK gimmick[G_TYPE][G_PTN][G_MAX], ENEMY enemy[E_TYPE][E_PTN][E_MAX] );

//�{�X�̏o���A�j���[�V����.
void DispAnimation( BOSS* boss, SCROLL* scroll, PLAYER* player, CXInput* pXInput );
//�{�X�̎��S�A�j���[�V����.
bool DeadAnimation( BOSS* boss, SCROLL* scroll, PLAYER* player, CXInput* pXInput );

//�{�X�G���A�̓G�̕\���̃f�[�^�擾.
bool GetBEDispData( BOSS* boss );

//�R���g���[���[�܂��̓L�[�{�[�h�̂ǂ����������ꂽ.
bool GetAsyncAnyKey( PLAYER* Player, CXInput* pXInput, XInStTrg* XInKey );
