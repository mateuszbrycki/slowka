#ifndef WORDS_H
#define WORDS_H
#include <QString>
#include <QFile>
#include <set>
#include <QTextStream>
#include <QTreeWidgetItem>
#include "sqlite/sqlite3.h"
#include "models/modelwords.h"

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

    Word* randomed; //wylosowane słówko do wyświetlenia
    int randomedForm; //wylosowana forma słówka, która zostanie wypisana do formularza na początku
    //QString filename; //nazwa pliku zawierającego słówka
    //sqlite3* db;
    //int numberOfRows;

    Words();
    Words(QString filname);
    ~Words();

    void randomAll(); //wylosowanie słowa oraz formy
    void editWord(int number, int form, QString newValue); //edycja słówka
    void saveWords(); //zapis słówek
    void deleteWord(int number); //usunięcie słówka o danym numerze
    void getAllWords(QList<QTreeWidgetItem *>* treeItems);
    void addWord(Word newWord);

private:
    modelWords* _model;
    Word* randomWord(); //losowanie słowa z wszystkich dostępnych
    int randomForm(); //losowanie pola formularza do wypełnienia
};

#endif // WORDS_H
