// ConsoleApplication10.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "bits/stdc++.h"
using namespace std;

struct FInfo
{
	int team;
	int sec;
};

int n;
int arTime[3]{};
int arScore[3]{};
queue<FInfo> qData{};

int main()
{
	scanf_s("%d", &n);
	int team, m, s;
	for (int i = 0; i < n; i++)
	{
		scanf_s("%d %d:%d", &team, &m, &s);
		arScore[team]++;
		// 이미 이기고 있는 상황이면..? 큐에 들어와있는 상황이고. 뭐 할 필요x
		if (!qData.empty() && qData.front().team == team)
			continue;
		s += 60 * m;
		if (qData.empty())
		{
			qData.push({ team, s });
		}
		else
		{
			if (arScore[qData.front().team] == arScore[team])
			{
				// 안에 있던 초와 지금 현재초만큼 상대방이 이기고 있던 시간임. 
				arTime[qData.front().team] += s - qData.front().sec;
				qData.pop();
			}
		}
	}

	if(!qData.empty())
		arTime[qData.front().team] += (48*60) - qData.front().sec;

	for (int i = 1; i <= 2; i++)
	{
		string min = (arTime[i] / 60) >= 10 ? to_string((arTime[i] / 60)) : "0" + to_string((arTime[i] / 60));
		int sec = arTime[i] - (stoi(min) * 60);
		string sSec = sec >= 10 ? to_string(sec) : "0" + to_string(sec);

		// 00:03 이런식으로 뽑혀야 하는데?....
		printf("%s:%s\n", min.c_str(), sSec.c_str());
	}
}
/*
10:23 -> 14:30 = 4:07
10:23 -> 14:20 = 3:57


idea
이기고 있는 팀과 시간을 queue에 넣어준다. 
배열에는 각 팀의 이기고 있는 시간의 합을 저장해준다. += 해주면됨.
스코어배열도 있어야 함

0:0은 아무도 이기고 있는게 아니다.
1. 골을 넣으면 큐를 확인
	1) 비어있으면 큐에 들어간다.
	2) 큐에 누가 들어가 있다면 스코어 배열에서 상대방 골 수 확인한다
		2-1) 내가 골 넣어서 이제 동점된거면 큐에있는 놈 빼주고 그놈 시간 추가해준다.
		2-2) 내 스코어 하나 추가해준다 
		2-3) 큐에 누군가 나온다면 누군가 바로 들어갈 수는 없다.
2. 마지막 48분 경기 끝나면 
	1) 큐가 비어있으면 비긴상태로 끝난거니까 할 일 x
	2) 큐에 들어가있으면 그놈 빼고 48-골넣은시각만큼 시간추가해줌.,
*/