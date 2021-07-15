#pragma once
#include <iostream>
//#include "D:\Boost\Numcpp\NumCpp\include\NumCpp.hpp"
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include "T2.h"
#include <iomanip>
using namespace std;


void CEL_VolunteeringSystem()
{//前期准备
	//map：2021年的高考分数与排名
	map<int, map<int, int>> Cumulate_ScoreAndRank_2021;
	//map：专业与历年排名对应
	map<string, vector<int>> professionAndRankings;
	//map：专业与历年分数对应
	map<string, vector<int>> professionAndScores;
	//map：专业与学院对应
	map<string, string> professionAndAcademy;
	//map：专业与学科对应
	map<string, string> professionAndSubject;
	//map：往年专业与今年的大类对应
	map<string, string> professionAndTypes;

	//计算2021年高考分数对应的排名，将他们用map连接起来
	ifstream finput("2021年广东省高考普通类（物理）一分一段数据.txt");
	Calculate(finput, 2021, Cumulate_ScoreAndRank_2021);

	//map:专业，学院，学科，历年排名
	Files_CloseAndOpen("理科排名-中山大学2018-2020年本科批次录取统计（广东）.txt", finput);
	vector<int> rank;
	string profession = "", academy = "", subject = "";
	string garbish;
	int min_2020, avg_2020, min_2019, avg_2019, min_2018, avg_2018;
	while (!finput.eof())
	{
		getline(finput, garbish);

		for (int i = 1; i <= 75; i++)
		{
			rank.clear();
			finput >> profession >> academy >> subject
				>> min_2020 >> avg_2020 >> min_2019 >> avg_2019
				>> min_2018 >> avg_2018;

			rank.push_back(min_2020);
			rank.push_back(avg_2020);
			rank.push_back(min_2019);
			rank.push_back(avg_2019);
			rank.push_back(min_2018);
			rank.push_back(avg_2018);
			professionAndAcademy[profession] = academy;
			professionAndSubject[profession] = subject;
			professionAndRankings[profession] = rank;
		}
	}

	//map:专业，学院，学科，历年分数
	Files_CloseAndOpen("中山大学2018-2020年本科批次录取统计（广东）.txt", finput);
	vector<int> scores;
	while (!finput.eof())
	{
		scores.clear();
		finput >> profession >> academy >> subject
			>> min_2020 >> avg_2020 >> min_2019 >> avg_2019
			>> min_2018 >> avg_2018;
		if (finput.fail())  break;
		scores.push_back(min_2020);
		scores.push_back(avg_2020);
		scores.push_back(min_2019);
		scores.push_back(avg_2019);
		scores.push_back(min_2018);
		scores.push_back(avg_2018);
		professionAndScores[profession] = scores;
	}

	//map:往年的专业以及今年的专业
	Files_CloseAndOpen("往年专业与2021年专业对应表.txt", finput);
	string type;
	while (!finput.eof())
	{
		finput >> profession >> academy >> subject
			>> min_2020 >> avg_2020 >> min_2019 >> avg_2019
			>> min_2018 >> avg_2018 >> type;
		if (finput.fail()) break;
		professionAndTypes[profession] = type;
	}

	//正式处理
		//得到考生排名
	int distinguish;
	int ranks;
	int score;
	int inf, sup;
	cout << "高考志愿填报系统" << endl
		<< "\t请选择输入方式：" << endl
		<< "\t\t1.输入高考成绩" << endl
		<< "\t\t2.输入高考排名" << endl;
	cin >> distinguish;
	if (distinguish == 1)
	{
		cout << "请输入成绩：" << endl;
		cin >> score;
		ranks = Cumulate_ScoreAndRank_2021[2021][score];
	}
	else
	{
		cout << "请输入排名：" << endl;
		cin >> ranks;
	}
	//遍历各专业历年排名，如果有一年平均值80%-120%之间的排名包含考生排名，那么值得选择
	vector<string> Candidate_Profession;
	for (auto it = professionAndRankings.begin(); it != professionAndRankings.end(); it++)
	{
		int sup, inf;
		int flag = 0;
		for (int i = 1; i <= 5; i += 2)
		{
			sup = (*it).second[i] * 1.2;
			inf = (*it).second[i] * 0.8;
			if ((inf <= ranks) && (ranks <= sup))
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1) Candidate_Profession.push_back(it->first);
	}
	//得到对应的大类
	set<string> Candidate_Types;
	for (auto it2 = Candidate_Profession.begin(); it2 != Candidate_Profession.end(); it2++)
	{
		Candidate_Types.emplace(*it2);
	}
	if (Candidate_Types.empty())
	{
		cout << "中山大学没有适合你的大类，请选择其他学校!" << endl;
	}
	else
	{
		//输出对应结果
		cout << endl << "建议填报的大类如下：" << endl;
		for (auto it3 = Candidate_Types.begin(); it3 != Candidate_Types.end(); it3++)
		{
			cout << "\t" << *it3 << endl;
		}

		ofstream foutput("建议填报理由.txt");
		cout << "理由(近三年对应专业的录取分数和排名)见文件“建议填报理由.txt”" << endl;
		foutput << "专业" << "\t" << "学院" << "\t" << "分科" << "\t" << "大类" << "\t"
			<< "2020年最低排位" << "\t" << "2020年平均排位" << "\t"
			<< "2019年最低排位" << "\t" << "2019年平均排位" << "\t"
			<< "2018年最低排位" << "\t" << "2018年平均排位" << endl;
		for (auto it4 = Candidate_Profession.begin(); it4 != Candidate_Profession.end(); it4++)
		{
			foutput << *it4 << "\t" << professionAndAcademy[*it4] << "\t" << professionAndSubject[*it4] << "\t" << professionAndTypes[*it4] << "\t"
				<< professionAndRankings[*it4][0] << "\t" << professionAndRankings[*it4][1] << "\t"
				<< professionAndRankings[*it4][2] << "\t" << professionAndRankings[*it4][3] << "\t"
				<< professionAndRankings[*it4][4] << "\t" << professionAndRankings[*it4][5] << endl;
		}
	}

}