#include <windows.h>
#include <xnamath.h>
#include <iostream>
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
	XMVECTOR Q = XMVectorSet(-20.0f, 35.0f, -50.0f, 1.0f);
	XMVECTOR T = XMVectorSet(10.0f, 0.0f, 30.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(Q, T, up);
	cout << V;
}