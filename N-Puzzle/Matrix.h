#pragma once
#include <iostream>
#include <vector>
using namespace std;

class MATRIX
{
public:
	vector <int> vMatrix; //vector chứa các giá trị của ma trận
private:
	int BlankPos; // vị trí ô trống;
	int Rank; //Cấp ma trận
	int Length; //Chiều dài của ma trận
public:
	MATRIX();
	void Creat(int);
	void Shuffle();
	int Heuristic();
	int GetRank() { return Rank; }
	int GetLength() { return Length; }
	int GetBlankPos() { return BlankPos; }
	void SetBlankPos(int t) { BlankPos = t; }
	bool operator!=(const MATRIX&);
	MATRIX MoveUp();
	MATRIX MoveDown();
	MATRIX MoveLeft();
	MATRIX MoveRight();
	~MATRIX();
};