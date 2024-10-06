#pragma once
#include <Windows.h>
#include <iostream>

// Functions to print formatted information

namespace console {
    // Change color
    void setConsoleColor(int textColor, int backgroundColor) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        int colorAttribute = textColor + (backgroundColor << 4);
        SetConsoleTextAttribute(hConsole, colorAttribute);
    }

    // Change font size
    void setSize(int size) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        GetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
        cfi.dwFontSize.Y = size;
        SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
    }

    // Print text with address
    void print(const char* text, void* address) {
        WORD lightPurple = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        WORD red = FOREGROUND_RED | FOREGROUND_INTENSITY;
        WORD green = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        WORD defaultColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        setConsoleColor(lightPurple, 0);
        printf("[ ");

        setConsoleColor(address == 0 ? red : green, 0);
        printf("%s", text);

        setConsoleColor(lightPurple, 0);
        printf(" ]");

        setConsoleColor(red, 0);
        printf(" : ");

        if (address == 0) {
            setConsoleColor(red, 0);
            printf("0\n");
        }
        else {

            setConsoleColor(defaultColor, 0);
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "%p", address);
            char* p = buffer;
            while (*p == '0' || *p == 'x') {
                p++;
            }
            printf("0x%s\n", p);
        }

        setConsoleColor(defaultColor, 0);
    }


    // print text
    void print(const char* text) {
        WORD lightPurple = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        WORD defaultColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        setConsoleColor(lightPurple, 0);
        printf("[ ");

        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY, 0);
        printf("%s", text);

        setConsoleColor(lightPurple, 0);
        printf(" ]\n");
        setConsoleColor(defaultColor, 0);
    }

    // print header
    void printHeader(const char* text) {
        WORD lightPurple = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        WORD defaultColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        setConsoleColor(lightPurple, 0);
        printf("[ ");

        setConsoleColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN, 0);
        printf("INFO");

        setConsoleColor(lightPurple, 0);
        printf(" ] : ");

        setConsoleColor(FOREGROUND_INTENSITY | defaultColor, 0);
        printf("%s\n", text);

        setConsoleColor(defaultColor, 0);
    }

    // print error
	void printError(const char* text) {
        WORD lightPurple = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        WORD defaultColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        setConsoleColor(lightPurple, 0);
        printf("[ ");

        setConsoleColor(FOREGROUND_INTENSITY | FOREGROUND_RED, 0);
        printf("ERROR");

        setConsoleColor(lightPurple, 0);
        printf(" ] : ");

		setConsoleColor(FOREGROUND_INTENSITY | defaultColor, 0);
		printf("%s\n", text);
        setConsoleColor(defaultColor, 0);
    }

    // Create console
    void create() {
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        setSize(23);
    }

    // Destroy console
    void destroy() {
        HWND consoleWindow = GetConsoleWindow();
        FreeConsole();
        PostMessage(consoleWindow, WM_CLOSE, 0, 0);
        PostMessage(consoleWindow, WM_DESTROY, 0, 0);
    }

}