#pragma once

#include <windows.h>
#include <tchar.h>
#include <fmod.h>
#include "BaseType.h"
#include "InputDevice.h"

enum{
	Login,
	HowTo,
	Stage,
};
enum
{
	idle,
	left,
	right,
	up,
	down,
};

class MainGame
{
public :
	MainGame();
	virtual ~MainGame();

	void Attach(HWND);
	void Input(DWORD tick);
	void Update(DWORD tick, HWND hWnd);
	void Draw(DWORD tick);
	void Load();

private :
	void remove();
	void init(void);
	void release(void);

private :
	HWND hOwner;
	HDC hMainDC;
	HDC hMemDC;
	HBITMAP hMemBitmap;
	HBITMAP hOldMemBitmap;

private :
	HBITMAP hCursor;
	HBITMAP hStart;
	HBITMAP hStage;
	HBITMAP hButton;
	HBITMAP hHowTo;
	HBITMAP hBazzi;
	int MouseState;
	Point ptMouse;
	Point Ccenter;
	bool button_01;
	bool button_02;
	bool button_snd;
	int MainState;
	int CMove;
	int C_speed;
	int Animation;
	bool stop;
	bool button_03;
	bool door;
	Size sBitmap;
	Size sGray;
	Rect Tile[13][15];
	DWORD update_delay;
	DWORD update_dt_any;
	DWORD update_dt_mov;
	DWORD draw_dt;
	DWORD input_dt;

private:
	FMOD_SYSTEM* f_System; // FMOD 사운드 시스템 변수 선언
	FMOD_SOUND* sound[10]; // 사운드 출력하기 위한 객체 생성
	FMOD_CHANNEL* channel[5]; // 사운드를 출력할 채널 생성
};