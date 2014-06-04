#ifndef WORDS_H
#define WORDS_H
#include <QString>
#include <QFile>
#include <set>
#include <QTextStream>
#include <QTreeWidgetItem>
#include "sqlite/sqlite3.h"

class Words
{
public:

    //struktura będąca reprezentacją jednego czasownika nieregularnego
    struct Word
    {
        QString firstForm;
        QString secondForm;
        QString thirdForm;
        QString translation;
        Word()
        {
            firstForm = "";
            secondForm = "";
            thirdForm = "";
            translation = "";
        }

        //konstruktor
        Word(QString first, QString second, QString third, QString translation)
                : firstForm(first),
                  secondForm(second),
                  thirdForm(third),
                  translation(translation)
        {}

        bool operator<(const Words::Word& compareTo) const
        {
            return (this->firstForm < compareTo.firstForm);
        }

        bool operator=(const Word& newWord)
        {
            firstForm = newWord.firstForm;
            secondForm = newWord.secondForm;
            thirdForm = newWord.thirdForm;
            translation = newWord.translation;
        }
    };

    //std::multiset<Word> words; //wszystkie słowa pobrane z pliku
    Word* randomed; //wylosowane słówko do wyświetlenia
    int randomedForm; //wylosowana forma słówka, która zostanie wypisana do formularza na początku
    QString filename; //nazwa pliku zawierającego słówka
    sqlite3* db;
    int numberOfRows;

    Words(const QString filename);
    ~Words();

    void randomAll(); //wylosowanie słowa oraz formy
    void editWord(int number, int form, QString newValue); //edycja słówka
    void saveWords(); //zapis słówek
    void deleteWord(int number); //usunięcie słówka o danym numerze
    void getAllWords(QList<QTreeWidgetItem *>* treeItems);
    void addWord(Word newWord);

private:
    Word* randomWord(); //losowanie słowa z wszystkich dostępnych
    int randomForm(); //losowanie pola formularza do wypełnienia

    sqlite3* connectDB();
    void doQuery(sqlite3* db, const char* query, int(*callback)(void *NotUsed, int argc, char **argv, char **azColName));
    void doQuery(sqlite3* db, const char* query, int(*callback)(void *NotUsed, int argc, char **argv, char **azColName), void* arg = 0);

    int getNumberOfRows();
    Word* getWordByID(const int& id);
    void editWordByID(const int& id, Words::Word newWord);
    void deleteWordByID(const int& id);

    static int setNumberOfRows(void *NotUsed, int argc, char **argv, char **azColName);
    static int setWord(void *NotUsed, int argc, char **argv, char **azColName);
    static int editWord(void *NotUsed, int argc, char **argv, char **azColName);
    static int deleteWord(void *NotUsed, int argc, char **argv, char **azColName);
    static int addWord(void *NotUsed, int argc, char **argv, char **azColName);
    static int printAllWords(void *NotUsed, int argc, char **argv, char **azColName);

    void getSQLError(const int& open, const char* zErrMsg);
};

#endif // WORDS_H
