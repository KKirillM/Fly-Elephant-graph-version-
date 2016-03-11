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

//загрузка начального и конечного слов из файла
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

	//если в файле меньше было меньше двух слов или слова разной длины, то ошибка
	if (wordCounter < 2 || w.startWord.size() != w.endWord.size())
		return 1;

	return 0;
}

//запись начального и конечного слов
void WordChain::SetSourceWords(const string& w1, const string& w2)
{
	startWord = w1;
	endWord = w2;
}

//получить путь от исходного слова к конечному в векторе resultChain
void WordChain::MakeChain(Dictionary& d)
{
	Graph grph; 
	size_t wordLen = startWord.size();	
	size_t dictLen = d.GetCount();
	string dictWord;
	Word wrd("");
	bool found = false;

	//провер€ем исходные слова на равенство длины
	if (startWord.size() != endWord.size())
		throw exception("Source words have different length");

	//провер€ем словарь на наличие слов
	if (!d.GetCount())
		throw exception("Dictionary is empty");

	//строим граф на базе исходного словар€,
	//сперва добавл€ем вершины в граф (это слова, длина которых равна длине исходных слов)
	grph.AddVertex(startWord);
	for (; dictLen > 0; --dictLen)
	{
		dictWord = d[dictLen-1];

		if (dictWord.size() == wordLen && startWord != dictWord && endWord != dictWord)
			grph.AddVertex(dictWord);
	}
	grph.AddVertex(endWord);

	//далее добавл€ем рЄбра между вершинами графа
	const size_t graphLen = grph.VertexCount();
	for (size_t i = 0; i < graphLen ; ++i)
	{
		for (size_t j = 0; j < graphLen; ++j)
		{
			wrd.SetWord(grph.GetVertex(j));

			if (GetDiffCount(grph.GetVertex(i), wrd) == 1 && j != i)
			{
				grph.GetVertex(i).AddEdge(j);
			}
		}
	}									

	//ищем кратчайший путь
	found = grph.FindShortPath(0, grph.VertexCount()-1, resultChain);

	if (!found)
		throw exception("Not enough words in dictionary");
}

//вывести на консоль цепочку слов
void WordChain::PrintChain() const
{
	cout << endl << "Result word chain:" << endl;
	for (auto it = resultChain.begin(); it != resultChain.end(); ++it)
		cout << *it << endl;
}

//вспомогательна€ ф-ци€ дл€ подсчета кол-ва различающихс€ символов в двух словах
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
	static size_t wayLen = INT_MAX;								//длина пути
	static vector<size_t> road(1, start);						//номера вершин текущего пути, внесли первую вершину в маршрут
	static vector<bool> vertexInWay(VertexCount(), false);		//метки вершин графа включЄнных в путь (true, если i-а€ вершина включена в путь)
	static bool found = false;									//путь пока не найден
	static size_t vertexCounter = 1;							//счетчик пройденных вершин
	static vector<size_t> exludedVertex;						//номера вершин через которые необходимый путь проложить нельз€
	size_t nextVertex;											//номер вершины куда делаем шаг

	vertexInWay[0] = true;

	if (start == finish && wayLen > vertexCounter)				//путь найден и он короче предыдущего найденного если он есть
	{
		wayLen = vertexCounter;
		found = true;
		resultChain.clear();
		for (size_t i = 0; i < wayLen; ++i)
			resultChain.push_back(GetVertex(road[i]));
	}
	else
	{
		for (nextVertex = 0; nextVertex < VertexCount(); ++nextVertex)
		{
			int weight = 0;

			if (exludedVertex.end() != find(exludedVertex.begin(), exludedVertex.end(), nextVertex))	//если данную вершину уже проходили и через неЄ нельз€ попасть к финишной вершине, то пропускаем еЄ
				continue;

			if (start != nextVertex)
				weight = Find(start, nextVertex);

			if (weight && !vertexInWay[nextVertex])
			{
				if (wayLen != INT_MAX && road.size() == wayLen+1)	//выходим из данного цикла если текущий путь длиннее предыдущего найденного
					break;

				road.push_back(nextVertex);							//включить вершину в путь
				vertexInWay[nextVertex] = true;						//пометить как влючЄную
				++vertexCounter;
				FindShortPath(nextVertex, finish, resultChain);		//вызвать себ€ дл€ поиска следующей точки
				--vertexCounter;
				road.pop_back();
				vertexInWay[nextVertex] = false;
			}
		}

		exludedVertex.push_back(start);
	}

	return found ? true : false;
}

//проверка наличи€ пути из вершины с индексом vertexFirst в веришну vertexSecond (0, если пути нет, 1 если есть)
int WordChain::Graph::Find(const size_t vertexStart, const size_t vertexFinish)
{
	for (size_t i = GetVertex(vertexStart).EdgesCount(); i > 0; --i)
	{
		if (vertexFinish == GetVertex(vertexStart).GetEdge(i-1))
			return 1;
	}

	return 0;
}