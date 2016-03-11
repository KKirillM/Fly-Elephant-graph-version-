// FlyAndElephant.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Dictionary.h"
#include "WordChain.h"
#include "Test.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Dictionary d;
	WordChain w;
	int ret = 0;

	setlocale(LC_ALL, "Russian");

	try
	{
		if (argc < 3)
			throw exception("\nUsage: FlyAndElephant words_file dictionary_file\n\n"
							"\twords_file      - путь к файлу с исходными словами\n"
							"\tdictionary_file - путь к файлу со словарём\n\n"
							"Example: FlyAndElephant C:\\word.txt C:\\dictionary.txt\n\n");

		ret = WordChain::GetWordsFromFile(argv[1], w);
		if (ret)
			throw exception("Words file error");

		ret = Dictionary::GetDictFromFile(argv[2], d);
		if (ret)			
			throw exception("Dictionary file error");

		w.MakeChain(d);
		w.PrintChain();	
	}
	catch(exception& e)
	{
		cout << e.what() << endl;
		return 1;
 	}

	return 0;
}

