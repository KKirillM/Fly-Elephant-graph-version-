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

	//��������� �������� ����� �� ��������� �����
	if (startWord.size() != endWord.size())
		throw exception("Source words have different length");

	//��������� ������� �� ������� ����
	if (!d.GetCount())
		throw exception("Dictionary is empty");

	//������ ���� �� ���� ��������� �������,
	//������ ��������� ������� � ���� (��� �����, ����� ������� ����� ����� �������� ����)
	//������������� ����� � ���� �� ����������
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

	const size_t startIndex = grph.FindVertex(startWord);
	const size_t endIndex = grph.FindVertex(endWord);

	/* * * * * * * * * * 
	 * ����� � ������� *
	 * * * * * * * * * */

	//vector<bool> vertexInWay(grph.VertexCount(), false);	//����� ������ ����� ���������� � ������� ���� (true, ���� i-�� ������� �������� � ����)
	//vector<bool> exludedVertex(grph.VertexCount(), false);	//������ ������ ����� ������� ����������� ���� ��������� ������

	//vertexInWay[startIndex] = true;
	//exludedVertex[startIndex] = true;

	//���� ���������� ����
	//grph.FindShortPath1(startIndex, endIndex, resultChain, vertexInWay, exludedVertex);

	/* * * * * * * * * * 
	 * ����� � ������  *
	 * * * * * * * * * */

	//���� ���������� ����
	grph.FindShortPath2(startIndex, endIndex, resultChain);

	if (!resultChain.size())
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

bool WordChain::Graph::FindShortPath1(const size_t start, const size_t finish, vector<string>& resultChain, vector<bool>& vertexInWay, vector<bool>& exludedVertex)
{
	static size_t wayLen = INT_MAX;								//����� ����
	static vector<size_t> road(1, start);						//������ ������ �������� ����, ������ ������ ������� � �������
	bool found = false;											//���� ���� �� ������
	static size_t vertexCounter = 1;							//������� ���������� ������
	size_t nextVertex;											//����� ������� ���� ������ ���

	//��� ��� �������
	/*for (size_t i = 0; i < road.size(); ++i)
		cout << (string)GetVertex(road[i]) << " ";
	cout << endl;*/

	if (start == finish)				//���� ������ 
	{
		found = true;
		if (wayLen > vertexCounter)		//� �� ������ ����������� ���������� ���� �� ����
		{
			wayLen = vertexCounter;
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

			if (exludedVertex[nextVertex])	//���� ������ ������� ��� ��������� � ����� �� ������ ������� � �������� �������, �� ���������� �
				continue;

			if (start != nextVertex)
				weight = FindPath(start, nextVertex);

			if (weight && !vertexInWay[nextVertex])
			{
				if (wayLen != INT_MAX && road.size() == wayLen+1)	//������� �� ������� ����� ���� ������� ���� ������� ����������� ����������
					break;

				road.push_back(nextVertex);							//�������� ������� � ����
				vertexInWay[nextVertex] = true;						//�������� ��� ��������
				++vertexCounter;									//��������� ���������� ������ � ������ ����
				if (FindShortPath1(nextVertex, finish, resultChain, vertexInWay, exludedVertex))		//������� ���� ��� ������ ��������� �����
					found = true;									//����� ������� nextVertex ����� ����� �� ����� ����
				--vertexCounter;
				road.pop_back();
				vertexInWay[nextVertex] = false;
			}
		}

		if (!found)
			exludedVertex[start] = true;							//����� ������ ������� �� ������ ������� � ����� ����
	}

	return found ? true : false;
}

void WordChain::Graph::FindShortPath2(const size_t start, const size_t finish, vector<string>& resultChain)
{
	queue<size_t> turn;														//������� �������� ������ ������
	vector<bool> used(VertexCount(), false);								//����� ���������� ������ ����� (true, ���� i-�� ������� ��������)
	vector<size_t> path(VertexCount(), 0);									//������ ��� ���������� �����
	vector<size_t> minPath;
	size_t nextVertex;

	turn.push(start);														//�������� ��������� ������� � �������
	path[start] = -1;

	while (turn.size())														//���� ������� �� �����
	{
		nextVertex = turn.front();	turn.pop();								//��������� �� ������� ��������� �������
		
		if (!used[nextVertex])
		{
			for (size_t i = GetVertex(nextVertex).EdgesCount(); i > 0; --i)	//������� � ������� ��� ������� � �������� ������� ���������
			{
				size_t currVertex = GetVertex(nextVertex).GetEdge(i-1);
				
				if (!used[currVertex])
				{
					turn.push(currVertex);
					path[currVertex] = nextVertex;
				}
			}
			
			used[nextVertex] = true;										//������� ����������

			if (nextVertex == finish)										//���������� �������� �������, ����� ����������� ����� �����
				break;
		}
	}

	if (!used[finish])														//�������� ������� �� ��������
		return;
	else																	//�������� ���������� ����
	{
		for (size_t i = finish; i != -1; i = path[i])
			minPath.push_back(i);

		reverse(minPath.begin(), minPath.end());

		for (size_t i = 0; i < minPath.size(); ++i)
			resultChain.push_back(GetVertex(minPath[i]));
	}
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