#ifndef MODELWORDS_H
#define MODELWORDS_H
#include "sqlite/sqlite3.h"
#include <QString>
#include <QTreeWidgetItem>

class modelWords
{
public:
    modelWords(QString db);
    modelWords();
    ~modelWords();

    int* getNumberOfRows();
    QString* getWordByID(const int& id);
    void editWordByID(const int& id, QString newWord);
    void deleteWordByID(const int& id);
    void getAllWords(QList<QTreeWidgetItem *>* treeItems);
    void addWord(QString newWord);

private:
    sqlite3* db;

    static int setNumberOfRows(void *NotUsed, int argc, char **argv, char **azColName);
    static int setWord(void *NotUsed, int argc, char **argv, char **azColName);
    static int printAllWords(void *NotUsed, int argc, char **argv, char **azColName);

    void getSQLError(const int& open, const char* zErrMsg);
};

#endif // MODELWORDS_H
