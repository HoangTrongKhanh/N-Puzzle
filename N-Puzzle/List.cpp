#include "List.h"

Node::Node(const Node &n)
{
	Heuristic = n.Heuristic;
	Cost = n.Cost;
	F = n.F;
	pParent = n.pParent;
	cInfo = n.cInfo;
	State = n.State;
}

void Node::Init(int R)
{
	State.Creat(R);
	State.Shuffle();
	Heuristic = State.Heuristic();
	Cost = 0;
	cInfo = NULL;
	F = Heuristic + Cost;
	pParent = NULL;
}

bool Node::operator!=(Node* node)
{
	if (State != node->GetState())
		return true;
	return false;
}

Node* Node::nMoveUp()
{
	Node *p;
	p = new Node;
	*p = *this;
	p->SetState(p->State.MoveUp());
	if (p->State.GetRank()) {
		p->SetcInfo(30);
		p->Cost = Cost + 1;
		p->Heuristic = p->State.Heuristic();
		p->F = p->Cost + p->Heuristic;
		p->SetParent(this);
	}
	else p = NULL;
	return p;
}

Node* Node::nMoveDown()
{
	Node *p;
	p = new Node;
	*p = *this;
	p->SetState(p->State.MoveDown());
	if (p->State.GetRank()) {
		p->SetcInfo(31);
		p->Cost = Cost + 1;
		p->Heuristic = p->State.Heuristic();
		p->F = p->Cost + p->Heuristic;
		p->SetParent(this);
	}
	else p = NULL;
	return p;
}

Node* Node::nMoveLeft()
{
	Node *p;
	p = new Node;
	*p = *this;
	p->SetState(p->State.MoveLeft());
	if (p->State.GetRank()) {
		p->SetcInfo(17);
		p->Cost = Cost + 1;
		p->Heuristic = p->State.Heuristic();
		p->F = p->Cost + p->Heuristic;
		p->SetParent(this);
	}
	else p = NULL;
	return p;
}

Node* Node::nMoveRight()
{
	Node *p;
	p = new Node;
	*p = *this;
	p->SetState(p->State.MoveRight());
	if (p->State.GetRank()) {
		p->SetcInfo(16);
		p->Cost = Cost + 1;
		p->Heuristic = p->State.Heuristic();
		p->F = p->Cost + p->Heuristic;
		p->SetParent(this);
	}
	else p = NULL;
	return p;
}



List::List()
{
	pHead = pTail = NULL;
}

//Hàm AddTail
void List::Add(Node *t)
{
	Node *node;
	node = new Node;
	*node = *t;


	if (!pHead) {
		pHead = pTail = node;
	}
	else {
		pTail->pNext = node;
		pTail = node;
	}
}

//Xóa tất cả các phần tử trong danh sách
void List::Clear()
{
	Node *p;
	p = pHead;
	while (p) {
		pHead = p->pNext;
		delete p;
		p = pHead;
	}
	pHead = pTail = NULL;
}

//Xóa nút node trong danh sách
void List::Remove(Node *node)
{
	Node *p, *q;
	p = q = pHead;
	while (p && p != node) {
		q = p;
		p = p->pNext;
	}
	if (p && p != pHead) {
		q->pNext = p->pNext;
	}
	delete p;
}

//Thay nodeDel bằng nodeReplace trong danh sách
void List::Replace(Node *nodeDel, Node *nodeReplace)
{
	Node *p;
	p = pHead;
	if (p == nodeDel) {
		nodeReplace->pNext = p->pNext;
		pHead = nodeReplace;
		delete p;
	}
	else {
		Node *q;
		q = pHead;
		while (p && p != nodeDel) {
			q = p;
			p = p->pNext;
		}
		q->pNext = nodeReplace;
		nodeReplace->pNext = p->pNext;
		delete p;
	}
}

//Hàm kiểm  tra Node node có nằm trong danh sách hay k
// trả về null nếu k thành công
Node* List::CheckExist(Node *node)
{
	Node *p;
	p = pHead;
	while (p && p != node) {
		p = p->pNext;
	}
	return p;
}

//Trích phần tử đầu tiên ra khỏi danh sách
Node* List::PickHead()
{
	Node *p;
	p = NULL;
	if (pHead) {
		p = pHead;
		if (pHead == pTail)
			pHead = pTail = NULL;
		else
			pHead = p->pNext;
		p->pNext = NULL;
	}
	return p;
}

List::~List()
{
	pHead = pTail = NULL;
	delete pHead;
	delete pTail;
}

