#include "sqlite\sqlite3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Word
{
	string firstForm;
	string secondForm;
	string thirdForm;
	string translation;

	Word() {};
	Word(string first, string second, string third, string translation) :
		firstForm(first),
		secondForm(second),
		thirdForm(third),
		translation(translation)
	{}
};

int rows;
Word randomedWord;


//callback, funkcja, kóra zostanie wywo³ana po wykonaniu zapytania QUERY
void doQuery(sqlite3* db, const char* query, int(*callback)(void *NotUsed, int argc, char **argv, char **azColName))
{
	char *zErrMsg;

	int open = sqlite3_exec(db, query, callback, 0, &zErrMsg);

	if (open != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		cout << endl << "Zapytanie zostalo poprawnie wykonane!" << endl;
	}
}

//wywo³ywana po zapytaniu COUNT
static int setNumberOfRows(void *NotUsed, int argc, char **argv, char **azColName)
{
	rows = atoi(argv[0]);
	return 0;
}

//generuj¹ca zapytanie COUNT
void getNumberOfRows(sqlite3 *db)
{	
	string query = "SELECT Count(*) FROM words";

	doQuery(db, query.c_str(), setNumberOfRows);
}

static int setWord(void *NotUsed, int argc, char **argv, char **azColName)
{
	randomedWord.firstForm = argv[1];
	randomedWord.secondForm = argv[2];
	randomedWord.thirdForm = argv[3];
	randomedWord.translation = argv[4];

	return 0;
}

void getWordById(sqlite3* db, const int& id)
{
	string query = "SELECT * FROM words WHERE id = " + to_string(id) + ";";

	doQuery(db, query.c_str(), setWord);
}

int editWord(void *NotUsed, int argc, char **argv, char **azColName)
{
	cout << "Edytowano slowo! "<< argv[0] << endl;
	return 0;
}

void editWordById(sqlite3* db, const int& id, Word& newWord)
{
	string query = "UPDATE words SET "\
		"first = \'" + newWord.firstForm + "\'," \
		"second = \'" + newWord.secondForm + "\'," \
		"third = \'" + newWord.thirdForm + "\'," \
		"translation = \'" + newWord.translation + "\'" \
		" WHERE id = " + to_string(id) + ";";

	doQuery(db, query.c_str(), editWord);
}

int main()
{
	sqlite3 * db;
	int open = sqlite3_open("words.db", &db);
	char *zErrMsg = 0,
		*query;

	if (open)
	{
		cout << "Problem z otworzeniem bazy danych" << endl;
		cout << sqlite3_errmsg(db) << endl;
	}
	else {
		cout << "Otworzono baze!" << endl;
	}

	query = "SELECT Count(*) FROM words";

	getNumberOfRows(db);
	getWordById(db, 20);
	editWordById(db, 1, Word("arise", "arose", "arisen", "pojawiæ siê"));

	cout << endl << "Ilosc wierszy: " << rows << endl;
	cout << "Wylosowane slowo to: " << randomedWord.firstForm << endl;
	sqlite3_close(db);

	cin.get();
	cin.get();
}