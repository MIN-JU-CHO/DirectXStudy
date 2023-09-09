#include <windows.h>
#include <xnamath.h>
#include <iostream>
using namespace std;

// XMVECTOR 객체 출력 cout << OVERLOAD
ostream& operator<<(ostream& os, FXMVECTOR v)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);
	os << "(" << dest.x << "," << dest.y << "," << dest.z << "," << dest.w << ")";
	return os;
}

// XMMATRIX 객체 출력 cout << OVERLOAD
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

int main()
{
	// SSE2 지원 여부 점검
	if (!XMVerifyCPUSupport())
	{
		cout << "xna math not supported" << endl;
		return 0;
	}

	// A, B=I, C= A*B, D = A^T, det = det(A), E = A^(-1), F=A*E
	XMMATRIX A = XMMatrixSet(1.0f, 0, 0, 0,
		0, 2.0f, 0, 0,
		0, 0, 4.0f, 0,
		1.0f, 2.0f, 3.0f, 1.0f);
	XMMATRIX B = XMMatrixIdentity();
	XMMATRIX C = XMMatrixMultiply(A, B);
	XMMATRIX D = XMMatrixTranspose(A);
	XMVECTOR det = XMMatrixDeterminant(A);
	XMMATRIX E = XMMatrixInverse(&det, A);
	XMMATRIX F = XMMatrixMultiply(A, E);

	cout << "A = " << endl << A << endl;
	cout << "B = I = " << endl << B << endl;
	cout << "C = A*B = " << endl << C << endl;
	cout << "D = A^T = " << endl << D << endl;
	cout << "det A = " << endl << det << endl;
	cout << "E = A^(-1) = " << endl << E << endl;
	cout << "F = A*E = " << endl << F << endl;

	return 0;
}