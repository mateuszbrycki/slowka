#include "words.h"
#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include <string>
#include "sqlite/sqlite3.h"



/*konstruktor
 @param filename nazwa pliku, w którym znajdują się słówka
*/
Words::Words(const QString filename)
{
    _model = new modelWords(filename);
}

/* wylosowanie słówka  */
Words::Word* Words::randomWord()
{
    int max = (*_model->getNumberOfRows());
    int randomWord = qrand() % ((max + 1) - 1) + 1; //losowanie slowa

    QStringList newWordList = (*_model->getWordByID(randomWord)).split(QRegExp("\\$"));

    Word* newWord = new Word(newWordList[1].remove(" "), newWordList[2].remove(" "), newWordList[3].remove(" "), newWordList[4].remove(" "));

    return newWord;
}

/* losowanie pola formularza */
int Words::randomForm()
{
    return qrand() % ((4 + 1) - 1) + 1;
}

/* losowanie słowa jak i formy */
void Words::randomAll()
{
    randomed = randomWord();
    randomedForm = randomForm();
}

/* edycja słowa z listy
@param number numer słówka
@param form edytowana forma słówka
@param newValue nowa wartość pola form
*/
 void Words::editWord(int number, int form, QString newValue)
 {
     //w zależności od zmienianej formy dodajemy nowe słówko
     //QString * changed = _model->getWordByID(number);

     QStringList changedList = (*_model->getWordByID(number)).split(QRegExp("\\$"));
     QString newWord;
     for(int x = 0; x < changedList.size(); x++)
     {
         if(x != form)
         {
             newWord += changedList[x] + "$";
         } else {
             newWord += newValue + "$";
         }
     }

     _model->editWordByID(number, newWord);
 }

 void Words::addWord(Word newWord)
 {
     QString newValue = newWord.firstForm + "$" + newWord.secondForm + "$" + newWord.thirdForm + "$" + newWord.translation;

     _model->addWord(newValue);
 }

 void Words::getAllWords(QList<QTreeWidgetItem *>* treeItems)
 {
     _model->getAllWords(treeItems);
 }

 /* usunięcie słówka
@param number numer usuwanego słówka
*/
 void Words::deleteWord(int number)
 {
    _model->deleteWordByID(number);
 }


 Words::~Words()
 {
 }



