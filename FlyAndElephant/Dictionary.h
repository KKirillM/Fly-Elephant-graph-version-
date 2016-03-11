#pragma once

class Dictionary
{
public:
	Dictionary(void);
	~Dictionary(void);

	//загрузка словаря из файла
	static int GetDictFromFile(const std::string& path, Dictionary& d);

	//добавление одного слова в словарь
	void AddWord(const std::string& word);

	//получить нужное слово по индексу
	const std::string& operator[](const size_t indx) const;

	//получить кол-во слов в словаре
	size_t GetCount() const;

	//очистить словарь
	void EraseDictionary();

private:
	std::vector<std::string> dict;
};

