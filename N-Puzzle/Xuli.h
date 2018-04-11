
#ifndef XULI_H
#define XULI_H
#include "Matrix.h"
#include "List.h"
#include "graphic.h"
#include <fstream>
#include <string>
#include <conio.h>
#include <Windows.h>

bool CheckState(vector <MATRIX>, MATRIX);

Node* PickFirst(vector<vector<List>> &Open);

Node* CopyFirst(vector<vector<List>> Open);

int BinarySearch(vector <MATRIX> Close, MATRIX n);

Node* Astar(Node *);

void AstarPath(Node* Goal, vector <char> &Path);

void Drawing(Node*, int isGoal);

void DrawingFrame(int Rank);

void DrawingChange(Node* node, int parent_BlankPos, int Color);

void DrawingPauseTable(int FrameColor, int Color);

int Choose();

Node* Move(Node*, int&);

void Intro();

int ChooseGame(int);

void Suggest(Node *current);

int Solve(Node *&current);

int CheckPlay();

void Loop();

#endif 

