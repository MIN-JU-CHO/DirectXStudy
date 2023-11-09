#include <windows.h>
#include <xnamath.h>
#include <iostream>
#include "MathHelper.h"
using namespace std;
// XMMATRIX °´Ã¼ Ãâ·Â cout << OVERLOAD
ostream& operator<<(ostream& os, CXMMATRIX m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			os << m(i, j) << '\t';
		}
		os << endl;
	}
	return os;
}

int main(void)
{
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, 4 / 3, 1.0f, 100.0f);
	cout << perspective;
}
