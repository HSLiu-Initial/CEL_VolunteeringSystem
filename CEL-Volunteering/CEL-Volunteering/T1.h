#pragma once

#include <iostream>
//#include "D:\Boost\Numcpp\NumCpp\include\NumCpp.hpp"
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

//便于令创建的map按值排序
bool cmp(pair<string, int> a, pair<string, int> b)
{
	return a.second < b.second;
}
//按value而不是key排序
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
//输出前十个元素
void Output(vector< pair<string, int> > vec)
{
	int i = 1;
	auto it = vec.begin();
	it++;
	while (it != vec.end() && i <= 10)
	{
		cout << "    " << "    " << i << ":" << (*it).first << " 六项有效平均分：" << (*it).second << endl;
		it++;
		i++;
	}
}
//输出后十个元素
void Output_Reverse(vector< pair<string, int> > vec)
{
	int i = 1;
	auto it = vec.rbegin();
	while (it != vec.rend() && i <= 10)
	{
		cout << "    " << "    " << i << ":" << (*it).first << " 六项有效平均分：" << (*it).second << endl;
		it++;
		i++;
	}
}

void PopularAndUnpopularProfession()
{
	map<string, int> ProfessionAndAverageScore_Liberal,
		ProfessionAndAverageScore_Science;
	ifstream finput("中山大学2018-2020年本科批次录取统计（广东）.txt");
	string profession = "", academy = "", subject = "";
	vector<int> score;
	int average_score = 0, temp1 = 0, sum = 0;

	//输入数据，创建键值对
	while (!finput.eof())
	{
		if (finput.fail())  break;
		//处理文科,共31个学院
		for (int i = 1; i <= 31; i++)
		{
			//每阅读一行以后将profession academy subject average_score以及score初始化
			profession = academy = subject = "";
			average_score = 0;
			score.clear();
			temp1 = sum = 0;

			//依次输入专业，学院，文理科情况
			finput >> profession >> academy >> subject;
			//将历年分数输入并计算出有效平均值
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

			//添加键值对
			ProfessionAndAverageScore_Liberal.insert({ profession,average_score });
		}
		//处理理科，共86个专业
		for (int i = 1; i <= 86; i++)
		{
			//每阅读一行以后将profession academy subject average_score以及score初始化
			profession = academy = subject = "";
			average_score = 0;
			score.clear();
			temp1 = sum = 0;

			//依次输入专业，学院，文理科情况
			finput >> profession >> academy >> subject;
			//将历年分数输入并计算出有效平均值
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
			//添加键值对
			ProfessionAndAverageScore_Science.insert({ profession,average_score });
		}



	}
	//将map按值排序
	vector< pair<string, int> > vec1 = SortByValue_Up(ProfessionAndAverageScore_Liberal),
		vec2 = SortByValue_Up(ProfessionAndAverageScore_Science);

	cout << "文科热门冷门专业情况如下：" << endl
		<< "    " << "十大热门专业" << endl;
	Output_Reverse(vec1);
	cout << endl
		<< "    " << "十大冷门专业" << endl;
	Output(vec1);

	cout << "理科热门冷门专业情况如下：" << endl
		<< "    " << "十大热门专业" << endl;
	Output_Reverse(vec2);
	cout << endl
		<< "    " << "十大冷门专业" << endl;
	Output(vec2);

}