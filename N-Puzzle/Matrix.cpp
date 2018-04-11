#include "Matrix.h"
#include <time.h>
using namespace std;


MATRIX::MATRIX()
{
	Length = Rank = 0;
	BlankPos = 0;
}

// Hàm tạo các giá trị ban đầu cho một mảng ma trận
void MATRIX::Creat(int R)
{
	Rank = R;
		
	Length = Rank * Rank;
		
	for (int i = 1; i < Length; i++)
		vMatrix.push_back(i);

	vMatrix.push_back(0);
	BlankPos = Length - 1;
}

//Hàm trộn thứ tự các giá trị của ma trận
void MATRIX::Shuffle()
{
	int j;
	MATRIX temp;
	srand(time(NULL));
	for (int i = 0; i < Rank*Rank*Rank; i++) {
		do {
			j = rand() % 4;
			temp = *this;
			switch (j) {
			case 0: temp = temp.MoveUp(); break;
			case 1: temp = temp.MoveDown(); break;
			case 2: temp = temp.MoveLeft(); break;
			default: temp = temp.MoveRight();
			}
		} while (temp.vMatrix.size() == 0);
		*this = temp;
	}
}

//Trả về chi phí thực hiện khi về được trạng thái cuối cùng
int MATRIX::Heuristic()
{
	// n: tổng bình khoảng cách Manhattan
	int n = 0;
	// a: chỉ số phạt cặp ô hàng xóm với nhau và nằm ngược vị trí nhau
	int a = 0;
	int Temp, Temp2;

	for (int i = 0; i < Length; i++) {

		if (!vMatrix[i]) {
			n += abs((i / Rank) - (Rank - 1))*abs((i / Rank) - (Rank - 1)) 
				+ abs((i % Rank) - (Rank - 1))*abs((i % Rank) - (Rank - 1));
		}
		else {
			Temp = i / Rank;
			Temp2 = (vMatrix[i] - 1) / Rank;
			n += abs(Temp - Temp2)*abs(Temp - Temp2);
			Temp = i % Rank;
			Temp2 = (vMatrix[i] - 1) % Rank;
			n += abs(Temp - Temp2)*abs(Temp - Temp2);
		}

		if ((i % Rank != Rank - 1) && vMatrix[i] == i + 2 && vMatrix[i + 1] == i + 1)
			a += 2;
		if ((i < Length - Rank) && vMatrix[i] == i + Rank + 1 && vMatrix[i + Rank] == i + 1)
			a += 2;
	}
	return n - (int)(.15f*n +.5f) + a;
}

//Chỉ so sánh trên vector, còn các thuộc tính khác k quan tâm.
bool MATRIX::operator!=(const MATRIX &m)
{
	if (vMatrix != m.vMatrix)
		return true;
	return false;
}

MATRIX MATRIX::MoveUp() 
{
	MATRIX m;
	if (BlankPos - Rank >= 0) {
		m = *this;
		swap(m.vMatrix[BlankPos], m.vMatrix[BlankPos - Rank]);
		m.SetBlankPos(BlankPos - Rank);
	}
	return m;
}

MATRIX MATRIX::MoveDown()
{
	MATRIX m;
	if (BlankPos + Rank < Length) {
		m = *this;
		swap(m.vMatrix[BlankPos], m.vMatrix[BlankPos + Rank]);
		m.SetBlankPos(BlankPos + Rank);
	}
	return m;
}

MATRIX MATRIX::MoveLeft()
{
	MATRIX m;
	if (BlankPos % Rank != 0) {
		m = *this;
		swap(m.vMatrix[BlankPos], m.vMatrix[BlankPos - 1]);
		m.SetBlankPos(BlankPos - 1);
	}
	return m;
}

MATRIX MATRIX::MoveRight()
{
	MATRIX m;
	if (BlankPos % Rank != Rank - 1) {
		m = *this;
		swap(m.vMatrix[BlankPos], m.vMatrix[BlankPos + 1]);
		m.SetBlankPos(BlankPos + 1);
	}
	return m;
}

MATRIX::~MATRIX()
{
	vMatrix.clear();
}