#include "bits/stdc++.h"
using namespace std;

string s{};
string arData[3] = { "pi","ka","chu" };

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> s;
	
	// 예외처리?
	// 최적화는 안될까요?

	// 5천개면 충분.
	for (int i = 0; i < s.length(); i++)
	{
		string tmps = s.substr(i, 2);
		string tmps2 = s.substr(i, 3);
		if (i <= s.length()-2 && (tmps == arData[0] || tmps == arData[1]))
		{
			i += 1;
		}
		else if(i <= s.length() - 3 && tmps2 == arData[2])
		{
			i += 2;
		}
		else
		{
			cout << "NO" << "\n";
			return 0;
		}
	}
	cout << "YES" << "\n";
}

