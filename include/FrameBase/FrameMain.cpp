#include <conio.h>
#include "surfaceManager.h"
#include "../CustomSurface/EditorSurface.h"

int main()
{
	StartFrame();
	while (!SurfaceManager::GetInstance().Empty())
		SurfaceManager::GetInstance().PostEvent(_getch());
	return 0;
}
