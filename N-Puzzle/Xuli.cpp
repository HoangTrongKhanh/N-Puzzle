#include "Xuli.h"

//Kiểm tra State đã duyệt chưa. Nếu chưa trả về 1;
bool CheckState(vector <MATRIX> v, MATRIX State)
{
	int i = 0;
	while (i < v.size() && v[i] != State)
		i++;
	if (i == v.size())
		return true;
	return false;
}

//Trích phần tử có chi phí nhỏ nhất từ Open
Node* PickFirst(vector<vector<List>> &Open)
{
	for (int j = 0; j < 12; j++)
		for (int i = 0; i < Open.size(); i++) {
			Node *p;
			p = Open[i][j].PickHead();
			if (p)
				return p;
		}
	return NULL;
}

//Lấy giá trị nút có chi phí nhỏ nhất từ Open
Node* CopyFirst(vector<vector<List>> Open)
{
	for (int j = 0; j < 12; j++)
		for (int i = 0; i < Open.size(); i++) {
			Node *p;
			p = Open[i][j].GetpHead();
			if (p)
				return p;
		}
	return NULL;
}

//Tìm kiếm nhị phân trên vector
int BinarySearch(vector <MATRIX> Close, MATRIX n)
{
	if (Close.empty()) return -1;
	int Left, Right, Mid;
	Left = 0;
	Right = Close.size() - 1;
	Mid = (Left + Right) / 2;
	while (Left <= Right) {
		Mid = (Left + Right) / 2;
		if (Close[Mid].Heuristic() < n.Heuristic())
			Left = Mid + 1;
		else if (Close[Mid].Heuristic() > n.Heuristic())
			Right = Mid - 1;
		else {
			Left = Mid + 1;
			Mid = (Left + Right) / 2;
		}
	}
	return Mid;
}

//A* dùng để tìm đường đi ngắn nhất
Node* Astar(Node *nodeInput)
{
	vector <vector <List>> vectorOpen; 
	vector<vector <vector <MATRIX>>>Close;
	Node *Temp, *Head, *Check;
	int F, H, InsertPos;
	//vectorOpen và Close và các vector lưu giữ trạng thái khi chơi
	//vectorOpen là một tập hợp 2 chiều các List
	//với chỉ số của chiều đầu tiên ứng với phần dư của giá tri hàm F (F = Cost + Heuristic)
	//với mỗi chiều đó là một List có chỉ số là phần nguyên của giá trị hàm F

	//tương tự như vectorOpen, Close là một vector MATRIX 3 chiều
	//Chiều đầu tiên của Close có chỉ số là phần dư của giá trị hàm H (hàm Heuristic)
	//với mỗi chiều của chiều đầu tiên là một vector có chỉ số là phần nguyên của giá trị hàm H

	//việc tổ chức dưới dạng các vector nhiều chiều
	//nên chúng ta chỉ tìm trong 1 chiều nhỏ, giới hạn vùng nên việc tìm kiếm nhanh hơn

	//Node Head là node có giá trị F thấp nhất trong vectorOpen
	//Node Temp là các node phát triển của Head
	//Node Check là kết quả trả về của hàm tìm kiếm Temp trong vectorOpen
	//Nếu Check khác NULL thì sẽ kiểm tra và thay thế nếu Check có chi phí cao hơn Temp

	//Khai báo kích thước của các vector
	Close.resize(10);
	vectorOpen.resize(10);
	for (int i = 0; i < Close.size(); i++) {
		Close[i].resize(12);
		vectorOpen[i].resize(12);
	}


	F = nodeInput->GetF();
	//Add nodeInput vào trong List có chỉ số là [F % 10][F / 10]
	vectorOpen[F % 10][F / 10].Add(nodeInput);
	Head = nodeInput;


	while (CopyFirst(vectorOpen)) {
		if (!_kbhit()) { // nếu không có bất kì phím nào được ấn
			//thì thực hiện phần lệnh bên trong
		// lệnh này nhằm giúp cho ng dùng có thể chủ động thoát 
			// khi cảm thấy chương trình giải quá lâu mà không có đáp án
			if (CopyFirst(vectorOpen)->GetHeuristic() == 0) {
				//nếu Node có F nhỏ nhất trong vectorOpen có Heuristic
				//là 0, tức đó là nút Goal thì dừng ct và trả về chính nút đó
				return CopyFirst(vectorOpen);
			}
			else {
				Head = PickFirst(vectorOpen); // lấy ra nút có giá trị F nhỏ nhất
				//trong vectorOpen và gán cho Head
				F = Head->GetF();

				Temp = new Node;
				*Temp = *Head; //Lấy giá trị vùng nhớ của Head gán cho Temp

				H = Temp->GetHeuristic();

				//Tìm vị trí thích hợp trong cho nút Head trong tập Close
				InsertPos = BinarySearch(Close[H % 10][H / 10], Temp->GetState());
			
				if (InsertPos == -1) // nếu vector Close 
					//có chỉ số của chiều Close[H % 10][H / 10] rỗng thì add vào đó
					Close[H % 10][H / 10].push_back(Temp->GetState());
				else
					//Add vào vị trí thích hợp trong Close
					//có chỉ số của chiều  Close[H % 10][H / 10]
					Close[H % 10][H / 10].insert
					(Close[H % 10][H / 10].begin() + InsertPos + 1, Temp->GetState());

				for (int i = 0; i < 4; i++) {
					//Phát triển các trạng thái có thể có của Head
					//Temp là null không thể Move được
					Temp = new Node;
					*Temp = *Head;
					switch (i) {
					case 0: Temp = Temp->nMoveUp(); break;
					case 1: Temp = Temp->nMoveDown(); break;
					case 2: Temp = Temp->nMoveLeft(); break;
					case 3: Temp = Temp->nMoveRight();
					}
					
					if (Temp) { // nếu Temp != NULL
						F = Temp->GetF();

						//Kiểm tra Temp có tồn tại trong vectorOpen
						Check = vectorOpen[F % 10][F / 10].CheckExist(Temp);
						H = Temp->GetHeuristic();

						//nếu Check là NULL và Temp không có trong chiều Close[H % 10][H / 10]
						if (!Check && CheckState(Close[H % 10][H / 10], Temp->GetState())) {
							//Thêm vào đuôi vectorOpen có chiều vectorOpen[F % 10][F / 10]
							//vì trong chiều vectorOpen[F % 10][F / 10] thì các giá trị có cùng F
							vectorOpen[F % 10][F / 10].Add(Temp);
						}
						else {
							// nếu Check != NULL tức Temp đã có trong vectorOpen
							// kiểm tra nếu Temp có chi phí nhỏ hơn bằng 
							// chi phi của Check thì thay thế chỗ của Check là Temp
							if (Check && Temp->GetF() <= Check->GetF())
								vectorOpen[F % 10][F / 10].Replace(Check, Temp);
						}
					}
				}
			}
		}
		else break; // nếu có bất kì phím nào được nhấn
		// thoát khỏi chưng trình
	}
	return NULL;
}

//Hàm dựng lại đường đi từ nút hiện tại về Goal
void AstarPath(Node* Goal, vector <char> &Path)
{
	int i = 0;
	while (Goal->GetcInfo()) {
		Path.push_back(Goal->GetcInfo());
		Goal = Goal->GetParent();
	}
	reverse(Path.begin(), Path.end());
}

//Hàm vẽ các ô số được luu trong node
void Drawing(Node* node, int isGoal)
{
	MATRIX matrix;
	matrix = node->GetState();
	if (!isGoal)
		textcolor(6);
	else
		textcolor(2);
	int xx, yy;
	if (matrix.GetRank() == 5) {
		if (!isGoal)
			xx = 130;
		else xx = 470;
		yy = 30;
	}
	else if (matrix.GetRank() == 4) {
		if (!isGoal)
			xx = 150;
		else xx = 490;
		yy = 50;
	}
	else {
		if (!isGoal)
			xx = 180;
		else xx = 520;
		yy = 70;
	}

	//Vẽ khung
	for (int k = 0; k < 2 * matrix.GetRank() + 1; k++) {
		gotoxy(xx, yy);
		if (k == 0) {
			cout << (char)201;
			for (int i = 0; i < matrix.GetRank(); i++) {
				for (int j = 0; j < 4; j++)
					cout << (char)205;
				if (i != matrix.GetRank() - 1)
					cout << (char)203;
				else {
					cout << (char)187;
					yy += 10;
				}
			}
		}
		else if (k == 2 * matrix.GetRank()) {
			cout << (char)200;
			for (int i = 0; i < matrix.GetRank(); i++) {
				for (int j = 0; j < 4; j++)
					cout << (char)205;
				if (i != matrix.GetRank() - 1)
					cout << (char)202;
				else  {
					cout << (char)188;
					yy += 10;
				}
			}
		}
		else if (k % 2 == 0) {
			cout << (char)204;
			for (int i = 0; i < matrix.GetRank(); i++) {
				for (int j = 0; j < 4; j++)
					cout << (char)205;
				if (i != matrix.GetRank() - 1)
					cout << (char)206;
				else  {
					cout << (char)185;
					yy += 10;
				}
			}
		}
		else {
			for (int n = 1; n <= 2; n++) {
				gotoxy(xx, yy);
				cout << (char)186;
				for (int i = 0; i < matrix.GetRank(); i++) {
					for (int j = 0; j < 4; j++)
						cout << (char)32;
					cout << (char)186;
				}
				yy += 10;
			}
		}
	}

	//Xuất số vào đúng vị trí
	int x, y;
	if (!isGoal)
		textcolor(10);
	else textcolor(6);
	for (int i = 0; i < matrix.vMatrix.size(); i++) {
		switch (i %  matrix.GetRank()) {
		case 0: x = 20 + xx; break;
		case 1: x = 70 + xx; break;
		case 2: x = 120 + xx; break;
		case 3: x = 170 + xx; break;
		case 4: x = 220 + xx;
		}
		switch (i / matrix.GetRank()) {
		case 0: y = 20 + yy; break;
		case 1: y = 50 + yy; break;
		case 2: y = 80 + yy; break;
		case 3: y = 110 + yy; break;
		case 4: y = 140 + yy;
		}
		if (matrix.GetRank() == 3)
			y -= 100;
		else if (matrix.GetRank() == 4)
			y -= 130;
		else y -= 160;
		gotoxy(x, y);
		if (matrix.vMatrix[i])
			cout << matrix.vMatrix[i];
		else cout << "  ";
	}
}

//Hàm vẽ khung ngoài và trạng thái đích
void DrawingFrame(int Rank)
{
	textcolor(10);
	cout << endl;
	cout << "       " << (char)201;
	for (int i = 0; i < 67; i++) {
		if (i != 35)
			cout << (char)205;
		else cout << (char)203;
	}
	cout << (char)187 << endl;

	for (int i = 0; i < 20; i++) {
		cout << "       " << (char)186;
		for (int j = 0; j < 67; j++)
			if (j != 35) 
				cout << (char)32;
			else cout << (char)186;
		cout << (char)186 << endl;
	}
	cout << "       " << (char)200;

	for (int i = 0; i < 67; i++)
		if (i != 35)
			cout << (char)205;
		else cout << (char)202;
	cout << (char)188 << endl;

	Node *Goal;
	Goal = new Node;
	Goal->Init(Rank);
	MATRIX State;
	State.Creat(Rank);
	Goal->SetState(State);
	Drawing(Goal, 1);
	if (Rank == 3) {
		gotoxy(550, 170);
	}
	else if (Rank == 4) {
		gotoxy(550, 180);
	}
	else
		gotoxy(550, 190);
	textcolor(2);
	cout << "Goal state";
	gotoxy(0, 230);
	cout << "Press Enter to Pause and other choose.";
}

//Vẽ lại các vị trí đã thay đổi trong khi di chuyển
void DrawingChange(Node* p, int parent_BlankPos, int Color)
{
	textcolor(Color);
	if (Color == 12)
		Sleep(500);
	int xx, yy;
	
	if (p->GetState().GetRank() == 5) {
		xx = 130;
		yy = 30;
	}
	else if (p->GetState().GetRank() == 4) {
		xx = 150;
		yy = 50;
	}
	else {
		xx = 180;
		yy = 70;
	}

	for (int i = 0; i < 2; i++) {
		int Temp, x, y;
		Temp = (!i) ? p->GetState().GetBlankPos() : parent_BlankPos;
		switch (Temp % p->GetState().GetRank()) {
		case 0: x = 20 + xx; break;
		case 1: x = 70 + xx; break;
		case 2: x = 120 + xx; break;
		case 3: x = 170 + xx; break;
		case 4: x = 220 + xx;
		}
		switch (Temp / p->GetState().GetRank()) {
		case 0: y = 20 + yy; break;
		case 1: y = 50 + yy; break;
		case 2: y = 80 + yy; break;
		case 3: y = 110 + yy; break;
		case 4: y = 140 + yy;
		}
		gotoxy(x, y);
		if (i)
			cout << p->GetState().vMatrix[parent_BlankPos];
		else cout << "  ";
	}
	if (Color == 12) {
		Sleep(500);
		for (int i = 0; i < 2; i++) {
			int Temp, x, y;
			Temp = (i) ? p->GetState().GetBlankPos() : parent_BlankPos;
			switch (Temp % p->GetState().GetRank()) {
			case 0: x = 20 + xx; break;
			case 1: x = 70 + xx; break;
			case 2: x = 120 + xx; break;
			case 3: x = 170 + xx; break;
			case 4: x = 220 + xx;
			}
			switch (Temp / p->GetState().GetRank()) {
			case 0: y = 20 + yy; break;
			case 1: y = 50 + yy; break;
			case 2: y = 80 + yy; break;
			case 3: y = 110 + yy; break;
			case 4: y = 140 + yy;
			}
			gotoxy(x, y);
			textcolor(10);
			if (i)
				cout << p->GetState().vMatrix[parent_BlankPos];
			else cout << "  ";
		}
	}
}

//Vẽ khung dừng khi nhấn phím Enter
void DrawingPauseTable(int FrameColor, int CharColor)
{
	gotoxy(120, 80);
	textcolor(FrameColor);
	cout << (char)201;
	for (int i = 0; i < 20; i++) {
		if (i != 9)
			cout << (char)205;
		else {
			if (FrameColor != 0) textcolor(79);
			else textcolor(0);
			cout << "Pause";
			textcolor(FrameColor);
		}
	}
	cout << (char)187;
	gotoxy(120, 90);
	for (int i = 0; i < 6; i++) {
		gotoxy(120, 90 + i * 10);
		cout << (char)186;
		for (int j = 0; j < 24; j++)
			cout << (char)32;
		cout << (char)186;
	}
	gotoxy(120, 150);
	cout << (char)200;
	for (int i = 0; i < 24; i++)
		cout << (char)205;
	cout << (char)188;

	gotoxy(210, 100);
	textcolor(CharColor);
	cout << "Continue";
	gotoxy(210, 110);
	cout << "Suggest";
	gotoxy(220, 120);
	cout << "Solve";
	gotoxy(220, 130);
	cout << "Menu";

}

//Hàm chọn trong bảng Pause
int Choose()
{
	int k = 0, y, x;
	y = 100;
	gotoxy(200, 100);
	textcolor(10);
	cout << (char)42;
	cout << "Continue";

	while (!k) {
		if (_kbhit()) {
			char c = _getch();
			if (c == -32) {
				c = _getch();
				if (c == 72) {
					switch (y) {
					case 100: break;
					case 110: x = 200; gotoxy(x, y); textcolor(15); cout << " Suggest"; y -= 10; textcolor(10);
						gotoxy(x, y); cout << (char)42 << "Continue"; break;
					case 120:  x = 210; gotoxy(x, y); textcolor(15); cout << " Solve"; y -= 10; textcolor(10); x = 200;
						gotoxy(x, y); cout << (char)42 << "Suggest"; break;
					case 130: x = 210; gotoxy(x, y); textcolor(15); cout << " Menu"; y -= 10; textcolor(10);
						gotoxy(x, y); cout << (char)42 << "Solve"; break;
					}
				}
				else if (c == 80)
						switch (y) {
						case 100: x = 200; gotoxy(x, y); textcolor(15); cout << " Continue"; y += 10; textcolor(10);
							gotoxy(x, y); cout << (char)42 << "Suggest"; break;
						case 110: x = 200; gotoxy(x, y); textcolor(15); cout << " Suggest"; y += 10; textcolor(10); x = 210;
							gotoxy(x, y); cout << (char)42 << "Solve"; break;
						case 120:  x = 210; gotoxy(x, y); textcolor(15); cout << " Solve"; y += 10; textcolor(10);
							gotoxy(x, y); cout << (char)42 << "Menu"; break;
						case 130: break;
						}
			}
			else if (c == 13) {
				switch (y) {
				case 100: k = 1; break;
				case 110: k = 2; break;
				case 120: k = 3; break;
				case 130: k = 4;
				}
				return k;
			}
		}
	}
	return k;
}

//Di chuyển node và vẽ lên màn hình
Node* Move(Node *node, int &Check)
{
	Node *p;
	p = new Node;
	*p = *node;
	int k = 0;
	char c;
	c = _getch();
	if (c == -32) {
		c = _getch();
		if (c == 72) {
			p = p->nMoveUp();
			k = node->GetState().GetBlankPos();
		}
		else if (c == 80) {
			k = node->GetState().GetBlankPos();
			p = p->nMoveDown();
		}
		else if (c == 75) {
			p = p->nMoveLeft();
			k = node->GetState().GetBlankPos();
		}
		else if (c == 77) {
			p = p->nMoveRight();
			k = node->GetState().GetBlankPos();
		}
		if (p && p != node) {
			DrawingChange(p, k, 10);
		}
		else if (!p) {
			p = new Node;
			*p = *node;
		}
	}
	else if (c == 13) {
		Check = 1;
	}
	return p;
}

//Vẽ chữ N-Puzzle
void Intro()
{
	initgraph();
	textcolor(10);

	gotoxy(0, 30);
	cout << "  " << (char)201;
	for (int i = 0; i < 74; i++) {
		cout << (char)205;
	}
	cout << (char)187 << endl;
	for (int i = 0; i < 15; i++) {
		cout << "  " << (char)186;
		for (int j = 0; j < 74; j++)
			cout << (char)32;
		cout << (char)186 << endl;
	}
	cout << "  " << (char)200;
	for (int i = 0; i < 74; i++)
		cout << (char)205;
	cout << (char)188 << endl;

	textcolor(6);
	fstream f;
	f.open("n-puzzle.txt", ios::in);
	string s;
	int i = 0;
	while (!f.eof()) {
		gotoxy(30, 50 + i * 10);
		i++;
		getline(f, s);
		cout << s;
	}
}

//Bảng chọn Play
int ChooseGame(int Index)
{
	int x;
	x = 150;
	if (Index) {
		textcolor(46);
		gotoxy(300, 120);
		cout << "Choose level" << endl;

		gotoxy(150, 150);
		textcolor(15);
		cout << "3x3  ";
		gotoxy(370, 150);
		cout << "4x4   ";
		gotoxy(590, 150);
		cout << "5x5   ";
		gotoxy(0, 250);
	}
	else {
		gotoxy(150, 150);
		textcolor(15);
		cout << "Play";
		gotoxy(370, 150);
		cout << "Guide";
		gotoxy(590, 150);
		cout << "Exit";
		gotoxy(0, 250);
	}

	while (!_kbhit()) {
		gotoxy(x, 150);
		textcolor(0);
		if (Index)
			cout << "3x3";
		else cout << "Play";

		Sleep(250);
		gotoxy(x, 150);
		textcolor(15);
		if (Index)
			cout << "3x3";
		else cout << "Play";
		Sleep(250);
	}

	while (true) {
		if (_kbhit()) {
			char c;
			c = _getch();
			if (c == -32) {
				c = _getch();
				if (c == 75) {
					switch (x) {
					case 150: break;
					case 370: x -= 220; break;
					case 590: x -= 220;
					}
				}
				else if (c == 77) {
					switch (x) {
					case 150: x += 220; break;
					case 370: x += 220; break;
					case 590: break;
					}
				}
			}
			else if (c == 13) {
				switch (x) {
				case 150: return 3;
				case 370: return 4;
				case 590: return 5;
				}
			}
		}
		while (!_kbhit()) {
			gotoxy(x, 150);
			if (x == 150) {
				textcolor(0);
				if (Index)
					cout << "3x3";
				else cout << "Play";
				Sleep(250);
				gotoxy(x, 150);
				textcolor(15);
				if (Index)
					cout << "3x3";
				else cout << "Play";
				Sleep(250);
			}
			else if (x == 370) {
				textcolor(0);
				if (Index)
					cout << "4x4";
				else cout << "Guide";
				Sleep(250);
				gotoxy(x, 150);
				textcolor(15);
				if (Index)
					cout << "4x4";
				else cout << "Guide";
				Sleep(250);
			}
			else {
				textcolor(0);
				if (Index)
					cout << "5x5";
				else cout << "Exit";
				Sleep(250);
				gotoxy(x, 150);
				textcolor(15);
				if (Index)
					cout << "5x5";
				else cout << "Exit";
				Sleep(250);
			}
		}
	}
}

//Gợi ý di chuyển
void Suggest(Node *current)
{
	Node *Temp, *Nodemin;
	Nodemin = NULL;
	for (int i = 0; i < 4; i++) {
		Temp = new Node;
		*Temp = *current;
		switch (i) {
		case 0: Temp = Temp->nMoveUp(); break;
		case 1: Temp = Temp->nMoveDown(); break;
		case 2: Temp = Temp->nMoveLeft(); break;
		case 3: Temp = Temp->nMoveRight();
		}
		if (Temp && !Nodemin) {
			Nodemin = new Node;
			*Nodemin = *Temp;
		}
		else if (Temp && Temp->GetHeuristic() <= Nodemin->GetHeuristic())
			*Nodemin = *Temp;
	}
	DrawingChange(Nodemin, current->GetState().GetBlankPos(), 12);
}

//Giải nghiệm
int Solve(Node *&current)
{
	gotoxy(100, 80);
	textcolor(12);
	cout << (char)201;
	for (int i = 0; i < 30; i++) {
		if (i != 9)
			cout << (char)205;
	}
	cout << (char)187;
	gotoxy(100, 90);
	for (int i = 0; i < 6; i++) {
		gotoxy(100, 90 + i * 10);
		cout << (char)186;
		for (int j = 0; j < 29; j++)
			cout << (char)32;
		cout << (char)186;
	}
	gotoxy(100, 150);
	cout << (char)200;
	for (int i = 0; i < 29; i++)
		cout << (char)205;
	cout << (char)188;

	string s;
	textcolor(7);
	s = "System are finding their way    to solve this problem. If it's too long, press any   key to exit. Please wait . . .";
	int k = 0;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == 'S' || s[i] == 'I' || s[i] =='P' || i == 29 || i == 82) {
			gotoxy(110, 90 + k * 10);
			k++;
		}
		cout << s[i];
		Sleep(50);
	}

	Node *goal;
	goal = Astar(current);

	if (goal) {
		for (int i = 0; i < k; i++) {
			gotoxy(110, 90 + i * 10);
			cout << "                             ";
		}
		s = "This problem can be solved in";
		gotoxy(110, 90);
		for (int i = 0; i < s.length(); i++) {
			cout << s[i];
			Sleep(50);
		}
		gotoxy(110, 100);
		cout << " " << goal->GetCost() - current->GetCost() - 1;
		s = " steps. Choose OK to solve this automatically.";
		for (int i = 0; i < s.length(); i++) {
			if (s[i] == 'O')
				textcolor(46);
			else if (i == 17)
				textcolor(7);
			if (s[i] == 'C')
				gotoxy(110, 110);
			else if (i == 32)
				gotoxy(110, 120);
			cout << s[i];
			Sleep(50);
		}
		gotoxy(240, 140);
		textcolor(46);
		cout << "OK";
		char c;
		do {
			c = _getch();
		} while (c != 13);



		gotoxy(100, 80);
		textcolor(0);
		cout << (char)201;
		for (int i = 0; i < 30; i++) {
			if (i != 9)
				cout << (char)205;
		}
		cout << (char)187;
		gotoxy(100, 90);
		for (int i = 0; i < 6; i++) {
			gotoxy(100, 90 + i * 10);
			cout << (char)186;
			for (int j = 0; j < 29; j++)
				cout << (char)32;
			cout << (char)186;
		}
		gotoxy(100, 150);
		cout << (char)200;
		for (int i = 0; i < 29; i++)
			cout << (char)205;
		cout << (char)188;

		for (int i = 0; i < k + 1; i++) {
			gotoxy(110, 90 + i * 10);
			cout << "                             ";
		}

		Drawing(current, 0);

		vector <char> Path;
		AstarPath(goal, Path);

		Node *p;
		p = new Node;
		*p = *current;
		textcolor(2);
		for (int i = current->GetCost(); i < Path.size(); i++) {
			switch (Path[i]) {
			case 30: p = p->nMoveUp(); break;
			case 31: p = p->nMoveDown(); break;
			case 16: p = p->nMoveRight(); break;
			case 17: p = p->nMoveLeft();
			}
			k = p->GetParent()->GetState().GetBlankPos();
			DrawingChange(p, k, 10);
			gotoxy(90, 20);
			cout << "Move: " << p->GetCost();
			Sleep(600);
		}
		*current = *goal;
	}
	else {
		gotoxy(100, 80);
		textcolor(0);
		cout << (char)201;
		for (int i = 0; i < 30; i++) {
			if (i != 9)
				cout << (char)205;
		}
		cout << (char)187;
		gotoxy(100, 90);
		for (int i = 0; i < 6; i++) {
			gotoxy(100, 90 + i * 10);
			cout << (char)186;
			for (int j = 0; j < 29; j++)
				cout << (char)32;
			cout << (char)186;
		}
		gotoxy(100, 150);
		cout << (char)200;
		for (int i = 0; i < 29; i++)
			cout << (char)205;
		cout << (char)188;
		Drawing(current, 0);
		return 0;
	}
	return 1;
}

//Hàm chọn có chơi tiếp hay k
int CheckPlay()
{
	gotoxy(200, 180);
	textcolor(15);
	cout << "Play again?";
	textcolor(10);
	gotoxy(170, 200);
	cout << "YES";
	textcolor(15);
	gotoxy(310, 200);
	cout << "NO";
	int x;
	x = 170;
	while (true) {
		if (_kbhit) {
			char c;
			c = _getch();
			if (c == -32) {
				c = _getch();
				if (c == 75) {
					x = 170;
					gotoxy(x, 200);
					textcolor(10);
					cout << "YES";
					gotoxy(310, 200);
					textcolor(15);
					cout << "NO";
				}
				else if (c == 77) {
					x = 310;
					gotoxy(x, 200);
					textcolor(10);
					cout << "NO";
					gotoxy(170, 200);
					textcolor(15);
					cout << "YES";
				}
			}
			else if (c == 13) {
				if (x == 310)
					return 0;
				else
					return 1;
			}
		}

	}
	return 0;
}

//Vòng lặp của game
void Loop()
{
	Node *m;
	
	Intro();
	x: int i = ChooseGame(0);
	if (i == 3) {
		i = ChooseGame(1);
		m = new Node;
		m->Init(i);

		initgraph();
		DrawingFrame(i);
		Drawing(m, 0);
		Node *node;
		node = new Node;
		*node = *m;
		gotoxy(90, 20);
		textcolor(2);
		cout << "Move: " << node->GetCost();

		int x = 0, k;
		i = 0;
		while (node->GetHeuristic()) {
			if (_kbhit) {
				node = Move(node, x);
				gotoxy(90, 20);
				textcolor(2);
				cout << "Move: " << node->GetCost();
				if (x) {
					DrawingPauseTable(12, 15);
					k = Choose();
					DrawingPauseTable(0, 0);
					Drawing(node, 0);

					if (k == 1) {}
					else if (k == 2) {
						Suggest(node);
					}
					else if (k == 3) {
						i = Solve(node);
					}
					else {
						initgraph();
						Intro();
							goto x;
					}
					x = 0;
				}
				if (i) {
					k = CheckPlay();
					if (k) {
						initgraph();
						Intro();
						goto x;
					}
				}
				if (!node->GetHeuristic()) {
					k = CheckPlay();
					if (k) {
						initgraph();
						Intro();
						goto x;
					}
				}
			}
		}

	}
	else if (i == 4) {
		textcolor(46);
		gotoxy(370, 120);
		cout << "Guide";
		textcolor(15);
		gotoxy(70, 140);
		cout << "Press cursor keys to slide the number into the empty space.";
		gotoxy(70, 150);
		cout << "Rearrange the tiles off until it is a goal state.";
		gotoxy(70, 170);
		cout << "Press Enter to back.";

		char c;
		do {
			c = _getch();
		} while (c != 13);
		textcolor(0);
		gotoxy(370, 120);
		cout << "Guide";
		gotoxy(70, 140);
		cout << "Press cursor keys to slide the number into the empty space.";
		gotoxy(70, 150);
		cout << "Rearrange the tiles off until it is a goal state.";
		gotoxy(70, 170);
		cout << "Press Enter to back.";

		goto x;
	}
	textcolor(0);
	gotoxy(0, 230);
}
