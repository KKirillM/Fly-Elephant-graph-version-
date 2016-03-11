#pragma once

class Dictionary;

class WordChain
{
public:
	WordChain(void);
	~WordChain(void);

	//загрузка начального и конечного слов из файла
	static int GetWordsFromFile(const std::string& path, WordChain& w);

	//запись начального и конечного слов
	void SetSourceWords(const std::string& w1, const std::string& w2);

	//получить путь от исходного слова к конечному в векторе resultChain, используя полученный словарь
	void MakeChain(Dictionary& d);

	//вывести на консоль цепочку слов
	void PrintChain() const;

private:
	//вспомогательная ф-ция для подсчета кол-ва различающихся символов в двух словах
	size_t GetDiffCount(const std::string& w1, const std::string& w2);

private:
	std::string startWord;
	std::string endWord;
	std::vector<std::string> resultChain;

private:
	class Word 
	{
	public:
		Word(std::string str) : word (str) {}
		~Word() {}

		//установить значение слова
		void SetWord(const std::string& str) { word = str; }

		//добавить ребро для данного слова
		void AddEdge(int x) { edges.push_back(x); }

		//получить нужное ребро
		const size_t GetEdge(const size_t i) const { return edges.at(i); }

		//кол-во рёбер соединяющих данное слово
		const size_t EdgesCount() { return edges.size(); }

		operator std::string() const { return word; }

	private:
		std::string word;
		std::vector<size_t> edges;
	};

	class Graph
	{
	public:
		Graph() {}
		~Graph() { graph.clear(); }

		//добавление вершины в граф
		void AddVertex(const Word& w) { graph.push_back(w); }

		//получить значение нужной вершины
		Word& GetVertex(const size_t i) { return graph[i]; }

		//получить кол-во вершин графа
		const size_t VertexCount() const { return graph.size(); }

		//рекурсивный поиск кратчайшего пути в графе между двумя вершинами
		bool FindShortPath(const size_t start,							//индекс начальной вершины
							const size_t finish,						//индекс конечной вершины
							std::vector<std::string>& resultChain);		//результирующая цепочка слов

	private:

		//проверка наличия пути из вершины с индексом vertexStart в веришну vertexFinish (0, если пути нет, 1 если есть)
		int Find(const size_t vertexStart, const size_t vertexFinish);

	private:
		std::vector<Word> graph;
	};
};

