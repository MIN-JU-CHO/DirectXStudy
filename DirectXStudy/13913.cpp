#include <iostream>
#include <string>
#include <vector>

using namespace std;

int minimum = 0;
int K = 0;

vector<bool> v(100001, false);

void dfs(int depth, int N)
{
	if (N == K)
	{
		minimum = min(depth, K);
		return;
	}
	else if (N < 0 || N > K)
	{
		return;
	}
	else if (v[N] == true)
	{
		return;
	}
	else
	{
		cout << "depth: " << depth << " N: " << N << endl;
		v[N] = true;
		if (0 <= N - 1 <= K)
		{
			dfs(depth + 1, N - 1);
			v[N - 1] = false;
		}
		if (0 <= N + 1 <= K)
		{
			dfs(depth + 1, N + 1);
			v[N + 1] = false;
		}
		if (0 <= 2 * N <= K)
		{
			dfs(depth + 1, 2 * N);
			v[2 * N] = false;
		}
	}
}

int main(void)
{
	int N;
	cin >> N >> K;


	dfs(0, N);
	cout << minimum;

}