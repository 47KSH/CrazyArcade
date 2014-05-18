#pragma once

#include <windows.h>

struct Point : public POINT
{
	Point(const LONG& _x = 0, const LONG& _y = 0);
	Point ToScreen(HWND);
	Point ToClient(HWND);
};

// wrapper struct
struct Size : public SIZE
{
	Size(const LONG& _cx = 0, const LONG& _cy = 0);
	friend Size operator * (const LONG& v, const Size& obj);
	friend Size operator * (const Size& obj, const LONG& v);
};

struct Rect : public RECT
{
	Rect(const LONG& l = 0, const LONG& t = 0, const LONG& r = 0, const LONG& b = 0);
	Rect(const Point& lt, const Point& rb);
	Point lefttop() const;
	Point rightbottom() const;
	LONG width() const;
	LONG height() const;
	Rect ToScreen(HWND);
	Rect ToClient(HWND);

	Rect Offset(const Size& diff) const;
	Rect operator << (const Size& diff) const;
	Rect operator >> (const Size& diff) const;
};
