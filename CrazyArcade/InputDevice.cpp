#include "InputDevice.h"

InputProcessor::InputProcessor()
{
	::memset(cKey, 0, sizeof(BYTE)*count);
}
InputProcessor::~InputProcessor()
{
}

void InputProcessor::Update(DWORD tick)
{
	::GetKeyboardState(cKey);
}
bool InputProcessor::IsPressed(const short& vkey)
{
	return ((cKey[vkey] & 0x80) == 0x80);
}
bool InputProcessor::operator [] (const short& vkey)
{
	return IsPressed(vkey);
}
Point InputProcessor::pos() const
{
	Point tmp;
	::GetCursorPos(&tmp);

	return tmp;
}

