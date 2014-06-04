#include "sqlite\sqlite3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

#define TABLE_NAME "words"; //nazwa tabeli w bazie danych
#define FILE_NAME "words.db"; //nazwa pliku z baz¹ danych

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
}

//wywo³ywana po zapytaniu COUNT
static int setNumberOfRows(void *NotUsed, int argc, char **argv, char **azColName)
{
	rows = atoi(argv[0]);
	return 0;
}

//generuj¹ca zapytanie COUNT
void getNumberOfRows(sqlite3 *db, int(*callback)(void *NotUsed, int argc, char **argv, char **azColName))
{	
	string query = "SELECT Count(*) FROM words;";

	doQuery(db, query.c_str(), callback);
}

static int setWord(void *NotUsed, int argc, char **argv, char **azColName)
{
	randomedWord.firstForm = argv[1];
	randomedWord.secondForm = argv[2];
	randomedWord.thirdForm = argv[3];
	randomedWord.translation = argv[4];

	return 0;
}

void getWordById(sqlite3* db, const int& id, int(*callback)(void *NotUsed, int argc, char **argv, char **azColName))
{
	string query = "SELECT * FROM words WHERE id = " + to_string(id) + ";";

	doQuery(db, query.c_str(), callback);
}

int editWord(void *NotUsed, int argc, char **argv, char **azColName)
{
	cout << "Edytowano slowo! "<< argv[0] << endl;
	return 0;
}

void editWordById(sqlite3* db, const int& id, Word& newWord, int(*callback)(void *NotUsed, int argc, char **argv, char **azColName))
{
	string query = "UPDATE words SET "\
		"first = \'" + newWord.firstForm + "\'," \
		"second = \'" + newWord.secondForm + "\'," \
		"third = \'" + newWord.thirdForm + "\'," \
		"translation = \'" + newWord.translation + "\'" \
		" WHERE id = " + to_string(id) + ";";

	doQuery(db, query.c_str(), callback);
}

int deleteWord(void *NotUsed, int argc, char **argv, char **azColName)
{
	cout << "Usuniêto s³owo! " << endl;
	return 0;
}

void deleteWordById(sqlite3* db, const int& id, int(*callback)(void *NotUsed, int argc, char **argv, char **azColName))
{
	string query = "DELETE FROM words WHERE id = " + to_string(id) + ";";

	doQuery(db, query.c_str(), callback);
}

int addWord(void *NotUsed, int argc, char **argv, char **azColName)
{
	cout << "Dodano s³owo! " << endl;
	return 0;
}

void addWord(sqlite3* db, Word& newWord, int(*callback)(void *NotUsed, int argc, char **argv, char **azColName))
{
	string query = "INSERT INTO words (first, second, third, translation, wrong, good, user_order) "  \
		"VALUES(\'" + newWord.firstForm + "\'" \
			", \'" + newWord.secondForm  + "\'" \
			", \'" + newWord.thirdForm + "\'" \
			", \'" + newWord.translation + "\'" \
		", 0, 0, 0); ";

	doQuery(db, query.c_str(), callback);
}

int printAllWords(void *NotUsed, int argc, char **argv, char **azColName)
{
	cout << argv[1] << "\t" << argv[2] << "\t" << argv[3] << "\t" << argv[4] << endl;
	return 0;
}


void getAllWords(sqlite3* db, int(*callback)(void *NotUsed, int argc, char **argv, char **azColName))
{
	string query = "SELECT * FROM words;";

	doQuery(db, query.c_str(), callback);
}



int main()
{
	sqlite3 * db;
	int open = sqlite3_open("words.db", &db);
	char *zErrMsg = 0;

	if (open)
	{
		cout << "Problem z otworzeniem bazy danych" << endl;
		cout << sqlite3_errmsg(db) << endl;
	}
	else {
		cout << "Otworzono baze!" << endl;
	}

	getNumberOfRows(db, setNumberOfRows); //iloœæ rekordów w bazie
	getWordById(db, 20, setWord); //wybranie s³ówka nr 20
	getAllWords(db, printAllWords);
	editWordById(db, 1, Word("arise", "arose", "arisen", "pojawiæ siê"), editWord); //edycja s³ówka o id = 1
	//addWord(db, Word("be", "was/were", "been", "byæ"), addWord);
	//deleteWordById(db, 144, deleteWord);



	cout << endl << "Ilosc wierszy: " << rows << endl;
	cout << "Wylosowane slowo to: " << randomedWord.firstForm << endl;

	sqlite3_close(db);

	cin.get();
	cin.get();
}