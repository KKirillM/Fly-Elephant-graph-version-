#include "stdafx.h"
#include "Dictionary.h"

using namespace std;


Dictionary::Dictionary(void)
{
}


Dictionary::~Dictionary(void)
{
}


//загрузка словаря из файла
int Dictionary::GetDictFromFile(const string& path, Dictionary& d)
{
	ifstream f(path);
	string str;

	if (!f.is_open())
		return 1;

	while (!f.eof())
	{
		getline(f, str);
		d.AddWord(str);
	}

	return 0;
}

//добавление одного слова в словарь
void Dictionary::AddWord(const string& word)
{
	dict.push_back(word);
}

//получить нужное слово по индексу
const string& Dictionary::operator[](const size_t indx) const
{
	return dict.at(indx);
}

//получить кол-во слов в словаре
size_t Dictionary::GetCount() const
{
	return dict.size();
}

//очистить словарь
void Dictionary::EraseDictionary()
{
	dict.clear();
}