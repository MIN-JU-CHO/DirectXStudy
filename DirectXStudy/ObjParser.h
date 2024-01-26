#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "GeometryGenerator.h"

using namespace std;
class ObjGenerator : GeometryGenerator
{
	ifstream file;
	float GetReal(string str, int begin, int end) const;
public:
	void CreateObj(string file_path, MeshData& meshData);
};
