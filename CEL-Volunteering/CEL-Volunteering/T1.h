#pragma once

#include <iostream>
//#include "D:\Boost\Numcpp\NumCpp\include\NumCpp.hpp"
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

//���������map��ֵ����
bool cmp(pair<string, int> a, pair<string, int> b)
{
	return a.second < b.second;
}
//��value������key����
vector< pair<string, int> > SortByValue_Up(map<string, int> mp)
{
	vector< pair<string, int> > vec;
	for (map<string, int>::iterator it = mp.begin(); it != mp.end(); it++)
	{
		vec.push_back(pair<string, int>(it->first, it->second));
	}
	sort(vec.begin(), vec.end(), cmp);
	return vec;
}
//���ǰʮ��Ԫ��
void Output(vector< pair<string, int> > vec)
{
	int i = 1;
	auto it = vec.begin();
	it++;
	while (it != vec.end() && i <= 10)
	{
		cout << "    " << "    " << i << ":" << (*it).first << " ������Чƽ���֣�" << (*it).second << endl;
		it++;
		i++;
	}
}
//�����ʮ��Ԫ��
void Output_Reverse(vector< pair<string, int> > vec)
{
	int i = 1;
	auto it = vec.rbegin();
	while (it != vec.rend() && i <= 10)
	{
		cout << "    " << "    " << i << ":" << (*it).first << " ������Чƽ���֣�" << (*it).second << endl;
		it++;
		i++;
	}
}

void PopularAndUnpopularProfession()
{
	map<string, int> ProfessionAndAverageScore_Liberal,
		ProfessionAndAverageScore_Science;
	ifstream finput("��ɽ��ѧ2018-2020�걾������¼ȡͳ�ƣ��㶫��.txt");
	string profession = "", academy = "", subject = "";
	vector<int> score;
	int average_score = 0, temp1 = 0, sum = 0;

	//�������ݣ�������ֵ��
	while (!finput.eof())
	{
		if (finput.fail())  break;
		//�����Ŀ�,��31��ѧԺ
		for (int i = 1; i <= 31; i++)
		{
			//ÿ�Ķ�һ���Ժ�profession academy subject average_score�Լ�score��ʼ��
			profession = academy = subject = "";
			average_score = 0;
			score.clear();
			temp1 = sum = 0;

			//��������רҵ��ѧԺ����������
			finput >> profession >> academy >> subject;
			//������������벢�������Чƽ��ֵ
			for (int j = 1; j <= 6; j++)
			{
				finput >> temp1;
				score.emplace_back(temp1);
			}
			int count = 0;
			/*for (int l = 1; l <= 6; l++)
			{
				if (score[l] != 0)
				{
					count++;
				}
			}*/
			for (auto k = score.begin(); k != score.end(); k++)
			{
				if ((*k) != 0)
					count++;
				sum += (*k);
			}
			if (count == 0)
			{
				cout << profession;
			}
			else { average_score = sum / count; }

			//��Ӽ�ֵ��
			ProfessionAndAverageScore_Liberal.insert({ profession,average_score });
		}
		//������ƣ���86��רҵ
		for (int i = 1; i <= 86; i++)
		{
			//ÿ�Ķ�һ���Ժ�profession academy subject average_score�Լ�score��ʼ��
			profession = academy = subject = "";
			average_score = 0;
			score.clear();
			temp1 = sum = 0;

			//��������רҵ��ѧԺ����������
			finput >> profession >> academy >> subject;
			//������������벢�������Чƽ��ֵ
			for (int j = 1; j <= 6; j++)
			{
				finput >> temp1;
				score.emplace_back(temp1);
			}
			int count = 0;
			for (auto k = score.begin(); k != score.end(); k++)
			{
				if (*k != 0)
					count++;
				sum += (*k);
			}
			if (count == 0)
			{
				cout << profession;
			}
			else { average_score = sum / count; }
			//��Ӽ�ֵ��
			ProfessionAndAverageScore_Science.insert({ profession,average_score });
		}



	}
	//��map��ֵ����
	vector< pair<string, int> > vec1 = SortByValue_Up(ProfessionAndAverageScore_Liberal),
		vec2 = SortByValue_Up(ProfessionAndAverageScore_Science);

	cout << "�Ŀ���������רҵ������£�" << endl
		<< "    " << "ʮ������רҵ" << endl;
	Output_Reverse(vec1);
	cout << endl
		<< "    " << "ʮ������רҵ" << endl;
	Output(vec1);

	cout << "�����������רҵ������£�" << endl
		<< "    " << "ʮ������רҵ" << endl;
	Output_Reverse(vec2);
	cout << endl
		<< "    " << "ʮ������רҵ" << endl;
	Output(vec2);

}