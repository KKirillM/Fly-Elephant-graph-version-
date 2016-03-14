#include "stdafx.h"
#include "Dictionary.h"
#include "WordChain.h"

using namespace std;

WordChain::WordChain(void)
{
}


WordChain::~WordChain(void)
{
}

//�������� ���������� � ��������� ���� �� �����
int WordChain::GetWordsFromFile(const string& path, WordChain& w)
{
	ifstream f(path);
	int wordCounter = 0;

	if (!f.is_open())
		return 1;

	if (getline(f, w.startWord) && !f.eof())
		wordCounter++;

	if (getline(f, w.endWord))
		wordCounter++;

	//���� � ����� ������ ���� ������ ���� ���� ��� ����� ������ �����, �� ������
	if (wordCounter < 2 || w.startWord.size() != w.endWord.size())
		return 1;

	return 0;
}

//������ ���������� � ��������� ����
void WordChain::SetSourceWords(const string& w1, const string& w2)
{
	startWord = w1;
	endWord = w2;
}

//�������� ���� �� ��������� ����� � ��������� � ������� resultChain
void WordChain::MakeChain(Dictionary& d)
{
	Graph grph; 
	size_t wordLen = startWord.size();	
	size_t dictLen = d.GetCount();
	string dictWord;
	Word wrd("");
	bool found = false;

	//��������� �������� ����� �� ��������� �����
	if (startWord.size() != endWord.size())
		throw exception("Source words have different length");

	//��������� ������� �� ������� ����
	if (!d.GetCount())
		throw exception("Dictionary is empty");

	//������ ���� �� ���� ��������� �������,
	//������ ��������� ������� � ���� (��� �����, ����� ������� ����� ����� �������� ����)
	//������������� ����� � ���� �� ����������
	grph.AddVertex(startWord);
	grph.AddVertex(endWord);
	for (; dictLen > 0; --dictLen)
	{
		dictWord = d[dictLen-1];

		if (dictWord.size() == wordLen)
			grph.AddVertex(dictWord);
	}

	//����� ��������� ���� ����� ��������� �����
	const size_t graphLen = grph.VertexCount();
	for (size_t i = 0; i < graphLen ; ++i)
	{
		for (size_t j = 0; j < graphLen; ++j)
		{
			wrd.SetWord(grph.GetVertex(j));

			if (GetDiffCount(grph.GetVertex(i), wrd) == 1 && j != i)
				grph.GetVertex(i).AddEdge(j);
		}
	}									

	//���� ���������� ����
	found = grph.FindShortPath(grph.FindVertex(startWord), grph.FindVertex(endWord), resultChain);

	if (!found)
		throw exception("Not enough words in dictionary");
}

//������� �� ������� ������� ����
void WordChain::PrintChain() const
{
	cout << endl << "Result word chain:" << endl;
	for (auto it = resultChain.begin(); it != resultChain.end(); ++it)
		cout << *it << endl;
}

//��������������� �-��� ��� �������� ���-�� ������������� �������� � ���� ������
size_t WordChain::GetDiffCount(const std::string& w1, const std::string& w2)
{
	size_t diffCount = 0;

	for(auto i = w1.begin(), j = w2.begin(); i < w1.end(); ++i, ++j)
	{
		if (*i != *j) 
			++diffCount;
	}

	return diffCount;
}

bool WordChain::Graph::FindShortPath(const size_t start, 
									 const size_t finish, 
									 vector<string>& resultChain)
{
	static size_t wayLen = INT_MAX;								//����� ����
	static vector<size_t> road(1, start);						//������ ������ �������� ����, ������ ������ ������� � �������
	static vector<bool> vertexInWay(VertexCount(), false);		//����� ������ ����� ���������� � ���� (true, ���� i-�� ������� �������� � ����)
	static bool found = false;									//���� ���� �� ������
	static size_t vertexCounter = 1;							//������� ���������� ������
	static vector<size_t> exludedVertex;						//������ ������ ����� ������� ����������� ���� ��������� ������
	size_t nextVertex;											//����� ������� ���� ������ ���
	static vector<bool> vertexCanWay(VertexCount(), false);		//����� ������ ����� ����� ������� �������� ������ ����

	vertexInWay[0] = true;

	//��� ��� �������
	for (size_t i = 0; i < road.size(); ++i)
		cout << (string)GetVertex(road[i]) << " ";
	cout << endl;

	if (start == finish)				//���� ������ 
	{
		for (size_t i = 0; i < vertexInWay.size(); ++i)
		{
			if (vertexInWay[i]) 
				vertexCanWay[i] = true;
		}
		if (wayLen > vertexCounter)		//� �� ������ ����������� ���������� ���� �� ����
		{
			wayLen = vertexCounter;
			found = true;
			resultChain.clear();
			for (size_t i = 0; i < wayLen; ++i)
				resultChain.push_back(GetVertex(road[i]));
		}
	}
	else
	{
		for (nextVertex = 0; nextVertex < VertexCount(); ++nextVertex)
		{
			int weight = 0;

			if (exludedVertex.end() != find(exludedVertex.begin(), exludedVertex.end(), nextVertex))	//���� ������ ������� ��� ��������� � ����� �� ������ ������� � �������� �������, �� ���������� �
				continue;

			if (start != nextVertex)
				weight = FindPath(start, nextVertex);

			if (weight && !vertexInWay[nextVertex])
			{
				if (wayLen != INT_MAX && road.size() == wayLen+1)	//������� �� ������� ����� ���� ������� ���� ������� ����������� ����������
					break;

				road.push_back(nextVertex);							//�������� ������� � ����
				vertexInWay[nextVertex] = true;						//�������� ��� ��������
				++vertexCounter;
				FindShortPath(nextVertex, finish, resultChain);		//������� ���� ��� ������ ��������� �����
				--vertexCounter;
				road.pop_back();
				vertexInWay[nextVertex] = false;
			}
		}

		if (/*start != finish*/!vertexCanWay[start])
			exludedVertex.push_back(start);
	}

	return found ? true : false;
}

//�������� ������� ���� �� ������� � �������� vertexFirst � ������� vertexSecond (0, ���� ���� ���, 1 ���� ����)
int WordChain::Graph::FindPath(const size_t vertexStart, const size_t vertexFinish)
{
	for (size_t i = GetVertex(vertexStart).EdgesCount(); i > 0; --i)
	{
		if (vertexFinish == GetVertex(vertexStart).GetEdge(i-1))
			return 1;
	}

	return 0;
}