#include "Main.h"
#include "CSoundManager.h"
#include <time.h>	//time()�֐����g�p���邽�߂ɕK�v.

//�گ�ފ֐�.
DWORD WINAPI ThreadFunc( LPVOID vdParam )
{
	//�ϐ��錾.
	GameWindow* gameWnd = nullptr;	//�ްѳ���޳�\����.

	DWORD frames	  = 0;	//�ڰѐ�.
	DWORD beforeTime  = 0;	//�O�̎���.
	char  MsgFPS[128]  = "";
	char  MsgE_St[128] = "";

	CXInput*	pXInput;	//�R���g���[���̃C���X�^���X�𐶐�.
	XInStTrg	XInKey;		//�R���g���[���̃X�e�B�b�N�A�g���K�[.

	HDC		hMemDC		= nullptr;	//������޲���÷��.
	HFONT	hFont		= nullptr;	//̫�������.
	HDC		hWorkDC		= nullptr;	//��ƗpDC.
	HBITMAP	hWorkBmp	= nullptr;	//��ƗpBITMAP�n���h��.
	HDC		hColorDC	= nullptr;	//�P�F�pDC.
	HBITMAP	hColorBmp	= nullptr;	//�P�F�pBITMAP�n���h��.
	HDC		hPlgDC		= nullptr;	//�摜�̉�]�pDC.
	HBITMAP hPlgBmp		= nullptr;	//�摜�̉�]�pBITMAP�n���h��.
	HDC		hDeatDC		= nullptr;	//���S�V�[���pDC.
	HBITMAP hDeatBmp	= nullptr;	//���S�V�[���pBITMAP�n���h��.
	HBITMAP hscroll		= nullptr;	//�Q�[���w�i(�X�N���[��).
	HBITMAP hhp_Gmck	= nullptr;	//HP�o�[�A����.
	HBITMAP hPHp		= nullptr;	//�v���C���[��HP�o�[.
	HBITMAP hGmck		= nullptr;	//�X�e�[�W�M�~�b�N.
	HBITMAP hTree		= nullptr;	//�X�e�[�W�M�~�b�N(��).
	HBITMAP hTreeHit	= nullptr;	//�X�e�[�W�M�~�b�N(��)�_���[�W.
	HBITMAP hability	= nullptr;	//�\��.
	HBITMAP harea		= nullptr;	//�\�̓G���A�A�����w�i.
	HBITMAP hLetter		= nullptr;	//����.
	HBITMAP hChara		= nullptr;	//�L�����N�^�[.
	HBITMAP hPSlime		= nullptr;	//�L�����N�^�[�̌�����.
	HBITMAP hCharaHit	= nullptr;	//�L�����N�^�[�_���[�W.
	HBITMAP hPSlimeHit	= nullptr;	//�L�����N�^�[�̌����ڕύX��_���[�W.
	HBITMAP hCharaDeat	= nullptr;	//�L�����N�^�[���S.
	HBITMAP hSlime		= nullptr;	//�X���C��.
	HBITMAP hShot		= nullptr;	//�e.
	HBITMAP hboss		= nullptr;	//�{�X,�v���C���[�̒e.
	HBITMAP hSleep		= nullptr;	//�G(��)�������G�t�F�N�g.
	HBITMAP hMainLR		= nullptr;	//�Q�[�����C���̍��E.
	HBITMAP hPause		= nullptr;	//�|�[�Y�摜.

	//----- �v���C���[ -----.
	static PLAYER	Player;
	Player.PushKey[enPushKey::Space]	= false;
	Player.PushKey[enPushKey::RT]		= false;
	Player.PushKey[enPushKey::LT]		= false;

	//----- �G -----.
	static ENEMY	Enemy[E_TYPE][E_PTN][E_MAX];
	PLANT		Plant[E_PTN][E_MAX];
	//----- �{�X -----.
	static BOSS	Boss;
	//----- �}�b�v -----.
	MAP			map;
	map.LastNo = 0;
	//----- �X�e�[�W�M�~�b�N -----.
	GIMMICK		gimmick[G_TYPE][G_PTN][G_MAX];
	POISON		Poison[G_MAX][P_MAX];
	int			PTime[G_MAX];
	int			PTimeCnt[G_MAX];
	LEAF		WoodLeaf[W_HITMAX];
	//----- �p��̍��� -----.		
	const int ENG_W[enENG_No::max] = { 160, 208, 192, 176, 192, 160, 152, 160, 96, 104, 96, 56 };
	//----- �Q�[���I�[�o�[ -----.
	const int GO_DispX [14 + 1] = { 584, 573, 592, 608, 592, 559, 573, 595, 592, 576, 608, 607, 559, 573,		0 };
	const int GO_DispY [14 + 1] = { 411, 429, 408, 394, 416, 406, 418, 370, 370, 370, 370, 370, 372, 370,		0 };
	const int GO_StartX[14 + 1] = { 128, 0,   0,   256, 256, 0,   0,   224, 128, 256, 0  , 64,  0,   128,		0 };
	const int GO_StartY[14 + 1] = { 0,   192, 0,   80,  0,   0,   96,  256, 256, 224, 256, 256, 128, 224,		0 };
	const int GO_wDest [14 + 1] = { 128, 128, 128, 128, 128, 160, 128, 160, 96,  128, 64,  64,  160, 128,		0 };
	const int GO_hDest [14 + 1] = { 112, 64,  96,  64,  80,  128, 96,  32,  32,  32,  32,  32,  32,  32 ,		0 };
	ANIMATION BatAnima;

	//�w�i�X�N���[���p.
	SCROLL  scroll;	//y���W.

	HBITMAP	hTitle		= nullptr;	//���ى摜.
	HBITMAP hsky		= nullptr;	//��.
	HBITMAP hcloud		= nullptr;	//�_.
	HBITMAP hGameOver	= nullptr;	//�Q�[���I�[�o�[�摜.
	HBITMAP hDead		= nullptr;	//�Q�[���I�[�o�[�̎��S�O���t�B�b�N�̉摜.
	HBITMAP hDeadG		= nullptr;	//�Q�[���I�[�o�[�̎��S�O���t�B�b�N(�M�~�b�N)�̉摜.
	HBITMAP hEnding		= nullptr;	//�G���f�B���O�摜.
	HBITMAP hEnSlime	= nullptr;	//�G���f�B���O�̃X���C���摜.
	HBITMAP hEnClear	= nullptr;	//�G���f�B���O�̃Q�[���N���A.
	HBITMAP	hEnGround	= nullptr;	//�G���f�B���O�̒n�ʉ摜.
	HBITMAP hEnEnemy	= nullptr;	//�G���f�B���O�̓G�̉摜.
	HBITMAP hEnEng		= nullptr;	//�G���f�B���O�̕���.
	HBITMAP hEnEneEng	= nullptr;	//�G���f�B���O�̓G�̕���.
	HBITMAP hEnHitNum	= nullptr;	//�G���f�B���O�̃q�b�g��.

	//�G���f�B���O.
	static ENDING	Ending;

	//�ō�Hit����ǂݍ���.
	FILE *fp;

	fp = fopen( "Data\\CSV\\BESTHIT.csv", "r" );	//�t�@�C�����J��.
	if ( fp == NULL ) {
		//�t�@�C�����J���Ȃ��ꍇ.
		Ending.BestHit = 0;
	}
	else{
		fscanf( fp, "%d", &Ending.BestHit );
	}

	//�t�@�C�������.
	fclose( fp );


	//���.
	enScene	Scene	= enScene::Title;	//����.

	//�|�[�Y.
	bool	pause		= false;
	bool	Stop		= false;
	int		pauseArea	= 0;
	int		pauseAlpha	= 255;

	//�t�F�[�h.
	FADE	Fade	= { enFade::None, 0, enScene::Title, 255, true };

	//he .
	static TITLE	Title;
	//������.
	Title.Alpha						= 255;	//���ق�PressEnterKey�Ŏg��.
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

	//�ްѳ���޳�\���̂��擾.
	gameWnd = (GameWindow*)vdParam;	//�擪���ڽ�𷬽Ă��Ĉڂ�.

	//���Ԃ̎擾.
	beforeTime = timeGetTime();		//���݂̎��Ԃ��擾.

	//�u���V�̍쐬.
	HBRUSH hGBrush	= CreateSolidBrush( RGB( 0x00, 0xFF, 0x00 ) );	//�ΐF�u���V.
	HBRUSH hBBrush	= CreateSolidBrush( RGB( 0x00, 0x00, 0x00 ) );	//���F�u���V.
	HBRUSH hGrBrush = CreateSolidBrush( RGB( 0x22, 0x22, 0x22 ) );	//�D�F�u���V.
	HBRUSH hRBrush	= CreateSolidBrush( RGB( 0xFF, 0x00, 0x00 ) );	//�ԐF�u���V.

//-------------------------------------------------------------------.
//	�N������[WM_CREATE].
//-------------------------------------------------------------------.
	//���DC�̍쐬.
	hMemDC = CreateCompatibleDC( nullptr );

	//��ƗpDC�̍쐬.
	hWorkBmp = CreateCompatibleBitmap(
		gameWnd->hScreenDC, WND_W, WND_H );				//��ƗpBITMAP�̍쐬.
	hWorkDC = CreateCompatibleDC( gameWnd->hScreenDC );	//��ƗpDC�̍쐬.
	SelectObject( hWorkDC, hWorkBmp );					//DC��BITMAP��R�t��.

	//�P�F�pDC�̍쐬.
	hColorBmp = CreateCompatibleBitmap(
		gameWnd->hScreenDC, WND_W, WND_H );				//�P�F�pBITMAP�̍쐬.
	hColorDC = CreateCompatibleDC( gameWnd->hScreenDC );//�P�F�pBITMAP�̍쐬.
	SelectObject( hColorDC, hColorBmp );				//DC��BITMAP��R�t��.

	//�摜�̉�]�pDC�̍쐬.
	hPlgBmp = CreateCompatibleBitmap(
		gameWnd->hScreenDC, WND_W, WND_H );				//�摜�̉�]�pBITMAP�̍쐬.
	hPlgDC = CreateCompatibleDC( gameWnd->hScreenDC );	//�摜�̉�]�pDC�̍쐬.
	SelectObject( hPlgDC, hPlgBmp );					//DC��BITMAP��R�t��.
	//�w�i��ΐF�ɂ��Ă���.
	SelectObject( hPlgDC, hGBrush );
	PatBlt(
		hDeatDC,		//�P�F�pDC�֏o��.
		0, 0,			//�\���ʒux,y.
		C_SIZE, C_SIZE,	//�摜�̕�,����.
		PATCOPY );

	//���S�V�[���pDC�̍쐬.
	hDeatBmp = CreateCompatibleBitmap(
		gameWnd->hScreenDC, WND_W, WND_H );				//�摜�̉�]�pBITMAP�̍쐬.
	hDeatDC = CreateCompatibleDC( gameWnd->hScreenDC );	//�摜�̉�]�pDC�̍쐬.
	SelectObject( hDeatDC, hDeatBmp );					//DC��BITMAP��R�t��.
	//�w�i���D�F�ɂ��Ă���.
	SelectObject( hDeatDC, hGrBrush );
	PatBlt(
		hDeatDC,				//�P�F�pDC�֏o��.
		0, 0,					//�\���ʒux,y.
		C_SIZE * 3, C_SIZE * 3,	//�摜�̕�,����.
		PATCOPY );

	//�R���g���[���̃C���X�^���X�𐶐�.
	pXInput = new CXInput( 0 );

	//�T�E���h�Ǘ��N���X�̃C���X�^���X����.
	CSoundManager::GetInstance()->SetWnd( gameWnd->hWnd );
	if ( CSoundManager::GetInstance()->LoadData() == false ) return 0;

	//̫�Ă̍쐬.
	hFont = CreateFont(
		13,								//�����̍���.
		0,								//�����̕�.
		0,								//�p�x.
		0,								//�ް�ײ݊p�x.
		FW_REGULAR,						//�����̑���(FW_BOLD:�������AFW_REGULAR:�ʏ�).
		FALSE,							//�Α�(TRUE:�L��).
		FALSE,							//����(TRUE:�L��).
		FALSE,							//�ł�������(TRUE:�L��).
		SHIFTJIS_CHARSET,				//�������.
		OUT_DEFAULT_PRECIS,				//�o�͐��x.
		CLIP_DEFAULT_PRECIS,			//�د��ݸސ��x.
		DEFAULT_QUALITY,				//�o�͕i��.
		VARIABLE_PITCH | FF_ROMAN,		//���߯���̫��̧��.
		nullptr/*"HG�n�p�p�߯�ߑ�"*/ );	//����(nullptr:���ݎg���Ă��鏑��).
	SelectObject( gameWnd->hScreenDC, hFont );

	//BITMAP�̓ǂݍ���.
	/*���C��*/
	if ( LoadBmp( &hscroll,		"Data\\BMP\\background\\background.bmp"						) == false ) return 0;		//�X�N���[���̓ǂݍ���.
	if ( LoadBmp( &hhp_Gmck,	"Data\\BMP\\iroiro\\iroiro.bmp"								) == false ) return 0;		//HP�o�[�A�M�~�b�N.
	if ( LoadBmp( &hPHp,		"Data\\BMP\\iroiro\\hp_1204.bmp"							) == false ) return 0;		//�v���C���[��HP�o�[.
	if ( LoadBmp( &hGmck,		"Data\\BMP\\iroiro\\syougaibutu.bmp"						) == false ) return 0;		//�X�e�[�W�M�~�b�N.
	if ( LoadBmp( &hTree,		"Data\\BMP\\iroiro\\syuusei_1130.bmp"						) == false ) return 0;		//�X�e�[�W�M�~�b�N(��).
	if ( LoadBmp( &hTreeHit,	"Data\\BMP\\iroiro\\tuika_0120.bmp"							) == false ) return 0;		//�X�e�[�W�M�~�b�N(��)�_���[�W.
	if ( LoadBmp( &hability,	"Data\\BMP\\passibu_subete\\passibu_1120.bmp"				) == false ) return 0;		//�\�͂̓ǂݍ���.
	if ( LoadBmp( &harea,		"Data\\BMP\\passibu_kouka_hyouji\\shuusei_nado_1231.bmp"	) == false ) return 0;		//�\�̓G���A.
	if ( LoadBmp( &hLetter,		"Data\\BMP\\iroiro\\moji_1210.bmp"							) == false ) return 0;		//����.
	if ( LoadBmp( &hChara,		"Data\\BMP\\kyara\\kyara_1130.bmp"							) == false ) return 0;		//��׸���̓ǂݍ���.
	if ( LoadBmp( &hPSlime,		"Data\\BMP\\kyara\\sulim_matome_2_0119.bmp"					) == false ) return 0;		//��׸���̓ǂݍ���.
	if ( LoadBmp( &hCharaHit,	"Data\\BMP\\kyara\\kyara_hit_1130.bmp"						) == false ) return 0;		//�L�����N�^�[�_���[�W�̓ǂݍ���.
	if ( LoadBmp( &hPSlimeHit,	"Data\\BMP\\kyara\\sulim_hit_matome_0120.bmp"				) == false ) return 0;		//�L�����N�^�[�_���[�W�̓ǂݍ���.
	if ( LoadBmp( &hCharaDeat,	"Data\\BMP\\kyara\\deth_1211.bmp"							) == false ) return 0;		//�L�����N�^�[���S�̓ǂݍ���.
	if ( LoadBmp( &hSlime,		"Data\\BMP\\iroiro\\shuusei_1225.bmp"						) == false ) return 0;		//�X���C���̓ǂݍ���.
	if ( LoadBmp( &hShot,		"Data\\BMP\\kyara\\tama_remake.bmp"							) == false ) return 0;		//�e�̓ǂݍ���.
	if ( LoadBmp( &hboss,		"Data\\BMP\\kyara\\boss_jiki_tama.bmp"						) == false ) return 0;		//�{�X,�v���C���[�̒e�̓ǂݍ���.
	if ( LoadBmp( &hSleep,		"Data\\BMP\\kyara\\tuika_2_0120.bmp"						) == false ) return 0;		//�G(��)�̐������G�t�F�N�g.
	if ( LoadBmp( &hMainLR,		"Data\\BMP\\scene\\gamescreen_right_left_1231.bmp"			) == false ) return 0;		//�Q�[�����C���̍��E.
	if ( LoadBmp( &hPause,		"Data\\BMP\\scene\\pouse.bmp"								) == false ) return 0;		//�|�[�Y�摜.
	/*���̑��V�[��*/
	if ( LoadBmp( &hTitle,		"Data\\BMP\\scene\\strat_1216.bmp"							) == false ) return 0;		//���ى摜�̓ǂݍ���.
	if ( LoadBmp( &hsky,		"Data\\BMP\\scene\\sky_0109.bmp"							) == false ) return 0;		//��摜�̓ǂݍ���.
	if ( LoadBmp( &hcloud,		"Data\\BMP\\scene\\gamestrat_shuusei_0109.bmp"				) == false ) return 0;		//�_�摜�̓ǂݍ���.
	if ( LoadBmp( &hGameOver,	"Data\\BMP\\scene\\gameover.bmp"							) == false ) return 0;		//�ްѵ��ް�摜�̓ǂݍ���.
	if ( LoadBmp( &hDead,		"Data\\BMP\\scene\\gameover_nado_0106.bmp"					) == false ) return 0;		//�ްѵ��ް�摜�̓ǂݍ���.
	if ( LoadBmp( &hDeadG,		"Data\\BMP\\scene\\gameover_tuika_0107.bmp"					) == false ) return 0;		//�ްѵ��ް�摜�̓ǂݍ���.
	if ( LoadBmp( &hEnding,		"Data\\BMP\\scene\\haikei.bmp"								) == false ) return 0;		//���ިݸމ摜�̓ǂݍ���.
	if ( LoadBmp( &hEnSlime,	"Data\\BMP\\scene\\gameclear_slimu.bmp"						) == false ) return 0;		//���ިݸމ摜�̓ǂݍ���.
	if ( LoadBmp( &hEnClear,	"Data\\BMP\\scene\\gameclear_zairyou.bmp"					) == false ) return 0;		//���ިݸމ摜�̓ǂݍ���.
	if ( LoadBmp( &hEnGround,	"Data\\BMP\\scene\\kusa_golem.bmp"							) == false ) return 0;		//���ިݸމ摜�̓ǂݍ���.
	if ( LoadBmp( &hEnEnemy,	"Data\\BMP\\scene\\gameclear_kyara.bmp"						) == false ) return 0;		//���ިݸމ摜�̓ǂݍ���.
	if ( LoadBmp( &hEnEng,		"Data\\BMP\\scene\\moji_nado.bmp"							) == false ) return 0;		//���ިݸމ摜�̓ǂݍ���.
	if ( LoadBmp( &hEnEneEng,	"Data\\BMP\\scene\\teki_moji.bmp"							) == false ) return 0;		//���ިݸމ摜�̓ǂݍ���.
	if ( LoadBmp( &hEnHitNum,	"Data\\BMP\\scene\\moji_hit.bmp"							) == false ) return 0;		//���ިݸމ摜�̓ǂݍ���.

	//�^�C�g��BGM�̍Đ�.
	CSoundManager::GetInstance()->PlayLoop( CSoundManager::BGM_Title );

	//�����̏�����(��̐ݒ�).
	srand( ( unsigned int ) time( nullptr ) );

	//����޳��������܂Ŗ���ٰ��.
	while( gameWnd->isWndClose == FALSE )
	{
		DWORD nowTime		= 0;	//���݂̎���.
		DWORD progressTime	= 0;	//�o�߂̎���.
		DWORD idealtime		= 0;	//���z�̎���.

		nowTime = timeGetTime();	//���݂̎��Ԃ��擾.

		progressTime = nowTime - beforeTime;	//�o�ߎ���.

		//���z���Ԃ̎Z�o.
		// ���݂��ڰѐ� �~ ( 1000 �� FPS�l ).
		idealtime = (DWORD) ( frames * ( 1000.0f / gameWnd->dwFPS ) );

		//�R���g���[���[.
		pXInput->Update();
		XInKey.stL_X = pXInput->GetLThumbX();								//L�X�e�B�b�N��X���W���X�V.
		XInKey.stL_Y = pXInput->GetLThumbY();								//L�X�e�B�b�N��Y���W���X�V.
		XInKey.stR_X = pXInput->GetRThumbX();								//R�X�e�B�b�N��X���W���X�V.
		XInKey.stR_Y = pXInput->GetRThumbY();								//R�X�e�B�b�N��Y���W���X�V.
		XInKey.trgL = pXInput->GetLTrigger();								//L�g���K�[�̏�Ԃ��X�V.
		XInKey.trgR = pXInput->GetRTrigger();								//R�g���K�[�̏�Ԃ��X�V.

		//�t���ϗpCOS�p�x(���̐���).
		Title.LCosAng++;
		if ( Title.LCosAng >= 360 ){
			Title.LCosAng = 0;
		}

//-------------------------------------------------------------------.
//	�ްя���[WM_TIMER].
//-------------------------------------------------------------------.
		//��݂��Ƃ̏���.
		if ( ( pause == false ) && ( Stop == false ) ) {
			switch ( Scene ) {
			case enScene::Title:	//����.
			{
				//BGM�Đ�( ���[�v ).
				CSoundManager::PlayLoop( CSoundManager::BGM_Title );

				//��w�i��۰ُ���.
				if ( Title.UPCloudX1 < WND_W ) {
					Title.UPCloudX1 += 0.9f;		//��۰ْl�����炷.
				}
				else {
					Title.UPCloudX1 = -WND_W;		//�����l�ɖ߂�.
				}
				if ( Title.UPCloudX2 < WND_W ) {
					Title.UPCloudX2 += 0.9f;		//��۰ْl�����炷.
				}
				else {
					Title.UPCloudX2 = -WND_W;		//�����l�ɖ߂�.
				}

				//���w�i��۰ُ���.
				if ( Title.DownCloudX1 < WND_W ) {
					Title.DownCloudX1 += 0.2f;		//��۰ْl�����炷.0.2
				}
				else {
					Title.DownCloudX1 = -WND_W;		//�����l�ɖ߂�.
				}
				if ( Title.DownCloudX2 < WND_W ) {
					Title.DownCloudX2 += 0.20f;		//��۰ْl�����炷.0.2
				}
				else {
					Title.DownCloudX2 = -WND_W;		//�����l�ɖ߂�.
				}

				//PressEnterKey��_�ł�����.
				if ( Title.Alpha <= 255 ) {		// 0(�s����) ~ 255(����).
					Title.Alpha += 3;			//�����ɂ��Ă���.
					if ( Title.Alpha >= 255 ) {	//�����ɂȂ�����.
						Title.Alpha = 510;		// 255 => 511 �ɂ���(����).
					}
				}
				else {							// 256(�s����) ~ 511(����).
					Title.Alpha -= 3;			//�s�����ɂ��Ă���.
					if ( Title.Alpha <= 255 ) {	//�s�����ɂȂ�����.
						Title.Alpha = 0;		// 256 => 0 �ɂ���(�s����).
					}
				}

				//�X���C���̐G�o�̃A�j���[�V����.
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

				//�t���ς̏o��.
				if ( Title.LTimeCnt >= Title.LTime ) {
					//���̏o���^�C�������߂�.
					Title.LTimeCnt = 0;
					Title.LTime = rand() % 10 + 5;

					//�o���������߂�.
					Title.LDispMAX = rand() % 5 + 2;
					int DispCnt = 0;

					//�t���ς̏o���o��.
					for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
						if ( Title.Leaf[LNo].DispFlag == false ) {
							//�o��.
							Title.Leaf[LNo].DispFlag	= true;
							Title.Leaf[LNo].x			= -16;
							Title.Leaf[LNo].y			= rand() % WND_H - WND_H / 3;
							Title.Leaf[LNo].FallPower	= rand() % 2 + 1;
							Title.Leaf[LNo].Speed		= rand() % 7 + 2;
							Title.Leaf[LNo].PtnX		= rand() % 3;
							Title.Leaf[LNo].PtnY		= rand() % 2;
							Title.Leaf[LNo].Ang			= 0;

							DispCnt++;	//�o�����J�E���g.

							//�S�ďo������.
							if ( DispCnt >= Title.LDispMAX ) {
								Title.LDispMAX = 0;
								break;
							}
						}
					}
				}
				else {
					//�t���ς̑҂����ԃJ�E���g�{.
					Title.LTimeCnt++;
				}

				//��𕑂��t���ς̓���.
				for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
					if ( Title.Leaf[LNo].DispFlag == true ) {
						//��]������.
						Title.Leaf[LNo].Ang += 0.02;
						if ( Title.Leaf[LNo].Ang >= 360 ){
							Title.Leaf[LNo].Ang = 0;
						}

						//���Ŕ�΂�.
						Title.Leaf[LNo].x += Title.Leaf[LNo].Speed + static_cast< int >( cos( RAD( Title.LCosAng ) - 10 ) * Title.Leaf[LNo].Speed / 2 );
						Title.Leaf[LNo].y += Title.Leaf[LNo].FallPower;		//����.

						//��ʊO�ɏo����.
						if ( Title.Leaf[LNo].x > WND_W ) {
							Title.Leaf[LNo].DispFlag = false;
						}
					}
				}

				if ( GetAsyncAnyKey( &Player, pXInput, &XInKey ) ) {
					if ( Player.PushKey[enPushKey::Space] == false ) {
						//����SE��炷.
						CSoundManager::PlaySE( CSoundManager::SE_Decision );
						
						//����������.
						{
							//-----���@�̏�����-----.
							//�����z�u.
							Player.KeyStop				= false;
							Player.chara.x				= ( GWND_W / 2 ) - ( C_SIZE / 2 );	//����ݸ�(���𒆉��ɑ�����).
							Player.chara.y				= WND_H;							//�␳�l:16��ɔz�u.
							Player.chara.ang			= 0;								//�p�x.
							Player.chara.State			= enCharaState::Living;				//������.
							Player.chara.ExpAnimCnt		= 0;								//�����J�E���g.
							Player.chara.chgCnt			= 0;								//���߃J�E���g.
							Player.chara.ShotDirAng		= 20;								//�e���m�̊Ԃ̊Ԋu.
							Player.chara.healCnt		= 0;								//�񕜑҂����ԃJ�E���g.
							Player.XInArea				= 0;								//�R���g���[���̍폜�ꏊ.
							Player.HPFrameNo			= enHPFrame::HPHigh;				//HP�̃t���[���̎��.
							for ( int KeyNo = 0; KeyNo < enPushKey::Max; KeyNo++ ) {
								Player.PushKey[KeyNo] = false;
							}
							//�A�j���[�V����.
							Player.animation.No			= 0;
							Player.animation.max		= 2;
							Player.animation.cnt		= 0;
							Player.animation.cntMax		= 25;
							Player.animation.Flag		= false;
							//�^�[�Q�b�g������.
							Player.target.x				= GWND_W / 2 - C_SIZE / 2;
							Player.target.y				= -C_SIZE;
							//�����X�e�[�^�X.
							Player.initial_status.hp = 15;
							Player.initial_status.atk	= 1;
							Player.initial_status.dir	= 1;
							Player.initial_status.num	= 1;
							Player.initial_status.cspd	= 5;
							Player.status				= Player.initial_status;	//�\�͂��擾.
							//�\�̓G���A�̏�����.
							for ( int i = 0; i < ABLT_MAX; i++ ) {
								Player.chara.abArea[i] = 0;
							}
#ifdef _DEBUG				/*** �� ********* �\�̓G���A *********** �� ***/
							Player.chara.abArea[0] = pene;
							Player.chara.abArea[1] = atkUP;
							Player.chara.abArea[2] = atkUP;
							Player.chara.abArea[3] = numUP;
#endif						/*** �� ********* �\�̓G���A *********** �� ***/
							P_UpdateAblt( &Player );	//�\�͂̎擾��Ԃ̍X�V.
							//�\�͓���폜�A�j���[�V����.
							for ( int abltNo = 0; abltNo < ABLT_MAX; abltNo++ ) {
								Player.DGAbi[abltNo].xy.x		= 0;
								Player.DGAbi[abltNo].xy.y		= 0;
								Player.DGAbi[abltNo].AbiNo		= 0;
								Player.DGAbi[abltNo].DispFlag	= false;
								Player.DGAbi[abltNo].ExpaFlag	= false;
								Player.DGAbi[abltNo].Expa		= 0.0f;
								Player.DGAbi[abltNo].Alpha		= 255;
							}

							//�o���A�̏�����.
							for ( int bNo = 0; bNo < ABLT_MAX * BARRMAX; bNo++ ) {
								Player.barrier[bNo].x			= WND_W;
								Player.barrier[bNo].y			= WND_H;
								Player.barrier[bNo].C_x			= 0;
								Player.barrier[bNo].C_y			= 0;
								Player.barrier[bNo].length_x	= C_SIZE;
								Player.barrier[bNo].length_y	= C_SIZE;
								Player.barrier[bNo].ang			= 15 * bNo + 13 * ( bNo / BARRMAX );
							}

							//�񕜃G�t�F�N�g�̏�����.
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

							//-----���@�̒e�̏�����-----.
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

							//-----�G�@�̏�����-----.
							for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
								for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
									for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
										//�����z�u.
										Enemy[typeNo][ptnNo][eNo].chara.x			= WND_W;
										Enemy[typeNo][ptnNo][eNo].chara.y			= WND_H;
										Enemy[typeNo][ptnNo][eNo].chara.ang			= 0;
										Enemy[typeNo][ptnNo][eNo].chara.State		= enCharaState::Wait;	//������.
										Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt	= 0;
										Enemy[typeNo][ptnNo][eNo].chara.chgCnt		= 0;
										Enemy[typeNo][ptnNo][eNo].chara.ShotDirAng	= 20;
										Enemy[typeNo][ptnNo][eNo].chara.healCnt		= 0;
										Enemy[typeNo][ptnNo][eNo].EGetAbNo			= 0;
										//�A�j���[�V�����̏�����.
										Enemy[typeNo][ptnNo][eNo].animetion.No		= 0;
										Enemy[typeNo][ptnNo][eNo].animetion.max		= 2;
										Enemy[typeNo][ptnNo][eNo].animetion.cnt		= 0;
										Enemy[typeNo][ptnNo][eNo].animetion.Flag	= false;
										//�A�j���[�V�����J�E���g�̏���ݒ�.
										int AnimeMAX[E_TYPE]						= { 25, 25, 25, 20, 10 };	//�n�`:10, �R�E����:20, �n��ɂ���G:25.
										Enemy[typeNo][ptnNo][eNo].animetion.cntMax	= AnimeMAX[typeNo];
										//�^�[�Q�b�g������.
										Enemy[typeNo][ptnNo][eNo].target.x			= GWND_W / 2 - C_SIZE / 2;
										Enemy[typeNo][ptnNo][eNo].target.y			= -C_SIZE;
										//�X�e�[�^�X�̎擾.
										E_Status( Enemy );
										Enemy[typeNo][ptnNo][eNo].status			= Enemy[typeNo][ptnNo][eNo].initial_status;	//�\�͂��擾.
										//�\�͂̎擾.
										E_Ability( Enemy );
										E_UpdateAblt( Enemy, typeNo, ptnNo, eNo );
										//�o���A�̏�����.
										for ( int bNo = 0; bNo < ( ABLT_MAX - 1 ) * BARRMAX; bNo++ ) {
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].x		= WND_W;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].y		= WND_H;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].C_x		= 0;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].C_y		= 0;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].length_x = C_SIZE;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].length_y = C_SIZE;
											Enemy[typeNo][ptnNo][eNo].barrier[bNo].ang		= 15 * bNo + 13 * ( bNo / BARRMAX );
										}
										//�񕜃G�t�F�N�g�̏�����.
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
										//����]������.
										Enemy[typeNo][ptnNo][eNo].half_rot.E_height = 90;
										Enemy[typeNo][ptnNo][eNo].half_rot.M_x		= 0;
										Enemy[typeNo][ptnNo][eNo].half_rot.M_y		= -C_SIZE;
										//�G�̏�����.
										Enemy[typeNo][ptnNo][eNo].ran				= enEmRanAng::STOP;
										Enemy[typeNo][ptnNo][eNo].EmRanCnt			= 0;
										Enemy[typeNo][ptnNo][eNo].EshotCnt			= 0;
										Enemy[typeNo][ptnNo][eNo].EmMoveCnt			= 0;
										Enemy[typeNo][ptnNo][eNo].appFlag			= false;
										Enemy[typeNo][ptnNo][eNo].BossFlag			= false;
										Enemy[typeNo][ptnNo][eNo].LBE_ang			= 0;
										Enemy[typeNo][ptnNo][eNo].attack			= 0;
										//�G�̒e.
										for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
											for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x				= WND_W;
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y				= WND_H;
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag	= false;
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].homingCnt		= 0;
												Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt		= BEAMMAX;
												if ( typeNo == 0 ) {		//�H���A��.
													Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].sspd = PS_SPD / 2;
												}
												else if ( typeNo == 4 ) {	//�n�`.
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
							//�G(��)�̏�����.
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

							//�{�X.
							Boss.DispFlag			= false;
							Boss.charaSize[0]		= C_SIZE * 2;
							Boss.charaSize[1]		= C_SIZE * 1;
							Boss.charaSize[2]		= C_SIZE * 1;
							//�������W.
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
							//����������.
							Boss.AttackMove							= enBossMoveNo::EarlyForm1;
							Boss.AttackMovecnt						= 0;
							Boss.AttackCnt							= 0;
							Boss.Blowcnt							= 0;
							Boss.PatternCnt							= 1;							
							for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ) {
								//�����ݒ�.
								Boss.chara[typeNo].x				= WND_W;
								Boss.chara[typeNo].y				= WND_H;
								Boss.chara[typeNo].ang				= RAD( 0 );
								Boss.chara[typeNo].State			= enCharaState::Wait;
								Boss.chara[typeNo].ExpAnimCnt		= 0;
								Boss.chara[typeNo].chgCnt			= 0;
								Boss.chara[typeNo].ShotDirAng		= 20;
								Boss.chara[typeNo].healCnt			= 0;
								//�A�j���[�V����.
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
								//�U�����.
								Boss.AttackNo[typeNo]				= enBossAttackNo::BWait;
								//�����X�e�[�^�X.
								Boss.initial_status[typeNo].atk		= 2;
								Boss.initial_status[typeNo].dir		= 1;
								Boss.initial_status[typeNo].num		= 1;
								Boss.initial_status[typeNo].cspd	= 5;
								Boss.status[typeNo]					= Boss.initial_status[typeNo];	//�\�͂��擾.
								//�\�̓G���A�̏�����.
								for ( int abltNo = 0; abltNo < ABLT_MAX; abltNo++ ) {
									Boss.chara[typeNo].abArea[abltNo] = 0;
								}
								//-----�{�X�̒e�̏�����-----.
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
								//-----�˂��グ��̏�����-----.
								for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
									for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
										Boss.rock[typeNo][numNo][dirNo].x			= WND_W;
										Boss.rock[typeNo][numNo][dirNo].y			= WND_H;
										Boss.rock[typeNo][numNo][dirNo].Rang		= 0;
										Boss.rock[typeNo][numNo][dirNo].cnt			= 0;
										Boss.rock[typeNo][numNo][dirNo].DispFlag	= false;

										//�X�e�[�W�M�~�b�N(��)�̏o���p�x.
										Boss.RockRAngData[numNo][dirNo]				= 0;
										Boss.RockDispNoData[numNo][dirNo]			= 0;
									}
								}
								//��̐��A�����������A�e���m�̊Ԋu.
								Boss.rockData[typeNo].Num	= 1;
								Boss.rockData[typeNo].Dir	= 1;
								Boss.rockData[typeNo].Ang	= 20;
								Boss.rockData[typeNo].Spaa	= 8;
								Boss.rockData[typeNo].Time	= 100;
								Boss.rockData[typeNo].iAng	= 90;
							}
							//�{�X�G���A�̓G�̕\���f�[�^���擾.
							GetBEDispData( &Boss );
							//�{�X�̐�����΂��G�t�F�N�g.
							Boss.BlowAway.y			= 0;
							Boss.BlowAway.DispFlag	= false;

							//-----�X�e�[�W�M�~�b�N-----.
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
							//�X�e�[�W�M�~�b�N(�ŏ�)�̃A�j���[�V����.
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

							//-----�}�b�v�̏�����-----.
							map.cnt			= 0;
							map.dispCnt		= 0;
							for ( int hNo = 0; hNo < MAP_H; hNo++ ) {
								for ( int wNo = 0; wNo < MAP_W; wNo++ ) {
									map.Data[hNo][wNo] = 0;
								}
							}
#ifdef _DEBUG				/*** �� ********* �J�n�ʒu[ �{�X���O:100 ] *********** �� ***/
							map.dispCnt = 0;

#endif						/*** �� ********* �J�n�ʒu[ �{�X���O:100 ] *********** �� ***/
							GetMap( &map );
							DispMap( &map, Enemy, Plant, gimmick, &Boss, &scroll, &Player );

							//----- �Q�[���I�[�o�[�A�j���[�V���� -----.
							BatAnima.cnt	= 0;
							BatAnima.cntMax = 10;
							BatAnima.No		= 0;
							BatAnima.max	= 2;
							BatAnima.Flag	= false;

							//-----�w�i��۰�-----.
							//�����z�u.
							scroll.y			= WND_H;
							scroll.spd			= 1;
							scroll.stop			= true;
							scroll.BossDAng		= 0;
							scroll.BossDispY	= 0;
							scroll.BossDispYCnt = 0;
							scroll.Start		= true;

							//-----�G���f�B���O-----.
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
							Ending.SDispMAX					= 0;	//�t���ς̏o����.
							Ending.STime					= 0;	//���̗t���ς̏o���҂�����.
							Ending.STimeCnt					= 0;	//���̗t���ς̏o���҂����ԃJ�E���g.
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

							//Hit�����ő�Hit����荂���ꍇ�㏑������.
							if ( Ending.Hit > Ending.BestHit ){
								//�ō�Hit�����㏑��.
								Ending.BestHit = Ending.Hit;
							}
							Ending.Hit						= 0;

							// -1 �F ��.
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

							//�Q�[�����C��BGM�̍Đ�.
							CSoundManager::Stop( CSoundManager::BGM_Title );
							CSoundManager::PlayLoop( CSoundManager::BGM_Main );

							//�t�F�[�h�C��(�Q�[�����C����).
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
					//����޳����邱�Ƃ�Windows�ɒm�点��.
					PostMessage( gameWnd->hWnd, WM_CLOSE, 0, 0 );
				}
				break;
			}
			case enScene::GameMain:	//�ް�Ҳ�.
			{
				//BGM�Đ�( ���[�v ).
				if ( Boss.chara[enBoss::Main].State == enCharaState::Wait ){
					CSoundManager::PlayLoop( CSoundManager::BGM_Main );
				}
				else{
					CSoundManager::PlayLoop( CSoundManager::BGM_Boss );
				}

				//HP�o�[�̌�����No���w��.
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

				//�v���C���[�̑���.
				if ( Player.KeyStop != true ) {
					PlayerOp( &Player, gimmick, pXInput, XInKey, &scroll, Scene, Ending, &pause, &Fade );
				}
				
				//�v���C���[�e�̓���.
				P_ShotControl( &Player, &Boss, Enemy, Scene );

				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							//�G�̒e�̓���.
							E_ShotControl( Enemy, typeNo, ptnNo, eNo, Player );
						}
					}
				}

				//�{�X�̓���.
				if ( ( Boss.chara[enBoss::Main].State == enCharaState::Living ) ||
					 ( Boss.chara[enBoss::Main].State == enCharaState::Invi ) ) {
					B_AttackMove( &Boss, Player, gimmick, Enemy );
				}
				for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ) {
					B_AttackControl( &Boss, typeNo, &scroll, &Player, gimmick, Enemy );
				}

				//�e�̓����蔻��.
				S_CollisionDetection( &Player, Enemy, gimmick, WoodLeaf, &Boss );

				//���@�̐����m�F.
				if ( Player.chara.State == enCharaState::Living ) {
					for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
						for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
							for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
								//�G�@�̐����m�F.
								if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) {
									//���@�ƓG�@�̓����蔻��.
									if ( CollisionDetection(
										Player.chara.x + PH_CORR,			Player.chara.y + PH_CORR,			PH_SIZE,	PH_SIZE,
										Enemy[typeNo][ptnNo][eNo].chara.x,	Enemy[typeNo][ptnNo][eNo].chara.y,	C_SIZE,		C_SIZE ) )
									{
										//�v���C���[�̃_���[�WSE��炷.
										CSoundManager::PlaySE( CSoundManager::SE_PDamage );

										//HP�����炷.
										Player.status.hp -= Enemy[typeNo][ptnNo][eNo].status.atk;
										if ( Player.status.hp <= 0 ) {
											Player.chara.State		= enCharaState::Dying;	//��Ԃ����S���ɂ���.
											Player.chara.ExpAnimCnt = 0;					//������Ұ��ݶ�����0�ɂ���.
											Player.DeadNo			= typeNo;				//�G(���)�Ŏ��S.
										}
										else {
											//��Ԃ𖳓G�ɂ���.
											Player.chara.State = enCharaState::Invi;
											Player.chara.inviTime = INVITIME * 2;
										}

										break;	//�G�@�Ɠ������for���𔲂���.
									}
								}
							}
						}
					}
				}
				//���@�̎��S��.
				else if ( Player.chara.State == enCharaState::Dying ) {
					//���S�A�j���[�V�����p���W���炵.
					Player.chara.x--;
					Player.chara.y += 2;
					Player.chara.ExpAnimCnt++;

					if ( Player.chara.ExpAnimCnt > 15 ) {

						//�t�F�[�h�C��(�Q�[���I�[�o�[��).
						Stop				= true;
						Fade.FadeState		= enFade::in;
						Fade.EndScene		= enScene::GameOver;

						Player.chara.State	= enCharaState::Living;	//��Ԃ𐶑����ɂ���.

						//�����ʒu�ɖ߂�.
						Player.chara.x		= -WND_W;
						Player.chara.y		= -WND_H;

						Title.Alpha			= 255;

						//�Q�[���I�[�o�[BGM�̍Đ�.
						CSoundManager::Stop		( CSoundManager::BGM_Main );
						CSoundManager::Stop		( CSoundManager::BGM_Boss );
						CSoundManager::PlayLoop	( CSoundManager::BGM_GameOver );
					}
				}

				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							//�G�@�����S��.
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dying ) {

								//�G�@�̔�������.
								Enemy[typeNo][ptnNo][eNo].chara.x--;
								Enemy[typeNo][ptnNo][eNo].chara.y += 2;
								Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt++;
								if ( Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt > 15 )//�����摜��0�`15��16��.
								{
									//���S�A�j���[�V�����p���W���炵.
									Enemy[typeNo][ptnNo][eNo].chara.x += 15;
									Enemy[typeNo][ptnNo][eNo].chara.y -= 30;
									//��Ԃ����S�ɂ���.
									Enemy[typeNo][ptnNo][eNo].chara.State	= enCharaState::Dead;
									Enemy[typeNo][ptnNo][eNo].ran			= enEmRanAng::STOP;
								
									//�\�͂��Z�b�g.
									if ( ( Enemy[typeNo][ptnNo][eNo].chara.abArea[0] <= enAbility::homing ) && ( Player.chara.abArea[0] == 0 ) ) {
										Enemy[typeNo][ptnNo][eNo].EGetAbNo = Enemy[typeNo][ptnNo][eNo].chara.abArea[0];
									}
									else {
										int getAbl = 0;	//�����_���Ŏ擾�ł���\�͂�I��.
										//�擾�\�͂����܂�܂Ń��[�v.
										do {
											//�����_���őI��.
											getAbl = rand() % 10;

											//�����_���őI�񂾏ꏊ�̔\�͂��擾�ł��邩.
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
					//�{�X�����S��.
					if ( Boss.chara[typeNo].State == enCharaState::Dying ) {
						if ( typeNo == enBoss::Main ){
							//�����s�\��.
							Player.KeyStop = true;
							//���S�A�j���[�V�������s���A�A�j���[�V�����I���ʒm���󂯎������.
							if ( DeadAnimation( &Boss, &scroll, &Player, pXInput ) == true ){
								//��ʊO��.
								for ( int type = 0; type < Boss.BOSS_TYPE; type++ ){
									Boss.chara[type].x = -WND_W;
									Boss.chara[type].y = -WND_H;
								}

								//�t�F�[�h�C��(�G���f�B���O��).
								Stop				= true;
								Fade.FadeState		= enFade::in;
								Fade.EndScene		= enScene::Ending;

								Title.Alpha			= 255;

								//����\��.
								Player.KeyStop		= false;

								//�G���f�B���OBGM�̍Đ�.
								CSoundManager::Stop	   ( CSoundManager::BGM_Boss	);
								CSoundManager::PlayLoop( CSoundManager::BGM_Ending	);
							}
						}
						else{
							if ( Boss.chara[enBoss::Main].State != enCharaState::Dying ){
								//���S�A�j���[�V�����p���W���炵.
								Boss.chara[typeNo].x--;
								Boss.chara[typeNo].y += 2;
								Boss.chara[typeNo].ExpAnimCnt++;
								if ( Boss.chara[typeNo].ExpAnimCnt > 15 )//�����摜��0�`15��16��.
								{
									if ( Boss.chara[0].ExpAnimCnt <= 15 ) {
										//��Ԃ����S�ɂ���( ���E�̎�̂� ).
										Boss.chara[typeNo].State = enCharaState::Dead;
									}
								}
							}
						}
					}
				}
				if ( Boss.BlowAway.DispFlag == true ) {
					//������΂��G�t�F�N�g�̈ړ�.
					Boss.BlowAway.y += 20;

					//��ʊO�ɏo��.
					if ( Boss.BlowAway.y > GWND_H ) {
						Boss.BlowAway.DispFlag = false;
					}
				}

				//�G�̔\�͂��擾.
				if ( ( Player.chara.abArea[0] == 0 ) || ( Player.chara.abArea[1] == 0 ) ||
					 ( Player.chara.abArea[2] == 0 ) || ( Player.chara.abArea[3] == 0 ) )
				{
					//���@�̐����m�F.
					if ( Player.chara.State == enCharaState::Living ) {
						for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
							for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
								for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
									//�G�@�̎��S�m�F.
									if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dead ) {
										//���@�ƓG�@�̓����蔻��.
										if ( CollisionDetection(
											Player.chara.x,						Player.chara.y,						C_SIZE, C_SIZE,
											Enemy[typeNo][ptnNo][eNo].chara.x,	Enemy[typeNo][ptnNo][eNo].chara.y,	C_SIZE, C_SIZE ) )
										{
											//�\�͂̎擾.
											if ( ( Player.chara.abArea[0] == 0 ) || ( Player.chara.abArea[1] == 0 ) ||
												 ( Player.chara.abArea[2] == 0 ) || ( Player.chara.abArea[3] == 0 ) )
											{
												if ( ( Enemy[typeNo][ptnNo][eNo].EGetAbNo <= enAbility::homing ) && ( Player.chara.abArea[0] == 0 ) ) {
													//�\�͎擾SE��炷.
													CSoundManager::PlaySE( CSoundManager::SE_AGet );

													//�󂩂�\�͈ڍs.
													Player.DGAbi[0].xy.x		= 5 + ( ABLT_SIZE + 5 ) * 0;
													Player.DGAbi[0].xy.y		= WND_H - ABLT_SIZE - 6;
													Player.DGAbi[0].DispFlag	= true;
													Player.DGAbi[0].ExpaFlag	= false;
													Player.DGAbi[0].AbiNo		= -1;
													Player.DGAbi[0].Expa		= 0;
													Player.DGAbi[0].Alpha		= 200;

													//�����ڂ̕ύX.
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

													//�\�͎擾.
													Player.chara.abArea[0] = Enemy[typeNo][ptnNo][eNo].EGetAbNo;
													Enemy[typeNo][ptnNo][eNo].EGetAbNo = 0;

													//�G�@�̏���.
													Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Wait;
													Enemy[typeNo][ptnNo][eNo].chara.x = GWND_W * 2;
													Enemy[typeNo][ptnNo][eNo].appFlag = false;
												}
												else if ( Enemy[typeNo][ptnNo][eNo].EGetAbNo > enAbility::homing ) {
													if( ( Player.chara.abArea[1] == 0 ) || ( Player.chara.abArea[2] == 0 ) || ( Player.chara.abArea[3] == 0 ) ) {
														for( int ablNo = 1; ablNo < ABLT_MAX; ablNo++ ) {
															if( Player.chara.abArea[ablNo] == 0 ) {
																//�\�͎擾SE��炷.
																CSoundManager::PlaySE( CSoundManager::SE_AGet );

																//�󂩂�\�͂Ɉڍs.
																Player.DGAbi[ablNo].xy.x		= 5 + ( ABLT_SIZE + 5 ) * ablNo;
																Player.DGAbi[ablNo].xy.y		= WND_H - ABLT_SIZE - 6;
																Player.DGAbi[ablNo].DispFlag	= true;
																Player.DGAbi[ablNo].ExpaFlag	= false;
																Player.DGAbi[ablNo].AbiNo		= Player.chara.abArea[ablNo];
																Player.DGAbi[ablNo].Expa		= 0;
																Player.DGAbi[ablNo].Alpha		= 200;

																//�����ڂ̕ύX.
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

																//�\�͎擾.
																Player.chara.abArea[ablNo] = Enemy[typeNo][ptnNo][eNo].EGetAbNo;
																Enemy[typeNo][ptnNo][eNo].EGetAbNo = 0;
																break;
															}
														}

														//�G�@�̏���.
														Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Wait;
														Enemy[typeNo][ptnNo][eNo].chara.x = GWND_W * 2;
														Enemy[typeNo][ptnNo][eNo].appFlag = false;
													}
												}
												//�\�͂̎擾��Ԃ̎擾.
												P_UpdateAblt( &Player );
											}
											break;	//�G�@�Ɠ������for���𔲂���.
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
							//�G�ړ�.
							E_ran( scroll, Player, Enemy, Plant, &Boss, gimmick, typeNo, ptnNo, eNo );
						}
					}
					//���������G�̈ړ�.
					LBE_ran( scroll, Player, Enemy, Plant, &Boss, gimmick, typeNo );
				}

				//�G(��)�̏�ԃA�j���[�V����.
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ){
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[0][ptnNo][eNo].chara.State == enCharaState::Living ) ||
							 ( Enemy[0][ptnNo][eNo].chara.State == enCharaState::Invi ) )
						{
							for ( int pNo = 0; pNo < 2; pNo++ ){
								if ( Plant[ptnNo][eNo].Sleep[pNo].DispFlag == true ){

									//�X�N���[���ɂ��Y�ړ�.
									Plant[ptnNo][eNo].Sleep[pNo].y += scroll.spd;

									if ( Plant[ptnNo][eNo].State == enPlantState::Sleeping ){

										//�E��Ɉړ������Ă���.
										Plant[ptnNo][eNo].Sleep[pNo].x += 0.5;
										Plant[ptnNo][eNo].Sleep[pNo].y -= 0.5;

										//�����ɂ��Ă���.
										Plant[ptnNo][eNo].Sleep[pNo].Alpha -= 3;
										//�����x�ɂ��zZ�̕ω�.
										if ( Plant[ptnNo][eNo].Sleep[pNo].Alpha <= 85 ){
											Plant[ptnNo][eNo].Sleep[pNo].ptnNo	= 0;

											//�V����zZ�̏o��.
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

										//���S�ɓ����ɂȂ���.
										if ( Plant[ptnNo][eNo].Sleep[pNo].Alpha <= 0 ){
											Plant[ptnNo][eNo].Sleep[pNo].Alpha		= 255;
											Plant[ptnNo][eNo].Sleep[pNo].ptnNo		= 2;
											Plant[ptnNo][eNo].Sleep[pNo].DispFlag	= false;
										}
									}
								}
							}
							if ( Plant[ptnNo][eNo].State == enPlantState::WokeUp ){
								//���W�𑵂���.
								Plant[ptnNo][eNo].Woke.x = Enemy[0][ptnNo][eNo].chara.x + C_SIZE / 2 - 16 / 2;
								Plant[ptnNo][eNo].Woke.y = Enemy[0][ptnNo][eNo].chara.y - 16 / 2;
								if ( Plant[ptnNo][eNo].Woke.AlphaFlag == false ){
									//�A�j���[�V�����p���W���炵.
									Plant[ptnNo][eNo].Woke.AnimaX += 1;
									Plant[ptnNo][eNo].Woke.AnimaY += 2;
									Plant[ptnNo][eNo].Woke.AnimaCnt++;
									if ( Plant[ptnNo][eNo].Woke.AnimaCnt > 15 ){
										//�A�j���[�V�����p���W���炵�̏�����.
										Plant[ptnNo][eNo].Woke.AnimaX = 0;
										Plant[ptnNo][eNo].Woke.AnimaY = 0;
										Plant[ptnNo][eNo].Woke.AnimaCnt = 0;

										Plant[ptnNo][eNo].Woke.AlphaFlag = true;
									}
								}
								else{
									//�����ɂ��Ă���.
									Plant[ptnNo][eNo].Woke.Alpha -= 3;
									if ( Plant[ptnNo][eNo].Woke.Alpha <= 0 ){
										//�N����Ԃɂ���.
										Plant[ptnNo][eNo].Woke.Alpha	= 255;
										Plant[ptnNo][eNo].State			= enPlantState::StayUp;
									}
								}
							}
						}
					}
				}

				//�v���C���[�̔\�͎擾�A�j���[�V����.
				if ( Player.AbGetFlag == true ){
					//�A�j���[�V�����p���W���炵.
					Player.GetAnimaX += 1;
					Player.GetAnimaY += 2;
					Player.GetAnimaCnt++;
					if ( Player.GetAnimaCnt > 15 ) {
						//�A�j���[�V�����p���W���炵�̏�����.
						Player.GetAnimaX = 0;
						Player.GetAnimaY = 0;

						Player.GetAnimaCnt	= 0;
						Player.AbGetFlag	= false;
						Player.SlimeNo		= Player.NewSlimeNo;
						Player.NewSlimeNo	= 0;
					}
				}

				//�v���C���[�̃_���[�W�摜.
				if ( Player.Damage[0].DispFlag == true ){
					Player.Damage[0].x = Player.chara.x + C_SIZE - 16 - 20;
					Player.Damage[0].y = Player.chara.y + 20;
				}
				if ( Player.Damage[1].DispFlag == true ){
					Player.Damage[1].x = Player.chara.x + 10;
					Player.Damage[1].y = Player.chara.y + C_SIZE - 16 - 10;
				}

				//�X�e�[�W�M�~�b�N(�ŏ�)�̃A�j���[�V�����̕\��.
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					if ( gimmick[2][1][gNo].State != enGimmickState::GWait ) {
						if ( PTimeCnt[gNo] >= PTime[gNo] ) {
							int DispNum		= rand() % 4 + 1;
							PTimeCnt[gNo]	= 0;
							PTime[gNo]		= rand() % 31 + 50;

							for ( int pNo = 0; pNo < DispNum; pNo++ ) {
								//�����ݒ�.
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

				//�X�e�[�W�M�~�b�N(�ŏ�)�̃A�j���[�V�����̓���.
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					for ( int pNo = 0; pNo < P_MAX; pNo++ ) {
						if ( Poison[gNo][pNo].DispFlag == true ) {
							if ( gimmick[2][1][gNo].State != enGimmickState::GWait ) {
								//�����ɂȂ�Ȃ����ɂ����Ă���.
								Poison[gNo][pNo].y		-= Poison[gNo][pNo].Spd;
								Poison[gNo][pNo].Alpha	-= Poison[gNo][pNo].AlSpd;

								//���S�ɓ����ɂȂ���.
								if ( Poison[gNo][pNo].Alpha < 0 ) {
									Poison[gNo][pNo].Alpha		= 255;
									Poison[gNo][pNo].DispFlag	= false;
								}
							}
							else {
								//������.
								PTime[gNo]					= 0;
								PTimeCnt[gNo]				= 0;
								Poison[gNo][pNo].Alpha		= 255;
								Poison[gNo][pNo].DispFlag	= false;
							}
						}
					}
				}

				//�X�e�[�W�M�~�b�N(�ŏ�)�̃J�E���g.
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					if ( gimmick[2][1][gNo].State != enGimmickState::GWait ) {
						PTimeCnt[gNo]++;
					}
				}

				//�\�͍폜�A�j���[�V����.
				DeleteAblt( &Player );
				//�o���A�̐���.
				P_Barr( &Player );
				E_Barr( Enemy );
				//�񕜂̐���.
				P_heal( &Player );
				E_heal( Enemy );

				//�M�~�b�N.
				Gimmick_Ran( gimmick, scroll, &Player );

				if ( Boss.chara[enBoss::Main].State != enCharaState::Dying ){
					//�G�̃J�E���g+.
					for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
						for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
							for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
								if ( Enemy[typeNo][ptnNo][eNo].appFlag == true || Enemy[typeNo][ptnNo][eNo].BossFlag == true ) {
									if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living || Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) {
										//�G�J�E���g.
										Enemy[typeNo][ptnNo][eNo].EmRanCnt++;
										Enemy[typeNo][ptnNo][eNo].EshotCnt++;
									}
									//�A�j���[�V����.
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
									//���G���Ԃ̌���.
									if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) {
										Enemy[typeNo][ptnNo][eNo].chara.inviTime--;
										if ( Enemy[typeNo][ptnNo][eNo].chara.inviTime == 0 ) {
											Enemy[typeNo][ptnNo][eNo].chara.State = enCharaState::Living;
										}
									}
									//�񕜃G�t�F�N�g�A�j���[�V����.
									for ( int hNo = 0; hNo < H_MAX; hNo++ ){
										if ( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].DispFlag == true ){
											//�񕜃G�t�F�N�g�A�j���[�V����.
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cnt++;
											if ( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cnt >= Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cntMax ){
												//���W�𑵂���.
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].x = rand() % ( C_SIZE - C_SIZE / 2 ) + Enemy[typeNo][ptnNo][eNo].chara.x;
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].y = rand() % ( C_SIZE / 3 ) + ( Enemy[typeNo][ptnNo][eNo].chara.y + ( C_SIZE - C_SIZE / 2 ) );

												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.cnt = 0;
												Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No++;
												if ( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No >= Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.max ){
													Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No = 0;
												}
											}
											//�����Ȃ�Ȃ����ɏグ��.
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].UpY	+= 2;
											Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].Alpha	-= 5;
											//�A���t�@�l��0�ȉ��ɂȂ�����񕜃G�t�F�N�g������.
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

					//�v���C���[�̃J�E���g+.
					//�V���b�g�J�E���g�𑝂₷.
					Player.shotCnt++;
					//�A�j���[�V����.
					Player.animation.cnt++;
					if ( Player.animation.cnt >= Player.animation.cntMax ) {
						Player.animation.cnt = 0;
						Player.animation.No++;
						if ( Player.animation.No >= Player.animation.max ) {
							Player.animation.No = 0;
						}
					}
					//�񕜃G�t�F�N�g�A�j���[�V����.
					for ( int hNo = 0; hNo < H_MAX; hNo++ ){
						if ( Player.HeelAnima[hNo].DispFlag == true ){
							//�񕜃G�t�F�N�g�A�j���[�V����.
							Player.HeelAnima[hNo].animation.cnt++;
							if ( Player.HeelAnima[hNo].animation.cnt >= Player.HeelAnima[hNo].animation.cntMax ){
								//���W�𑵂���.
								Player.HeelAnima[hNo].x = rand() % ( C_SIZE - C_SIZE / 2 ) + Player.chara.x;
								Player.HeelAnima[hNo].y = rand() % ( C_SIZE / 3 ) + ( Player.chara.y + ( C_SIZE - C_SIZE / 2 ) );

								Player.HeelAnima[hNo].animation.cnt = 0;
								Player.HeelAnima[hNo].animation.No++;
								if ( Player.HeelAnima[hNo].animation.No >= Player.HeelAnima[hNo].animation.max ){
									Player.HeelAnima[hNo].animation.No = 0;
								}
							}
							//�����Ȃ�Ȃ����ɏグ��.
							Player.HeelAnima[hNo].UpY += 2;
							Player.HeelAnima[hNo].Alpha -= 5;
							//�A���t�@�l��0�ȉ��ɂȂ�����񕜃G�t�F�N�g������.
							if ( Player.HeelAnima[hNo].Alpha <= 0 ){
								Player.HeelAnima[hNo].animation.cnt = 0;
								Player.HeelAnima[hNo].animation.No = 0;
								Player.HeelAnima[hNo].Alpha = 255;
								Player.HeelAnima[hNo].UpY = 0;
								Player.HeelAnima[hNo].DispFlag = false;
							}
						}
					}
					//���G���Ԃ̌���.
					if ( Player.chara.State == enCharaState::Invi ) {
						//�R���g���[���U��.
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

					//�{�X�̃J�E���g+.
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
						//���G���Ԃ̌���.
						if ( Boss.chara[typeNo].State == enCharaState::Invi ) {
							Boss.chara[typeNo].inviTime--;
							if ( Boss.chara[typeNo].inviTime == 0 ) {
								Boss.chara[typeNo].State = enCharaState::Living;
							}
						}
					}

					//�X�e�[�W�M�~�b�N�̃J�E���g+
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

				//�{�X�̏o�����o.
				if ( Boss.DispFlag == true ) {
					DispAnimation( &Boss, &scroll, &Player, pXInput );
				}

				//��𕑂��t���ς̓���.
				for ( int LNo = 0; LNo < W_HITMAX; LNo++ ) {
					if ( WoodLeaf[LNo].DispFlag == true ) {
						//��]������.
						WoodLeaf[LNo].Ang += 0.02;
						if ( WoodLeaf[LNo].Ang >= 360 ){
							WoodLeaf[LNo].Ang = 0;
						}

						//���Ŕ�΂�.
						WoodLeaf[LNo].x += WoodLeaf[LNo].Speed + static_cast< int >( cos( RAD( Title.LCosAng ) - 10 ) * WoodLeaf[LNo].Speed / 2 );
						WoodLeaf[LNo].y += WoodLeaf[LNo].FallPower;		//����.

						//��ʊO�ɏo����.
						if ( WoodLeaf[LNo].x > C_CORR + GWND_W ) {
							WoodLeaf[LNo].DispFlag = false;
						}
					}
				}

				//�w�i��۰ُ���.
				if ( scroll.stop == false ) {
					if ( scroll.y > -WND_H ) {
						scroll.y -= scroll.spd;	//��۰ْl�����炷.
					}
					else {
						scroll.y = WND_H;		//�����l�ɖ߂�.
					}

					//�}�b�v�J�E���g.
					CntMap( &scroll, &map, Enemy, Plant, gimmick, &Boss, &Player );
				}

//#ifdef _DEBUG	/* �f�o�b�N���� */
				//�X�N���[���X�g�b�v.
				if ( GetAsyncKeyState( VK_F1 ) & 0x0001 ){
					if ( scroll.stop == false ) { scroll.stop = true;  }
					else						{ scroll.stop = false; }
				}
				//�Q�[���I�[�o�[��ʂ�.
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

				//�G���f�B���O��.
				if ( GetAsyncKeyState( VK_F4 ) & 0x8000 ) {
					scroll.stop = true;
					CSoundManager::Stop( CSoundManager::BGM_Main );
					CSoundManager::Stop( CSoundManager::BGM_Boss );
					Boss.chara[enBoss::Main].State = enCharaState::Dying;
				}

				//�X�N���[�����΂�.
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
			case enScene::GameOver:	//�ްѵ��ް.
			{
				//BGM�Đ�( ���[�v ).
				CSoundManager::PlayLoop( CSoundManager::BGM_GameOver );

				//PressEnterKey��_�ł�����.
				if ( Title.Alpha <= 255 ) {			// 0(�s����) ~ 255(����).
					Title.Alpha += 2;				//�����ɂ��Ă���.
					if ( Title.Alpha >= 255 ) {		//�����ɂȂ�����.
						Title.Alpha = 511;			// 255 => 511 �ɂ���(����).
					}
				}
				else {								// 256(�s����) ~ 511(����).
					Title.Alpha -= 2;				//�s�����ɂ��Ă���.
					if ( Title.Alpha <= 256 ) {		//�s�����ɂȂ�����.
						Title.Alpha = 0;			// 256 => 0 �ɂ���(�s����).
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

						//����SE��炷.
						CSoundManager::PlaySE( CSoundManager::SE_Decision );

						//�������قɓ����Ă��Ă����v�Ȃ悤�ɏ�����.
						Title.Alpha				= 255;	//���ق�PressEnterKey�Ŏg��.
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
						//�t�F�[�h�C��(�^�C�g����).
						Stop			= true;
						Fade.FadeState	= enFade::in;
						Fade.EndScene	= enScene::Title;

						//�^�C�g��BGM�̍Đ�.
						CSoundManager::Stop( CSoundManager::BGM_GameOver );
						CSoundManager::PlayLoop( CSoundManager::BGM_Title );
					}
				}
				else {
					Player.PushKey[enPushKey::Space] = false;
				}
				break;
			}
			case enScene::Ending:	//���ިݸ�.
			{
				//BGM�Đ�( ���[�v ).
				CSoundManager::PlayLoop( CSoundManager::BGM_Ending );

				//�\�̓G���A���Z�b�g.
				Player.chara.abArea[0] = enAbility::dirUP;
				Player.chara.abArea[1] = enAbility::numUP;
				Player.chara.abArea[2] = enAbility::numUP;
				Player.chara.abArea[3] = enAbility::numUP;
				//�\�͂̎擾��Ԃ̎擾.
				P_UpdateAblt( &Player );

				//���t���[���ŏ��͑ҋ@�ɖ߂�.
				//Ending.Slime.Action = enAction::AcWait;

				//�W�����v.
				if ( GetAsyncKeyState( VK_UP ) & 0x8000 || XInKey.trgL >= CXInput::INPUT_TRIGGER_MAX ) {
					if ( Ending.EnEng[ENGMAX - 1].y > 300.0f ) {
						if ( Ending.Slime.Jumping == false ) {

							//�W�����vSE�̍Đ�.
							CSoundManager::PlaySE( CSoundManager::SE_Jump );

							Ending.Slime.Jumping	= true;	//�W�����v���ɂ���.
							Ending.Slime.JumpAcc	= 0.0f;	//�l��������.
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

						//���ˏ����A�j���[�V����CNT������.
						Player.chgAniCnt	= 0;
						Player.chgAniFlag	= false;

						// chgCnt = -1�F�u -1 �v�Œe�̔���.
						Player.chara.chgCnt = -1;
						Player.chgNumCnt	-= 1;
						P_ShotFlag( &Player, Scene, Ending );
					}
					else {
						if ( Player.chara.chgCnt > 0 ) {
							//���o�p��[4][9]���g�p.
							Player.shot[4][9].x = ( Player.chara.x + C_SIZE / 2 );
							Player.shot[4][9].y = ( Player.chara.y );

							Player.chara.chgCnt -= 3;
							if ( Player.chara.chgCnt <= 0 ) {
								Player.chara.chgCnt = 0;
							}
						}
					}
				}

				//�W�����v������.
				if ( Ending.Slime.Jumping == true )
				{
					//����: y = Vot - 1/2gt^2.
					Ending.Slime.y -= static_cast< int >( Ending.Slime.JumpPower - Ending.Slime.JumpAcc );
					Ending.Slime.JumpAcc += Ending.Slime.Gravity;		//�d�͉���.

					//���̏o��.
					if ( Ending.DTimeCnt >= Ending.DTime ) {
						//���̏o���^�C�������߂�.
						Ending.DTimeCnt = 0;
						Ending.DTime = rand() % 3 + 1;

						//���̏o��.
						for ( int dNo = 0; dNo < DROPMAX; dNo++ ) {
							if ( Ending.Drop[dNo].DispFlag == false ) {
								//�o��.
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

					//�n�ʂ�艺�ɂȂ���.
					if ( Ending.Slime.y >= Ending.Slime.GroundPos )
					{
						//�W�����vSE�̍Đ�.
						CSoundManager::PlaySE( CSoundManager::SE_Landing );

						Ending.Slime.y			= Ending.Slime.GroundPos;	//�n�ʂɂ��낦��.
						Ending.Slime.Jumping	= false;					//�W�����v������.
					}
				}

				//��ʊO����.
				if ( Ending.Slime.x < 0 ) {
					Ending.Slime.x = 0;
				}
				if ( Ending.Slime.x > WND_W - C_SIZE * 2 ) {
					Ending.Slime.x = WND_W - C_SIZE * 2;
				}

				//�A�j���[�V��������.
				Ending.Slime.animation.cnt++;
				if ( Ending.Slime.animation.cnt >= Ending.Slime.animation.cntMax ) {
					Ending.Slime.animation.cnt = 0;
					Ending.Slime.animation.No++;
					if ( Ending.Slime.animation.No >= Ending.Slime.animation.max ) {
						Ending.Slime.animation.No = 0;
					}
				}

				//���̏o��.
				if ( Ending.STimeCnt >= Ending.STime ) {
					//���̏o���^�C�������߂�.
					Ending.STimeCnt = 0;
					Ending.STime = rand() % 10 + 5;

					//�o���������߂�.
					Ending.SDispMAX = rand() % 5 + 1;
					int DispCnt = 0;

					//���̏o��.
					for ( int sNo = 0; sNo < STARMAX; sNo++ ) {
						if ( Ending.Star[sNo].DispFlag == false ) {
							//�o��.
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

							DispCnt++;	//�o�����J�E���g.

							//�S�ďo������.
							if ( DispCnt >= Ending.SDispMAX ) {
								Ending.SDispMAX = 0;
								break;
							}
						}
					}
				}
				else {
					//�t���ς̑҂����ԃJ�E���g�{.
					Ending.STimeCnt++;
				}

				//���̓���.
				for ( int sNo = 0; sNo < STARMAX; sNo++ ) {
					if ( Ending.Star[sNo].DispFlag == true ) {
						//���ꐯ�Ȃ�.
						if ( Ending.Star[sNo].Shooting == true ){
							Ending.Star[sNo].x -= Ending.Star[sNo].Spd;
							Ending.Star[sNo].y += Ending.Star[sNo].Spd;
						}
						//�����o���������ł�����.
						if ( Ending.Star[sNo].Alpha <= 255 ) {												// 0(����) ~ 255(�s����).
							if ( Ending.Star[sNo].Shooting == true ){
								Ending.Star[sNo].Alpha += Ending.Star[sNo].AlSpd * Ending.Star[sNo].Spd;	//�s�����ɂ��Ă���.
							}
							else{
								Ending.Star[sNo].Alpha += Ending.Star[sNo].AlSpd;							//�s�����ɂ��Ă���.
							}
							if ( Ending.Star[sNo].Alpha >= 255 ) {											//�s�����ɂȂ�����.
								Ending.Star[sNo].Alpha = 510;												// 255 => 511 �ɂ���(�s����).
							}
						}
						else {																				// 256(����) ~ 511(�s����).
							if ( Ending.Star[sNo].Shooting == true ){
								Ending.Star[sNo].Alpha -= Ending.Star[sNo].AlSpd * Ending.Star[sNo].Spd;	//�s�����ɂ��Ă���.
							}
							else{
								Ending.Star[sNo].Alpha -= Ending.Star[sNo].AlSpd;							//�s�����ɂ��Ă���.
							}
							if ( Ending.Star[sNo].Alpha <= 255 ) {											//�����ɂȂ�����.
								Ending.Star[sNo].Alpha		= 0;											// 256 => 0 �ɂ���(����).
								Ending.Star[sNo].Spd		= 0;
								Ending.Star[sNo].Shooting	= false;
								Ending.Star[sNo].DispFlag	= false;
							}
						}
					}
				}

				//���̓���.
				for ( int dNo = 0; dNo < DROPMAX; dNo++ ) {
					if ( Ending.Drop[dNo].DispFlag == true ) {
						//���ɗ��Ƃ��Ă���.
						Ending.Drop[dNo].y += 8;

						//�n�ʂɕt����.
						if ( Ending.Drop[dNo].y > WND_H ) {
							Ending.Drop[dNo].DispFlag = false;
						}
					}
				}

				//�v���C���[�����W�𑵂���.
				Player.chara.x = Ending.Slime.x + C_SIZE / 2;
				Player.chara.y = Ending.Slime.y + C_SIZE / 2;

				//�v���C���[�e�̓���.
				P_ShotControl( &Player, &Boss, Enemy, Scene );

				//�V���b�g�J�E���g�𑝂₷.
				Player.shotCnt++;

				//�����ҋ@������"GAME CLEAR"����ɂ����Ă���.
				if ( Ending.StopCnt <= 50 ) {
					Ending.StopCnt++;
				}
				else {
					Ending.ClearY--;
					if ( Ending.ClearY < -126 ) {
						Ending.Start = true;
					}
				}

				//�X�^�b�t���[���̕\��.
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

				//�X�^�b�t���[���̈ړ�.
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

				//�X�N���[���҂����ԃJ�E���g�{.
				if ( Ending.Start == true ) {
					Ending.DispCnt++;
				}

				//�^�C�g���ɖ߂�.
				if ( ( ( Ending.EnEng[ENGMAX - 3].DispFlag == false ) && ( Ending.EnEng[ENGMAX - 1].y <= 300.0f ) ) || ( GetAsyncKeyState( VK_F2 ) & 0x8000 ) ) {
					//�������قɓ����Ă��Ă����v�Ȃ悤�ɏ�����.
					Title.Alpha				= 255;	//���ق�PressEnterKey�Ŏg��.
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

					//�t�F�[�h�C��(�^�C�g����).
					Stop			= true;
					Fade.FadeState	= enFade::in;
					Fade.EndScene	= enScene::Title;

					//Hit�����ő�Hit����荂���ꍇ�ۑ�����.
					if ( Ending.Hit > Ending.BestHit ){

						FILE *fp = fopen( "Data\\CSV\\BESTHIT.csv", "w" );

						if ( fp == NULL ){		//�G���[���N������NULL��Ԃ�.
							return 0;
						}
						fprintf( fp, "%d", Ending.Hit );

						fclose( fp );			//�t�@�C�������.
					}

					//�^�C�g��BGM�̍Đ�.
					CSoundManager::Stop( CSoundManager::BGM_Ending );
					CSoundManager::PlayLoop( CSoundManager::BGM_Title );
				}

				//�e�ƃX�^�b�t���[���̓����蔻��.
				for ( int numNo = 0; numNo < Player.status.num; numNo++ ) {
					for ( int dirNo = 0; dirNo < Player.status.dir; dirNo++ ) {
						//���@�̒e�����˂���Ă���.
						if ( Player.shot[numNo][dirNo].DispShotFlag == true ) {
							for ( int eNo = 0; eNo < ENGMAX; eNo++ ) {
								//�X�^�b�t���[�����o�Ă���.
								if ( ( Ending.EnEng[eNo].HitFlag == false ) && ( Ending.EnEng[eNo].DispFlag == true ) && ( eNo != 32 ) && ( eNo != 34 ) ) {
									//���@�̒e�ƃX�^�b�t���[���̓����蔻��.
									if ( ( ( Player.ability.chg == 0 ) && ( CollisionDetection(
										 ( int ) Player.shot[numNo][dirNo].x,			( int ) Player.shot[numNo][dirNo].y,			S_SIZE,						S_SIZE,
										 ( int ) Ending.EnEng[eNo].x,					( int ) Ending.EnEng[eNo].y,					Ending.EnEng[eNo].wDest,	Ending.EnEng[eNo].hDest ) ) ) ||

										 ( ( Player.ability.chg != 0 ) && ( CollisionDetection(
										 ( int ) Player.shot[numNo][dirNo].x - S_SIZE,	( int ) Player.shot[numNo][dirNo].y - S_SIZE,	S_SIZE * 2,					S_SIZE * 2,
											( int ) Ending.EnEng[eNo].x,				( int ) Ending.EnEng[eNo].y,					Ending.EnEng[eNo].wDest,	Ending.EnEng[eNo].hDest ) ) ) ) {

										//�G�̃_���[�WSE��炷.
										CSoundManager::PlaySE( CSoundManager::SE_EDamage );

										//�X�^�b�t���[����HP����.
										Ending.EnEng[eNo].HitFlag = true;

										//1000�ȏ�ɂȂ�ƕ\�����o�O��̂�999�Ŏ~�߂�.
										if ( Ending.Hit < 1000 ){
											//�ō�Hit���𒴂�������SE��炷.
											if ( ( Ending.Hit <= Ending.BestHit ) && ( Ending.Hit + 1 > Ending.BestHit ) ){
												//�ō�Hit���X�VSE��炷.
												CSoundManager::PlaySE( CSoundManager::SE_Best );
											}

											//Hit�����J�E���g.
											Ending.Hit++;
											if ( Ending.Hit >= 1000 ){
												Ending.Hit = 999;
											}
										}

										//���@�̒e�̒��e��̏���.
										if ( Player.ability.pene == 0 ) {	//�ђʂ������Ă��Ȃ�.
											if ( Player.ability.beam != 0 ) {
												//�r�[���̃q�b�g��+2.
												Player.shot[numNo][dirNo].beamCnt -= 2;
												if ( Player.shot[numNo][dirNo].beamCnt == 0 ) {

													Player.shot[numNo][dirNo].DispShotFlag	= false;	//�����׸ނ��~�낷.
													Player.shot[numNo][dirNo].beamCnt		= BEAMMAX;	//�r�[���̃q�b�g�񐔂�������.
													Player.shot[numNo][dirNo].x				= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
													Player.shot[numNo][dirNo].y				= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
												}
											}
											else {
												Player.shot[numNo][dirNo].DispShotFlag		= false;	//�����׸ނ��~�낷.
												Player.shot[numNo][dirNo].x					= -WND_W;	//���@�̒ex���W:��ʂ̊O��.
												Player.shot[numNo][dirNo].y					= -WND_H;	//���@�̒ey���W:��ʂ̊O��.
											}
										}
									}
								}
							}
						}
					}
				}

				//�e�����������X�^�b�t���[���̓���.
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
			default:	//��L�ȊO�̏ꍇ.
				//��{�����ɂ͗��Ȃ��͂�.
				break;
			}
		}
		//�t�F�[�h�C��.
		if ( Fade.FadeState == enFade::in ) {
			if ( Fade.FadeAlpha < Fade.EndAlpha ){
				Fade.FadeAlpha += 5;
				if ( Fade.FadeAlpha >= Fade.EndAlpha ){
					Fade.FadeAlpha = Fade.EndAlpha;
					if ( Fade.InOutFlag == true ) {
						Fade.FadeState	= enFade::out;
						Scene			= Fade.EndScene;

						//�������قɓ����Ă��Ă����v�Ȃ悤�ɏ�����.
						Title.Alpha		= 255;
					}
				}
			}
		}
		//�t�F�[�h�A�E�g.
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
		//�|�[�Y.
		if ( pause == true ){

			//BGM�Đ�( ���[�v ).
			if ( Boss.chara[enBoss::Main].State == enCharaState::Wait ) {
				CSoundManager::PlayLoop( CSoundManager::BGM_Main );
			}
			else {
				CSoundManager::PlayLoop( CSoundManager::BGM_Boss );
			}

			//�J�[�\���̓_��.
			if ( pauseAlpha <= 255 ) {		// 0(�s����) ~ 255(����).
				pauseAlpha += 3;			//�����ɂ��Ă���.
				if ( pauseAlpha >= 255 ) {	//�����ɂȂ�����.
					pauseAlpha = 510;		// 255 => 511 �ɂ���(����).
				}
			}
			else {							// 256(�s����) ~ 511(����).
				pauseAlpha -= 3;			//�s�����ɂ��Ă���.
				if ( pauseAlpha <= 255 ) {	//�s�����ɂȂ�����.
					pauseAlpha = 0;			// 256 => 0 �ɂ���(�s����).
				}
			}

			//�� or LSU.
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
			//�� LSD.
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
			//�X�y�[�X�L�[ or RT.
			if ( GetAsyncKeyState( VK_SPACE ) & 0x8000 || XInKey.trgR >= CXInput::INPUT_TRIGGER_MAX ){
				Player.PushKey[enPushKey::Space] = true;
			}
			else if ( Player.PushKey[enPushKey::Space] == true ) {

				Player.PushKey[enPushKey::Space] = false;

				if ( pauseArea == 0 ){
					//����SE��炷.
					CSoundManager::PlaySE( CSoundManager::SE_Decision );

					//�t�F�[�h�A�E�g.
					pause = false;
					Fade.InOutFlag = true;
					Fade.FadeState = enFade::out;
					Fade.EndAlpha = 255;
				}
				else{
					//����SE��炷.
					CSoundManager::PlaySE( CSoundManager::SE_Decision );

					//�������قɓ����Ă��Ă����v�Ȃ悤�ɏ�����.
					Title.Alpha				= 255;	//���ق�PressEnterKey�Ŏg��.
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
					//�t�F�[�h�C��(�^�C�g����).
					pause			= false;
					Stop			= true;
					Fade.InOutFlag	= true;
					Fade.EndAlpha	= 255;
					Fade.FadeState	= enFade::in;
					Fade.EndScene	= enScene::Title;

					//�^�C�g��BGM�̍Đ�.
					CSoundManager::Stop( CSoundManager::BGM_Main );
					CSoundManager::Stop( CSoundManager::BGM_Boss );
					CSoundManager::PlayLoop( CSoundManager::BGM_Title );
				}
			}
		}

//-------------------------------------------------------------------.
//	�`�揈��[WM_PAINT].
//-------------------------------------------------------------------.
		//��݂��Ƃ̏���.
		switch( Scene ){
		case enScene::Title:	//����.
		{
			//���ى摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hsky );
			//���ى摜�̕\��.
			BitBlt(
				gameWnd->hScreenDC,				//���޲���÷��.
				0, 0,							//�\���ʒux,y���W.
				WND_W, WND_H,					//�摜�̕�,����.
				hMemDC,							//���DC.
				0, 0,							//���摜��x,y���W.
				SRCCOPY );						//��߰����.

			//���ى摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hcloud );
			//��̉_1�̕\��.
			TransBlt(
				gameWnd->hScreenDC,				//���޲���÷��.
				( int ) ( Title.UPCloudX1 ), 0, //�\���ʒux,y���W.
				256, 128,						//�摜�̕�,����.
				hMemDC,							//���DC.	
				0, 256 * 2 );					//���摜��x,y���W.
			//���ى摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hcloud );
			//��̉_2�̕\��.
			TransBlt(
				gameWnd->hScreenDC,				//���޲���÷��.
				( int ) ( Title.UPCloudX2 ), 0,	//�\���ʒux,y���W.
				446, 128,						//�摜�̕�,����.
				hMemDC,							//���DC.	
				256, 256 * 2 );					//���摜��x,y���W.

			//���ى摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hcloud );
			//���̉_1�̕\��.
			TransBlt(
				gameWnd->hScreenDC,				//���޲���÷��.
				( int ) ( Title.DownCloudX1 ),	//�\���ʒux���W.
				WND_H - ( 256 + 45 ),			//�\���ʒuy���W.
				WND_W, 256,						//�摜�̕�,����.
				hMemDC,							//���DC.	
				0, 256 );						//���摜��x,y���W.
			//���ى摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hcloud );
			//���̉_2�̕\��.
			TransBlt(
				gameWnd->hScreenDC,				//���޲���÷��.
				( int ) ( Title.DownCloudX2 ),	//�\���ʒux���W.
				WND_H - ( 256 + 45 ),			//�\���ʒuy���W.
				WND_W, 256,						//�摜�̕�,����.
				hMemDC,							//���DC.	
				0, 0 );							//���摜��x,y���W.

			//���ى摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hTitle );
			//��𕑂��t���ϕ`��.
			for ( int LNo = 0; LNo < LEAFMAX; LNo++ ) {
				if ( Title.Leaf[LNo].DispFlag == true ) {
					TransPlgBlt(
						gameWnd->hScreenDC,									//�f�o�C�X�R���e�L�X�g.
						Title.Leaf[LNo].x,									//�\���ʒux���W.
						Title.Leaf[LNo].y,									//�\���ʒuy���W.
						16, 16,												//�摜�̕�,����.
						hMemDC,												//������DC.
						LOGO_W + 25 + 16 * Title.Leaf[LNo].PtnX,			//���摜��x���W.
						WND_H + TITLE_H + 80 + 16 * Title.Leaf[LNo].PtnY, 	//���摜��y���W.
						Title.Leaf[LNo].Ang,								//��]�p�x.
						hPlgDC );											//��]�pDC.
				}
			}

			//�X���C���摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hSlime );
			//�X���C���̕\��.
			TransAlBlend(
				gameWnd->hScreenDC,				//�f�o�C�X�R���e�L�X�g.
				WND_W - 128 * 2, WND_H - 115,	//�\���ʒux,y���W.
				128, 64,						//�摜�̕�,����.
				hMemDC,							//������DC.
				128 * Title.Panimation.No,		//���摜��x���W.
				192,							//���摜��y���W.
				200,							//�A���t�@�l.
				hWorkDC );						//��ƗpDC.

			//�X���C���摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hSlime );
			//�n�ʂ̕\��.
			TransBlt(
				gameWnd->hScreenDC,				//�f�o�C�X�R���e�L�X�g.
				0, WND_H - 96,					//�\���ʒux,y���W.
				WND_W, 96,						//�摜�̕�,����.
				hMemDC,							//������DC.
				0, 0 );							//���摜��x,y���W.

			//���ى摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hTitle );
			//���S�̕\��.
			TransBlt(
				gameWnd->hScreenDC,				//�f�o�C�X�R���e�L�X�g.
				145, 131,						//�\���ʒux,y���W.
				LOGO_W, LOGO_H,					//�摜�̕�,����.
				hMemDC,							//������DC.
				0, WND_H + TITLE_H );			//���摜��x,y���W.

			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hcloud );
			//"GAME START"�摜�̕\��.
			TransBlt(
				gameWnd->hScreenDC,				//���޲���÷��.
				509, 550,						//�\���ʒux,y���W.
				256, 64,						//�摜�̕�,����.
				hMemDC,							//���DC.
				256 + 448, 256 * 2 );			//���摜��x,y���W.

			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hcloud );
			//"PRESS ANY BUTTON"�摜�̕\��.
			TransAlBlend(
				gameWnd->hScreenDC,				//���޲���÷��.
				544, 595,						//�\���ʒux,y���W.
				200, 16,						//�摜�̕�,����.
				hMemDC,							//���DC.
				256 + 448, 256 * 2 + 64,		//���摜��x,y���W.
				Title.Alpha,					//�A���t�@�l.
				hWorkDC );

			break;
		}
		case enScene::GameMain:	//�ް�Ҳ�.
		{
			//�Q�[���w�i(�X�N���[��)�̉摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hscroll );
			//�Q�[���w�i(�X�N���[��)�̕\��.
			BitBlt(
				gameWnd->hScreenDC,					//���޲���÷��.
				( WND_W / 2 ) - ( GWND_W / 2 ),		//�\���ʒux���W.
				0,									//�\���ʒuy���W.
				GWND_W, GWND_H,						//�摜�̕�,����.
				hMemDC,								//���DC.
				0, static_cast< int >( scroll.y ),	//���摜��x,y���W.
				SRCCOPY );							//��߰����.
			//�Q�[���w�i(�T�u�X�N���[��)�̕\��.
			if ( scroll.y < 0 ) {
				BitBlt(
					gameWnd->hScreenDC,									//���޲���÷��.
					( WND_W / 2 ) - ( GWND_W / 2 ),						//�\���ʒux���W.
					0,													//�\���ʒuy���W.
					GWND_W, static_cast< int >( -scroll.y ),			//�摜�̕�,����.
					hMemDC,												//���DC.
					0,													//���摜��x���W.
					static_cast< int >( GWND_H * 2 + scroll.y ),		//���摜��y���W.
					SRCCOPY );											//��߰����.
			}

			//�{�X�̓˂��グ��.
			for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ) {
				for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
					for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
						if ( Boss.rock[typeNo][numNo][dirNo].DispFlag == true ) {
							//�L�����N�^�̉摜��������DC�փR�s�[.
							SelectObject( hMemDC, hboss );
							//�{�X�̓˂��グ��̕\��.
							TransBlt(
								gameWnd->hScreenDC,																//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( C_CORR + Boss.rock[typeNo][numNo][dirNo].x ),				//�\���ʒux���W.
								static_cast< int >( Boss.rock[typeNo][numNo][dirNo].y ),						//�\���ʒuy���W.
								S_SIZE * 2, ( S_SIZE / 2 ) * ( Boss.rock[typeNo][numNo][dirNo].cnt / 10 + 1 ),	//�摜�̕�,����.
								hMemDC,																			//������DC.
								S_SIZE * 3,																		//���摜��x���W
								S_SIZE * 8 );																	//���摜��y���W.
						}
					}
				}
			}

			//�X�e�[�W�M�~�b�N.
			for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
					for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
						if ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GWait ) {
							if ( typeNo == 0 ) {	//��.
								if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GLiving ) {		//��.
									//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hTree );
									//�X�e�[�W�M�~�b�N�̕\��.
									TransBlt(
										gameWnd->hScreenDC,														//���޲���÷��.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//�\���ʒux���W.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//�\���ʒuy���W.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//�摜�̕�,����.
										hMemDC,																	//���DC.
										0, C_SIZE + gimmick[typeNo][ptnNo][gNo].hSize * ptnNo );				//���摜��x,y���W.
								}
								else if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GInvi ) {	//��(�_���[�W).
									//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hTreeHit );
									//�X�e�[�W�M�~�b�N�̕\��.
									TransBlt(
										gameWnd->hScreenDC,														//���޲���÷��.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//�\���ʒux���W.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//�\���ʒuy���W.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//�摜�̕�,����.
										hMemDC,																	//���DC.
										C_SIZE * 4 + gimmick[typeNo][ptnNo][gNo].wSize * ptnNo, BLOW_H );			//���摜��x,y���W.
								}
								else if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GDead ) {	//�؂芔.
									//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hTree );
									//�X�e�[�W�M�~�b�N�̕\��.
									TransBlt(
										gameWnd->hScreenDC,																//���޲���÷��.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),					//�\���ʒux���W.
										static_cast< int >( C_SIZE + gimmick[typeNo][ptnNo][gNo].y - C_SIZE / 2 - 3 ),	//�\���ʒuy���W.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//�摜�̕�,����.
										hMemDC,																			//���DC.
										C_SIZE * 3,																		//���摜��x���W.
										gimmick[typeNo][ptnNo][gNo].hSize * ptnNo );									//���摜��y���W.
								}
							}
							else if ( typeNo == 1 ) {	//��.
								//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
								SelectObject( hMemDC, hGmck );
								if ( gimmick[typeNo][ptnNo][gNo].RockFlag == false ) {
									//�X�e�[�W�M�~�b�N�̕\��.
									TransBlt(
										gameWnd->hScreenDC,														//���޲���÷��.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//�\���ʒux���W.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//�\���ʒuy���W.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//�摜�̕�,����.
										hMemDC,																	//���DC.
										C_SIZE * 2 + gimmick[typeNo][ptnNo][gNo].hSize * ptnNo, 0 );			//���摜��x,y���W.
								}
								else {
									for ( int bty = 1; bty < Boss.BOSS_TYPE; bty++ ) {
										if ( Boss.AttackNo[bty] == enBossAttackNo::BRock ) {
											//�X�e�[�W�M�~�b�N�̕\��.
											TransBlt(
												gameWnd->hScreenDC,															//���޲���÷��.
												static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),				//�\���ʒux���W.
												static_cast< int >( gimmick[typeNo][ptnNo][gNo].y + ( gimmick[typeNo][ptnNo][gNo].hSize - ( gimmick[typeNo][ptnNo][gNo].hSize * 100 / Boss.animation[bty].cntMax * Boss.animation[bty].cnt / 100 ) ) ),						//�\���ʒuy���W.
												gimmick[typeNo][ptnNo][gNo].wSize,
												( gimmick[typeNo][ptnNo][gNo].hSize * 100 / Boss.animation[bty].cntMax ) * Boss.animation[bty].cnt / 100,				//�摜�̕�,����.
												hMemDC,																		//���DC.
												C_SIZE * 2 + gimmick[typeNo][ptnNo][gNo].hSize * ptnNo, 0 ); 				//���摜��x,y���W.
										}
									}
								}
							}
							else if ( typeNo == 2 ) {	//��.
								if( ptnNo == 0 ){	//��̂Ƃ�.
									//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hPSlime );
									//�X�e�[�W�M�~�b�N�̕\��.
									TransBlt(
										gameWnd->hScreenDC,														//���޲���÷��.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//�\���ʒux���W.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//�\���ʒuy���W.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//�摜�̕�,����.
										hMemDC,																	//���DC.
										C_SIZE * 2, C_SIZE * 2 );												//���摜��x,y���W.
								}
								else{				//�ł̏�.
									//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hSlime );
									//�X�e�[�W�M�~�b�N�̕\��.
									TransBlt(
										gameWnd->hScreenDC,														//���޲���÷��.
										static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//�\���ʒux���W.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//�\���ʒuy���W.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//�摜�̕�,����.
										hMemDC,																	//���DC.
										C_SIZE * 11, 192 );														//���摜��x,y���W.

									//�ŏ��̂Ԃ��Ԃ��A�j���[�V����.
									for ( int pNo = 0; pNo < P_MAX; pNo++ ) {
										if ( Poison[gNo][pNo].DispFlag == true ) {
											//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
											SelectObject( hMemDC, hTreeHit );
											//�Ԃ��Ԃ��̕\��.
											TransAlBlend(
												gameWnd->hScreenDC,												//���޲���÷��.
												static_cast< int >( C_CORR + Poison[gNo][pNo].x ),				//�\���ʒux���W.
												static_cast< int >( Poison[gNo][pNo].y ),						//�\���ʒuy���W.
												18, 18,															//�摜�̕�,����.
												hMemDC,															//���DC.
												447 + 18 * ( Poison[gNo][pNo].PtnNo / 2 ),						//���摜��x���W.
												BLOW_H + 18 * ( Poison[gNo][pNo].PtnNo % 2 ),					//���摜��y���W.
												Poison[gNo][pNo].Alpha,
												hWorkDC );
										}
									}
								}
							}
							else if ( typeNo == 3 ) {	//�|��.
								//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
								SelectObject( hMemDC, harea );
								//�X�e�[�W�M�~�b�N�̕\��.
								TransBlt(
									gameWnd->hScreenDC,															//���޲���÷��.
									static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),				//�\���ʒux���W.
									static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),						//�\���ʒuy���W.
									gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,		//�摜�̕�,����.
									hMemDC,																		//���DC.
									ABLT_W + C_SIZE * ptnNo, ( PHP_H * 2 + 6 ) * 3 + C_SIZE * 2 );				//���摜��x,y���W.
							}
						}
					}
				}
			}

			//�G(���S).
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[typeNo][ptnNo][eNo].appFlag == true ) || ( Enemy[typeNo][ptnNo][eNo].BossFlag == true ) ) {
							//�G�@�����S.
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dead )
							{
								if ( typeNo != 0 ) {	//�ԈȊO.
									//��׸��̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hCharaDeat );
									//�G�@�̕\��.
									TransBlt(
										gameWnd->hScreenDC,													//���޲���÷��.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),	//�\���ʒux���W.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),			//�\���ʒuy���W.
										C_SIZE, C_SIZE,														//�摜�̕�,����.
										hMemDC,																//���DC.
										C_SIZE * ( ( typeNo + 1 ) % 3 ),									//���摜��x���W.
										C_SIZE * ( ( typeNo + 1 ) / 3 ) );									//���摜��y���W.
								}
								else {	//��.
									//��׸��̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hCharaDeat );
									//�G�@�̕\��.
									TransBlt(
										gameWnd->hScreenDC,													//���޲���÷��.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),	//�\���ʒux���W.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),			//�\���ʒuy���W.
										C_SIZE, C_SIZE,														//�摜�̕�,����.
										hMemDC,																//���DC.
										C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No / 3 ), 0 );		//���摜��x,y���W.
								}
							}
						}
					}
				}
			}

			//�{�X�̐�����΂��G�t�F�N�g.
			if ( Boss.BlowAway.DispFlag == true ) {
				//��׸��̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hTreeHit );
				//�G�@�̕\��.
				TransAlBlend(
					gameWnd->hScreenDC,													//���޲���÷��.
					static_cast< int >( C_CORR + 0 ),									//�\���ʒux���W.
					static_cast< int >( Boss.BlowAway.y ),								//�\���ʒuy���W.
					GWND_W, BLOW_H,														//�摜�̕�,����.
					hMemDC,																//���DC.
					0, 0,																//���摜��x,y���W.
					200,																//�A���t�@�l.
					hWorkDC );															//��ƗpDC.
			}

			//���@��������.
			if ( Player.chara.State == enCharaState::Living )
			{
				if ( Player.SlimeNo == enSlime::SNormal ){
					if ( Player.AbGetFlag == true ){
						//��׸��̉摜�����DC�ֺ�߰.
						SelectObject( hMemDC, hEnEng );
						//���@�̕\��.
						ExpaTransAlBlend(
							gameWnd->hScreenDC,													//�f�o�C�X�R���e�L�X�g.
							static_cast< int >( C_CORR + Player.chara.x - Player.GetAnimaX ),	//�\���ʒux���W( �␳�l:13 ).
							static_cast< int >( Player.chara.y + Player.GetAnimaY ),			//�\���ʒuy���W( �␳�l:13 ).
							C_SIZE, C_SIZE,														//�摜�̕�,����.
							hMemDC,																//������DC.
							288 + C_SIZE * Player.animation.No,									//���摜��x���W.
							336,																//���摜��y���W.
							200,																//�A���t�@�l.
							hWorkDC,															//��ƗpDC.
							C_SIZE + Player.GetAnimaCnt * 2,									//�T�C�Y�ύX��̕�.
							C_SIZE - Player.GetAnimaCnt * 2,									//�T�C�Y�ύX��̍���.
							false );															//�����g������Ȃ�(�E���g��).
					}
					else{
						//��׸��̉摜�����DC�ֺ�߰.
						SelectObject( hMemDC, hEnEng );
						//���@�̕\��.
						TransPlgAlBlend(
							gameWnd->hScreenDC,													//�f�o�C�X�R���e�L�X�g.
							static_cast< int >( C_CORR + Player.chara.x - 13 ),					//�\���ʒux���W( �␳�l:13 ).
							static_cast< int >( Player.chara.y - 13 ),							//�\���ʒuy���W( �␳�l:13 ).
							C_SIZE, C_SIZE,														//�摜�̕�,����.
							hMemDC,																//������DC.
							288 + C_SIZE * Player.animation.No,									//���摜��x���W.
							336,																//���摜��y���W.
							Player.chara.ang,													//�摜�p�x.
							hPlgDC,																//��]�pDC.
							200,																//�A���t�@�l.
							hWorkDC );															//��ƗpDC.
					}
				}
				else{
					if ( Player.AbGetFlag == true ){
						//��׸��̉摜�����DC�ֺ�߰.
						SelectObject( hMemDC, hPSlime );
						//���@�̕\��.
						ExpaTransAlBlend(
							gameWnd->hScreenDC,													//�f�o�C�X�R���e�L�X�g.
							static_cast< int >( C_CORR + Player.chara.x - Player.GetAnimaX ),	//�\���ʒux���W( �␳�l:13 ).
							static_cast< int >( Player.chara.y + Player.GetAnimaY ),			//�\���ʒuy���W( �␳�l:13 ).
							C_SIZE, C_SIZE,														//�摜�̕�,����.
							hMemDC,																//������DC.
							C_SIZE * ( ( Player.SlimeNo * 2 ) % 4 + Player.animation.No ),		//���摜��x���W.
							C_SIZE * ( ( Player.SlimeNo * 2 ) / 4 ),							//���摜��y���W.
							200,																//�A���t�@�l.
							hWorkDC,															//��ƗpDC.
							C_SIZE + Player.GetAnimaCnt * 2,									//�T�C�Y�ύX��̕�.
							C_SIZE - Player.GetAnimaCnt * 2,									//�T�C�Y�ύX��̍���.
							false );															//�����g������Ȃ�(�E���g��).
					}
					else{
						//��׸��̉摜�����DC�ֺ�߰.
						SelectObject( hMemDC, hPSlime );
						//���@�̕\��.
						TransPlgAlBlend(
							gameWnd->hScreenDC,													//�f�o�C�X�R���e�L�X�g.
							static_cast< int >( C_CORR + Player.chara.x - 13 ),					//�\���ʒux���W( �␳�l:13 ).
							static_cast< int >( Player.chara.y - 13 ),							//�\���ʒuy���W( �␳�l:13 ).
							C_SIZE, C_SIZE,														//�摜�̕�,����.
							hMemDC,																//������DC.
							C_SIZE * ( ( Player.SlimeNo * 2 ) % 4 + Player.animation.No ),		//���摜��x���W.
							C_SIZE * ( ( Player.SlimeNo * 2 ) / 4 ),							//���摜��y���W.
							Player.chara.ang,													//�摜�p�x.
							hPlgDC,																//��]�pDC.
							235,																//�A���t�@�l.
							hWorkDC );															//��ƗpDC.
					}
				}
			}
			//���@�����G.
			else if ( Player.chara.State == enCharaState::Invi )
			{
				if ( Player.SlimeNo == enSlime::SNormal ){
					//��׸��̉摜�����DC�ֺ�߰.
					SelectObject( hMemDC, hEnEng );
					//���@�̕\��.
					TransPlgAlBlend(
						gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Player.chara.x - 13 ),						//�\���ʒux���W( �␳�l:13 ).
						static_cast< int >( Player.chara.y - 13 ),								//�\���ʒuy���W( �␳�l:13 ).
						C_SIZE, C_SIZE,															//�摜�̕�,����.
						hMemDC,																	//������DC.
						416 + C_SIZE * Player.animation.No,										//���摜��x���W.
						336,																	//���摜��y���W.
						Player.chara.ang,														//�摜�p�x.
						hPlgDC,																	//��]�pDC.
						200,																	//�A���t�@�l.
						hWorkDC );																//��ƗpDC.
				}
				else{
					//��׸��̉摜�����DC�ֺ�߰.
					SelectObject( hMemDC, hPSlimeHit );
					//���@�̕\��.
					TransPlgAlBlend(
						gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Player.chara.x - 13 ),						//�\���ʒux���W( �␳�l:13 ).
						static_cast< int >( Player.chara.y - 13 ),								//�\���ʒuy���W( �␳�l:13 ).
						C_SIZE, C_SIZE,															//�摜�̕�,����.
						hMemDC,																	//������DC.
						C_SIZE * ( ( Player.SlimeNo * 2 ) % 4 + Player.animation.No ),			//���摜��x���W.
						C_SIZE * ( ( Player.SlimeNo * 2 ) / 4 ),								//���摜��y���W.
						Player.chara.ang,														//�摜�p�x.
						hPlgDC,																	//��]�pDC.
						235,																	//�A���t�@�l.
						hWorkDC );																//��ƗpDC.
				}
			}
			//���@�����S��.
			else if ( Player.chara.State == enCharaState::Dying )
			{
				if ( Player.SlimeNo == enSlime::SNormal ){
					//��׸��̉摜�����DC�ֺ�߰.
					SelectObject( hMemDC, hSlime );
					//���@�̕\��.
					ExpaTransAlBlend(
						gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Player.chara.x ),							//�\���ʒux���W( �␳�l:13 ).
						static_cast< int >( Player.chara.y ),									//�\���ʒuy���W( �␳�l:13 ).
						C_SIZE, C_SIZE,															//�摜�̕�,����.
						hMemDC,																	//������DC.
						128 * 3 + C_SIZE * ( Player.animation.No * 2 ),							//���摜��x���W.
						192,																	//���摜��y���W.
						200,																	//�A���t�@�l.
						hWorkDC,																//��ƗpDC.
						C_SIZE + Player.chara.ExpAnimCnt * 2,									//�T�C�Y�ύX��̕�.
						C_SIZE - Player.chara.ExpAnimCnt * 2,									//�T�C�Y�ύX��̍���.
						false );																//�����g������Ȃ�(�E���g��).
				}
				else{
					//��׸��̉摜�����DC�ֺ�߰.
					SelectObject( hMemDC, hPSlime );
					//���@�̕\��.
					ExpaTransAlBlend(
						gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Player.chara.x ),							//�\���ʒux���W( �␳�l:13 ).
						static_cast< int >( Player.chara.y ),									//�\���ʒuy���W( �␳�l:13 ).
						C_SIZE, C_SIZE,															//�摜�̕�,����.
						hMemDC,																	//������DC.
						C_SIZE * ( ( Player.SlimeNo * 2 ) % 4 + Player.animation.No ),			//���摜��x���W.
						C_SIZE * ( ( Player.SlimeNo * 2 ) / 4 ),								//���摜��y���W.
						235,																	//�A���t�@�l.
						hWorkDC,																//��ƗpDC.
						C_SIZE + Player.chara.ExpAnimCnt * 2,									//�T�C�Y�ύX��̕�.
						C_SIZE - Player.chara.ExpAnimCnt * 2,									//�T�C�Y�ύX��̍���.
						false );																//�����g������Ȃ�(�E���g��).
				}
			}

			//�v���C���[�̃_���[�W�摜.
			for( int i = 0; i < 2; i++ ){
				if ( Player.Damage[i].DispFlag == true ){
					//��׸��̉摜�����DC�ֺ�߰.
					SelectObject( hMemDC, hPSlime );
					//���@�̕\��.
					TransBlt(
						gameWnd->hScreenDC,										//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Player.Damage[i].x ),		//�\���ʒux���W( �␳�l:13 ).
						static_cast< int >( Player.Damage[i].y ),				//�\���ʒuy���W( �␳�l:13 ).
						23, 23,													//�摜�̕�,����.
						hMemDC,													//������DC.
						C_SIZE * 3,												//���摜��x���W.
						C_SIZE * 2 + 16 * 2 );									//���摜��y���W.
				}
			}

			//�v���C���[�̉񕜃G�t�F�N�g.
			for ( int hNo = 0; hNo < H_MAX; hNo++ ){
				if ( Player.HeelAnima[hNo].DispFlag == true ){
					//��׸��̉摜�����DC�ֺ�߰.
					SelectObject( hMemDC, harea );
					//���@�̕\��.
					TransAlBlend(
						gameWnd->hScreenDC,																								//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Player.HeelAnima[hNo].x ),															//�\���ʒux���W( �␳�l:13 ).
						static_cast< int >( Player.HeelAnima[hNo].y - Player.HeelAnima[hNo].UpY ),										//�\���ʒuy���W( �␳�l:13 ).
						C_SIZE / 2, C_SIZE / 2,																							//�摜�̕�,����.
						hMemDC,																											//������DC.
						ABLT_W + C_SIZE * 3 + C_SIZE / 2 * Player.HeelAnima[hNo].PtnX,													//���摜��x���W.
						( PHP_H * 2 + 5 ) * 3 + C_SIZE * Player.HeelAnima[hNo].PtnY + C_SIZE / 2 * Player.HeelAnima[hNo].animation.No,	//���摜��y���W.
						Player.HeelAnima[hNo].Alpha,																					//�A���t�@�l.
						hWorkDC );																										//��ƗpDC.
				}
			}

			//�{�X�{��.
			if ( Boss.chara[enBoss::Main].State == enCharaState::Anima ) {	//�o��A�j���[�V����.
				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽�{�̂̕\��.
				TransBlt(
					gameWnd->hScreenDC,															//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x ),					//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::Main].y ),							//�\���ʒuy���W.
					Boss.charaSize[enBoss::Main],												//�摜�̕�.
					( Boss.charaSize[enBoss::Main] / Boss.DispAnima.max ) * Boss.DispAnima.No,	//�摜�̍���.
					hMemDC,																		//���DC.
					0, 0 );																		//���摜��x,y���W.
			}
			else if ( Boss.chara[enBoss::Main].State == enCharaState::Living ) {	//������.
				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽�{�̂̕\��.
				TransBlt(
					gameWnd->hScreenDC,											//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x ),	//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::Main].y ),			//�\���ʒuy���W.
					Boss.charaSize[enBoss::Main], Boss.charaSize[enBoss::Main],	//�摜�̕�,����.
					hMemDC,														//���DC.
					0, 0 );														//���摜��x,y���W.
			}
			else if ( Boss.chara[enBoss::Main].State == enCharaState::Invi ) {	//���G����.
				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, harea );
				//�޽�{�̂̕\��.
				TransBlt(
					gameWnd->hScreenDC,												//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x ),		//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::Main].y ),				//�\���ʒuy���W.
					Boss.charaSize[enBoss::Main], Boss.charaSize[enBoss::Main],		//�摜�̕�,����.
					hMemDC,															//���DC.
					ABLT_W + 1, ( PHP_H * 2 + 6 ) * 3 );							//���摜��x,y���W.
			}
			else if ( Boss.chara[enBoss::Main].State == enCharaState::Dying ) {	//���S��.
				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽�{�̂̕\��.
				TransBlt(
					gameWnd->hScreenDC,																					//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x ),											//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::Main].y ),													//�\���ʒuy���W.
					Boss.charaSize[enBoss::Main],																		//�摜�̕�.
					( Boss.charaSize[enBoss::Main] / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ),	//�摜�̍���.
					hMemDC,																								//���DC.
					0, 0 );																								//���摜��x,y���W.
			}

			//�{�X�E��.
			if ( Boss.chara[enBoss::RHand].State == enCharaState::Anima ) {	//�o��A�j���[�V����.
				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hTree );
				//�޽�E��̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																				//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::RHand].x - 20 ),								//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::RHand].y - 5 - Boss.animation[enBoss::Main].No ),		//�\���ʒuy���W.
					Boss.charaSize[enBoss::RHand],																	//�摜�̕�.
					( ( Boss.charaSize[enBoss::RHand] * 100 / Boss.DispAnima.max ) * Boss.DispAnima.No ) / 100,		//�摜�̍���.
					hMemDC,																							//���DC.
					0, 0,																							//���摜��x,y���W.
					Boss.chara[enBoss::RHand].ang,																	//�摜�p�x.
					hPlgDC );																						//�摜��]�pDC.

				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽�E���̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																												//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x + Boss.charaSize[0] - 20 ),												//�\���ʒux.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::RHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),//�\���ʒuy.
					C_SIZE / 2 + S_SIZE,																											//�摜�̕�.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * Boss.DispAnima.No ) / 100,													//�摜�̍���.
					hMemDC,																															//���DC.
					0, C_SIZE * 2 + C_SIZE / 2,																										//���摜��x,y.
					Boss.chara[enBoss::RHand].ang,																									//�p�x.
					hPlgDC );																														//�摜��]�pDC.
			}
			else if ( Boss.chara[enBoss::RHand].State == enCharaState::Living ) {	//������.
				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hTree );
				//�޽�E��̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																		//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::RHand].x - 20 ),						//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::RHand].y - 5 - Boss.animation[enBoss::Main].No ),//�\���ʒuy���W.
					Boss.charaSize[enBoss::RHand], Boss.charaSize[enBoss::RHand],							//�摜�̕�,����.
					hMemDC,																					//���DC.
					0, 0,																					//���摜��x,y���W.
					Boss.chara[enBoss::RHand].ang,															//�p�x.
					hPlgDC );																				//�摜��]�pDC.

				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽�E���̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																										//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x + Boss.charaSize[enBoss::Main] - 20 ),							//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::RHand] - 20 + ( Boss.animation[0].No / 2 ) ),	//�\���ʒuy���W.
					C_SIZE / 2 + S_SIZE, C_SIZE / 2,																						//�摜�̕�,����.
					hMemDC,																													//���DC.
					0, C_SIZE * 2 + C_SIZE / 2,																								//���摜��x,y���W.
					Boss.chara[enBoss::RHand].ang,																							//�p�x.
					hPlgDC );																												//�摜��]�pDC.
			}
			else if ( Boss.chara[enBoss::RHand].State == enCharaState::Invi ) {	//���G����.
				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, harea );
				//�޽�E��̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																										//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::RHand].x - 20 ),														//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::RHand].y - 5 - Boss.animation[enBoss::Main].No ),								//�\���ʒuy���W.
					Boss.charaSize[enBoss::RHand], Boss.charaSize[enBoss::RHand],															//�摜�̕�,����.
					hMemDC,																													//���DC.
					ABLT_W + C_SIZE * 2, ( PHP_H * 2 + 5 ) * 3 + C_SIZE + 3,																	//���摜��x,y���W.
					Boss.chara[enBoss::RHand].ang,																							//�p�x.
					hPlgDC );																												//�摜��]�pDC.

				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, harea );
				//�޽�E���̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																										//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x + Boss.charaSize[enBoss::Main] - 20 ),							//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::RHand] - 20 + ( Boss.animation[0].No / 2 ) ),	//�\���ʒuy���W.
					C_SIZE / 2 + S_SIZE, C_SIZE / 2,																						//�摜�̕�,����.
					hMemDC,																													//���DC.
					ABLT_W + C_SIZE * 2, ( PHP_H * 2 + 5 ) * 3 + C_SIZE * 2 + 3,																//���摜��x,y���W.
					Boss.chara[enBoss::RHand].ang,																							//�p�x.
					hPlgDC );																												//�摜��]�pDC.
			}
			else if ( Boss.chara[enBoss::RHand].State == enCharaState::Dying ) {	//���S��.

				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hTree );
				//�޽�E��̕\��.
				ExpaTransBlt(
					gameWnd->hScreenDC,																																				//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::RHand].x - 20 ),																								//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::RHand].y - 5 - Boss.animation[enBoss::Main].No ),																		//�\���ʒuy���W.
					Boss.charaSize[enBoss::RHand],																																	//�摜�̕�.
					( ( Boss.charaSize[enBoss::RHand] * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100,											//�摜�̍���.
					hMemDC,																																							//���DC.
					0, 0,																																							//���摜��x,y���W.
					Boss.charaSize[enBoss::RHand] + Boss.chara[enBoss::RHand].ExpAnimCnt * 2,																						//�T�C�Y�ύX��̕�.
					( ( Boss.charaSize[enBoss::RHand] * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100 - Boss.chara[enBoss::RHand].ExpAnimCnt * 2,	//�T�C�Y�ύX��̍���.
					false );																																						//�����g������Ȃ�(�E���g��).

				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽�E���̕\��.
				ExpaTransBlt(
					gameWnd->hScreenDC,																																				//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x + Boss.charaSize[enBoss::Main] - 20 ),																	//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::RHand] - 20 + ( Boss.animation[0].No / 2 ) ),											//�\���ʒuy���W.
					C_SIZE / 2 + S_SIZE,																																			//�摜�̕�.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100,															//�摜�̍���.
					hMemDC,																																							//���DC.
					0, C_SIZE * 2 + C_SIZE / 2,																																		//���摜��x,y���W.
					C_SIZE / 2 + S_SIZE + Boss.chara[enBoss::RHand].ExpAnimCnt * 2,																									//�T�C�Y�ύX��̕�.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100 - Boss.chara[enBoss::RHand].ExpAnimCnt * 2,				//�T�C�Y�ύX��̍���.
					false );																																						//�����g������Ȃ�(�E���g��).
			}

			//�{�X�̍���.
			if ( Boss.chara[enBoss::LHand].State == enCharaState::Anima ) {	//�o��A�j���[�V����.
				//��׸��̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽����̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																				//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::LHand].x - 5 ),									//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::LHand].y - 5 - Boss.animation[enBoss::Main].No ),		//�\���ʒuy���W.
					Boss.charaSize[enBoss::LHand],																	//�摜�̕�.
					( ( Boss.charaSize[enBoss::LHand] * 100 / Boss.DispAnima.max ) * Boss.DispAnima.No ) / 100,		//�摜�̍���.
					hMemDC,																							//���DC.
					Boss.charaSize[enBoss::Main], 0,																//���摜��x,y���W.
					Boss.chara[enBoss::LHand].ang,																	//�摜�p�x.
					hPlgDC );																						//�摜��]�pDC.

				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽�����̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																													//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x - Boss.charaSize[enBoss::LHand] / 2 ),										//�\���ʒux���W.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::LHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),	//�\���ʒuy���W.
					C_SIZE / 2,																															//�摜�̕�.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * Boss.DispAnima.No ) / 100,														//�摜�̍���.
					hMemDC,																																//���DC.
					0, C_SIZE * 2,																														//���摜��x,y���W.
					Boss.chara[enBoss::LHand].ang,																										//�p�x.
					hPlgDC );																															//�摜��]DC.
			}
			else if ( Boss.chara[enBoss::LHand].State == enCharaState::Living ) {	//������.
				//��׸��̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽����̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																		//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::LHand].x - 5 ),							//�\���ʒux.
					static_cast< int >( Boss.chara[enBoss::LHand].y - 5 - Boss.animation[enBoss::Main].No ),//�\���ʒuy.
					Boss.charaSize[enBoss::LHand], Boss.charaSize[enBoss::LHand],							//�摜�̕�,����.
					hMemDC,																					//���DC.
					Boss.charaSize[enBoss::Main], 0,														//���摜��x,y.
					Boss.chara[enBoss::LHand].ang,															//�p�x.
					hPlgDC );																				//��ƗpDC.

				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽�����̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																												//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x - Boss.charaSize[enBoss::LHand] / 2 ),									//�\���ʒux.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::LHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),//�\���ʒuy.
					C_SIZE / 2, C_SIZE / 2,																											//�摜�̕�,����.
					hMemDC,																															//���DC.
					0, C_SIZE * 2,																													//���摜��x,y.
					Boss.chara[enBoss::LHand].ang,																									//�p�x.
					hPlgDC );																														//��ƗpDC.
			}
			else if ( Boss.chara[enBoss::LHand].State == enCharaState::Invi ) {	//���G����.
				//��׸��̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, harea );
				//�޽����̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																												//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::LHand].x - 5 ),																	//�\���ʒux.
					static_cast< int >( Boss.chara[enBoss::LHand].y - 5 - Boss.animation[enBoss::Main].No ),										//�\���ʒuy.
					Boss.charaSize[enBoss::LHand], Boss.charaSize[enBoss::LHand],																	//�摜�̕�,����.
					hMemDC,																															//���DC.
					ABLT_W + C_SIZE * 2, ( PHP_H * 2 + 5 ) * 3 + 3,																						//���摜��x,y.
					Boss.chara[enBoss::LHand].ang,																									//�p�x.
					hPlgDC );																														//��ƗpDC.

				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, harea );
				//�޽�����̕\��.
				TransPlgBlt(
					gameWnd->hScreenDC,																												//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x - Boss.charaSize[enBoss::LHand] / 2 ),									//�\���ʒux.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::LHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),//�\���ʒuy.
					C_SIZE / 2, C_SIZE / 2,																											//�摜�̕�,����.
					hMemDC,																															//���DC.
					ABLT_W + C_SIZE * 2, ( PHP_H * 2 + 5 ) * 3 + C_SIZE * 2 + C_SIZE / 2 + 3,															//���摜��x,y.
					Boss.chara[enBoss::LHand].ang,																									//�p�x.
					hPlgDC );																														//��ƗpDC.
			}
			else if ( Boss.chara[enBoss::LHand].State == enCharaState::Dying ) {	//���S��.
				//��׸��̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽����̕\��.
				ExpaTransBlt(
					gameWnd->hScreenDC,																												//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::LHand].x - 5 ),																	//�\���ʒux.
					static_cast< int >( Boss.chara[enBoss::LHand].y - 5 - Boss.animation[enBoss::Main].No ),										//�\���ʒuy.
					Boss.charaSize[enBoss::LHand],																									//�摜�̕�.
					( ( Boss.charaSize[enBoss::LHand] * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100,			//�摜�̍���.
					hMemDC,																															//���DC.
					Boss.charaSize[enBoss::Main], 0,																								//���摜��x,y.
					Boss.charaSize[enBoss::LHand] + Boss.chara[enBoss::LHand].ExpAnimCnt * 2,
					( ( Boss.charaSize[enBoss::LHand] * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100 - Boss.chara[enBoss::LHand].ExpAnimCnt * 2,
					false );

				//�{�X�̉摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hboss );
				//�޽�����̕\��.
				ExpaTransBlt(
					gameWnd->hScreenDC,																												//���޲���÷��.
					static_cast< int >( C_CORR + Boss.chara[enBoss::Main].x - Boss.charaSize[enBoss::LHand] / 2 ),									//�\���ʒux.
					static_cast< int >( Boss.chara[enBoss::Main].y + Boss.charaSize[enBoss::LHand] - 20 + ( Boss.animation[enBoss::Main].No / 2 ) ),//�\���ʒuy.
					C_SIZE / 2,																														//�摜�̕�.
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100,							//�摜�̍���.
					hMemDC,																															//���DC.
					0, C_SIZE * 2,																													//���摜��x,y.
					C_SIZE / 2 + Boss.chara[enBoss::LHand].ExpAnimCnt * 2,
					( ( ( C_SIZE / 2 ) * 100 / Boss.DispAnima.max ) * ( Boss.DispAnima.max - Boss.DispAnima.No ) ) / 100 - Boss.chara[enBoss::LHand].ExpAnimCnt * 2,
					false );
			}

			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[typeNo][ptnNo][eNo].appFlag == true ) || ( Enemy[typeNo][ptnNo][eNo].BossFlag == true ) ) {
							//�G�@��������.
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living )
							{
								if ( typeNo != 0 ) {	//�ԈȊO.
									//��׸��̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hChara );
									//�G�@(�ԈȊO)�̕\��.
									TransBlt(
										gameWnd->hScreenDC,																//���޲���÷��.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//�\���ʒux���W.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//�\���ʒuy���W.
										C_SIZE, C_SIZE,																	//�摜�̕�,����.
										hMemDC,																			//���DC.
										C_SIZE * ( ( typeNo - 1 ) % 2 * 2 + Enemy[typeNo][ptnNo][eNo].animetion.No ),	//���摜��x���W( %2 : 0, 1, 0, 1, 0, ... ).
										C_SIZE * 2 + C_SIZE * ( ( typeNo - 1 ) / 2 ) );									//���摜��y���W( /2 : 0, 0, 1, 1, 2, ... ).
								}
								else {	//��.
									//��׸��̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hChara );
									//�G�@(��)�̕\��.
									TransBlt(
										gameWnd->hScreenDC,																//���޲���÷��.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//�\���ʒux���W.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//�\���ʒuy���W.
										C_SIZE, C_SIZE,																	//�摜�̕�,����.
										hMemDC,																			//���DC.
										C_SIZE + C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No % 3 ),				//���摜��x���W( %3 : 0, 1, 2, 0, 1, 2, ... ).
										C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No / 3 ) );						//���摜��y���W( /3 : 0, 0, 0, 1, 1, 1, ... ).
								}
							}
							//�G�@�����G.
							else if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) {
								if ( typeNo != 0 ) {
									//��׸��̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hCharaHit );
									//�G�@�̕\��.
									TransBlt(
										gameWnd->hScreenDC,																//���޲���÷��.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//�\���ʒux���W.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//�\���ʒuy���W.
										C_SIZE, C_SIZE,										 							//�摜�̕�,����.
										hMemDC,																			//���DC.
										C_SIZE * ( ( typeNo - 1 ) % 2 * 2 + Enemy[typeNo][ptnNo][eNo].animetion.No ),	//���摜��x���W( %2 : 0, 1, 0, 1, 0, 1, ... ).
										C_SIZE * 2 + C_SIZE * ( ( typeNo - 1 ) / 2 ) );									//���摜��y���W( /2 : 0, 0, 1, 1, 2, 2, ... ).
								}
								else {
									//��׸��̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hCharaHit );
									//�G�@�̕\��.
									TransBlt(
										gameWnd->hScreenDC,														//���޲���÷��.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),		//�\���ʒux���W.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),				//�\���ʒuy���W.
										C_SIZE, C_SIZE,															//�摜�̕�,����.
										hMemDC,																	//���DC.
										C_SIZE + C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No % 3 ),		//���摜��x���W( %3 : 0, 1, 2, 0, 1, 2, ... ).
										C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No / 3 ) );				//���摜��y���W( /3 : 0, 0, 0, 1, 1, 1, ... ).
								}
							}
							//�G�@�����S��.
							else if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dying )
							{
								if ( typeNo != 0 ) {
									//��׸��̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hChara );
									//�G�@(�ԈȊO)�̕\��.
									ExpaTransBlt(
										gameWnd->hScreenDC,																//���޲���÷��.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//�\���ʒux���W.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//�\���ʒuy���W.
										C_SIZE, C_SIZE,																	//�摜�̕�,����.
										hMemDC,																			//���DC.
										C_SIZE * ( ( typeNo - 1 ) % 2 * 2 + Enemy[typeNo][ptnNo][eNo].animetion.No ),	//���摜��x���W.
										C_SIZE * 2 + C_SIZE * ( ( typeNo - 1 ) / 2 ),									//���摜��y���W.
										C_SIZE + Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt * 2,						//�T�C�Y�ύX��̕�.
										C_SIZE - Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt * 2,						//�T�C�Y�ύX��̍���.
										false );																		//�����g������Ȃ�(�E���g��).
								}
								else {
									//��׸��̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hChara );
									//�G�@(��)�̕\��.
									ExpaTransBlt(
										gameWnd->hScreenDC,																//���޲���÷��.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x ),				//�\���ʒux���W.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),						//�\���ʒuy���W.
										C_SIZE, C_SIZE,																	//�摜�̕�,����.
										hMemDC,																			//���DC.
										C_SIZE + C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No % 3 ),				//���摜��x���W.
										C_SIZE * ( Enemy[typeNo][ptnNo][eNo].animetion.No / 3 ),						//���摜��y���W.
										C_SIZE + Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt * 2,						//�T�C�Y�ύX��̕�.
										C_SIZE - Enemy[typeNo][ptnNo][eNo].chara.ExpAnimCnt * 2,						//�T�C�Y�ύX��̍���.
										false );																		//�����g������Ȃ�(�E���g��).
								}
							}
						}
						//�G�̉񕜃G�t�F�N�g.
						for ( int hNo = 0; hNo < H_MAX; hNo++ ){
							if ( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].DispFlag == true ){
								//��׸��̉摜�����DC�ֺ�߰.
								SelectObject( hMemDC, harea );
								//���@�̕\��.
								TransAlBlend(
									gameWnd->hScreenDC,																																		//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].x ),																				//�\���ʒux���W( �␳�l:13 ).
									static_cast< int >( Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].y - Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].UpY ),										//�\���ʒuy���W( �␳�l:13 ).
									C_SIZE / 2, C_SIZE / 2,																																	//�摜�̕�,����.
									hMemDC,																																					//������DC.
									ABLT_W + C_SIZE * 3 + C_SIZE / 2 * Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].PtnX,																		//���摜��x���W.
									( PHP_H * 2 + 5 ) * 3 + C_SIZE * Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].PtnY + C_SIZE / 2 * Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].animation.No,	//���摜��y���W.
									Enemy[typeNo][ptnNo][eNo].HeelAnima[hNo].Alpha,																											//�A���t�@�l.
									hWorkDC );																																				//��ƗpDC.
							}
						}
					}
				}
			}

			for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
				for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
					if ( ( Player.shot[numNo][dirNo].DispShotFlag == true ) || ( Player.chara.chgCnt != 0 ) ) {
						if ( Player.ability.beam != 0 ) {
							//�L�����N�^�̉摜��������DC�փR�s�[.
							SelectObject( hMemDC, hboss );
							//�v���C���[�̃r�[���e�̕\��.
							TransPlgBlt(
								gameWnd->hScreenDC,																													//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( C_CORR + Player.shot[numNo][dirNo].x - S_SIZE - ( 8 - 7 * ( BEAMMAX - Player.shot[numNo][dirNo].beamCnt ) ) ),	//�\���ʒux.
								static_cast< int >( Player.shot[numNo][dirNo].y - S_SIZE ),																			//�\���ʒuy.
								S_SIZE, S_SIZE * Player.shot[numNo][dirNo].beamCnt,																					//�摜�̕�,����.
								hMemDC,																																//������DC.
								S_SIZE * 7,																															//���摜��x���W
								S_SIZE * 8 + S_SIZE * ( BEAMMAX - Player.shot[numNo][dirNo].beamCnt ),																//���摜��y���W.
								RAD( 90 ) - Player.shot[numNo][dirNo].ang,
								hPlgDC );
						}
						else if ( Player.ability.chg != 0 ) {
							if ( ( Player.shot[numNo][dirNo].DispShotFlag == false ) && ( Player.chara.chgCnt != 0 ) ) {	//���ߏ��.
								//�L�����N�^�̉摜��������DC�փR�s�[.
								SelectObject( hMemDC, hboss );
								//�v���C���[�̗��ߒe�̕\��.
								ExpaTransAlBlend(
									gameWnd->hScreenDC,											//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( C_CORR + Player.shot[numNo][dirNo].x ),	//�\���ʒux.
									static_cast< int >( Player.shot[numNo][dirNo].y ),			//�\���ʒuy.
									S_SIZE * 2, S_SIZE * 2,										//�摜�̕�,����.
									hMemDC,														//������DC.
									S_SIZE * 5, S_SIZE * 10,									//���摜��y���W
									static_cast< int >( 50 + Player.chara.chgCnt + Player.chgAniCnt ),
									hWorkDC,
									static_cast< int >( S_SIZE * ( 2 + Player.chara.chgCnt / 75 ) ),	//���̊g��{��.
									static_cast< int >( S_SIZE *( 2 + Player.chara.chgCnt / 75 ) ) );	//�����̊g��{��.	
							}
							else {	//���ˌ�.
								//�L�����N�^�̉摜��������DC�փR�s�[.
								SelectObject( hMemDC, hboss );
								//�v���C���[�̗��ߒe�̕\��.
								ExpaTransBlt(
									gameWnd->hScreenDC,											//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( C_CORR + Player.shot[numNo][dirNo].x ),	//�\���ʒux.
									static_cast< int >( Player.shot[numNo][dirNo].y ),			//�\���ʒuy.
									S_SIZE * 2, S_SIZE * 2,										//�摜�̕�,����.
									hMemDC,														//������DC.
									S_SIZE * 5, S_SIZE * 10,									//���摜��y���W
									static_cast< int >( S_SIZE * ( 2 + Player.chara.chgCnt / 75 ) ),	//���̊g��{��.
									static_cast< int >( S_SIZE *( 2 + Player.chara.chgCnt / 75 ) ) );	//�����̊g��{��.	
							}
						}
						else if ( Player.ability.pene != 0 ) {
							//�L�����N�^�̉摜��������DC�փR�s�[.
							SelectObject( hMemDC, hboss );
							//�v���C���[�̊ђʒe�̕\��.
							TransBlt(
								gameWnd->hScreenDC,											//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( C_CORR + Player.shot[numNo][dirNo].x ),	//�\���ʒux.
								static_cast< int >( Player.shot[numNo][dirNo].y ),			//�\���ʒuy.
								S_SIZE, S_SIZE,												//�摜�̕�,����.
								hMemDC,														//������DC.
								S_SIZE * 3, S_SIZE * 11 );									//���摜��y���W
						}
						else {
							//�L�����N�^�̉摜��������DC�փR�s�[.
							SelectObject( hMemDC, hShot );
							//�v���C���[�̃m�[�}���e�̕\��.
							TransBlt(
								gameWnd->hScreenDC,											//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( C_CORR +  Player.shot[numNo][dirNo].x ),//�\���ʒux.
								static_cast< int >( Player.shot[numNo][dirNo].y ),			//�\���ʒuy.
								S_SIZE, S_SIZE,												//�摜�̕�,����.
								hMemDC,														//������DC.
								0, 0 );														//�摜��y���W.
						}	
					}
				}
			}

			//�G�̒e.
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
							for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
								if ( ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) || ( Enemy[typeNo][ptnNo][eNo].chara.chgCnt != 0 ) ) {
									switch ( typeNo )
									{
									case 0:	//����.
										if ( ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == false ) && ( Enemy[typeNo][ptnNo][eNo].chara.chgCnt != 0 ) ) {	//���ߏ��.
											//�L�����N�^�̉摜��������DC�փR�s�[.
											SelectObject( hMemDC, hShot );
											ExpaTransAlBlend(
												gameWnd->hScreenDC,																//�f�o�C�X�R���e�L�X�g.
												static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//�\���ʒux.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),			//�\���ʒuy.
												S_SIZE, S_SIZE,																	//�摜�̕�,����.
												hMemDC,																			//������DC.
												0, S_SIZE * 4,
												static_cast< int >( 50 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt ),
												hWorkDC,
												static_cast< int >( S_SIZE * ( 2 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt / 75 ) ),
												static_cast< int >( S_SIZE * ( 2 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt / 75 ) ) );
										}
										else {	//���ˌ�.
											//�L�����N�^�̉摜��������DC�փR�s�[.
											SelectObject( hMemDC, hShot );
											ExpaTransBlt(
												gameWnd->hScreenDC,																//�f�o�C�X�R���e�L�X�g.
												static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//�\���ʒux.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),			//�\���ʒuy.
												S_SIZE, S_SIZE,																	//�摜�̕�,����.
												hMemDC,																			//������DC.
												0, S_SIZE * 4,
												static_cast< int >( S_SIZE * ( 2 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt / 75 ) ),
												static_cast< int >( S_SIZE * ( 2 + Enemy[typeNo][ptnNo][eNo].chara.chgCnt / 75 ) ) );
										}
										break;
									case 1:	//����.
										//�L�����N�^�̉摜��������DC�փR�s�[.
										SelectObject( hMemDC, hShot );
										TransPlgBlt(
											gameWnd->hScreenDC,																			//�f�o�C�X�R���e�L�X�g.
											static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE * 2 ),	//�\���ʒux.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE * 2 ),			//�\���ʒuy.
											S_SIZE * 4, 50 - 10 * Enemy[typeNo][ptnNo][eNo].animetion.No,								//�摜�̕�,����.
											hMemDC,																						//������DC.
											S_SIZE * 2, 50 * Enemy[typeNo][ptnNo][eNo].animetion.No,									//���摜��x,y���W.
											RAD( 90 ) - Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang,								//�p�x.
											hPlgDC );																					//���DC.
										break;
									case 2:	//�ђ�.
										//�L�����N�^�̉摜��������DC�փR�s�[.
										SelectObject( hMemDC, hSlime );
										TransPlgBlt(
											gameWnd->hScreenDC,																	//�f�o�C�X�R���e�L�X�g.
											static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - 13 ),	//�\���ʒux.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2 ),	//�\���ʒuy.
											S_SIZE, S_SIZE * 3,																	//�摜�̕�,����.
											hMemDC,																				//������DC.
											C_SIZE * 13, 192,
											RAD( 90 ) - Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang,
											hPlgDC );
										break;
									case 3:	//�r�[��.
										//�L�����N�^�̉摜��������DC�փR�s�[.
										SelectObject( hMemDC, hShot );
										TransPlgBlt(
											gameWnd->hScreenDC,																																							//�f�o�C�X�R���e�L�X�g.
											static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE - ( 8 - 7 * ( BEAMMAX - Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt ) ) ),	//�\���ʒux.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE ),																								//�\���ʒuy.
											S_SIZE, S_SIZE * Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].beamCnt,																										//�摜�̕�,����.
											hMemDC,																																										//������DC.
											S_SIZE, 0,
											RAD( 90 ) - Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].ang,
											hPlgDC );
										break;
									case 4: //�ǔ�.
										//�L�����N�^�̉摜��������DC�փR�s�[.
										SelectObject( hMemDC, hSlime );
										TransPlgBlt(
											gameWnd->hScreenDC,																	//�f�o�C�X�R���e�L�X�g.
											static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - 5 ),	//�\���ʒux.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),				//�\���ʒuy.
											S_SIZE + 5, S_SIZE + S_SIZE / 2,													//�摜�̕�,����.
											hMemDC,																				//������DC.
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

			//�{�X�̒e.
			for ( int numNo = 0; numNo < Boss.BS_NUM; numNo++ ) {
				for ( int dirNo = 0; dirNo < Boss.BS_DIR; dirNo++ ) {
					if ( Boss.shot[numNo][dirNo].DispShotFlag == true ) {
						//�{�X�̒e�̉摜��������DC�փR�s�[.
						SelectObject( hMemDC, hSlime );
						//�{�X�̒e�̕\��.
						TransPlgBlt(
							gameWnd->hScreenDC,								//�f�o�C�X�R���e�L�X�g.
							( int ) ( C_CORR + Boss.shot[numNo][dirNo].x - 5 ),	//�\���ʒux.
							( int ) Boss.shot[numNo][dirNo].y,				//�\���ʒuy.
							S_SIZE + 5, S_SIZE + 5,							//�摜�̕�,����.
							hMemDC,											//������DC.
							C_SIZE * 13 + S_SIZE, 192 + S_SIZE + S_SIZE / 2,//���摜��x,y���W.
							RAD( 90 ) - Boss.shot[numNo][dirNo].ang,
							hPlgDC );
					}
				}
			}

			//�v���C���[�̃o���A�̕\��.
			if ( Player.ability.barrUP != 0 ) {
				for ( int bNo = 0; bNo < Player.ability.barrUP * BARRMAX; bNo++ ) {
					//�v���C���[�̃o���A�̉摜��������DC�փR�s�[.
					SelectObject( hMemDC, hboss );
					//�v���C���[�̃o���A�̕\��.
					TransBlt(
						gameWnd->hScreenDC,										//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Player.barrier[bNo].x ),	//�\���ʒux.
						static_cast< int >( Player.barrier[bNo].y ),			//�\���ʒuy.
						S_SIZE, S_SIZE,											//�摜�̕�,����.
						hMemDC,													//������DC.
						S_SIZE * 4,												//���摜��x���W
						S_SIZE * 11 );											//���摜��y���W.
				}
			}

			//�G�̃o���A�̕\��.
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( Enemy[typeNo][ptnNo][eNo].ability.barrUP != 0 ) {
							if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) {
								for ( int bNo = 0; bNo < Enemy[typeNo][ptnNo][eNo].ability.barrUP * BARRMAX; bNo++ ) {
									//�G�̃o���A�̉摜��������DC�փR�s�[.
									SelectObject( hMemDC, hTree );
									//�G�̃o���A�̕\��.
									TransBlt(
										gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].barrier[bNo].x ),//�\���ʒux.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].barrier[bNo].y ),			//�\���ʒuy.
										S_SIZE, S_SIZE,															//�摜�̕�,����.
										hMemDC,																	//������DC.
										C_SIZE * 3 + S_SIZE,													//���摜��x���W
										C_SIZE * 2 + S_SIZE );													//���摜��y���W.
								}
							}
						}
					}
				}
			}

			//���ى摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hPSlime );
			//��𕑂��t���ϕ`��.
			for ( int LNo = 0; LNo < W_HITMAX; LNo++ ) {
				if ( WoodLeaf[LNo].DispFlag == true ) {
					TransPlgBlt(
						gameWnd->hScreenDC,									//�f�o�C�X�R���e�L�X�g.
						C_CORR + WoodLeaf[LNo].x,							//�\���ʒux���W.
						WoodLeaf[LNo].y,									//�\���ʒuy���W.
						16, 16,												//�摜�̕�,����.
						hMemDC,												//������DC.
						C_SIZE * 3 + 16 * WoodLeaf[LNo].PtnX,				//���摜��x���W.
						C_SIZE * 2 + 16 * WoodLeaf[LNo].PtnY, 				//���摜��y���W.
						WoodLeaf[LNo].Ang,									//��]�p�x.
						hPlgDC );											//��]�pDC.
				}
			}

			//�X�e�[�W�M�~�b�N(��)�̗t����.
			for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
				for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
					if ( gimmick[0][ptnNo][gNo].State != enGimmickState::GWait ) {
						if ( gimmick[0][ptnNo][gNo].State == enGimmickState::GLiving ) {	//��.
							//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
							SelectObject( hMemDC, hTree );
							//�X�e�[�W�M�~�b�N�̕\��.
							TransAlBlend(
								gameWnd->hScreenDC,														//���޲���÷��.
								static_cast< int >( C_CORR + gimmick[0][ptnNo][gNo].x - C_SIZE / 2 ),	//�\���ʒux.
								static_cast< int >( gimmick[0][ptnNo][gNo].y - C_SIZE - C_SIZE / 2 + 5),//�\���ʒuy.
								C_SIZE * 2, C_SIZE * 2,													//�摜�̕�,����.
								hMemDC,																	//���DC.
								C_SIZE, ( C_SIZE * 2 ) * ptnNo,											//���摜��x,y���W.
								gimmick[0][ptnNo][gNo].TreeAlpha,
								hWorkDC );
						}
						if ( gimmick[0][ptnNo][gNo].State == enGimmickState::GInvi ) {	//��.
							//�X�e�[�W�M�~�b�N�̉摜�����DC�ֺ�߰.
							SelectObject( hMemDC, hTreeHit );
							//�X�e�[�W�M�~�b�N�̕\��.
							TransBlt(
								gameWnd->hScreenDC,															//���޲���÷��.
								static_cast< int >( C_CORR + gimmick[0][ptnNo][gNo].x - C_SIZE / 2 ),		//�\���ʒux.
								static_cast< int >( gimmick[0][ptnNo][gNo].y - C_SIZE - C_SIZE / 2 + 5 ),	//�\���ʒuy.
								C_SIZE * 2, C_SIZE * 2,														//�摜�̕�,����.
								hMemDC,																		//���DC.
								( C_SIZE * 2 ) * ptnNo, BLOW_H );												//���摜��x,y���W.
						}
					}
				}
			}

			//�G��HP�\��.
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) ||
							 ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) ){
							//HP�o�[�A�M�~�b�N�̉摜��������DC�փR�s�[.
							SelectObject( hMemDC, hhp_Gmck );
							//�G��HP�o�[(�w�i)�̕\��.
							ExpaTransBlt(
								gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x - 10 ),	//�\���ʒux(�␳�l�F10).
								static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y - 10 ),			//�\���ʒuy(�␳�l�F10).
								P_HP_W, P_HP_H,															//�摜�̕�,����.
								hMemDC,																	//������DC.
								0, C_SIZE * 2,															//���摜��x,y���W.
								P_HP_W / 3, P_HP_H / 3,													//�k����̕�,����.
								false );																//�\���ʒux,y���ړ������Ȃ�.

							//HP�o�[�A�M�~�b�N�̉摜��������DC�փR�s�[.
							SelectObject( hMemDC, hhp_Gmck );
							//�G��HP�o�[(���[�^�[)�̕\��.
							ExpaTransBlt(
								gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x - 10 ),	//�\���ʒux(�␳�l�F10).
								static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y - 10 ),			//�\���ʒuy(�␳�l�F10).
								P_HP_W, P_HP_H,															//�摜�̕�,����.
								hMemDC,																	//������DC.
								0, C_SIZE * 2 + P_HP_H,													//���摜��x,y���W.
								( ( P_HP_W * 100 / Enemy[typeNo][ptnNo][eNo].initial_status.hp ) * Enemy[typeNo][ptnNo][eNo].status.hp / 100 ) / 3,	//�k����̕�.
								P_HP_H / 3,																											//�k����̍���.
								false );																											//�\���ʒux,y���ړ������Ȃ�.
						}
					}
				}
			}

			//�G�̔\�͂̕\��.
			for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
				for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
					for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
						if ( ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) ||
							 ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Invi ) )
						{
							for ( int i = 0; i < ABLT_MAX; i++ ) {
								if ( Enemy[typeNo][ptnNo][eNo].chara.abArea[i] != 0 ) {
									//�\�̓G���A�̉摜��������DC�փR�s�[.
									SelectObject( hMemDC, hability );
									//�\�̓G���A�̕\��.
									ExpaTransBlt(
										gameWnd->hScreenDC,																				//�f�o�C�X�R���e�L�X�g.
										static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x + ( ABLT_SIZE / 3 ) * i - 10 ),	//�\���ʒux(�␳�l�F10).
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y - ABLT_SIZE / 2 + 3 ),					//�\���ʒuy(�␳�l�F 5).
										ABLT_SIZE, ABLT_SIZE,																			//�摜�̕�,����.
										hMemDC,																							//������DC.
										ABLT_SIZE * ( ( Enemy[typeNo][ptnNo][eNo].chara.abArea[i] + 1 ) % 4 ),							//���摜��x���W.
										ABLT_SIZE * ( ( Enemy[typeNo][ptnNo][eNo].chara.abArea[i] + 1 ) / 4 ),							//���摜��y���W.
										ABLT_SIZE / 3, ABLT_SIZE / 3,																	//�g��k����̕�,����.
										false );																						//�\���ʒux,y���ړ������Ȃ�.
								}
							}
						}
						else if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dead ) {
							//�G�̔\�͂̕\��.
							if ( Enemy[typeNo][ptnNo][eNo].EGetAbNo != 0 ) {
								//�\�̓G���A�̉摜��������DC�փR�s�[.
								SelectObject( hMemDC, hability );
								//�\�̓G���A�̕\��.
								ExpaTransBlt(
									gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( C_CORR + Enemy[typeNo][ptnNo][eNo].chara.x + 20 ),	//�\���ʒux(�␳�l�F10).
									static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y - 10 ),			//�\���ʒuy(�␳�l�F 5).
									ABLT_SIZE, ABLT_SIZE,													//�摜�̕�,����.
									hMemDC,																	//������DC.
									ABLT_SIZE * ( ( Enemy[typeNo][ptnNo][eNo].EGetAbNo + 1 ) % 4 ),			//���摜��x���W.
									ABLT_SIZE * ( ( Enemy[typeNo][ptnNo][eNo].EGetAbNo + 1 ) / 4 ),			//���摜��y���W.
									ABLT_SIZE / 3, ABLT_SIZE / 3,											//�g��k����̕�,����.
									false );																//�\���ʒux,y���ړ������Ȃ�.
							}
						}

					}
				}
			}

			//�G(��)�̏�ԂɃA�j���[�V����.
			for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
				for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
					if ( ( Enemy[0][ptnNo][eNo].chara.State == enCharaState::Living ) ||
						 ( Enemy[0][ptnNo][eNo].chara.State == enCharaState::Invi ) )
					{
						if ( Plant[ptnNo][eNo].State == enPlantState::Sleeping ){
							for ( int pNo = 0; pNo < 2; pNo++ ){
								if ( Plant[ptnNo][eNo].Sleep[pNo].DispFlag == true ){
									//��׸��̉摜�����DC�ֺ�߰.
									SelectObject( hMemDC, hSleep );
									//zZ�̕\��.
									TransAlBlend(
										gameWnd->hScreenDC,												//�f�o�C�X�R���e�L�X�g.
										static_cast< int >( C_CORR + Plant[ptnNo][eNo].Sleep[pNo].x ),	//�\���ʒux���W.
										static_cast< int >( Plant[ptnNo][eNo].Sleep[pNo].y ),			//�\���ʒuy���W.
										16, 16,															//�摜�̕�,����.
										hMemDC,															//������DC.
										16 * ( Plant[ptnNo][eNo].Sleep[pNo].ptnNo % 2 ),				//���摜��x���W.
										16 * ( Plant[ptnNo][eNo].Sleep[pNo].ptnNo / 2 ),				//���摜��y���W.
										Plant[ptnNo][eNo].Sleep[pNo].Alpha,								//�A���t�@�l.
										hWorkDC );														//��ƗpDC.
								}
							}
						}
						else if ( Plant[ptnNo][eNo].State == enPlantState::WokeUp ){
							//��׸��̉摜�����DC�ֺ�߰.
							SelectObject( hMemDC, hSleep );
							//���@�̕\��.
							ExpaTransAlBlend(
								gameWnd->hScreenDC,																			//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( C_CORR + Plant[ptnNo][eNo].Woke.x - Plant[ptnNo][eNo].Woke.AnimaX ),	//�\���ʒux���W( �␳�l:13 ).
								static_cast< int >( Plant[ptnNo][eNo].Woke.y + Plant[ptnNo][eNo].Woke.AnimaY ),				//�\���ʒuy���W( �␳�l:13 ).
								16, 16,																						//�摜�̕�,����.
								hMemDC,																						//������DC.
								16, 16,																						//���摜��x,y���W.
								Plant[ptnNo][eNo].Woke.Alpha,																//�A���t�@�l.
								hWorkDC,																					//��ƗpDC.
								16 * 2 + Plant[ptnNo][eNo].Woke.AnimaCnt * 2,													//�T�C�Y�ύX��̕�.
								16 * 2 - Plant[ptnNo][eNo].Woke.AnimaCnt * 2,													//�T�C�Y�ύX��̍���.
								false );																					//�����g������Ȃ�(�E���g��).
						}
					}
				}
			}

			for ( int i = 0; i < Boss.BOSS_TYPE; i++ ) {
				if ( ( Boss.chara[i].State == enCharaState::Living ) || ( Boss.chara[i].State == enCharaState::Invi ) ) {
					//HP�o�[�A�M�~�b�N�̉摜��������DC�փR�s�[.
					SelectObject( hMemDC, hhp_Gmck );
					//�{�X��HP�o�[(�w�i)�̕\��.
					ExpaTransBlt(
						gameWnd->hScreenDC,										//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Boss.chara[i].x ),			//�\���ʒux(�␳�l�F25).
						static_cast< int >( Boss.chara[i].y - ABLT_SIZE / 4 ),	//�\���ʒuy(�␳�l�F25).
						P_HP_W, P_HP_H,											//�摜�̕�,����.
						hMemDC,													//������DC.
						0, C_SIZE * 2,											//���摜��x,y���W
						P_HP_W / ( P_HP_W / Boss.charaSize[i] ),
						P_HP_H / ( P_HP_W / Boss.charaSize[i] ),
						false );
					//HP�o�[�A�M�~�b�N�̉摜��������DC�փR�s�[.
					SelectObject( hMemDC, hhp_Gmck );
					//�{�X��HP�o�[(���[�^�[)�̕\��.
					ExpaTransBlt(
						gameWnd->hScreenDC,											//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Boss.chara[i].x ),				//�\���ʒux(�␳�l�F25).
						static_cast< int >( Boss.chara[i].y - ABLT_SIZE / 4 ),		//�\���ʒuy(�␳�l�F25).
						P_HP_W, P_HP_H,												//�摜�̕�,����.
						hMemDC,														//������DC.
						0, C_SIZE * 2 + P_HP_H,										//���摜��x,y���W.
						( P_HP_W / ( P_HP_W / Boss.charaSize[i] ) * 100 / Boss.initial_status[i].hp ) * Boss.status[i].hp / 100,
						P_HP_H / ( P_HP_W / Boss.charaSize[i] ),
						false );
				}
			}

			//�w�i�̉摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hMainLR );
			//���w�i�̕\��.
			BitBlt(
				gameWnd->hScreenDC,					//���޲���÷��.
				0, 0,								//�\���ʒux,y���W.
				280,								//�摜�̕�.
				WND_H,								//�摜�̍���.
				hMemDC,								//���DC.
				0, 0,								//���摜��x,y���W.
				SRCCOPY );							//��߰����.

			//�w�i�̉摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hMainLR );
			//�E�w�i�̕\��.
			BitBlt(
				gameWnd->hScreenDC,					//���޲���÷��.
				WND_W - 280,						//�\���ʒux���W.
				0,									//�\���ʒuy���W.
				280,								//�摜�̕�.
				WND_H,								//�摜�̍���.
				hMemDC,								//���DC.
				280, 0,								//���摜��x,y���W.
				SRCCOPY );							//��߰����.

			//HP�o�[�̉摜��������DC�փR�s�[.
			SelectObject( hMemDC, hPHp );
			//�v���C���[��HP�o�[(�w�i)�̕\��.
			TransBlt(
				gameWnd->hScreenDC,		//�f�o�C�X�R���e�L�X�g.
				-4, 3 + PHP_H,			//�\���ʒux,y(�␳�l�F10).
				PHP_W, PHP_H,			//�摜�̕�,����.
				hMemDC,					//������DC.
				0, PHP_H * 3 + 10 );	//���摜��x,y���W
			//HP�o�[�̉摜��������DC�փR�s�[.
			SelectObject( hMemDC, hPHp );
			//�v���C���[��HP�o�[(���[�^�[)�̕\��.
			TransBlt(
				gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
				-4, 3 + PHP_H,															//�\���ʒux,y(�␳�l�F10).
				( PHP_W * 100 / Player.initial_status.hp ) * Player.status.hp / 100,	//�摜�̕�.
				PHP_H,																	//�摜�̍���.
				hMemDC,																	//������DC.
				0, PHP_H * 2 + 10 );													//���摜��x,y���W.
			//�X���C���摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hSlime );
			//�v���C���[��HP�o�[(�t���[��)�̕\��.
			TransBlt(
				gameWnd->hScreenDC,				//�f�o�C�X�R���e�L�X�g.
				-4, -2,							//�\���ʒux,y���W.
				PHP_W, PHP_H * 2 + 5,			//�摜�̕�,����.
				hMemDC,							//������DC.
				PHP_W * Player.HPFrameNo, 96 );	//���摜��x,y���W.

			//�����̉摜��������DC�փR�s�[.
			SelectObject( hMemDC, hLetter );
			//�v���C���[�̃X�e�[�^�X�\��.
			for ( int i = 0; i < DISP_STA; i++ ) {
				if ( i == 0 ) {
					//"DAMAGE"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,							//�f�o�C�X�R���e�L�X�g.
						4, 135 + ( ENG_H * 2 + ENG_H / 2 ) * i,	//�\���ʒux,y.
						ENG_W[i], ENG_H,							//�摜�̕�,����.
						hMemDC,										//������DC.
						0, NUM_H );									//���摜��x,y���W.
				}
				else {
					//�X�e�[�^�X�̕\��.
					TransBlt(
						gameWnd->hScreenDC,							//�f�o�C�X�R���e�L�X�g.
						6, 135 + ( ENG_H * 2 + ENG_H / 2 ) * i,		//�\���ʒux,y.
						ENG_W[i], ENG_H,							//�摜�̕�,����.
						hMemDC,										//������DC.
						0, NUM_H + ENG_H * ( 1 + i ) );				//���摜��x,y���W.
				}
				//":"�̕\��.
				TransBlt(
					gameWnd->hScreenDC,								//�f�o�C�X�R���e�L�X�g.
					211, 145 + ( ENG_H * 2 + ENG_H / 2 ) * i,		//�\���ʒux,y.
					16, 24,											//�摜�̕�,����.
					hMemDC,											//������DC.
					NUM_W * 10, NUM_H + ENG_H );					//���摜��x,y���W.

				//�ʂ��ƂɈꎞ�ۊ�( [0]:1�̈�, [1]:10�̈� ).
				int Dig[2] = {};

				//�ʕ���.
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
					Dig[0] = 10;	//�����\�����Ȃ�.
					Dig[1] = 10;	//�����\�����Ȃ�.
					break;
				}

				//�����̕\��.
				for ( int j = 0; j < 2; j++ ) {
					TransBlt(
						gameWnd->hScreenDC,							//�f�o�C�X�R���e�L�X�g.
						224 + NUM_W * j,							//�\���ʒux.
						141 + ( ENG_H * 2 + ENG_H / 2 ) * i,		//�\���ʒuy.
						NUM_W, NUM_H,								//�摜�̕�,����.
						hMemDC,										//������DC.
						NUM_W * Dig[j], 0 );						//���摜��x,y���W.
				}
			}

			//�R���g���[���[�̑I���G���A�\��.
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
						//�\�̓G���A�̉摜��������DC�փR�s�[.
						SelectObject( hMemDC, hability );
						//�\�̓G���A�̕\��.
						TransBlt(
							gameWnd->hScreenDC,				//�f�o�C�X�R���e�L�X�g.
							5 + ( ABLT_SIZE + 5 ) * i,		//�\���ʒux(�␳�l�F10).
							WND_H - ABLT_SIZE - 6,			//�\���ʒuy(�␳�l�F 5).
							ABLT_SIZE, ABLT_SIZE,			//�摜�̕�,����.
							hMemDC,							//������DC.
							0, 0 ); 						//���摜��x,y���W.
					}
					else {
						//�\�̓G���A�̉摜��������DC�փR�s�[.
						SelectObject( hMemDC, hability );
						//�\�̓G���A�̕\��.
						TransBlt(
							gameWnd->hScreenDC,				//�f�o�C�X�R���e�L�X�g.
							5 + ( ABLT_SIZE + 5 ) * i,		//�\���ʒux(�␳�l�F10).
							WND_H - ABLT_SIZE - 6,			//�\���ʒuy(�␳�l�F 5).
							ABLT_SIZE, ABLT_SIZE,			//�摜�̕�,����.
							hMemDC,							//������DC.
							C_SIZE, 0 ); 					//���摜��x,y���W.
					}
				}
				else{
					//�\�̓G���A�̉摜��������DC�փR�s�[.
					SelectObject( hMemDC, hability );
					//�\�̓G���A�̕\��.
					TransBlt(
						gameWnd->hScreenDC,										//�f�o�C�X�R���e�L�X�g.
						5 + ( ABLT_SIZE + 5 ) * i,								//�\���ʒux(�␳�l�F10).
						WND_H - ABLT_SIZE - 6,									//�\���ʒuy(�␳�l�F 5).
						ABLT_SIZE, ABLT_SIZE,									//�摜�̕�,����.
						hMemDC,													//������DC.
						ABLT_SIZE * ( ( Player.chara.abArea[i] + 1 ) % 4 ),		//���摜��x���W.
						ABLT_SIZE * ( ( Player.chara.abArea[i] + 1 ) / 4 ) );	//���摜��y���W.
				}
			}

			//�폜�\�̓A�j���[�V����.
			for ( int i = 0; i < ABLT_MAX; i++ ) {
				if ( Player.DGAbi[i].DispFlag == true ) {
					//�\�̓G���A�̉摜��������DC�փR�s�[.
					SelectObject( hMemDC, hability );
					//�\�̓G���A�̕\��.
					ExpaTransAlBlend(
						gameWnd->hScreenDC,										//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( Player.DGAbi[i].xy.x ),				//�\���ʒux(�␳�l�F10).
						static_cast< int >( Player.DGAbi[i].xy.y ),				//�\���ʒuy(�␳�l�F 5).
						ABLT_SIZE, ABLT_SIZE,									//�摜�̕�,����.
						hMemDC,													//������DC.
						ABLT_SIZE * ( ( Player.DGAbi[i].AbiNo + 1 ) % 4 ),		//���摜��x���W.
						ABLT_SIZE * ( ( Player.DGAbi[i].AbiNo + 1 ) / 4 ),		//���摜��y���W.
						Player.DGAbi[i].Alpha,
						hWorkDC,
						static_cast< int >( ABLT_SIZE * ( 1 + Player.DGAbi[i].Expa ) ),
						static_cast< int >( ABLT_SIZE * ( 1 + Player.DGAbi[i].Expa ) ),
						false );
				}
			}

			//�R���g���[���[�̑I���G���A�\��.
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

			//�\�͐���.
			for ( int i = 0; i < ABLT_MAX; i++ ) {
				//�\�͐����̉摜��������DC�փR�s�[.
				SelectObject( hMemDC, harea );
				//�\�̓G���A�w�i�̕\��.
				TransBlt(
					gameWnd->hScreenDC,					//�f�o�C�X�R���e�L�X�g.
					WND_W - ( 12 + ABLT_W ),			//�\���ʒux(�␳�l�F15).
					116 + ( ABLT_H + 24 ) * i,			//�\���ʒuy(�␳�l�F10).
					ABLT_W, ABLT_H,						//�摜�̕�,����.
					hMemDC,								//������DC.
					0, ABLT_H * i ); 					//���摜��x,y���W.
				//�A�C�R��.
				if ( Player.chara.abArea[i] == 0 ){
					if ( i == 0 ){	//�ԋ�.
						//�\�̓G���A�̉摜��������DC�փR�s�[.
						SelectObject( hMemDC, hability );
						//�\�̓G���A�̕\��.
						TransBlt(
							gameWnd->hScreenDC,				//�f�o�C�X�R���e�L�X�g.
							WND_W - ( 12 + ABLT_W ),		//�\���ʒux(�␳�l�F15).
							116 + ( ABLT_H + 24 ) * i,		//�\���ʒuy(�␳�l�F10).
							ABLT_SIZE, ABLT_SIZE,			//�摜�̕�,����.
							hMemDC,							//������DC.
							0, 0 ); 						//���摜��x���W.
					}
					else {	//��.
						//�\�̓G���A�̉摜��������DC�փR�s�[.
						SelectObject( hMemDC, hability );
						//�\�̓G���A�̕\��.
						TransBlt(
							gameWnd->hScreenDC,				//�f�o�C�X�R���e�L�X�g.
							WND_W - ( 12 + ABLT_W ),		//�\���ʒux(�␳�l�F15).
							116 + ( ABLT_H + 24 ) * i,		//�\���ʒuy(�␳�l�F10).
							ABLT_SIZE, ABLT_SIZE,			//�摜�̕�,����.
							hMemDC,							//������DC.
							C_SIZE, 0 ); 					//���摜��x���W.
					}
				}
				else{	//�A�C�R��.
					//�\�̓G���A�̉摜��������DC�փR�s�[.
					SelectObject( hMemDC, hability );
					//�\�̓G���A�̕\��.
					TransBlt(
						gameWnd->hScreenDC,										//�f�o�C�X�R���e�L�X�g.
						WND_W - ( 12 + ABLT_W ),								//�\���ʒux(�␳�l�F15).
						116 + ( ABLT_H + 24 ) * i,								//�\���ʒuy(�␳�l�F10).
						ABLT_SIZE, ABLT_SIZE,									//�摜�̕�,����.
						hMemDC,													//������DC.
						ABLT_SIZE * ( ( Player.chara.abArea[i] + 1 ) % 4 ),		//���摜��x���W.
						ABLT_SIZE * ( ( Player.chara.abArea[i] + 1 ) / 4 ) );	//���摜��y���W.
				}
				//�����̉摜��������DC�փR�s�[.
				SelectObject( hMemDC, hLetter );
				switch ( Player.chara.abArea[i] )
				{
				case enAbility::beam:
					//"BEAM"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::Beam], ENG_H,			//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 7,								//���摜��x���W.
						NUM_H + ENG_H * 7 );					//���摜��y���W.
					break;
				case enAbility::pene:
					//"PENE"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::Pane], ENG_H,			//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 7,								//���摜��x���W.
						NUM_H + ENG_H * 6 );					//���摜��y���W.
					break;
				case enAbility::chg:
					//"CHARGE"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::Charge], ENG_H,			//�摜�̕�,����.
						hMemDC,									//������DC.
						0, NUM_H + ENG_H * 6 );					//���摜��x,y���W.
					break;
				case enAbility::homing:
					//"HOMING"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::Homing], ENG_H,			//�摜�̕�,����.
						hMemDC,									//������DC.
						0, NUM_H + ENG_H * 7 );					//���摜��x,y���W.
					break;
				case enAbility::dirUP:
					//"SHOTDIR"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::ShotDir], ENG_H,		//�摜�̕�,����.
						hMemDC,									//������DC.
						0, NUM_H + ENG_H * 4 );					//���摜��x,y���W.
					//"+"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 170 ,					//�\���ʒux(�␳�l�F15).
						199 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						24, 24,									//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 9,								//���摜��x���W.
						NUM_H + ENG_H * 1 );					//���摜��y���W.
					//"2"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 193,					//�\���ʒux(�␳�l�F15).
						195 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						NUM_W, NUM_H,							//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 2, 0 );							//���摜��x,y���W.
					break;
				case enAbility::numUP:
					//"SHOTNUM"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::ShotNum], ENG_H,		//�摜�̕�,����.
						hMemDC,									//������DC.
						0, NUM_H + ENG_H * 5 );					//���摜��x,y���W.
					//"+"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 188,					//�\���ʒux(�␳�l�F15).
						199 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						24, 24,									//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 9,								//���摜��x���W.
						NUM_H + ENG_H * 1 );					//���摜��y���W.
					//"1"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 210,					//�\���ʒux(�␳�l�F15).
						195 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						NUM_W, NUM_H,							//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 1, 0 );							//���摜��x,y���W.
					break;
				case enAbility::hpUP:
					//"HEEL"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::Heel], ENG_H,			//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 7,								//���摜��x���W.
						NUM_H + ENG_H * 8 );					//���摜��y���W.
					//"+"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 95,					//�\���ʒux(�␳�l�F15).
						199 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						24, 24,									//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 9,								//���摜��x���W.
						NUM_H + ENG_H * 1 );					//���摜��y���W.
					//"1"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 120,					//�\���ʒux(�␳�l�F15).
						195 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						NUM_W, NUM_H,							//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 1, 0 );							//���摜��x,y���W.
					break;
				case enAbility::barrUP:
					//"BARRIER"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::Barrier], ENG_H,		//�摜�̕�,����.
						hMemDC,									//������DC.
						0, NUM_H + ENG_H * 8 );					//���摜��x,y���W.
					//"+"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 160,					//�\���ʒux(�␳�l�F15).
						199 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						24, 24,									//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 9,								//���摜��x���W.
						NUM_H + ENG_H * 1 );					//���摜��y���W.
					//"1"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 185,					//�\���ʒux(�␳�l�F15).
						195 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						NUM_W, NUM_H,							//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 1, 0 );							//���摜��x,y���W.
					break;
				case enAbility::atkUP:
					//"DAMAGE"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::Damage], ENG_H,			//�摜�̕�,����.
						hMemDC,									//������DC.
						0, NUM_H );								//���摜��x,y���W.
					//"+"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 160,					//�\���ʒux(�␳�l�F15).
						199 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						24, 24,									//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 9,								//���摜��x���W.
						NUM_H + ENG_H * 1 );					//���摜��y���W.
					//"2"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 185,					//�\���ʒux(�␳�l�F15).
						195 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						NUM_W, NUM_H,							//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 2, 0 );							//���摜��x,y���W.
					break;
				case enAbility::cspdUP:
					//"CHARASPD"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 10,					//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::CharaSpd], ENG_H,		//�摜�̕�,����.
						hMemDC,									//������DC.
						0, NUM_H + ENG_H * 2 );					//���摜��x,y���W.
					//"+"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 199,					//�\���ʒux(�␳�l�F15).
						199 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						24, 24,									//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 9,								//���摜��x���W.
						NUM_H + ENG_H * 1 );					//���摜��y���W.
					//"1"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 217,					//�\���ʒux(�␳�l�F15).
						195 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						NUM_W, NUM_H,							//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 1, 0 );							//���摜��x,y���W.
					break;
				case enAbility::sspdUP:
					//"SHOTSPD"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W - 5,						//�\���ʒux(�␳�l�F15).
						191 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						ENG_W[enENG_No::ShotSpd], ENG_H,		//�摜�̕�,����.
						hMemDC,									//������DC.
						0, NUM_H + ENG_H * 3 );					//���摜��x,y���W.
					//"+"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 185,					//�\���ʒux(�␳�l�F15).
						199 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						24, 24,									//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 9,								//���摜��x���W.
						NUM_H + ENG_H * 1 );					//���摜��y���W.
					//"1"�̕\��.
					TransBlt(
						gameWnd->hScreenDC,						//�f�o�C�X�R���e�L�X�g.
						WND_W - ABLT_W + 210,					//�\���ʒux(�␳�l�F15).
						195 + ( ABLT_H + 24 ) * i,				//�\���ʒuy(�␳�l�F10).
						NUM_W, NUM_H,							//�摜�̕�,����.
						hMemDC,									//������DC.
						NUM_W * 1, 0 );							//���摜��x,y���W.
					break;
				default:
					break;
				}
			}

#ifdef _DEBUG	/** �� ********************** �����蔻��G���A�`�� ************************** �� **/
			const int	Alpha = 100;		//�����蔻��̃A���t�@�l.
			const bool	AreaDisp = false;	//�����蔻��\��.

			if ( AreaDisp == true ) {
				//�P�F�pDC�ɐԐF�̃u���V���Z�b�g.
				SelectObject( hColorDC, hRBrush );
				//�ԐF�̉摜��p��.
				PatBlt(
					hColorDC,		//�P�F�pDC�֏o��.
					0, 0,			//�\���ʒux,y.
					WND_W, WND_H,	//�摜�̕�,����.
					PATCOPY );

				//��ʏ㓖���蔻��.
				AlBlend(
					gameWnd->hScreenDC,								//�f�o�C�X�R���e�L�X�g.
					280, GWND_H / 4 + S_SIZE - 2,					//�\���ʒux,y.
					GWND_W, 10,										//�摜�̕�,����.
					hColorDC,										//������DC.
					0, 0,
					Alpha );

				
				for ( int typeNo = 0; typeNo < Boss.BOSS_TYPE; typeNo++ ){
					//�{�X�̕\��.
					AlBlend(
						gameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( C_CORR + Boss.chara[typeNo].x ),		//�\���ʒux.
						static_cast< int >( Boss.chara[typeNo].y ),					//�\���ʒuy.
						Boss.charaSize[typeNo], Boss.charaSize[typeNo],				//�摜�̕�,����.
						hColorDC,													//������DC.
						0, 0,
						Alpha );

					//�{�X�̓˂��グ��̕\��.
					for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
						for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
							if ( Boss.rock[typeNo][numNo][dirNo].DispFlag == true ) {
								//�{�X�̓˂��グ��̕\��.
								AlBlend(
									gameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( C_CORR + Boss.rock[typeNo][numNo][dirNo].x ),					//�\���ʒux.
									static_cast< int >( Boss.rock[typeNo][numNo][dirNo].y ),							//�\���ʒuy.
									S_SIZE * 2, ( S_SIZE / 2 ) * ( Boss.rock[typeNo][numNo][dirNo].cnt / 10 + 1 ),		//�摜�̕�,����.
									hColorDC,																			//������DC.
									0, 0,
									Alpha );
							}
						}
					}
				}
				//�{�X�̒e�̕\��.
				for ( int numNo = 0; numNo < ROCK_NUM; numNo++ ) {
					for ( int dirNo = 0; dirNo < ROCK_DIR; dirNo++ ) {
						if ( Boss.shot[numNo][dirNo].DispShotFlag == true ) {
							//�{�X�̓˂��グ��̕\��.
							AlBlend(
								gameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( C_CORR + Boss.shot[numNo][dirNo].x ),							//�\���ʒux.
								static_cast< int >( Boss.shot[numNo][dirNo].y ),									//�\���ʒuy.
								S_SIZE, S_SIZE,																		//�摜�̕�,����.
								hColorDC,																			//������DC.
								0, 0,
								Alpha );
						}
					}
				}

				for ( int typeNo = 0; typeNo < G_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < G_PTN; ptnNo++ ) {
						for ( int gNo = 0; gNo < G_MAX; gNo++ ) {
							if ( gimmick[typeNo][ptnNo][gNo].State != enGimmickState::GWait ) {
								if ( typeNo == 0 ) {	//��.
									if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GLiving ) {		//��.
										//�X�e�[�W�M�~�b�N�̕\��.
										AlBlend(
											gameWnd->hScreenDC,														//���޲���÷��.
											static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),			//�\���ʒux.
											static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),					//�\���ʒuy.
											gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,	//�摜�̕�,����.
											hColorDC,																//���DC.
											0, 0,																	//���摜��x,y���W.
											Alpha );
									}
									else if ( gimmick[typeNo][ptnNo][gNo].State == enGimmickState::GDead ) {	//���ꂢ���.
										//�X�e�[�W�M�~�b�N�̕\��.
										AlBlend(
											gameWnd->hScreenDC,																//���޲���÷��.
											static_cast< int >( C_CORR + gimmick[typeNo][ptnNo][gNo].x ),					//�\���ʒux.
											static_cast< int >( C_SIZE + gimmick[typeNo][ptnNo][gNo].y - C_SIZE / 2 - 3 ),	//�\���ʒuy.
											gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//�摜�̕�,����.
											hColorDC,																		//���DC.
											0, 0,
											Alpha );;
									}
								}
								else if ( typeNo == 1 ) {	//��.
									//�X�e�[�W�M�~�b�N�̕\��.
									AlBlend(
										gameWnd->hScreenDC,																//���޲���÷��.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + gimmick[typeNo][ptnNo][gNo].x ),	//�\���ʒux.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),							//�\���ʒuy.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//�摜�̕�,����.
										hColorDC,																		//���DC.
										0, 0,
										Alpha );
								}
								else if ( typeNo == 2 ) {	//��.
									//�X�e�[�W�M�~�b�N�̕\��.
									AlBlend(
										gameWnd->hScreenDC,																//���޲���÷��.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + gimmick[typeNo][ptnNo][gNo].x ),	//�\���ʒux.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),							//�\���ʒuy.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//�摜�̕�,����.
										hColorDC,																		//���DC.
										0, 0,																			//���摜��x,y���W.
										Alpha );
								}
								else if ( typeNo == 3 ) {	//�|��.
									//�X�e�[�W�M�~�b�N�̕\��.
									AlBlend(
										gameWnd->hScreenDC,																//���޲���÷��.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + gimmick[typeNo][ptnNo][gNo].x ),	//�\���ʒux.
										static_cast< int >( gimmick[typeNo][ptnNo][gNo].y ),							//�\���ʒuy.
										gimmick[typeNo][ptnNo][gNo].wSize, gimmick[typeNo][ptnNo][gNo].hSize,			//�摜�̕�,����.
										hColorDC,																		//���DC.
										0, 0,																			//���摜��x,y���W.
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
								//�G�@��������.
								if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living )
								{
									//�G�@�̕\��.
									AlBlend(
										gameWnd->hScreenDC,																	//���޲���÷��.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].chara.x ),	//�\���ʒux.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),							//�\���ʒuy.
										C_SIZE, C_SIZE,																		//�摜�̕�,����.
										hColorDC,																			//���DC.
										0, 0,
										Alpha );
								}
								else if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Dead )
								{
									//�G�@�̕\��.
									AlBlend(
										gameWnd->hScreenDC,																	//���޲���÷��.
										static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].chara.x ),	//�\���ʒux.
										static_cast< int >( Enemy[typeNo][ptnNo][eNo].chara.y ),							//�\���ʒuy.
										C_SIZE, C_SIZE,																		//�摜�̕�,����.
										hColorDC,																			//���DC.
										0, 0,																				//���摜��x,y.
										Alpha );																			//�A���t�@�l.								
								}
							}
						}
					}
				}

				for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
					for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
						if ( Player.shot[numNo][dirNo].DispShotFlag == true ) {
							if ( Player.ability.beam == 1 ) {
								//�v���C���[�̃r�[���e�̕\��.
								AlBlend(
									gameWnd->hScreenDC,															//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.shot[numNo][dirNo].x ),	//�\���ʒux.
									static_cast< int >( Player.shot[numNo][dirNo].y ),							//�\���ʒuy.
									S_SIZE, S_SIZE,																//�摜�̕�,����.
									hColorDC,																	//������DC.
									0, 0,																		//���摜��x,y���W.
									Alpha );
							}
							else if ( Player.ability.chg == 1 ) {
								//�v���C���[�̗��ߒe�̕\��.
								AlBlend(
									gameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.shot[numNo][dirNo].x - S_SIZE ),//�\���ʒux.
									static_cast< int >( Player.shot[numNo][dirNo].y - S_SIZE ),							//�\���ʒuy.
									S_SIZE * 2, S_SIZE * 2,																//�摜�̕�,����.
									hColorDC,																			//������DC.
									0, 0,																				//���摜��y���W
									Alpha );
							}
							else {
								//�v���C���[�̃m�[�}���e�̕\��.
								AlBlend(
									gameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.shot[numNo][dirNo].x ),	//�\���ʒux.
									static_cast< int >( Player.shot[numNo][dirNo].y ),							//�\���ʒuy.
									S_SIZE, S_SIZE,																//�摜�̕�,����.
									hColorDC,																	//������DC.
									0, 0,																		//���摜��x,y.
									Alpha );
							}
						}
					}
				}

				//�G�̒e.
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
								for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
									if ( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].DispShotFlag == true ) {
										switch ( typeNo )
										{
										case 0:	//����.
											AlBlend(
												gameWnd->hScreenDC,																						//�f�o�C�X�R���e�L�X�g.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE ),	//�\���ʒux.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE ),							//�\���ʒuy.
												S_SIZE * 2, S_SIZE * 2,																					//�摜�̕�,����.
												hColorDC,																								//������DC.
												0, 0,																									//���摜��x,y.
												Alpha );
											break;
										case 1:	//����.
											AlBlend(
												gameWnd->hScreenDC,																							//�f�o�C�X�R���e�L�X�g.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x - S_SIZE / 2 ),	//�\���ʒux.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y - S_SIZE / 2 ),							//�\���ʒuy.
												S_SIZE * 2, S_SIZE * 2,																						//�摜�̕�,����.
												hColorDC,																									//������DC.
												0, 0,																										//���摜��x,y.
												Alpha );
											break;
										case 2:	//�ђ�.
											AlBlend(
												gameWnd->hScreenDC,																				//�f�o�C�X�R���e�L�X�g.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//�\���ʒux.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),							//�\���ʒuy.
												S_SIZE, S_SIZE,																					//�摜�̕�,����.
												hColorDC,																						//������DC.
												0, 0,																							//���摜��x,y.
												Alpha );
											break;
										case 3:	//�r�[��.
											AlBlend(
												gameWnd->hScreenDC,																				//�f�o�C�X�R���e�L�X�g.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//�\���ʒux.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),							//�\���ʒuy.
												S_SIZE, S_SIZE,																					//�摜�̕�,����.
												hColorDC,																						//������DC.
												0, 0,																							//���摜��x,y.
												Alpha );
											break;
										case 4: //�ǔ�.
											AlBlend(
												gameWnd->hScreenDC,																				//�f�o�C�X�R���e�L�X�g.
												static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].x ),	//�\���ʒux.
												static_cast< int >( Enemy[typeNo][ptnNo][eNo].shot[numNo][dirNo].y ),							//�\���ʒuy.
												S_SIZE, S_SIZE,																					//�摜�̕�,����.
												hColorDC,																						//������DC.
												0, 0,																							//���摜��x,y.
												Alpha );
											break;
										}
									}
								}
							}
						}
					}
				}

				//���@��������.
				if ( Player.chara.State == enCharaState::Living )
				{
					//���@�̕\��.
					AlBlend(
						gameWnd->hScreenDC,														//�f�o�C�X�R���e�L�X�g.
						static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.chara.x + PH_CORR ),//�\���ʒux( �␳�l:13 ).
						static_cast< int >( Player.chara.y + PH_CORR ),							//�\���ʒuy( �␳�l:13 ).
						PH_SIZE, PH_SIZE,														//�摜�̕�,����.
						hColorDC,																//������DC.
						0, 0,
						Alpha );
				}

				//�G�̃o���A�̕\��.
				for ( int typeNo = 0; typeNo < E_TYPE; typeNo++ ) {
					for ( int ptnNo = 0; ptnNo < E_PTN; ptnNo++ ) {
						for ( int eNo = 0; eNo < E_MAX; eNo++ ) {
							if ( Enemy[typeNo][ptnNo][eNo].ability.barrUP != 0 ) {
								if ( Enemy[typeNo][ptnNo][eNo].chara.State == enCharaState::Living ) {
									for ( int bNo = 0; bNo < Enemy[typeNo][ptnNo][eNo].ability.barrUP * BARRMAX; bNo++ ) {
										//�v���C���[�̃m�[�}���e�̕\��.
										AlBlend(
											gameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g.
											static_cast< int >( ( WND_W - GWND_W ) / 2 + Enemy[typeNo][ptnNo][eNo].barrier[bNo].x ),//�\���ʒux.
											static_cast< int >( Enemy[typeNo][ptnNo][eNo].barrier[bNo].y ),							//�\���ʒuy.
											S_SIZE, S_SIZE,																			//�摜�̕�,����.
											hColorDC,																				//������DC.
											0, 0,
											Alpha );
									}
								}
							}
						}
					}
				}
				//�v���C���[�̃o���A�̕\��.
				if ( Player.ability.barrUP != 0 ) {
					for ( int bNo = 0; bNo < Player.ability.barrUP * BARRMAX; bNo++ ) {
						//�v���C���[�̃m�[�}���e�̕\��.
						AlBlend(
							gameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g.
							static_cast< int >( ( WND_W - GWND_W ) / 2 + Player.barrier[bNo].x ),	//�\���ʒux.
							static_cast< int >( Player.barrier[bNo].y ),							//�\���ʒuy.
							S_SIZE, S_SIZE,															//�摜�̕�,����.
							hColorDC,																//������DC.
							0, 0,
							Alpha );
					}
				}
			}
#endif			/** �� ********************** �����蔻��G���A�`�� ************************** �� **/

			//�{�X�o���A�j���[�V����(��ʗh��).
			if ( ( Boss.DispFlag == true ) || ( Boss.chara[enBoss::Main].State == enCharaState::Dying ) ||
				 ( Boss.AttackNo[enBoss::RHand] == enBossAttackNo::BDownHand ) ) {
				BitBlt(
					gameWnd->hScreenDC,
					( WND_W / 2 ) - ( GWND_W / 2 ),
					static_cast< int >( scroll.BossDispY ),
					GWND_W, GWND_H,					//�摜�̕�,����.
					gameWnd->hScreenDC,				//���DC.
					( WND_W / 2 ) - ( GWND_W / 2 ),	//���摜��x���W.
					0,								//���摜��y���W.
					SRCCOPY );						//��߰����.
			}

			break;
		}
		case enScene::GameOver:	//�ްѵ��ް.
		{
			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hGameOver );
			//�ްѵ��ް�摜�̕\��.
			BitBlt(
				gameWnd->hScreenDC,	//���޲���÷��.
				0, 0,				//�\���ʒux,y���W.
				WND_W, WND_H,		//�摜�̕�,����.
				hMemDC,				//���DC.
				0, 0,				//���摜��x,y���W.
				SRCCOPY );			//��߰����.

			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hcloud );
			//"GAME START"�摜�̕\��.
			TransBlt(
				gameWnd->hScreenDC,				//���޲���÷��.
				509, 550,						//�\���ʒux,y���W.
				256, 64,						//�摜�̕�,����.
				hMemDC,							//���DC.
				256 + 448, 256 * 2 );			//���摜��x,y���W.

			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hcloud );
			//"PRESS ANY BUTTON"�摜�̕\��.
			TransAlBlend(
				gameWnd->hScreenDC,				//���޲���÷��.
				544, 595,						//�\���ʒux,y���W.
				200, 16,						//�摜�̕�,����.
				hMemDC,							//���DC.
				256 + 448, 256 * 2 + 64,		//���摜��x,y���W.
				Title.Alpha,					//�A���t�@�l.
				hWorkDC );

			if ( Player.DeadNo == enDeedNo::Gimmick ) {
				//�ްѵ��ް�摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hDeadG );
			}
			else {
				//�ްѵ��ް�摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hDead );
			}
			//���S�L�����摜�̕\��.
			TransBlt(
				gameWnd->hScreenDC,			//���޲���÷��.
				GO_DispX[Player.DeadNo],	//�\���ʒux���W.
				GO_DispY[Player.DeadNo],	//�\���ʒuy���W.
				GO_wDest[Player.DeadNo],	//�摜�̕�.
				GO_hDest[Player.DeadNo],	//�摜�̍���.
				hMemDC,						//���DC.
				GO_StartX[Player.DeadNo],	//���摜��x���W.
				GO_StartY[Player.DeadNo] );	//���摜��y���W.

			if ( Player.DeadNo != enDeedNo::DeedNone ) {
				if ( Player.DeadNo == enDeedNo::Gimmick ) {
					//�ްѵ��ް�摜�����DC�ֺ�߰.
					SelectObject( hMemDC, hDeadG );
				}
				else {
					//�ްѵ��ް�摜�����DC�ֺ�߰.
					SelectObject( hMemDC, hDead );
				}

				//���S�L�����摜�̕\��.
				TransBlt(
					gameWnd->hScreenDC,				//���޲���÷��.
					GO_DispX[7 + Player.DeadNo],	//�\���ʒux���W.
					GO_DispY[7 + Player.DeadNo],	//�\���ʒuy���W.
					GO_wDest[7 + Player.DeadNo],	//�摜�̕�.
					GO_hDest[7 + Player.DeadNo],	//�摜�̍���.
					hMemDC,							//���DC.
					GO_StartX[7 + Player.DeadNo],	//���摜��x���W.
					GO_StartY[7 + Player.DeadNo] );	//���摜��y���W.
			}

			if ( Player.DeadNo == enDeedNo::Bat ) {
				SelectObject( hMemDC, hDead );
				//�����摜�̕\��.
				TransBlt(
					gameWnd->hScreenDC,				//���޲���÷��.
					620 + 4 * BatAnima.No, 447,		//�\���ʒuz,y���W.
					40, 32,							//�摜�̕�,����.
					hMemDC,							//���DC.
					128 + 40 * BatAnima.No, 112 );	//���摜��x,y���W.
			}
			else if ( Player.DeadNo == enDeedNo::Gimmick ) {
				SelectObject( hMemDC, hDeadG );
				//�����摜�̕\��.
				TransBlt(
					gameWnd->hScreenDC,				//���޲���÷��.
					623 + 6 * BatAnima.No,			//�\���ʒux���W.
					437 + 3 * BatAnima.No,			//�\���ʒuy���W.
					32, 32,							//�摜�̕�,����.
					hMemDC,							//���DC.
					32 * BatAnima.No, 160 );		//���摜��x,y���W.
			}
			break;
		}
		case enScene::Ending:	//���ިݸ�.
		{
			//���ިݸމ摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hEnding );
			//���ިݸމ摜�̕\��.
			BitBlt(
				gameWnd->hScreenDC,				//���޲���÷��.
				0, 0,							//�\���ʒux,y���W.
				WND_W, WND_H,					//�摜�̕�,����.
				hMemDC,							//���DC.
				0, 0,							//���摜��x,y���W.
				SRCCOPY );						//��߰����.

			for ( int sNo = 0; sNo < STARMAX; sNo++ ){
				if ( Ending.Star[sNo].DispFlag == true ){
					//�ްѵ��ް�摜�����DC�ֺ�߰.
					SelectObject( hMemDC, hEnSlime );
					//���̕\��.
					TransAlBlend(
						gameWnd->hScreenDC,								//���޲���÷��.
						Ending.Star[sNo].x,								//�\���ʒux���W.
						Ending.Star[sNo].y,								//�\���ʒuy���W.
						16, 16,											//�摜�̕�,����.
						hMemDC,											//���DC.
						47 + 16 * ( Ending.Star[sNo].Ptn / 3 ),			//���摜��x���W.
						C_SIZE * 4 + 16 * ( Ending.Star[sNo].Ptn % 3 ),	//���摜��y���W.
						Ending.Star[sNo].Alpha,							//�A���t�@�l.
						hWorkDC );
				}
			}

			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hEnClear );
			//�n�ʉ摜�̕\��.
			TransBlt(
				gameWnd->hScreenDC,							//���޲���÷��.
				0, 464,										//�\���ʒux,y���W.
				WND_W, 256,									//�摜�̕�,����.
				hMemDC,										//���DC.
				0, 0 );										//���摜��y���W.

			for ( int dNo = 0; dNo < DROPMAX; dNo++ ){
				if ( Ending.Drop[dNo].DispFlag == true ){
					//�ްѵ��ް�摜�����DC�ֺ�߰.
					SelectObject( hMemDC, hEnSlime );
					//���̕\��.
					TransBlt(
						gameWnd->hScreenDC,								//���޲���÷��.
						Ending.Drop[dNo].x,								//�\���ʒux���W.
						Ending.Drop[dNo].y,								//�\���ʒuy���W.
						Ending.wDest[Ending.Drop[dNo].Ptn],				//�摜�̕�.
						Ending.hDest[Ending.Drop[dNo].Ptn],				//�摜�̍���.
						hMemDC,											//���DC.
						Ending.StartX[Ending.Drop[dNo].Ptn],			//���摜��x���W.
						Ending.StartY[Ending.Drop[dNo].Ptn] );			//���摜��y���W.
				}
			}

			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hEnSlime );
			//�X���C���摜�̕\��.
			TransAlBlend(
				gameWnd->hScreenDC,							//���޲���÷��.
				Ending.Slime.x,								//�\���ʒux���W.
				Ending.Slime.y,								//�\���ʒuy���W.
				C_SIZE * 2, C_SIZE * 2,						//�摜�̕�,����.
				hMemDC,										//���DC.
				C_SIZE * 2 * Ending.Slime.Action,			//���摜��x���W.
				C_SIZE * 2 * Ending.Slime.animation.No,		//���摜��y���W.
				200,										//�A���t�@�l.
				hWorkDC );									//��ƗpDC.


			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hEnGround );
			//�n�ʉ摜�̕\��.
			TransBlt(
				gameWnd->hScreenDC,							//���޲���÷��.
				0, WND_H - 64,								//�\���ʒux,y���W.
				WND_W, 64,									//�摜�̕�,����.
				hMemDC,										//���DC.
				0, 0 );										//���摜��y���W.

			//�ʂ��ƂɈꎞ�ۊ�( [0]:1�̈�, [1]:10�̈�, [3]:100�̈� ).
			int Dig[3] = {};

			//Hit���̌�����.
			Dig[2] = Ending.Hit / 100;
			Dig[1] = ( Ending.Hit % 100 ) / 10;
			Dig[0] = Ending.Hit % 10;

			//Hit���̕\��.
			for ( int nNo = 0; nNo < 3; nNo++ ){
				//�ްѵ��ް�摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hEnHitNum );
				//�����̕\��.
				TransBlt(
					gameWnd->hScreenDC,					//���޲���÷��.
					WND_W - NUM_W * ( nNo + 1 ) - 5,	//�\���ʒux���W( 5 : �␳�l ).
					WND_H - NUM_H - 5,					//�\���ʒuy���W( 5 : �␳�l ).
					NUM_W, NUM_H,						//�摜�̕�,����.
					hMemDC,								//���DC.
					NUM_W * Dig[nNo], 0 );				//���摜��x,y���W.
			}
			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hEnHitNum );
			//"HIT:"�̕\��.
			TransBlt(
				gameWnd->hScreenDC,						//���޲���÷��.
				WND_W - NUM_W * 3 - 75 - 5,				//�\���ʒux���W( 5 : �␳�l ).
				WND_H - NUM_H - 5,						//�\���ʒuy���W( 5 : �␳�l ).
				75, NUM_H,								//�摜�̕�,����.
				hMemDC,									//���DC.
				0, NUM_H );								//���摜��x,y���W.

			//�ō�Hit���̌�����.
			Dig[0] = Ending.BestHit / 100;
			Dig[1] = ( Ending.BestHit % 100 ) / 10;
			Dig[2] = Ending.BestHit % 10;

			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hEnHitNum );
			//"BEST"�̕\��.
			TransBlt(
				gameWnd->hScreenDC,						//���޲���÷��.
				5,										//�\���ʒux���W( 5 : �␳�l ).
				WND_H - NUM_H - 5,						//�\���ʒuy���W( 5 : �␳�l ).
				85, NUM_H,								//�摜�̕�,����.
				hMemDC,									//���DC.
				75, NUM_H );							//���摜��x,y���W.

			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hEnHitNum );
			//"HIT:"�̕\��.
			TransBlt(
				gameWnd->hScreenDC,						//���޲���÷��.
				85 + 5,									//�\���ʒux���W( 5 : �␳�l ).
				WND_H - NUM_H - 5,						//�\���ʒuy���W( 5 : �␳�l ).
				75, NUM_H,								//�摜�̕�,����.
				hMemDC,									//���DC.
				0, NUM_H );								//���摜��x,y���W.

			//�ō�Hit���̕\��.
			for ( int nNo = 0; nNo < 3; nNo++ ){
				//�ްѵ��ް�摜�����DC�ֺ�߰.
				SelectObject( hMemDC, hEnHitNum );
				//�����̕\��.
				TransBlt(
					gameWnd->hScreenDC,					//���޲���÷��.
					85 + 75 + NUM_W * nNo + 5,			//�\���ʒux���W( 5 : �␳�l ).
					WND_H - NUM_H - 5,					//�\���ʒuy���W( 5 : �␳�l ).
					NUM_W, NUM_H,						//�摜�̕�,����.
					hMemDC,								//���DC.
					NUM_W * Dig[nNo], 0 );				//���摜��x,y���W.
			}

			//�ްѵ��ް�摜�����DC�ֺ�߰.
			SelectObject( hMemDC, hEnClear );
			//"GAME CLEAR"�̕\��.
			TransBlt(
				gameWnd->hScreenDC,						//���޲���÷��.
				320, Ending.ClearY,						//�\���ʒux,y���W.
				640, 126,								//�摜�̕�,����.
				hMemDC,									//���DC.
				0, 256 );								//���摜��x,y���W.

			//�X�^�b�t���[���̉摜��������DC�փR�s�[.
			for ( int eNo = 0; eNo < ENGMAX; eNo++ ) {
				if ( Ending.EnEng[eNo].DispFlag == true ) {
					if ( ( 12 <= eNo ) && ( eNo <= 17 ) ) {
						//�ްѵ��ް�摜�����DC�ֺ�߰.
						SelectObject( hMemDC, hEnEneEng );
					}
					else if ( eNo == 32 ) {
						//���ى摜�����DC�ֺ�߰.
						SelectObject( hMemDC, hTitle );
					}
					else {
						//�ްѵ��ް�摜�����DC�ֺ�߰.
						SelectObject( hMemDC, hEnEng );
					}

					if ( ( eNo == 32 ) || ( eNo == 34 ) ) {	//���S.
						//�X�^�b�t���[���̕\��.
						TransBlt(
							gameWnd->hScreenDC,											//���޲���÷��.
							( int ) ( Ending.EnEng[eNo].x ),							//�\���ʒux���W.
							( int ) ( Ending.EnEng[eNo].y ),							//�\���ʒuy���W.
							Ending.EnEng[eNo].wDest,									//�摜�̕�.
							Ending.EnEng[eNo].hDest,									//�摜�̍���.
							hMemDC,														//���DC.
							Ending.EnEng[eNo].StartX,									//���摜��x���W.
							Ending.EnEng[eNo].StartY );									//���摜��y���W.
					}
					else {									//����.
						//�X�^�b�t���[���̕\��.
						TransPlgBlt(
							gameWnd->hScreenDC,											//���޲���÷��.
							( int ) ( Ending.EnEng[eNo].x ),							//�\���ʒux���W.
							( int ) ( Ending.EnEng[eNo].y - Ending.EnEng[eNo].Corr ),	//�\���ʒuy���W.
							Ending.EnEng[eNo].wDest,									//�摜�̕�.
							Ending.EnEng[eNo].hDest,									//�摜�̍���.
							hMemDC,														//���DC.
							Ending.EnEng[eNo].StartX,									//���摜��x���W.
							Ending.EnEng[eNo].StartY,									//���摜��y���W.
							RAD( Ending.EnEng[eNo].Dang ),
							hPlgDC );
					}
				}
			}

			for ( int numNo = 0; numNo < S_NUMMAX; numNo++ ) {
				for ( int dirNo = 0; dirNo < S_DIRMAX; dirNo++ ) {
					if ( ( Player.shot[numNo][dirNo].DispShotFlag == true ) || ( Player.chara.chgCnt != 0 ) ) {
						if ( Player.ability.beam != 0 ) {
							//�L�����N�^�̉摜��������DC�փR�s�[.
							SelectObject( hMemDC, hboss );
							//�v���C���[�̃r�[���e�̕\��.
							TransPlgBlt(
								gameWnd->hScreenDC,																													//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( Player.shot[numNo][dirNo].x - S_SIZE - ( 8 - 7 * ( BEAMMAX - Player.shot[numNo][dirNo].beamCnt ) ) ),			//�\���ʒux.
								static_cast< int >( Player.shot[numNo][dirNo].y - S_SIZE ),																			//�\���ʒuy.
								S_SIZE, S_SIZE * Player.shot[numNo][dirNo].beamCnt,																					//�摜�̕�,����.
								hMemDC,																																//������DC.
								S_SIZE * 7,																															//���摜��x���W
								S_SIZE * 8 + S_SIZE * ( BEAMMAX - Player.shot[numNo][dirNo].beamCnt ),																//���摜��y���W.
								RAD( 90 ) - Player.shot[numNo][dirNo].ang,
								hPlgDC );
						}
						else if ( Player.ability.chg != 0 ) {
							if ( ( Player.shot[numNo][dirNo].DispShotFlag == false ) && ( Player.chara.chgCnt != 0 ) ) {	//���ߏ��.
								//�L�����N�^�̉摜��������DC�փR�s�[.
								SelectObject( hMemDC, hboss );
								//�v���C���[�̗��ߒe�̕\��.
								ExpaTransAlBlend(
									gameWnd->hScreenDC,													//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( Player.shot[numNo][dirNo].x + 5 ),				//�\���ʒux.
									static_cast< int >( Player.shot[numNo][dirNo].y - 5 ),				//�\���ʒuy.
									S_SIZE * 2, S_SIZE * 2,												//�摜�̕�,����.
									hMemDC,																//������DC.
									S_SIZE * 5, S_SIZE * 10,											//���摜��y���W
									static_cast< int >( 50 + Player.chara.chgCnt + Player.chgAniCnt ),
									hWorkDC,
									static_cast< int >( S_SIZE * ( 2 + Player.chara.chgCnt / 75 ) ),	//���̊g��{��.
									static_cast< int >( S_SIZE *( 2 + Player.chara.chgCnt / 75 ) ) );	//�����̊g��{��.	
							}
							else {	//���ˌ�.
								//�L�����N�^�̉摜��������DC�փR�s�[.
								SelectObject( hMemDC, hboss );
								//�v���C���[�̗��ߒe�̕\��.
								ExpaTransBlt(
									gameWnd->hScreenDC,											//�f�o�C�X�R���e�L�X�g.
									static_cast< int >( Player.shot[numNo][dirNo].x ),			//�\���ʒux.
									static_cast< int >( Player.shot[numNo][dirNo].y ),			//�\���ʒuy.
									S_SIZE * 2, S_SIZE * 2,										//�摜�̕�,����.
									hMemDC,														//������DC.
									S_SIZE * 5, S_SIZE * 10,									//���摜��y���W
									static_cast< int >( S_SIZE * ( 2 + Player.chara.chgCnt / 75 ) ),	//���̊g��{��.
									static_cast< int >( S_SIZE *( 2 + Player.chara.chgCnt / 75 ) ) );	//�����̊g��{��.	
							}
						}
						else if ( Player.ability.pene != 0 ) {
							//�L�����N�^�̉摜��������DC�փR�s�[.
							SelectObject( hMemDC, hboss );
							//�v���C���[�̊ђʒe�̕\��.
							TransBlt(
								gameWnd->hScreenDC,											//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( Player.shot[numNo][dirNo].x ),			//�\���ʒux.
								static_cast< int >( Player.shot[numNo][dirNo].y ),			//�\���ʒuy.
								S_SIZE, S_SIZE,												//�摜�̕�,����.
								hMemDC,														//������DC.
								S_SIZE * 3, S_SIZE * 11 );									//���摜��y���W
						}
						else {
							//�L�����N�^�̉摜��������DC�փR�s�[.
							SelectObject( hMemDC, hShot );
							//�v���C���[�̃m�[�}���e�̕\��.
							TransBlt(
								gameWnd->hScreenDC,											//�f�o�C�X�R���e�L�X�g.
								static_cast< int >( Player.shot[numNo][dirNo].x ),			//�\���ʒux.
								static_cast< int >( Player.shot[numNo][dirNo].y ),			//�\���ʒuy.
								S_SIZE, S_SIZE,												//�摜�̕�,����.
								hMemDC,														//������DC.
								0, 0 );														//�摜��y���W.
						}
					}
				}
			}

			break;
		}
		default:	//��L�ȊO�̏ꍇ.
			//��{�����ɂ͗��Ȃ��͂�.
			break;
		}
		if ( Fade.FadeState != enFade::None ){	//�t�F�[�h.
			//�P�F�pDC�ɍ��F�̃u���V���Z�b�g.
			SelectObject( hColorDC, hBBrush );
			//���F�̉摜��p��.
			PatBlt(
				hColorDC,		//�P�F�pDC�֏o��.
				0, 0,			//�\���ʒux,y.
				WND_W, WND_H,	//�摜�̕�,����.
				PATCOPY );
			//�t�F�[�h�̉摜�����DC�ֺ�߰.
			AlBlend(
				gameWnd->hScreenDC,	//���޲���÷��.
				0, 0,				//�\���ʒux,y.
				WND_W, WND_H,		//�摜�̕�,����.
				hColorDC,			//���DC.
				0, 0,				//���摜��x,y���W.
				Fade.FadeAlpha );	//�A���t�@�l.
		}
		//�|�[�Y.
		if ( pause == true ){
			//�L�����N�^�̉摜��������DC�փR�s�[.
			SelectObject( hMemDC, hPause );
			BitBlt(
				gameWnd->hScreenDC,					//���޲���÷��.
				WND_W / 2 - PAUSE_W / 2,			//�\���ʒux���W.
				WND_H / 2 - PAUSE_H / 2,			//�\���ʒuy���W.
				PAUSE_W, PAUSE_H,					//�摜�̕�,����.
				hMemDC,								//���DC.
				0, 0,								//���摜��x,y���W.
				SRCCOPY );							//��߰����.

			//�L�����N�^�̉摜��������DC�փR�s�[.
			SelectObject( hMemDC, hPSlime );
			//�I����̉摜�����DC�ֺ�߰.
			TransAlBlend(
				gameWnd->hScreenDC,																		//���޲���÷��.
				WND_W / 2 - PAUSE_W / 2 + S_SIZE * 3,													//�\���ʒux.
				WND_H / 2 - PAUSE_H / 2 + S_SIZE * 5 + ( S_SIZE * 2 + S_SIZE / 2 + 2 ) * pauseArea,		//�\���ʒuy.
				23, 23,																					//�摜�̕�,����.
				hMemDC,																					//���DC.
				C_SIZE * 3 + 23,																		//���摜��x���W.
				C_SIZE * 2 + 16 * 2,																	//���摜��y���W.
				pauseAlpha, 																					//�A���t�@�l.
				hWorkDC );
		}

#ifdef _DEBUG	/** �� **********************�o�����̕ϐ��f�[�^�\��************************ �� **/
		//FPS�̕\��.
		TextOut( gameWnd->hScreenDC,
			WND_W - 100,
			0,
			MsgFPS, lstrlen( MsgFPS ) );

		//�o�����.
		if ( Scene == enScene::GameMain ){
			const static int	FSize	= 13;		//�t�H���g�T�C�Y.
			const static bool	MasDisp = false;	//���b�Z�[�W�\���t���O.

			if ( MasDisp == true ) {
				int cnt = 0;

				SetTextColor( gameWnd->hScreenDC, RGB( 155, 0, 0 ) );
				wsprintf( MsgE_St, "�{�X.PatternCnt = %d", Boss.PatternCnt );
				TextOut( gameWnd->hScreenDC,
					0,
					FSize * cnt,
					MsgE_St, lstrlen( MsgE_St ) );
				cnt++;

				SetTextColor( gameWnd->hScreenDC, RGB( 155, 0, 0 ) );
				wsprintf( MsgE_St, "�{�X.AttackCnt = %d", Boss.AttackCnt );
				TextOut( gameWnd->hScreenDC,
					0,
					FSize * cnt,
					MsgE_St, lstrlen( MsgE_St ) );
				cnt++;

				SetTextColor( gameWnd->hScreenDC, RGB( 155, 0, 0 ) );
				wsprintf( MsgE_St, "�{�X.AttackMovecnt = %d", Boss.AttackMovecnt );
				TextOut( gameWnd->hScreenDC,
					0,
					FSize * cnt,
					MsgE_St, lstrlen( MsgE_St ) );
				cnt++;

				SetTextColor( gameWnd->hScreenDC, RGB( 155, 0, 0 ) );
				wsprintf( MsgE_St, "�{�X.Blowcnt = %d", Boss.Blowcnt );
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
								wsprintf( MsgE_St, "�@ �G[%d][%d][%d].State = %d, HP = %d, Ran = %d, x = %3d, y = %3d", type, ptn, eno, Enemy[type][ptn][eno].chara.State, Enemy[type][ptn][eno].status.hp, Enemy[type][ptn][eno].ran, static_cast< int >( Enemy[type][ptn][eno].chara.x ), static_cast< int >( Enemy[type][ptn][eno].chara.y ) );
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
						wsprintf( MsgE_St, "�{�X[%d].State = %d, HP = %d, x = %3d, y = %3d", type, Boss.chara[type].State, Boss.status[type].hp, static_cast< int >( Boss.chara[type].x ), static_cast< int >( Boss.chara[type].y ) );
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
								wsprintf( MsgE_St, "�M�~�b�N[%d][%d][%d].State = %d, HP = %d, x = %3d, y = %3d", type, ptn, gno, gimmick[type][ptn][gno].State, gimmick[type][ptn][gno].hp, static_cast< int >( gimmick[type][ptn][gno].x ), static_cast< int >( gimmick[type][ptn][gno].y ) );
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
							wsprintf( MsgE_St, "���e.[%d][%d].x = %3d, y = %3d, Ang = %d, ", num, dir, static_cast< int >( Player.shot[num][dir].x ), static_cast< int >( Player.shot[num][dir].y ), static_cast< int >( DO( Player.shot[num][dir].ang ) ) );
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
										wsprintf( MsgE_St, "�G�e[%d][%d][%d].[%d][%d].x = %3d, y = %3d, Ang = %d", num, dir, type, ptn, eno, static_cast< int >( Enemy[type][ptn][eno].shot[num][dir].x ), static_cast< int >( Enemy[type][ptn][eno].shot[num][dir].y ), static_cast< int >( DO( Enemy[type][ptn][eno].shot[num][dir].ang ) ) );
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
								wsprintf( MsgE_St, "�{�X��[%d][%d][%d].x = %3d, y = %3d, Ang = %d", type, num, dir, static_cast< int >( Boss.rock[type][num][dir].x ), static_cast< int >( Boss.rock[type][num][dir].y ), static_cast< int >( DO( Boss.rock[type][num][dir].Rang ) ) );
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
							wsprintf( MsgE_St, "�{�X�e.[%d][%d].x = %3d, y = %3d, Ang = %d", num, dir, static_cast< int >( Boss.shot[num][dir].x ), static_cast< int >( Boss.shot[num][dir].y ), static_cast< int >( DO( Boss.shot[num][dir].ang ) ) );
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
#endif			/** �� ************************ �o�����̕ϐ��f�[�^�\�� *********************** �� **/

		//�`��̎��ԑ҂�.
		if( idealtime > progressTime ){
			Sleep( idealtime - progressTime );	//���ԑ҂�.
		}

		//�ĕ`��.
		InvalidateRect( gameWnd->hWnd, nullptr, FALSE );

		//1�b�o�߂����H.
		if( progressTime >= 1000 )
		{
			SetTextColor( gameWnd->hScreenDC, RGB( 0, 0, 0 ) );
			wsprintf( MsgFPS, "FPS[%03d]", frames );
			beforeTime = nowTime;	//���݂̎��ԂɍX�V.
			frames = 0;	//�ڰѐ���������.
		}
		frames++;	//�ڰѐ���UP.
	}

//-------------------------------------------------------------------.
//	�������[WM_DESTROY].
//-------------------------------------------------------------------.
	//BITMAP�̉��.
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

	//���S�V�[���pDC�̉��.
	DeleteObject( hDeatBmp );
	DeleteDC( hDeatDC );
	//�摜�̉�]�pDC�̉��.
	DeleteObject( hPlgBmp );
	DeleteDC( hPlgDC );
	//�P�F�pDC�̉��.
	DeleteObject( hColorBmp );
	DeleteDC( hColorDC );
	//��ƗpDC�̉��.
	DeleteObject( hWorkBmp );
	DeleteDC( hWorkDC );

	//�u���V�̉��.
	DeleteObject( hGBrush  );
	DeleteObject( hGrBrush );
	DeleteObject( hBBrush  );
	DeleteObject( hRBrush  );

	//̫�Ẳ��.
	DeleteObject( hFont );

	//���DC�̉��.
	DeleteDC( hMemDC );

	//�U���̒�~.
	pXInput->SetVibration( 0, 0 );

	//�گ�ޏI���ʒm.
	ExitThread( 0 );

	return 1;
}

//����޳�֐�.
LRESULT CALLBACK WindowProc(
	HWND hWnd,		//����޳�����.
	UINT uMsg,		//ү����.
	WPARAM wParam,	//ү���ނɑ΂�����.
	LPARAM lParam)	//ү���ނɑ΂�����.
{
	HDC hDC;		//���޲���÷��.
	PAINTSTRUCT ps;	//�`��\����.

	//---------------------------------------------.
	//	�گ�ފ֘A.
	//---------------------------------------------.
	static GameWindow gameWnd;	//�ްѳ���޳�\����.
	static HANDLE hThread;		//�گ�������.
	DWORD dwID;					//�گ��ID.
	HBITMAP hBmp;				//�ޯ�ϯ�������.

	//ү���ޕʂ̏���.
	switch (uMsg) {
	case WM_DESTROY:	//����޳���j�����ꂽ�Ƃ�.
		//���ع���݂̏I����Windows�ɒʒm����.
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:		//����޳�����������Ƃ�.
		//�����̏�����(��̐ݒ�).
		srand( (unsigned int)time( nullptr ) );
		
		//-----------------------------------------------.
		//	�گ�ނ̐ݒ�.
		//-----------------------------------------------.
		//�گ�ނɓn���l��ݒ�.
		gameWnd.hWnd = hWnd;	//����޳�����.
		gameWnd.size.cx = WND_W;//����޳��.
		gameWnd.size.cy = WND_H;//����޳����.
		gameWnd.dwFPS = FPS;	//FPS�l.

		//HDC�̍쐬.
		hDC = GetDC( hWnd );	//DC(���޲���÷��)���擾.

		//�ޯ�ϯ������ق��쐬.
		hBmp = CreateCompatibleBitmap(
			hDC,
			gameWnd.size.cx,
			gameWnd.size.cy );

		//DC�ƌ݊����̂���DC���쐬.
		gameWnd.hScreenDC = CreateCompatibleDC( hDC );

		//hScreenDC���ޯ�ϯ�ߗp�Ƃ��Ē�`.
		SelectObject( gameWnd.hScreenDC, hBmp );

		//�s�v�ɂȂ���DC�����.
		ReleaseDC( hWnd, hDC );

		//�گ�ނ̍쐬�Ǝ��s.
		hThread = CreateThread(
			nullptr,			//����ق𑼂���۾��Ƌ��L����ꍇ�͎w��.
			0,					//��������(��̫��:0).
			ThreadFunc,			//�گ�ފ֐���.
			(LPVOID)&gameWnd,	//�گ�ނɓn���\����.
			0,					//0:�쐬�Ɠ����Ɏ��s.
			&dwID );			//(out)�گ��ID.

		//�}�E�X�J�[�\��������.
		ShowCursor( FALSE );

		return 0;

	case WM_KEYDOWN:	//���������ꂽ�Ƃ�.
		////�-�ʂ̏���.
		//switch( wParam ){
		//case VK_ESCAPE:	//Escape��.
		//	//�t�F�[�h�C��.
		//	pause			= true;
		//	Fade.InOutFlag	= false;
		//	Fade.FadeState	= enFade::in;
		//	Fade.EndAlpha	= 127;

		//	//�}�E�X�J�[�\��������.
		//	ShowCursor( TRUE );

		//	if( MessageBox( nullptr,
		//		"�Q�[�����I�����܂����H", "�|�[�Y��",
		//		MB_YESNO ) == IDYES )
		//	{
		//		//����޳����邱�Ƃ�Windows�ɒm�点��.
		//		PostMessage( gameWnd.hWnd, WM_CLOSE, 0, 0 );
		//	}
		//	else { 
		//		//�t�F�[�h�A�E�g.
		//		Fade.InOutFlag	= true;
		//		Fade.FadeState	= enFade::out;
		//		Fade.EndAlpha	= 255;

		//		//�}�E�X�J�[�\��������.
		//		ShowCursor( FALSE );
		//	}
		//	break;
		//}

		return 0;

	case WM_CLOSE://����޳�����.
		//����޳�����׸ޗL��.
		//���گ�ޑ��Ŋm�F���邽�߂��׸�.
		gameWnd.isWndClose = TRUE;

		DWORD result;
		while( 1 ){
			//�گ�ނ��I�����������.
			GetExitCodeThread( hThread, &result );
			//�I�����������ق����.
			if( STILL_ACTIVE != result )
			{
				//CloseHandle�ŕ���.
				CloseHandle( hThread );
				//ٰ�߂𔲂���.
				break;
			}
		}
		//����޳��j������.
		DestroyWindow( hWnd );
		return 0;

	case WM_PAINT:		//����޳���X�V���ꂽ�Ƃ�.
		//�`��J�n.
		hDC = BeginPaint( hWnd, &ps );

		BitBlt(
			hDC,				//�\��DC.
			0, 0,				//�\����x,y���W.
			gameWnd.size.cx,	//�\�����镝.
			gameWnd.size.cy,	//�\�����鍂��.
			gameWnd.hScreenDC,	//�ޯ��ޯ̧(�گ�ޓ��ō쐬).
			0, 0,				//���摜��x,y.
			SRCCOPY );			//��߰����.

		//�`��I��.
		EndPaint( hWnd, &ps );
		return 0;
	}

	//Ҳ݂ɕԂ����.
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//Ҳ݊֐�.
int WINAPI WinMain(
	HINSTANCE hInstance,		//�ݽ�ݽ�ԍ�(����޳�̔ԍ�).
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASS wc;	//����޳�׽�\����.
	HWND hWnd;		//����޳�����.
	MSG msg;		//ү����.

	//--------------------------------------.
	//	����CPU�ɑΉ������� ��.
	//--------------------------------------.
	DWORD ProcessAffinitMask;	//CPU���w��.
	HANDLE hProcess;			//��۾������.
	DWORD errnum;				//�װNo.
	BOOL ret;					//�֐����A�l.

	//���g����۾�������ق��擾.
	hProcess = GetCurrentProcess();

	//���g�𓮍삳����CPU���w�肷��.
	ProcessAffinitMask = 1;	//CPU 1���g��(2�i���Ŏw��).
	ret = SetProcessAffinityMask(
		hProcess,
		ProcessAffinitMask );
	if( ret == FALSE )
	{
		errnum = GetLastError();	//�װ�����擾.
		return 0;
	}
	//--------------------------------------.
	//	����CPU�ɑΉ������� ��.
	//--------------------------------------.

	//--------------------------------------.
	//	����޳�׽�̓o�^.
	//--------------------------------------.
	//���ق̓o�^.
	//	CS_HREDRAW:���������̍ĕ`��.
	//	CS_VREDRAW:���������̍ĕ`��.
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//����޳�֐��̓o�^.
	//	DefWindowProc:��̫�ĳ���޳�֐�(��Ŏ����ō��).
	wc.lpfnWndProc = WindowProc;
	//�g��Ȃ�(0�Œ�).
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	//�ݽ�ݽ�ԍ��̓o�^.
	wc.hInstance = hInstance;
	//���݂̓o�^.
	//	nullptr:��̫��.
	wc.hIcon = LoadIcon( hInstance, "ICON" );
	//���ق̓o�^.
	//	nullptr:��̫��.
	wc.hCursor = nullptr;
	//����޳�̔w�i�F.
	//	LTGRAY_BRUSH:���邢�D�F.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	//�ƭ��̓o�^.
	//	nullptr:��̫��.
	wc.lpszMenuName = nullptr;
	//���ع���ݖ�.
	wc.lpszClassName = APP_NAME;

	//����޳��Windows�ɓo�^.
	if (RegisterClass(&wc) == 0)
	{
		//�װү���ޕ\��.
		MessageBox(nullptr,
			"����޳�׽�o�^���s",
			"�װү����",
			MB_YESNOCANCEL);
		//MB_OK:OK���݂̂�.
		//MB_YESNO:�͂��E������.
		//MB_OKCANCEL:OK�E��ݾ�.
		//MB_YESNOCANCEL:�͂��E�������E��ݾ�.
		return 0;
	}

	//------------------------------------------------.
	//	����޳�̈�̒���.
	//------------------------------------------------.
	RECT rect;			//��`�\����.
	DWORD dwStyle;		//����޳����.
	rect.top = 0;		//��.
	rect.left = 0;		//��.
	rect.right = WND_W;	//�E.
	rect.bottom = WND_H;//��.
	dwStyle = WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX;	//����޳���.
	//	WS_SYSMENU		�F �S�Ẵ{�^��.
	//	WS_MINIMIZEBOX	�F �ŏ����{�^��.
	//	WS_MAXIMIZEBOX	�F �ő剻�{�^��.
	//	(WS_MIN + WS_MAX)�ŗ��������� ��.

	//�t���X�N���[�����ǂ���.
	bool FullScreenFlag = false;
	//�t���X�N���[���ɂ��邩�̃��b�Z�[�W�{�b�N�X.
	if ( MessageBox( nullptr,
			"�t���X�N���[���ŋN�����܂����H",
			"�m�F", MB_YESNO ) == IDYES )
	{
		FullScreenFlag	= true;		//�t���X�N���[��.
		dwStyle			= WS_POPUP;	//�g�����E�B���h�E.
	}

	if( AdjustWindowRect(
		&rect,	//(in)��ʻ��ނ���������`�\����.(out)�v�Z����.
		dwStyle,//����޳���.
		FALSE ) == 0 )//�ƭ��������ǂ����w��.
	{
		MessageBox( nullptr,
			"����޳�̈�̒����Ɏ��s",
			"�װү����", MB_OK );
		return 0;
	}

	//------------------------------------------------.
	//	����޳�̍쐬.
	//------------------------------------------------.
	//�E�B���h�E�̕\���ʒu.
	int OffSet_x = 64, OffSet_y = 64;
	//�t���X�N���[���Ȃ獶��ɕύX.
	if ( FullScreenFlag == true ) {
		OffSet_x = 0;
		OffSet_y = 0;
	}

	hWnd = CreateWindow(
		APP_NAME,				//���ع���ݖ�.
		WND_TITLE,				//����޳����.
		dwStyle,				//���ʂ̳���޳.
		OffSet_x, OffSet_y,		//����޳�̕\���ʒu(x,y).
		rect.right - rect.left,	//����޳�̕�.
		rect.bottom - rect.top,	//����޳�̕��A����.
		nullptr,				//�e����޳�������.
		nullptr,				//�ƭ��ݒ�.
		hInstance,				//�ݽ�ݽ�ԍ�.
		nullptr);				//����޳�쐬���ɔ����������Ăɓn���ް�.

	if (hWnd == nullptr) {
		MessageBox(nullptr,
			"����޳�쐬���s",
			"�װү����", MB_OK);
		return 0;
	}

	//�t���X�N���[���Ȃ�𑜓x��ύX����.
	if ( FullScreenFlag == true ) {
		DEVMODE devMode;

		devMode.dmSize			= sizeof( DEVMODE );
		devMode.dmFields		= DM_PELSWIDTH | DM_PELSHEIGHT;
		devMode.dmPelsWidth		= WND_W;						//�𑜓x�̕�.
		devMode.dmPelsHeight	= WND_H;						//�𑜓x�̍���.
		ChangeDisplaySettings( &devMode, CDS_FULLSCREEN );		//�t���X�N���[���ɂ���.
	}

	//����޳�̕\��.
	ShowWindow( hWnd, SW_SHOW );
	//SW_SHOW:�\������.
	//SW_HIDE:�B��(��\��).


	//ү����ٰ��.
	while( GetMessage( &msg, nullptr, 0, 0 ) > 0 ){
		//����޳�֐���ү���ނ𑗂�.
		DispatchMessage( &msg );
	}

	return 0;
}


//�摜�̓ǂݍ��݊֐�.
//�߂�l�̌^�@�֐���( �������A�@������ ).
bool LoadBmp( HBITMAP* phBmp, const char* fileName )
{
	*phBmp = (HBITMAP) LoadImage(
		nullptr,					//�ݽ�ݽ.
		fileName,					//̧�ٖ�(�߽�܂�).
		IMAGE_BITMAP,				//�ޯ�ϯ��.
		0, 0,						//�ǂݍ��ލ��W.
		LR_LOADFROMFILE );			//̧�ق���ǂݍ���.

	if( *phBmp == nullptr ){
		MessageBox( nullptr, fileName, "�摜�ǂݍ��ݎ��s", MB_OK );
		return false;
	}

	return true;
}

//�P�F�摜��\������֐�.
void ColorBlt(
	int BltNo,		HDC hdcDest,  int xDest, int yDest, int wDest, int hDest,
	HDC hdcColor,	COLORREF RGB, int Alpha )
{
	//�u���V�̍쐬.
	HBRUSH hBrush = CreateSolidBrush( RGB );
	SelectObject( hdcColor, hBrush );

	//�P�F�摜�쐬.
	PatBlt(
		hdcColor,		//�P�F�pDC�֏o��.
		0, 0,			//�\���ʒux,y.
		wDest, hDest,	//�摜�̕�,����.
		PATCOPY );

	switch ( BltNo )
	{
	case enBltNo::Bit:
		BitBlt(
			hdcDest,							//�\����(�����ި���ڲ).
			xDest, yDest,						//�\���ʒux,y���W.
			wDest, hDest,						//�\������.
			hdcColor,							//�摜���ݒ肳�ꂽ���DC.
			0, 0,								//���摜�̎��o���J�nx,y���W.
			SRCCOPY );
		break;
	case enBltNo::Trans:
		TransBlt(
			hdcDest,							//�\����(�����ި���ڲ).
			xDest, yDest,						//�\���ʒux,y���W.
			wDest, hDest,						//�\������.
			hdcColor,							//�摜���ݒ肳�ꂽ���DC.
			0, 0 );								//���摜�̎��o���J�nx,y���W.
		break;
	case enBltNo::Blend:
		AlBlend(
			hdcDest,							//�\����(�����ި���ڲ).
			xDest, yDest,						//�\���ʒux,y���W.
			wDest, hDest,						//�\������.
			hdcColor,							//�摜���ݒ肳�ꂽ���DC.
			0, 0,								//���摜�̎��o���J�nx,y���W.
			Alpha );							//�A���t�@�l.
		break;
	default:
		break;
	}

	//�u���V�̉��.
	DeleteObject( hBrush );
}

//�摜�𓧉߂��ĕ\������֐�.
bool TransBlt(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc,  int xSrc,  int ySrc )
{
	if( TransparentBlt(
		hdcDest,							//�\����(�����ި���ڲ).
		xDest, yDest,						//�\���ʒux,y���W.
		wDest, hDest,						//�\������.
		hdcSrc,								//�摜���ݒ肳�ꂽ���DC.
		xSrc, ySrc,							//���摜�̎��o���J�nx,y���W.
		wDest, hDest,						//���摜�̎��o����,����.
		RGB( 0x00, 0xFF, 0x00 ) ) )			//���ߐF(�΂��w��).
	{
		return true;
	}
	return false;
}

//�g�債���摜�𓧉߂��ĕ\������֐�.
bool ExpaTransBlt(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int weDest, int heDest, bool xyFlag )
{
	if ( xyFlag == true ){
		xDest -= weDest / 2;
		yDest -= heDest / 2;
	}

	if ( TransparentBlt(
		hdcDest,					//�\����(�����ި���ڲ).
		xDest, yDest,				//�\���ʒux,y���W.
		weDest, heDest,				//�\����,����.
		hdcSrc,						//�摜���ݒ肳�ꂽ���DC.
		xSrc, ySrc,					//���摜�̎��o���J�nx,y���W.
		wDest, hDest,				//���摜�̎��o����,����.
		RGB( 0x00, 0xFF, 0x00 ) ) )	//���ߐF(�΂��w��).
	{
		return true;
	}
	return false;
}

//�摜�𔼓����ɂ��ĕ\������֐�.
bool AlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int Alpha )
{
	//BLENDFUNCTION�\����.
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, (BYTE)Alpha, 0 };

	//�������ɂ���摜�̕\��.
	if( AlphaBlend(
		hdcDest,		//���޲���÷��.
		xDest, yDest,	//�\���ʒux,y���W.
		wDest, hDest,	//�\����,����.
		hdcSrc,			//�ǂݍ��݌�DC.
		xSrc, ySrc,		//�ǂݍ��݌��摜x,y���W.
		wDest, hDest,	//�ǂݍ��݌��摜��,����.
		blendfunc ) )	//������̧ݸ���.
	{
		return true;
	}
	return false;
}

//��]�����摜�𓧉߂��ĕ\������֐�.
bool TransPlgBlt(
	HDC hdcDest, LONG xDest, LONG yDest, LONG wDest, LONG hDest,
	HDC hdcSrc,	 LONG xSrc,  LONG ySrc,  DOUBLE Ang, HDC hdcPlg )
{
 	DOUBLE	whDest = ( wDest / 2 );										//�摜�̕��̔���.
	DOUBLE	hhDest = ( hDest / 2 );										//�摜�̍����̔���.
	DOUBLE	radius = sqrt( ( whDest * whDest ) + ( hhDest * hhDest ) );	//��]���a.
	DOUBLE	theta = atan2( whDest, hhDest );							//�O�p�`�̊p�x �Ɗp�x.
	POINT	po[3];														//��`�̉�]���_.

	//��`�̍���̒��_.
	Ang -= ( theta + PI / 2 );	//-( �� + 90 )�x.
	po[0].x = ( LONG )( radius + cos( Ang ) * radius ); 
	po[0].y = ( LONG )( radius + sin( Ang ) * radius );
	//��`�̉E��̒��_.
	Ang += theta * 2;			//+( �� * 2 )�x.
	po[1].x = ( LONG )( radius + cos( Ang ) * radius );
	po[1].y = ( LONG )( radius + sin( Ang ) * radius );
	//��`�̍����̒��_.
	Ang += PI;				//+( 180 )�x.
	po[2].x = ( LONG )( radius + cos( Ang ) * radius );
	po[2].y = ( LONG )( radius + sin( Ang ) * radius );

	//���DC�̔w�i��΂œh��.
	PatBlt(
		hdcPlg,																//�P�F�pDC�֏o��.
		0, 0,																//�\���ʒux,y.
		static_cast< int >( radius * 2 ), static_cast< int >( radius * 2 ),	//�摜�̕�,����.
		PATCOPY );
	//���DC�ɉ�]�����摜���o��.
	PlgBlt(
		hdcPlg,					//�f�o�C�X�R���e�L�X�g.
		po,						//��`�̉�]���_.
		hdcSrc,					//������DC.
		xSrc, ySrc,				//���摜��x,y���W.
		wDest, hDest,			//�摜�̕�,����.
		NULL,
		0, 0 );
	//���DC�̉摜�𓧉߂��ăX�N���[���ɏo��.
	TransBlt(
		hdcDest,							//�f�o�C�X�R���e�L�X�g.
		xDest, yDest,						//�\���ʒux,y.
		static_cast< int >( radius * 2 ),	//�摜�̕�.
		static_cast< int >( radius * 2 ),	//�摜�̍���.
		hdcPlg,								//������DC.
		0, 0 );								//���摜��x,y���W.

	return true;
}

//��]�����摜�𓧉߂��Ĕ������ɂ���֐�.
bool TransPlgAlBlend(
	HDC hdcDest, LONG xDest, LONG yDest, LONG wDest, LONG hDest,
	HDC hdcSrc,  LONG xSrc,  LONG ySrc,  DOUBLE Ang, HDC hdcPlg, int Alpha, HDC hdcWork )
{
	//BLENDFUNCTION�\����.
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, ( BYTE )Alpha, 0 };

	DOUBLE	whDest = ( wDest / 2 );										//�摜�̕��̔���.
	DOUBLE	hhDest = ( hDest / 2 );										//�摜�̍����̔���.
	DOUBLE	radius = sqrt( ( whDest * whDest ) + ( hhDest * hhDest ) );	//��]���a.
	DOUBLE	theta = atan2( whDest, hhDest );							//�O�p�`�̊p�x �Ɗp�x.
	POINT	po[3];														//��`�̉�]���_.

	//��`�̍���̒��_.
	Ang -= ( theta + PI / 2 );	//-( �� + 90 )�x.
	po[0].x = ( LONG )( radius + cos( Ang ) * radius );
	po[0].y = ( LONG )( radius + sin( Ang ) * radius );
	//��`�̉E��̒��_.
	Ang += theta * 2;			//+( �� * 2 )�x.
	po[1].x = ( LONG )( radius + cos( Ang ) * radius );
	po[1].y = ( LONG )( radius + sin( Ang ) * radius );
	//��`�̍����̒��_.
	Ang += PI;				//+( 180 )�x.
	po[2].x = ( LONG )( radius + cos( Ang ) * radius );
	po[2].y = ( LONG )( radius + sin( Ang ) * radius );

	//���DC�̔w�i��΂œh��.
	PatBlt(
		hdcPlg,																//�P�F�pDC�֏o��.
		0, 0,																//�\���ʒux,y.
		static_cast< int >( radius * 2 ), static_cast< int >( radius * 2 ),	//�摜�̕�,����.
		PATCOPY );
	//���DC�ɉ�]�����摜���o��.
	PlgBlt(
		hdcPlg,					//�f�o�C�X�R���e�L�X�g.
		po,						//��`�̉�]���_.
		hdcSrc,					//������DC.
		xSrc, ySrc,				//���摜��x,y���W.
		wDest, hDest,			//�摜�̕�,����.
		NULL,
		0, 0 );
	//�\���ʒu�̃X�N���[���̉摜���o��.
	BitBlt(
		hdcWork,		//���DC�֏o��.
		0, 0,
		static_cast< int >( radius * 2 ),
		static_cast< int >( radius * 2 ),
		hdcDest,		//�f�B�X�v���C����؂���.
		xDest, yDest,
		SRCCOPY );
	//���DC�̉摜�𓧉߂��ăX�N���[���ɏo��.
	TransBlt(
		hdcWork,							//�f�o�C�X�R���e�L�X�g.
		0, 0,								//�\���ʒux,y.
		static_cast< int >( radius * 2 ),	//�摜�̕�.
		static_cast< int >( radius * 2 ),	//�摜�̍���.
		hdcPlg,								//������DC.
		0, 0 );								//���摜��x,y���W.
	//��ƗpDC�ɗp�ӂ������𔼓����ɂ��ĕ\��.
	AlphaBlend(
		hdcDest,		//�f�B�X�v���C�֏o��.
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

//�摜�𓧉߂��Ĕ������ɂ���.
bool TransAlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int Alpha, HDC hdcWork )
{
	//BLENDFUNCTION�\����.
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, ( BYTE ) Alpha, 0 };

	BitBlt(
		hdcWork,		//���DC�֏o��.
		0, 0,
		wDest, hDest,
		hdcDest,		//�f�B�X�v���C����؂���.
		xDest, yDest,
		SRCCOPY );
	TransparentBlt(
		hdcWork,		//���DC�֏o��.
		0, 0,
		wDest, hDest,
		hdcSrc,
		xSrc, ySrc,
		wDest, hDest,
		RGB( 0x00, 0xFF, 0x00 ) );
	AlphaBlend(
		hdcDest,		//�f�B�X�v���C�֏o��.
		xDest, yDest,
		wDest, hDest,
		hdcWork,
		0, 0,
		wDest, hDest,
		blendfunc );
	return true;
}

//�g�債���摜�𓧉߂��Ĕ������ɂ���.
bool ExpaTransAlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int Alpha, HDC hdcWork, int weDest, int heDest, bool xyFlag )
{
	//BLENDFUNCTION�\����.
	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, ( BYTE ) Alpha, 0 };

	if ( xyFlag == true ) {
		xDest -= weDest / 2;
		yDest -= heDest / 2;
	}

	BitBlt(
		hdcWork,		//���DC�֏o��.
		0, 0,
		weDest, heDest,
		hdcDest,		//�f�B�X�v���C����؂���.
		xDest, yDest,
		SRCCOPY );
	TransparentBlt(
		hdcWork,		//���DC�֏o��.
		0, 0,
		weDest, heDest,
		hdcSrc,
		xSrc, ySrc,
		wDest, hDest,
		RGB( 0x00, 0xFF, 0x00 ) );
	AlphaBlend(
		hdcDest,		//�f�B�X�v���C�֏o��.
		xDest, yDest,
		weDest, heDest,
		hdcWork,
		0, 0,
		weDest, heDest,
		blendfunc );
	return true;
}

//��`���m�̏Փ˔���.
bool CollisionDetection(
	double Ax, double Ay, int Aw, int Ah, //��`A��x,y���W�ƕ��ƍ���.
	double Bx, double By, int Bw, int Bh )//��`B��x,y���W�ƕ��ƍ���.
{
	//(Ax<=Bx+Bw)&&(Ax+Aw>=Bx)&&(Ay<=By+Bh)&&(Ay+Ah>=By).
	if( ( Ax < Bx + Bw ) && ( Ax + Aw > Bx ) &&
		( Ay < By + Bh ) && ( Ay + Ah > By ) )
	{
		//����������.
		return true;
	}
	return false;	//�O��Ă���.
}

//�R���g���[���[�܂��̓L�[�{�[�h�̂ǂ����������ꂽ.
bool GetAsyncAnyKey( PLAYER* Player, CXInput* pXInput, XInStTrg* XInKey )
{
	//�L�[�{�[�h.
	//for ( int vKeyNo = 8; vKeyNo <= 228; vKeyNo++ ){
	//	if ( GetAsyncKeyState( vKeyNo ) & 0x8000 ){
	//		return true;
	//	}
	//}
	//�L�[�{�[�h�̎g�p����S�Ẵ{�^��.
	int vKeyNo[9] = { 'A', 'S', 'D', 'F', VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN };
	for ( int KeyNo = 0; KeyNo < 9; KeyNo++ ) {
		if ( GetAsyncKeyState( vKeyNo[KeyNo] ) & 0x8000 ){
			return true;
		}
	}

	//�R���g���[���[���ڑ�����Ă���.
	if ( pXInput->IsConnect() == true ) {

		//�R���g���[���[�̑S�Ẵ{�^��.
		int XINPUTNo[14] = { XINPUT_GAMEPAD_DPAD_UP,		XINPUT_GAMEPAD_DPAD_DOWN,	XINPUT_GAMEPAD_DPAD_LEFT,	XINPUT_GAMEPAD_DPAD_RIGHT,
							 XINPUT_GAMEPAD_BACK ,			XINPUT_GAMEPAD_LEFT_THUMB,	XINPUT_GAMEPAD_RIGHT_THUMB,	XINPUT_GAMEPAD_LEFT_SHOULDER,
							 XINPUT_GAMEPAD_RIGHT_SHOULDER,	XINPUT_GAMEPAD_A,			XINPUT_GAMEPAD_B,			XINPUT_GAMEPAD_X,				XINPUT_GAMEPAD_Y
		};
		for ( int PadNo = 0; PadNo < 14; PadNo++ ) {
			if ( pXInput->IsKeyDown( XINPUTNo[PadNo] ) ) {
				return true;
			}
		}

		//�R���g���[���̍��E�̃g���K�[.
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