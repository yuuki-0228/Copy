#include "Main.h"

//”\—Ííœ.
void DeleteAblt( PLAYER* player )
{
	for ( int i = 0; i < ABLT_MAX; i++ ) {
		if ( player->DGAbi[i].DispFlag == true ) {
			player->DGAbi[i].xy.y -= 2;

			if ( player->DGAbi[i].xy.y < WND_H - ABLT_SIZE - C_SIZE / 2 ) {
				player->DGAbi[i].Expa += 0.01f;
				player->DGAbi[i].xy.x -= 0.25;
				player->DGAbi[i].Alpha -= 5;
				if ( player->DGAbi[i].Alpha <= 0 ) {
					player->DGAbi[i].Alpha = 0;
					player->DGAbi[i].DispFlag = false;
				}
			}
		}
	}
}
