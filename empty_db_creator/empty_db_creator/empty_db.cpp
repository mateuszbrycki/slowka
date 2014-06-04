#include "sqlite\sqlite3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void doQuery(sqlite3 * db, const char* query, char *zErrMsg)
{
	int open = sqlite3_exec(db, query, callback, 0, &zErrMsg);

	if (open != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		cout << "Zapytanie zostalo poprawnie wykonane!"<<endl;
	}
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

	query = "CREATE TABLE words("  \
		"ID INTEGER PRIMARY KEY		AUTOINCREMENT," \
		"FIRST			TEXT	NOT NULL," \
		"SECOND			TEXT	NOT NULL," \
		"THIRD			TEXT	NOT NULL," \
		"TRANSLATION	TEXT	NOT NULL," \
		"WRONG			INTEGER 	NOT NULL," \
		"GOOD			INTEGER 	NOT NULL," \
		"USER_ORDER		INTEGER		NOT NULL);";

	doQuery(db, query, zErrMsg);

	fstream file;
	file.open("slowka.txt");


	if (file.is_open())
	{
		while (!file.eof())
		{
			char split_char = '\t';
			string ch;

			getline(file, ch);
			istringstream split(ch); //rozdzielenie linii
			vector<string> tokens;

			for (string each; getline(split, each, split_char);)
			{
				tokens.push_back(each);

				//je¿eli dodano do tekenów 4 kolejne s³owa to dodajemy je do words
				if (tokens.size()% 4 == 0)
				{
					string instertQuery = "INSERT INTO words(FIRST, SECOND, THIRD, TRANSLATION, WRONG, GOOD, USER_ORDER)" \
						"VALUES ( " \
						" \' " + tokens[tokens.size() - 4] + "\', " \
						" \' " + tokens[tokens.size() - 3] + "\', " \
						" \' " + tokens[tokens.size() - 2] + "\', " \
						" \' " + tokens[tokens.size() - 1] + "\', " \
						" 0, 0, 0); ";

					doQuery(db, instertQuery.c_str(), zErrMsg);
					
				}
			};
		}
	}

	sqlite3_close(db);

	cin.get();
	cin.get();
}