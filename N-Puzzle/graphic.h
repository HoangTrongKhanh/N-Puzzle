#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <math.h>
#include <windows.h>
#include <Windows.h>
using namespace std;

#define M_PI 3.14159265358979323846

void initgraph(void);

void setcolorRGB(int r, int g, int b);

void gotoxy(int x, int y);

void textcolor(int x);

#endif
