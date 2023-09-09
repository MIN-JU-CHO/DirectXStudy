#include <cstdio>
#include <vector>
using namespace std;


int main(void)
{
	int N = 0;
	scanf("%d", &N);

	vector<long long> dp = vector<long long>(1000001, 0);
	dp[1] = 1;
	dp[2] = 2;
	for (int i = 3; i <= N; i++)
	{
		dp[i] = (dp[i - 2] + dp[i - 1])  % 15746;
	}
	printf("%lld", dp[N]);
}