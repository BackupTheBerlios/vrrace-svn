#include "main.h"

void writeText(int posx, int posy, string givenText, int r, int g, int b)
{
	SetBkMode(hDc, TRANSPARENT);
	SetTextColor(hDc, RGB(r, g, b));
	TextOut(hDc, posx, posy*12, givenText.c_str(), givenText.length());
	ReleaseDC(hWnd, hDc);
}