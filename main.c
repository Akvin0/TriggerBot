#include <stdio.h>
#include <Windows.h>

int IsInColorRange(COLORREF color1, COLORREF color2, int range);
int PerformLeftClick();

int main()
{
	int centerOffset = 1;
	int clickDelay = 400;
	int colorRange = 5;

	HDC hdc = GetDC(NULL);

	int x = GetSystemMetrics(SM_CXSCREEN) / 2 + centerOffset, y = GetSystemMetrics(SM_CYSCREEN) / 2 + centerOffset;

	COLORREF initColor = NULL;
	DWORD last = 0;
	int flag = 0;

	while (1)
	{
		if (GetAsyncKeyState(VK_INSERT))
		{
			break;
		}

		while (GetAsyncKeyState(VK_XBUTTON2))
		{
			if (GetTickCount64() - last < clickDelay)
			{
				continue;
			}

			if (!flag)
			{
				initColor = GetPixel(hdc, x, y);
				flag = 1;
			}

			COLORREF currentColor = GetPixel(hdc, x, y);
			if (!IsInColorRange(initColor, currentColor, colorRange))
			{
				if (!PerformLeftClick())
				{
					perror("SendInput");
				}

				last = GetTickCount64();

				printf("%d %d %d <<>> %d %d %d\n", GetRValue(initColor), GetGValue(initColor), GetBValue(initColor), GetRValue(currentColor), GetGValue(currentColor), GetBValue(currentColor));
			}
		}

		flag = 0;
	}

	ReleaseDC(NULL, hdc);

	return 0;
}

int IsInColorRange(COLORREF color1, COLORREF color2, int range)
{
	return abs(GetRValue(color1) - GetRValue(color2)) <= range && abs(GetGValue(color1) - GetGValue(color2)) <= range && abs(GetBValue(color1) - GetBValue(color2)) <= range;
}

int PerformLeftClick()
{
	INPUT input[2] = { 0 };

	input[0].type = INPUT_MOUSE;
	input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	input[1].type = INPUT_MOUSE;
	input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	return SendInput(2, input, sizeof(INPUT));
}
