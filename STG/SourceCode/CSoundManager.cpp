#include "CSoundManager.h"

CSoundManager::CSoundManager()
	: m_pSound	()
	, m_hWnd	()
{
	//サウンドのインスタンス生成.
	for ( int i = 0; i < enList::Max; i++ )
	{
		m_pSound[i] = new CSound();
	}
}

CSoundManager::~CSoundManager()
{
	ReleaseData();

	//サウンドの破棄.
	for ( int i = enList::Max - 1; i >= 0; i-- )
	{
		if ( m_pSound[i] != nullptr ) {
			delete m_pSound[i];
			m_pSound[i] = nullptr;
		}
	}
}

//サウンドデータ読込関数.
bool CSoundManager::LoadData()
{
	//ファイルで管理できる.
	//C++の連想配列とか使えればもっといい感じで書けるかもしれない.
	struct SoundList
	{
		int		No;
		char	Path[256];
		char	Alias[64];
		int		Volume;
	};
	SoundList SList[enList::Max] =
	{
		{enList::BGM_Title,		"Data\\BGM\\nc192553.mp3",					"BGM_Title",		1000 },
		{enList::BGM_Main,		"Data\\BGM\\bgm_maoudamashii_8bit17.mp3",	"BGM_Main",			1000 },
		{enList::BGM_Boss,		"Data\\BGM\\GB-RPG-B13-2.mp3",				"BGM_Boss",			1000 },
		{enList::BGM_GameOver,	"Data\\BGM\\bgm_maoudamashii_8bit20.mp3",	"BGM_GameOver",		1000 },
		{enList::BGM_Ending,	"Data\\BGM\\GB-RPG-B14-3.mp3",				"BGM_Ending",		1000 },
		{enList::SE_PSNorm,		"Data\\SE\\shoot2.mp3",						"SE_PSNorm",		1000 },
		{enList::SE_PSChg,		"Data\\SE\\launcher1.mp3",					"SE_PSChg",			1000 },
		{enList::SE_PSPene,		"Data\\SE\\laser_beam.mp3",					"SE_PSPene",		1000 },
		{enList::SE_PSBeam,		"Data\\SE\\laser1.mp3",						"SE_PSBeam",		1000 },
		{enList::SE_ESChg,		"Data\\SE\\launcher4.mp3",					"SE_ESChg",			300  },
		{enList::SE_ESNorm,		"Data\\SE\\damage7.mp3",					"SE_ESNorm",		100  },
		{enList::SE_ESPene,		"Data\\SE\\attack1.mp3",					"SE_ESPene",		300  },
		{enList::SE_ESBeam,		"Data\\SE\\beam0.mp3",						"SE_ESBeam",		100  },
		{enList::SE_EShoming,	"Data\\SE\\handy_lasergun.mp3",				"SE_EShoming",		700  },
		{enList::SE_SChgCnd,	"Data\\SE\\get.mp3",						"SE_SChgCnd",		700  },
		{enList::SE_PDamage,	"Data\\SE\\damage2.mp3",					"SE_PDamage",		1000 },
		{enList::SE_EDamage,	"Data\\SE\\damage1.mp3",					"SE_EDamage",		700  },
		{enList::SE_GDamage,	"Data\\SE\\heavy_punch1.mp3",				"SE_GDamage",		300  },
		{enList::SE_GRockDest,	"Data\\SE\\rock-break1.mp3",				"SE_GRockDest",		500  },
		{enList::SE_GTreeDest,	"Data\\SE\\rock-crack1.mp3",				"SE_GTreeDest",		500  },
		{enList::SE_Heel,		"Data\\SE\\magic-status-cure2.mp3",			"SE_Heel",			500  },
		{enList::SE_Barr,		"Data\\SE\\katana-clash3.mp3",				"SE_Barr",			1000 },
		{enList::SE_AGet,		"Data\\SE\\powerup04.mp3",					"SE_AGet",			700  },
		{enList::SE_ADelete,	"Data\\SE\\powerup03.mp3",					"SE_ADelete",		700  },
		{enList::SE_BEarth,		"Data\\SE\\Earth.mp3",						"SE_BEarth",		1000 },
		{enList::SE_BPANT,		"Data\\SE\\nc191002.mp3",					"SE_BPANT",			500	 },
		{enList::SE_BPANTCrush,	"Data\\SE\\se_maoudamashii_battle12.mp3",	"SE_BPANTCrush",	500	 },
		{enList::SE_BROCK,		"Data\\SE\\nc225820.mp3",					"SE_BROCK",			500  },
		{enList::SE_BROCKUP,	"Data\\SE\\nc225827.mp3",					"SE_BROCKUP",		500  },
		{enList::SE_BROCKCrush,	"Data\\SE\\nc117248.mp3",					"SE_BROCKCrush",	500  },
		{enList::SE_BollA,		"Data\\SE\\Blowawa.mp3",					"SE_BollA",			1000 },
		{enList::SE_Jump,		"Data\\SE\\Jump.mp3",						"SE_Jump",			500  },
		{enList::SE_Landing,	"Data\\SE\\Landing.mp3",					"SE_Landing",		1000 },
		{enList::SE_Decision,	"Data\\SE\\decision39.mp3",					"SE_Decision",		1000 },
		{enList::SE_Best,		"Data\\SE\\Best.mp3",						"SE_Best",			1000 },
	}; 
	for ( int i = 0; i < enList::Max; i++ ) {
		if ( m_pSound[SList[i].No]->Open(
			SList[i].Path,
			SList[i].Alias,
			m_hWnd ) == false )
		{
			return false;
		}
		//音量設定.
		m_pSound[SList[i].No]->SetVolume( SList[i].Volume );
	}
	return true;
}

//サウンドデータ解放関数.
void CSoundManager::ReleaseData()
{
	//音声ファイルを閉じる.
	for ( int i = enList::Max - 1; i >= 0; i-- )
	{
		m_pSound[i]->Close();
	}
}
