#include "MainGame.h"

MainGame::MainGame()
: hOwner(NULL)
, hMainDC(NULL)
, hMemDC(NULL)
, hMemBitmap(NULL)
, hOldMemBitmap(NULL)
, update_delay(100)
, update_dt_any(0)
, update_dt_mov(0)
, input_dt(0)
, draw_dt(0)
, C_speed(80)
{
	hStage = NULL;
	hCursor = NULL;
	hStart = NULL;
	hButton = NULL;
	hBazzi = NULL;
	CMove = idle;
	button_01 = false;
	button_02 = false;
	button_snd = false;
	button_03 = false;
	door = true;
	stop = false;
	Animation = 0;
	MainState = Login;
	Ccenter = { 600, 530 };
	for (int j = 0; j < 13; j++)
	for (int i = 0; i < 15; i++)
		Tile[j][i] = { 0 };
	FMOD_SYSTEM* f_System = NULL;
	FMOD_SOUND* sound[10] = { 0 };
	FMOD_CHANNEL* channel[5] = { 0 };
}
MainGame::~MainGame()
{
	remove();
	release();
}

void MainGame::Attach(HWND hWnd)
{
	release();

	hOwner = hWnd;

	init();
}
void MainGame::Input(DWORD tick)
{
	InputDevice.Update(tick);

	ptMouse = InputDevice.pos().ToClient(hOwner);

	FMOD_System_Update(f_System); // 사운드가 스트리밍 되면서 나올 수 있게...


	if (InputDevice[VK_LEFT])
	{
		CMove = left;
		stop = false;
		input_dt = 0;
	}
	if (InputDevice[VK_RIGHT])
	{
		CMove = right;
		stop = false;
		input_dt = 0;
	}
	if (InputDevice[VK_UP])
	{
		CMove = up;
		stop = false;
		input_dt = 0;
	}
	if (InputDevice[VK_DOWN])
	{
		CMove = down;
		stop = false;
		input_dt = 0;
	}
	if (InputDevice[VK_SPACE])
	{
		C_speed = 90;
	}
	if (!InputDevice[VK_LEFT] && !InputDevice[VK_RIGHT] && !InputDevice[VK_DOWN] && !InputDevice[VK_SPACE] && !InputDevice[VK_UP])
	{
		stop = true;
		if (input_dt > 3000)
		{
			CMove = idle;
			input_dt -= 3000;
		}
		input_dt += tick;
	}


}
void MainGame::Update(DWORD tick, HWND hWnd)
{
	if ((button_01 || button_02) && button_snd && (MainState == Login))
		FMOD_System_PlaySound(f_System, FMOD_CHANNEL_FREE, sound[3], 0, &channel[1]), button_snd = false;

	if (button_03 && button_snd && (MainState == HowTo))
		FMOD_System_PlaySound(f_System, FMOD_CHANNEL_FREE, sound[3], 0, &channel[1]), button_snd = false;

	if (button_02 && InputDevice[VK_LBUTTON])
		MainState = HowTo;

	if ((button_01 || button_03) && InputDevice[VK_LBUTTON])
		MainState = Stage;

	if (MainState == Login)
	{
		if (ptMouse.x > 150 && ptMouse.x < 350 && ptMouse.y > 500 && ptMouse.y < 570)
			button_01 = true;
		else if (ptMouse.x > 450 && ptMouse.x < 650 && ptMouse.y > 500 && ptMouse.y < 570)
			button_02 = true;
		else
		{
			button_snd = true;
			button_01 = false;
			button_02 = false;
		}
	}

	else if (MainState == HowTo)
	{
		if (ptMouse.x > 300 && ptMouse.x < 500 && ptMouse.y > 500 && ptMouse.y < 570)
			button_03 = true;
		else
		{
			button_snd = true;
			button_03 = false;
		}
	}

	else if (MainState == Stage)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		FMOD_Channel_Stop(channel[0]);
		if (door)
		{
			FMOD_System_PlaySound(f_System, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
			door = !door;
		}

		if (update_dt_any > update_delay)
		{
			if ((CMove == left) && !stop)
			{
				if (Animation < 280)
					Animation += 70;
				else
					Animation = 0;
			}
			if ((CMove == right) && !stop)
			{
				if (Animation < 280)
					Animation += 70;
				else
					Animation = 0;
			}
			if ((CMove == up) && !stop)
			{
				if (Animation < 280)
					Animation += 70;
				else
					Animation = 0;
			}
			if ((CMove == down) && !stop)
			{
				if (Animation < 280)
					Animation += 70;
				else
					Animation = 0;
			}
			if (CMove == idle)
			{
				if (Animation < 280)
					Animation += 70;
				else
					Animation = 140;
			}
			update_dt_any -= update_delay;
		}
		if (update_dt_mov > (update_delay - C_speed))
		{
			if ((CMove == left) && !stop)
			{
				if (Ccenter.x < 45)
					Ccenter.x += 5;
				Ccenter.x -= 5;
			}
			if ((CMove == right) && !stop)
			{
				if (Ccenter.x > 600)
					Ccenter.x -= 5;
				Ccenter.x += 5;
			}
			if ((CMove == up) && !stop)
			{
				if (Ccenter.y < 45)
					Ccenter.y += 5;
				Ccenter.y -= 5;
			}
			if ((CMove == down) && !stop)
			{
				if (Ccenter.y > 535)
					Ccenter.y -= 5;
				Ccenter.y += 5;
			}
			update_dt_mov -= (update_delay - C_speed);
		}
		update_dt_any += tick;
		update_dt_mov += tick;
	}
}
void MainGame::Draw(DWORD tick)
{

	Rect rc;
	::GetClientRect(hOwner, &rc);

	if (MainState == Login)
	{
		///////////////////////////시작화면///////////////////////
		HDC hBackgroundDC = ::CreateCompatibleDC(hMemDC);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBackgroundDC, hStart);

		::StretchBlt(hMemDC, 0, 0, rc.right + 1, rc.bottom + 1, hBackgroundDC, 0, 0, 369, 328, SRCCOPY);

		::SelectObject(hBackgroundDC, hOldBitmap);
		::DeleteDC(hBackgroundDC);

		///////////////////////////버튼///////////////////////
		HDC hButtonDC = ::CreateCompatibleDC(hMemDC);
		hOldBitmap = (HBITMAP)::SelectObject(hButtonDC, hButton);

		if (button_01)
			::GdiTransparentBlt(hMemDC, 150, 500, 200, 70, hButtonDC, 330, 0, 169, 59, RGB(255, 255, 255));
		else
			::GdiTransparentBlt(hMemDC, 150, 500, 200, 70, hButtonDC, 0, 0, 169, 59, RGB(255, 255, 255));
		if (button_02)
			::GdiTransparentBlt(hMemDC, 450, 500, 200, 70, hButtonDC, 330, 0, 169, 59, RGB(255, 255, 255));
		else
			::GdiTransparentBlt(hMemDC, 450, 500, 200, 70, hButtonDC, 0, 0, 169, 59, RGB(255, 255, 255));
		::GdiTransparentBlt(hMemDC, 190, 515, 120, 40, hButtonDC, 170, 9, 80, 30, RGB(255, 255, 255));
		::GdiTransparentBlt(hMemDC, 490, 515, 120, 40, hButtonDC, 250, 8, 80, 30, RGB(255, 255, 255));

		::SelectObject(hButtonDC, hOldBitmap);
		::DeleteDC(hButtonDC);

		///////////////////////////커서///////////////////////
		HDC hBitmapDC = ::CreateCompatibleDC(hMemDC);
		hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hCursor);

		::GdiTransparentBlt(hMemDC, ptMouse.x, ptMouse.y, 35, 37, hBitmapDC, 0, 0, 35, 37, RGB(0, 255, 0));

		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);
	}
	else if (MainState == HowTo)
	{
		///////////////게임 방법/////////////////////
		HDC hBackgroundDC = ::CreateCompatibleDC(hMemDC);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBackgroundDC, hHowTo);

		::GdiTransparentBlt(hMemDC, 0, 0, rc.right + 1, rc.bottom + 1, hBackgroundDC, 1, 1, 363, 333, RGB(255, 0, 0));
		::GdiTransparentBlt(hMemDC, 100, 180, 400, 70, hBackgroundDC, 368, 199, 307, 46, RGB(255, 0, 0));
		::GdiTransparentBlt(hMemDC, 100, 250, 400, 200, hBackgroundDC, 370, 40, 213, 153, RGB(255, 0, 0));
		::GdiTransparentBlt(hMemDC, 330, 350, 350, 100, hBackgroundDC, 370, 0, 308, 35, RGB(255, 0, 0));

		::SelectObject(hBackgroundDC, hOldBitmap);
		::DeleteDC(hBackgroundDC);

		///////////////////////////버튼///////////////////////
		HDC hButtonDC = ::CreateCompatibleDC(hMemDC);
		hOldBitmap = (HBITMAP)::SelectObject(hButtonDC, hButton);

		if (button_03)
			::GdiTransparentBlt(hMemDC, 300, 500, 200, 70, hButtonDC, 330, 0, 169, 59, RGB(255, 255, 255));
		else
			::GdiTransparentBlt(hMemDC, 300, 500, 200, 70, hButtonDC, 0, 0, 169, 59, RGB(255, 255, 255));
		::GdiTransparentBlt(hMemDC, 340, 515, 120, 40, hButtonDC, 170, 9, 80, 30, RGB(255, 255, 255));

		::SelectObject(hButtonDC, hOldBitmap);
		::DeleteDC(hButtonDC);

		///////////////////////////커서///////////////////////
		HDC hBitmapDC = ::CreateCompatibleDC(hMemDC);
		hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hCursor);

		::GdiTransparentBlt(hMemDC, ptMouse.x, ptMouse.y, 35, 37, hBitmapDC, 0, 0, 35, 37, RGB(0, 255, 0));

		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);
	}
	else if (MainState == Stage)
	{
		Rect Arena = { 5, 10, 635, 570 };

		HDC hBackgroundDC = ::CreateCompatibleDC(hMemDC);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBackgroundDC, hStage);

		::BitBlt(hMemDC, 0, 0, rc.right, rc.bottom, hBackgroundDC, 0, 0, SRCCOPY);

		hOldBitmap = (HBITMAP)::SelectObject(hBackgroundDC, hBazzi);

		if (CMove == left)
		{
			if (stop)
				::GdiTransparentBlt(hMemDC, Ccenter.x - 35, Ccenter.y - 40, 70, 80, hBackgroundDC, 0, 0, 70, 80, RGB(0, 255, 0));
			else
				::GdiTransparentBlt(hMemDC, Ccenter.x - 35, Ccenter.y - 40, 70, 80, hBackgroundDC, Animation, 0, 70, 80, RGB(0, 255, 0));
		}
		if (CMove == right)
		{
			if (stop)
				::GdiTransparentBlt(hMemDC, Ccenter.x - 35, Ccenter.y - 40, 70, 80, hBackgroundDC, 70, 80, 70, 80, RGB(0, 255, 0));
			else
				::GdiTransparentBlt(hMemDC, Ccenter.x - 35, Ccenter.y - 40, 70, 80, hBackgroundDC, Animation, 80, 70, 80, RGB(0, 255, 0));

		}
		if (CMove == up)
		{
			if (stop)
				::GdiTransparentBlt(hMemDC, Ccenter.x - 35, Ccenter.y - 40, 70, 80, hBackgroundDC, 0, 160, 70, 80, RGB(0, 255, 0));
			else
				::GdiTransparentBlt(hMemDC, Ccenter.x - 35, Ccenter.y - 40, 70, 80, hBackgroundDC, Animation, 160, 70, 80, RGB(0, 255, 0));
		}
		if (CMove == down)
		{
			if (stop)
				::GdiTransparentBlt(hMemDC, Ccenter.x - 35, Ccenter.y - 40, 70, 80, hBackgroundDC, 70, 240, 70, 80, RGB(0, 255, 0));
			else
				::GdiTransparentBlt(hMemDC, Ccenter.x - 35, Ccenter.y - 40, 70, 80, hBackgroundDC, Animation, 240, 70, 80, RGB(0, 255, 0));
		}
		if (CMove == idle)
		{
			::GdiTransparentBlt(hMemDC, Ccenter.x - 35, Ccenter.y - 40, 70, 80, hBackgroundDC, Animation, 320, 70, 80, RGB(0, 255, 0));
		}


		::SelectObject(hBackgroundDC, hOldBitmap);
		::DeleteDC(hBackgroundDC);
	}


	::BitBlt(hMainDC, 0, 0, rc.width(), rc.height(), hMemDC, 0, 0, SRCCOPY);

}
void MainGame::Load(void)
{
	Rect rc;
	::GetClientRect(hOwner, &rc);

	hStart = (HBITMAP)::LoadImage(NULL, _T("start.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	hStage = (HBITMAP)::LoadImage(NULL, _T("Stage.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	hCursor = (HBITMAP)::LoadImage(NULL, _T("cursor.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	hButton = (HBITMAP)::LoadImage(NULL, _T("button.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	hHowTo = (HBITMAP)::LoadImage(NULL, _T("HowTo.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	hBazzi = (HBITMAP)::LoadImage(NULL, _T("bazzi.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	ptMouse = Point(rc.width() / 2, rc.height() / 2);

	/*::SetCursorPos(ptMouse.ToScreen(hOwner).x, ptMouse.ToScreen(hOwner).y);*/
	::ShowCursor(FALSE);
}
void MainGame::remove(void)
{
	::ShowCursor(TRUE);

	::DeleteObject(hHowTo);
	::DeleteObject(hButton);
	::DeleteObject(hCursor);
	::DeleteObject(hStage);
	::DeleteObject(hStart);

}
void MainGame::init(void)
{
	if (!hOwner) return;

	Rect rc;
	::GetClientRect(hOwner, &rc);

	hMainDC = ::GetDC(hOwner);
	hMemDC = ::CreateCompatibleDC(hMainDC);
	hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rc.width(), rc.height());
	hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

	int tx, ty;
	tx = ty = 0;
	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 15; x++)
		{
			Tile[y][x].left = 5 + tx;
			Tile[y][x].right = 47 + tx;
			Tile[y][x].top = 10 + ty;
			Tile[y][x].bottom = 53 + ty;

			tx += 42;
		}
		tx = 0;
		ty += 43;
	}


	FMOD_System_Create(&f_System); // FMOD 생성 
	FMOD_System_Init(f_System, 32, FMOD_INIT_NORMAL, NULL); // 기본값으로 초기화
	FMOD_System_CreateSound(f_System, "login.mp3", FMOD_LOOP_NORMAL, 0, &sound[0]); // 사운드 생성
	FMOD_System_CreateSound(f_System, "game.wma", FMOD_LOOP_NORMAL, 0, &sound[1]);
	FMOD_System_CreateSound(f_System, "room.mp3", FMOD_LOOP_NORMAL, 0, &sound[2]);
	FMOD_System_CreateSound(f_System, "bt_on.mp3", FMOD_DEFAULT, 0, &sound[3]);
	FMOD_System_PlaySound(f_System, FMOD_CHANNEL_FREE, sound[0], 0, &channel[0]); // 해당 사운드를 재생
	FMOD_Channel_SetVolume(channel[0], 1.f);
	FMOD_Channel_SetVolume(channel[1], 1.f);
}
void MainGame::release(void)
{
	if (hMemDC)
		::SelectObject(hMemDC, hOldMemBitmap);

	if (hMemBitmap)
		::DeleteObject(hMemBitmap);

	if (hMemDC)
		::DeleteDC(hMemDC);

	if (hMainDC)
		::ReleaseDC(hOwner, hMainDC);

	for (int i = 0; i < 4; i++)
		FMOD_Sound_Release(sound[i]); // FMOD sound 객체 해제
	FMOD_System_Close(f_System); // FMOD system 객체 close
	FMOD_System_Release(f_System); // FMOD system 객체 해제
}
