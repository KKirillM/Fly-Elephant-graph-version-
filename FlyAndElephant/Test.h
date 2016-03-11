#pragma once

class Dictionary;
class WordChain;

class Test
{
public:
	Test();
	~Test(void);

	//вывод названия и результатов коткретного теста
	void PrintTest(const std::string& testName, Dictionary& d, WordChain& w);

	//выполнить набор тестов
	void MakeTests();
};

