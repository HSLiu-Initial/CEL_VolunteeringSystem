# 高考志愿填报系统分析报告

[TOC]



## 依赖项

高考志愿填报系统主要基于C++ STL。用到的Library如下：

```c++
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <iomanip>
```

## 第一题

第一题要求我们分析中山大学考生填报的“十大热门专业”和“十大冷门专业”并提供相关的指标。

### 思路

我的思路分为如下几步：

1. 清理数据。原数据存在如下几个问题

   1. 原格式为`UTF16`，并不是`C++`接受的格式。

   2. 大多数专业的数据并不完整，需要标记空白数据。

   因此我将`txt`格式文件转换为`csv`格式文件以后使用`Python pandas`进行了数据的清洗（并未体现在代码中）。
   
2. 将文理科分开，分别计算每个专业历年平均分和最低分的有效平均值（有效指的是只考虑有效数值），并将其与专业用`map`进行映射。

3. 按`value`对`map`进行排序，输出热门专业和冷门专业。

### 重要代码

部分重要函数如下：

#### 将文理科分开，分别计算每个专业历年平均分和最低分的有效平均值（有效指的是只考虑有效数值），并将其与专业用`map`进行映射。

```c++
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
			else{ average_score = sum / count; }
			
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
```



#### 令`map`按值排序。

```c++
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
```

#### 输出前十个元素与后十个元素

```C++
//输出前十个元素
void Output(vector< pair<string, int> > vec)
{
	int i = 1;
	auto it = vec.begin();
	it++;
	while (it != vec.end() && i <= 10)
	{
		cout << "    " << "    " << i << ":" << (*it).first <<" 六项有效平均分："<<(*it).second << endl;
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
```

### 结果与结论

![T1结果图](C:\Users\Legion\Desktop\CEL-Volunteering\T1结果图.png)

   ## 第二题                        

 ### 思路

第二题要求我们通过“2018-2020每年的一分一段”数据，完整补充“中山大学2018-2020年本科批次录取统计（广东）”中缺少的平均分和最低分的排位信息。由于一分一段表中的每一个分数的排名都已经给出，因此我们只需要用排名替换“中山大学2018-2020年本科批次录取统计（广东）”中的分数，本质上就是一个查找的过程。

最后我们将结果输出到一个txt文件中，方便第三问使用。	    

### 重要代码

#### 创建map：年份，分数，分段累计人数

```c++
void Calculate(ifstream& finput,int year, map<int, map<int, int>>& mp)
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

```

#### 将分数替换为排名

```C++
map<string,vector<int>> professionAndRankings;
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
```

#### 输出

```C++
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
```



### 结果

![T2结果图](C:\Users\Legion\Desktop\CEL-Volunteering\T2结果图.png)                                                                                                                                                                                                                                                                                                                  

## 第三题

第三题要求我们结合“2021年广东省高考普通类（物理）一分一段”和“中山大学2021年分专业招生计划（广东）”，输入考生的分数或排名，输出推荐填报的2021年中山大学大类专业志愿以及参考依据。

### 思路

我的思路分为以下几点：

1. 分析输入：题目要求我们输入高考成绩或者是排名，鉴于高考分数会随着题目难度升降，我选择将高考分数也转换成排名。

2. 分析评价“是否推荐的指标”：一般来说，省内985高校各专业的分数线比较稳定（因为招生多）。于是在这里我选取的评价指标是：如果考生排名在该专业历年排名的80%-120%之间则视为可以填报。这样既不会浪费分数也有较大概率能够进入该专业。

3. 具体操作：

   1. 参考 T2 的操作，建立`map:`2021年高考分数->2021年高考排名
   2. 建立`map:`专业，历年排名，历年分数，学院，学科，今年大类
   3. 遍历`map:`专业->历年排名选取合适的专业。
   4. 建立`set`得到各个专业对应的大类（不允许重复）
   5. 将推荐填报的大类专业输出到控制台，理由输出到文件中。

### 重要代码

#### `map:`专业，学院，学科，历年排名

```C++
Files_CloseAndOpen("理科排名-中山大学2018-2020年本科批次录取统计（广东）.txt",finput);
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
```



#### `map:`专业，学院，学科，历年分数

```C++
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
```



#### map:往年的专业以及今年的专业

```C++
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
```

#### 得到考生排名

```C++
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
```

#### 遍历各专业历年排名，如果有一年平均值80%-120%之间的排名包含考生排名，那么值得选择

```C++
vector<string> Candidate_Profession;
	for (auto it = professionAndRankings.begin(); it != professionAndRankings.end(); it++)
	{
		int sup,inf;
		int flag = 0;
 		for (int i = 1; i <= 5; i += 2)
		{
			sup= (*it).second[i] * 1.2;
			inf = (*it).second[i] * 0.8;
			if ((inf <= ranks ) && (ranks <= sup))
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1) Candidate_Profession.push_back(it->first);
	}
```

#### 得到对应的大类

```C++
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
```

### 结果

![T3结果图1](C:\Users\Legion\Desktop\CEL-Volunteering\T3结果图1.png)

![T3结果图2](C:\Users\Legion\Desktop\CEL-Volunteering\T3结果图2.png)





