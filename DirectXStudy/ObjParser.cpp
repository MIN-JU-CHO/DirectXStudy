#include "ObjParser.h"

void ObjGenerator::CreateObj(string file_path, MeshData& meshData)
{
	bool isV = true, isVt = false, isVn = false, isF = false;
	vector<XMFLOAT3> vp;
	vector<XMFLOAT2> vt;
	vector<XMFLOAT3> vn;
	vector<UINT> vindex, uvindex, nindex;
	string line;
	ifstream file(file_path);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (isV && line[0] == 'v' && line[1] == ' ')
			{
				vector<float> f(3);
				int begin = 2, end = 2;
				for (int i = 0; i < 3; ++i)
				{
					for (;end < line.size() && line[end] != ' '; ++end)
					{

					}
					f[i] = GetReal(line, begin, end - 1);
					begin = end + 1;
					++end;
				}
				XMFLOAT3 temp = XMFLOAT3(f[0], f[1], f[2]);
				vp.push_back(temp);
			}
			else if (isV && line[0] == 'v' && line[1] == 't')
			{
				isV = false;
				isVt = true;
			}
			if (isVt && line[0] == 'v' && line[1] == 't')
			{
				vector<float> f(2, 0);
				int begin = 3, end = 3;
				for (int i = 0; i < 2; ++i)
				{
					for (;end < line.size() && line[end] != ' '; ++end)
					{

					}
					f[i] = GetReal(line, begin, end - 1);
					begin = end + 1;
					++end;
				}
				XMFLOAT2 temp = XMFLOAT2(f[0], f[1]);
				vt.push_back(temp);
			}
			else if (isVt && line[0] == 'v' && line[1] == 'n')
			{
				isVt = false;
				isVn = true;
			}
			if (isVn && line[0] == 'v' && line[1] == 'n')
			{
				vector<float> f(3, 0);
				int begin = 3, end = 3;
				for (int i = 0; i < 3; ++i)
				{
					for (;end < line.size() && line[end] != ' '; ++end)
					{

					}
					f[i] = GetReal(line, begin, end - 1);
					begin = end + 1;
					++end;
				}
				XMFLOAT3 temp = XMFLOAT3(f[0], f[1], f[2]);
				vn.push_back(temp);
			}
			else if (isVn && line[0] == 'f')
			{
				isVn = false;
				isF = true;
			}
			if (isF && line[0] == 'f' && line[1] == ' ')
			{
				int begin = 2, end = 2;
				int b = 2;
				while(b--)
				{
					for (;end < line.size() && line[end] != '/' && line[end] != ' '; ++end)
					{

					}
					unsigned int a = (unsigned int)GetReal(line, begin, end - 1);
					vindex.push_back(a);
					begin = end + 1;
					++end;
					for (;end < line.size() && line[end] != '/' && line[end] != ' '; ++end)
					{

					}
					a = (unsigned int)GetReal(line, begin, end - 1);
					uvindex.push_back(a);
					begin = end + 1;
					++end;
					for (;end < line.size() && line[end] != '/' && line[end] != ' '; ++end)
					{

					}
					a = (unsigned int)GetReal(line, begin, end - 1);
					nindex.push_back(a);
					begin = end + 1;
					++end;
				}
			}
			else if (isF)
			{
				for (UINT i = 0; i <= vindex.size() - 4; ++i)
				{
					Vertex temp;
					if (vindex[i] - 1 < vp.size() && nindex[i] - 1 < vn.size() && uvindex[i] - 1 < vt.size())
					{
						temp.Position = vp[vindex[i] - 1];
						temp.Normal = vn[nindex[i] - 1];
						temp.TexC = vt[uvindex[i] - 1];
						meshData.Vertices.push_back(temp);
						UINT idxtemp[6] = {i, i + 1, i + 2, i + 1, i + 2, i + 3};
						for (int k = 0; k < 6; ++k)
						{
							meshData.Indices.push_back(idxtemp[k]);
						}
					}
				}
				isF = false;
				isV = true;
				/*vp.clear();
				vn.clear();
				vt.clear();*/
				//break;
			}
		}
		file.close();
	}
	else
	{
		return;
	}
}
float ObjGenerator::GetReal(string str, int begin, int end) const
{
	if (begin > end)
	{
		return 0;
	}
	bool minus = false;
	if (str[begin] == '-')
	{
		minus = true;
	}
	if (str[begin] == '-' || str[begin] == '+')
	{
		++begin;
	}
	bool is_int = true;
	float num = 0, decimal = 1;
	for (int i = begin; i <= end; ++i)
	{
		if (isdigit(str[i]) && is_int)
		{
			num *= 10;
			num += str[i] - '0';
		}
		// 첫 . 만 가능
		else if (str[i] == '.' && is_int)
		{
			is_int = false;
		}
		else if (isdigit(str[i]) && !is_int)
		{
			decimal /= 10;
			num += decimal * (str[i] - '0');
		}
		else
		{
			break;
		}
	}
	// 음수는 마지막에 처리해줘야 수 읽기의 덧셈 연산이 정상적으로 수행됨
	if (minus)
	{
		num *= -1;
	}
	return num;
}