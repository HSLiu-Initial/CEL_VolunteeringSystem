#pragma once
#include <iostream>
//#include "D:\Boost\Numcpp\NumCpp\include\NumCpp.hpp"
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

//�����-����-�ۼƷֶ���������һ��map
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

	ifstream finput("2018�߿�һ��һ��.txt");
	Calculate(finput, 2018, scoresAndRankings);
	Files_CloseAndOpen("2019�㶫ʡ�߿�һ��һ������.txt", finput);
	Calculate(finput, 2019, scoresAndRankings);
	Files_CloseAndOpen("2020�㶫�߿�һ��һ������.txt", finput);
	Calculate(finput, 2020, scoresAndRankings);

	Files_CloseAndOpen("��ɽ��ѧ2018-2020�걾������¼ȡͳ�ƣ��㶫��.txt", finput);
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

	ofstream foutput("�������-��ɽ��ѧ2018-2020�걾������¼ȡͳ�ƣ��㶫��.txt");
	foutput << "רҵ" << "\t" << "ѧԺ" << "\t" << "�ֿ�" << "\t"
		<< "2020�������λ" << "\t" << "2020��ƽ����λ" << "\t"
		<< "2019�������λ" << "\t" << "2019��ƽ����λ" << "\t"
		<< "2018�������λ" << "\t" << "2018��ƽ����λ" << endl;

	for (auto it = professionAndRankings.begin(); it != professionAndRankings.end(); it++)
	{
		foutput << it->first << "\t" << professionAndAcademy[it->first] << "\t" << professionAndSubject[it->first] << "\t";
		for (int i = 0; i < 6; i++)
		{
			foutput << it->second[i] << "\t";
		}
		foutput << endl;
	}

	cout << "�������CEL-Volunteering/CEL-Volunteering�ļ�����";
}