#pragma once

class Dictionary;

class WordChain
{
public:
	WordChain(void);
	~WordChain(void);

	//�������� ���������� � ��������� ���� �� �����
	static int GetWordsFromFile(const std::string& path, WordChain& w);

	//������ ���������� � ��������� ����
	void SetSourceWords(const std::string& w1, const std::string& w2);

	//�������� ���� �� ��������� ����� � ��������� � ������� resultChain, ��������� ���������� �������
	void MakeChain(Dictionary& d);

	//������� �� ������� ������� ����
	void PrintChain() const;

private:
	//��������������� �-��� ��� �������� ���-�� ������������� �������� � ���� ������
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

		//���������� �������� �����
		void SetWord(const std::string& str) { word = str; }

		//�������� ����� ��� ������� �����
		void AddEdge(int x) { edges.push_back(x); }

		//�������� ������ �����
		const size_t GetEdge(const size_t i) const { return edges.at(i); }

		//���-�� ���� ����������� ������ �����
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

		//���������� ������� � ����
		void AddVertex(const Word& w) { graph.push_back(w); }

		//�������� �������� ������ �������
		Word& GetVertex(const size_t i) { return graph[i]; }

		//�������� ���-�� ������ �����
		const size_t VertexCount() const { return graph.size(); }

		//����������� ����� ����������� ���� � ����� ����� ����� ���������
		bool FindShortPath(const size_t start,							//������ ��������� �������
							const size_t finish,						//������ �������� �������
							std::vector<std::string>& resultChain);		//�������������� ������� ����

	private:

		//�������� ������� ���� �� ������� � �������� vertexStart � ������� vertexFinish (0, ���� ���� ���, 1 ���� ����)
		int Find(const size_t vertexStart, const size_t vertexFinish);

	private:
		std::vector<Word> graph;
	};
};

