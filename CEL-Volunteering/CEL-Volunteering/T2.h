#pragma once
#include <iostream>
//#include "D:\Boost\Numcpp\NumCpp\include\NumCpp.hpp"
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

//将年份-分数-累计分段排名创建一个map
void Calculate(ifstream& finput, int year, map<int, map<int, int>>& mp)
{
	int score, rank;
	int num;
	//map<int, map<int, int>> mp;
	while (!finput.eof())
	{
		finput >> score >> num >> rank;
		if (finput.fail())  break;
		mp[year][score] = rank;
	}
	mp[year][0] = 0;
}

void Files_CloseAndOpen(string file_open, ifstream& finput)
{
	finput.close();
	finput.open(file_open);
}

void SupplementalRankingInformation()
{
	map<int, map<int, int>> scoresAndRankings;

	ifstream finput("2018高考一分一段.txt");
	Calculate(finput, 2018, scoresAndRankings);
	Files_CloseAndOpen("2019广东省高考一分一段数据.txt", finput);
	Calculate(finput, 2019, scoresAndRankings);
	Files_CloseAndOpen("2020广东高考一分一段数据.txt", finput);
	Calculate(finput, 2020, scoresAndRankings);

	Files_CloseAndOpen("中山大学2018-2020年本科批次录取统计（广东）.txt", finput);
	string profession = "", academy = "", subject = "";
	int min_2020, avg_2020, min_2019, avg_2019, min_2018, avg_2018;
	map<string, vector<int>> professionAndRankings;
	map<string, string> professionAndAcademy;
	map<string, string> professionAndSubject;
	vector<int> rank;

	while (!finput.eof())
	{
		if (finput.fail())  break;
		for (int i = 1; i <= 31; i++)
		{
			finput >> profession >> academy >> subject
				>> min_2020 >> avg_2020 >> min_2019 >> avg_2019
				>> min_2018 >> avg_2018;
		}
		for (int i = 1; i <= 86; i++)
		{
			rank.clear();

			finput >> profession >> academy >> subject
				>> min_2020 >> avg_2020 >> min_2019 >> avg_2019
				>> min_2018 >> avg_2018;

			rank.push_back(scoresAndRankings[2020][min_2020]);
			rank.push_back(scoresAndRankings[2020][avg_2020]);
			rank.push_back(scoresAndRankings[2019][min_2019]);
			rank.push_back(scoresAndRankings[2019][avg_2019]);
			rank.push_back(scoresAndRankings[2018][min_2018]);
			rank.push_back(scoresAndRankings[2018][avg_2018]);
			professionAndAcademy[profession] = academy;
			professionAndSubject[profession] = subject;
			professionAndRankings[profession] = rank;
		}
	}

	ofstream foutput("理科排名-中山大学2018-2020年本科批次录取统计（广东）.txt");
	foutput << "专业" << "\t" << "学院" << "\t" << "分科" << "\t"
		<< "2020年最低排位" << "\t" << "2020年平均排位" << "\t"
		<< "2019年最低排位" << "\t" << "2019年平均排位" << "\t"
		<< "2018年最低排位" << "\t" << "2018年平均排位" << endl;

	for (auto it = professionAndRankings.begin(); it != professionAndRankings.end(); it++)
	{
		foutput << it->first << "\t" << professionAndAcademy[it->first] << "\t" << professionAndSubject[it->first] << "\t";
		for (int i = 0; i < 6; i++)
		{
			foutput << it->second[i] << "\t";
		}
		foutput << endl;
	}

	cout << "已输出到CEL-Volunteering/CEL-Volunteering文件夹中";
}