#pragma once
#include "Matrix.h"

class Node
{
private:
	int Heuristic; // chi phí từ hiện tại đến đích
	int Cost; // chi phí từ trạng thái bắt đầu đến hiện tại
	int F; // F = Heuristic + Cost;
	MATRIX State;
	Node *pParent;
	Node *pNext;
	char cInfo;
	friend class List;
public:
	Node() { Heuristic = Cost = F = 0; cInfo = NULL; pParent = NULL; pNext = NULL; }
	Node(const Node &n);
	int GetHeuristic() { return Heuristic; }
	int GetCost() { return Cost; }
	int GetF() { F = Cost + Heuristic; return F; }
	char GetcInfo() { return cInfo; }
	MATRIX GetState() { return State; }
	void SetHeuristic(int x) { Heuristic = x; }
	void SetCost(int x) { Cost = x; }
	void SetState(MATRIX m) { State = m; }
	void SetParent(Node *p) { pParent = p; }
	void SetcInfo(char c) { cInfo = c; }
	void Init(int);
	bool operator!=(Node*);
	Node* GetParent() { return pParent; }
	Node* nMoveUp();
	Node* nMoveDown();
	Node* nMoveLeft();
	Node* nMoveRight();

	~Node() { pParent = NULL; delete pParent; pNext = NULL; delete pNext; }
};


class List
{
private:
	Node *pHead, *pTail;
public:
	List();
	Node *GetpHead() { return pHead; }
	Node *GetpTail() { return pTail; }
	void Add(Node*);
	void Clear();
	void Remove(Node*);
	void Replace(Node*, Node*);
	Node* CheckExist(Node *);
	Node* PickHead();
	~List();
};