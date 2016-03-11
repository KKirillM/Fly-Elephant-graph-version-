#include "stdafx.h"
#include "Test.h"
#include "Dictionary.h"
#include "WordChain.h"

using namespace std;


Test::Test(void)
{
}

Test::~Test(void)
{
}

//вывод названия и результатов коткретного теста
void Test::PrintTest(const string& testName, Dictionary& d, WordChain& w)
{
	cout << "Имя теста: " << testName << endl;

	try
	{
		w.MakeChain(d);
	}
	catch (exception& e)
	{
		cout << "Результат теста: " << e.what() << endl << endl;
		return;
	}

	w.PrintChain();	
	cout << "Тест пройден!" << endl << endl;
}

//выполнить набор тестов
void Test::MakeTests()
{
	Dictionary d;
	WordChain c;

	c.SetSourceWords("КОТ", "ТОН");
	PrintTest("пустой словарь", d, c);

	c.SetSourceWords("ОДИН", "ОДИНОКО");
	d.EraseDictionary();
	d.AddWord("ОДИН");
	d.AddWord("ДВА");
	PrintTest("исходные слова различной длины", d, c);

	c.SetSourceWords("УРА", "ОКА");
	d.EraseDictionary();
	d.AddWord("УРОК");
	d.AddWord("ИРА");
	d.AddWord("ОНА");
	d.AddWord("ИГРОК");
	PrintTest("недостаточно слов в словаре", d, c);

	c.SetSourceWords("КОТ", "ТОН");
	d.EraseDictionary();
	d.AddWord("КОТ");
	d.AddWord("ТОН");
	d.AddWord("НОТА");
	d.AddWord("КОТЫ");
	d.AddWord("РОТ");
	d.AddWord("РОТА");
	d.AddWord("ТОТ");
	PrintTest("правильные исходные данные", d, c);	
}
