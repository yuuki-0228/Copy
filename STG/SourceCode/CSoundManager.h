#pragma once
#include "CSound.h"


/************************************************
*	サウンドマネージャークラス(サウンド管理クラス).
*		singleton(シングルトン)で作成.
**/
class CSoundManager
{
public:
	//サウンドリスト列挙体.
	enum enList
	{
		BGM_Title,		//タイトル.
		BGM_Main,		//メイン.
		BGM_Boss,		//ボス戦.
		BGM_GameOver,	//ゲームオーバー.
		BGM_Ending,		//エンディング.

		SE_PSNorm,		//プレイヤーの通常弾.
		SE_PSChg,		//プレイヤーの溜め弾.
		SE_PSPene,		//プレイヤーの貫通弾.
		SE_PSBeam,		//プレイヤーのビーム弾.
		SE_ESChg,		//敵の溜め弾.
		SE_ESNorm,		//敵の通常弾.
		SE_ESPene,		//敵の貫通弾.
		SE_ESBeam,		//敵のビーム弾.
		SE_EShoming,	//敵の追尾弾.
		SE_SChgCnd,		//チャージ完了.
		SE_PDamage,		//プレイヤーのダメージ.
		SE_EDamage,		//敵のダメージ.
		SE_GDamage,		//ギミックダメージ.
		SE_GRockDest,	//ギミック[岩]破壊.
		SE_GTreeDest,	//ギミック[木]破壊.
		SE_Heel,		//キャラ回復.
		SE_Barr,		//キャラバリアで防ぐ.
		SE_AGet,		//能力取得.
		SE_ADelete,		//能力削除.

		SE_BEarth,		//ボスの入退出.
		SE_BPANT,		//ボスのパンチ.
		SE_BPANTCrush,	//ボスパンチ激突.
		SE_BROCK,		//岩出現音.
		SE_BROCKUP,		//岩突き出し.
		SE_BROCKCrush,	//地面叩き.
		SE_BollA,		//吹き飛ばし.

		SE_Jump,		//ジャンプ.
		SE_Landing,		//着地音.
		SE_Best,		//最高Hit数更新.

		SE_Decision,	//決定.

		//音が増えたらここに追加してください.
		Max
	};

public:
	//インスタンス取得(唯一のアクセス経路).
	static CSoundManager* GetInstance()
	{
		//唯一のインスタンスを作成する.
		//※staticで作成されたので2回目以降は無視される.
		static CSoundManager s_Instance;//s_:スタティックの意味.
		return &s_Instance;
	}

	~CSoundManager();

	//サウンドデータ読込関数.
	bool LoadData();
	//サウンドデータ解放関数.
	void ReleaseData();

	//ウィンドウハンドルを設定.
	void SetWnd( HWND hWnd ) { m_hWnd = hWnd; }
	//サウンドを取得.
	CSound* GetSound( enList list ) { return m_pSound[list]; }

	//SEを再生する.
	static void PlaySE( enList list )	{ CSoundManager::GetInstance()->m_pSound[list]->PlaySE(); }
	//ループで再生する.
	static void PlayLoop( enList list ) { CSoundManager::GetInstance()->m_pSound[list]->PlayLoop(); }
	//停止する.
	static void Stop( enList list )		{ CSoundManager::GetInstance()->m_pSound[list]->Stop(); }

private://外部からアクセス不可能.
	//生成やコピーを禁止する.
	CSoundManager();
	CSoundManager( const CSoundManager& rhs ) = delete;
	CSoundManager& operator = ( const CSoundManager& rhs ) = delete;

private:
	CSound*	m_pSound[enList::Max];
	HWND	m_hWnd;
};