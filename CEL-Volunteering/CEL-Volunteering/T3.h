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
{//ǰ��׼��
	//map��2021��ĸ߿�����������
	map<int, map<int, int>> Cumulate_ScoreAndRank_2021;
	//map��רҵ������������Ӧ
	map<string, vector<int>> professionAndRankings;
	//map��רҵ�����������Ӧ
	map<string, vector<int>> professionAndScores;
	//map��רҵ��ѧԺ��Ӧ
	map<string, string> professionAndAcademy;
	//map��רҵ��ѧ�ƶ�Ӧ
	map<string, string> professionAndSubject;
	//map������רҵ�����Ĵ����Ӧ
	map<string, string> professionAndTypes;

	//����2021��߿�������Ӧ����������������map��������
	ifstream finput("2021��㶫ʡ�߿���ͨ�ࣨ����һ��һ������.txt");
	Calculate(finput, 2021, Cumulate_ScoreAndRank_2021);

	//map:רҵ��ѧԺ��ѧ�ƣ���������
	Files_CloseAndOpen("�������-��ɽ��ѧ2018-2020�걾������¼ȡͳ�ƣ��㶫��.txt", finput);
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

	//map:רҵ��ѧԺ��ѧ�ƣ��������
	Files_CloseAndOpen("��ɽ��ѧ2018-2020�걾������¼ȡͳ�ƣ��㶫��.txt", finput);
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

	//map:�����רҵ�Լ������רҵ
	Files_CloseAndOpen("����רҵ��2021��רҵ��Ӧ��.txt", finput);
	string type;
	while (!finput.eof())
	{
		finput >> profession >> academy >> subject
			>> min_2020 >> avg_2020 >> min_2019 >> avg_2019
			>> min_2018 >> avg_2018 >> type;
		if (finput.fail()) break;
		professionAndTypes[profession] = type;
	}

	//��ʽ����
		//�õ���������
	int distinguish;
	int ranks;
	int score;
	int inf, sup;
	cout << "�߿�־Ը�ϵͳ" << endl
		<< "\t��ѡ�����뷽ʽ��" << endl
		<< "\t\t1.����߿��ɼ�" << endl
		<< "\t\t2.����߿�����" << endl;
	cin >> distinguish;
	if (distinguish == 1)
	{
		cout << "������ɼ���" << endl;
		cin >> score;
		ranks = Cumulate_ScoreAndRank_2021[2021][score];
	}
	else
	{
		cout << "������������" << endl;
		cin >> ranks;
	}
	//������רҵ���������������һ��ƽ��ֵ80%-120%֮�����������������������ôֵ��ѡ��
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
	//�õ���Ӧ�Ĵ���
	set<string> Candidate_Types;
	for (auto it2 = Candidate_Profession.begin(); it2 != Candidate_Profession.end(); it2++)
	{
		Candidate_Types.emplace(*it2);
	}
	if (Candidate_Types.empty())
	{
		cout << "��ɽ��ѧû���ʺ���Ĵ��࣬��ѡ������ѧУ!" << endl;
	}
	else
	{
		//�����Ӧ���
		cout << endl << "������Ĵ������£�" << endl;
		for (auto it3 = Candidate_Types.begin(); it3 != Candidate_Types.end(); it3++)
		{
			cout << "\t" << *it3 << endl;
		}

		ofstream foutput("���������.txt");
		cout << "����(�������Ӧרҵ��¼ȡ����������)���ļ������������.txt��" << endl;
		foutput << "רҵ" << "\t" << "ѧԺ" << "\t" << "�ֿ�" << "\t" << "����" << "\t"
			<< "2020�������λ" << "\t" << "2020��ƽ����λ" << "\t"
			<< "2019�������λ" << "\t" << "2019��ƽ����λ" << "\t"
			<< "2018�������λ" << "\t" << "2018��ƽ����λ" << endl;
		for (auto it4 = Candidate_Profession.begin(); it4 != Candidate_Profession.end(); it4++)
		{
			foutput << *it4 << "\t" << professionAndAcademy[*it4] << "\t" << professionAndSubject[*it4] << "\t" << professionAndTypes[*it4] << "\t"
				<< professionAndRankings[*it4][0] << "\t" << professionAndRankings[*it4][1] << "\t"
				<< professionAndRankings[*it4][2] << "\t" << professionAndRankings[*it4][3] << "\t"
				<< professionAndRankings[*it4][4] << "\t" << professionAndRankings[*it4][5] << endl;
		}
	}

}