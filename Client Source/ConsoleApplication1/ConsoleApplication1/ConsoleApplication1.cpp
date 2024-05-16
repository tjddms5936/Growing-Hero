// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "bits/stdc++.h"
using namespace std;

const int MAX = 987654321;
int a, b, c, d;
int dp[10000][10000];
int minCnt = MAX;

int FX(int x, int max);
int EX(int x);
void MoveXY(int& from, int fromMAX, int& to, int toMAX);

int go(int curA, int curB, int cnt);

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	// a:A용량 b:B용량 c:A목표용량 d:B목표용량
	cin >> a >> b >> c >> d;

	fill(&dp[0][0], &dp[9999][10000], -1);

	go(0, 0, 1);
	if (minCnt == MAX)
	{
		cout << "-1" << "\n";
		return 0;
	}

	cout << minCnt << "\n";
	return 0;
}

/*
idea
일단... 3가지 방법을 함수로 정의해주는게 나으려나

함수를 부를때, Cur용량과 MAX용량을 넣어주자.
전역으로 놓으면 안됨.

만약에 어떠한 Cur상태값이 나왔을 때 ex) 4,7
만약에 이전에 4,7에 대한 작업을 진행했다면? 할 필요x
만약에 어떠한 작업이든 진행하려는데 총 횟수가 이미 찾은 최소 횟수를 넘어가면? 더 검사할 필요 x 

반환형이 필요할까? Hmm.. YES. 끝까지가면 1반환해서 쭉쭉 더해오자. 
그럼 상태정보는 저장... dp[A양][B양] = A양,B양부터 해서 몇번째 만에 도달하는지 여부


흠...
dp로 풀어야할것같은데... 100만 x 100만은 너무 사이즈가 커서 안되고..
다른 방법으로 해야하나 ㅡ.ㅡ



*/

int FX(int x, int max)
{
	x = max;
	return x;
}

int EX(int x)
{
	x = 0;
	return x;
}

void MoveXY(int& from, int fromMAX, int& to, int toMAX)
{
	const int remained = toMAX - to;
	if (remained >= from)
	{
		to += from;
		from = 0;
		return;
	}

	to = toMAX;
	from -= remained;
}

int go(int curA, int curB, int cnt)
{
	if(curA > a || curB > b || cnt >= minCnt)
		return MAX;

	if (curA == c && curB == d)
	{
		minCnt = min(minCnt, cnt);
		return 1;
	}

	int& ret = dp[curA][curB];
	if (ret != -1)
		return ret;

	ret = MAX;
	// 현 시점에서 나올 수 있는 경우의 수가 6가지네?
	// ㅡㅡ...
	// case 1 : A를 채움, B를 채움으로 갔을 때 더 작은거.
	ret = min(ret,
		min(go(FX(curA, a), curB, cnt + 1), go(curA, FX(curB, a), cnt + 1))
	);
	ret = min(ret,
		min(go(EX(curA), curB, cnt + 1), go(curA, EX(curB), cnt + 1))
	);
	
	int tmpA = curA;
	int tmpB = curB;

	MoveXY(tmpA, a, tmpB, b);
	ret = min(ret, go(tmpA, tmpB, cnt + 1));

	tmpA = curA;
	tmpB = curB;
	MoveXY(tmpB, b, tmpA, a);
	ret = min(ret, go(tmpA, tmpB, cnt + 1));

	return ret;
}
