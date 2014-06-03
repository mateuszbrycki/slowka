#include "words.h"
#include <QDebug>

/*konstruktor
 @param filename nazwa pliku, w którym znajdują się słówka
*/
Words::Words(const QString filename) : filename(filename)
{
    this->getWords(); //pobranie słów
}

/* pobranie słówek z pliku */
void Words::getWords()
{
    QFile * file = new QFile(filename);
    if(!file->open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Uwaga", "Problem z otworzeniem pliku.");
        return;
    }

    QString line;
    QStringList lineList;
    QTextStream instream(file);

    while(!instream.atEnd())
    {
        line = instream.readLine();
        lineList = line.split(QRegExp("\t")); //pobranie linii i rozdzielenie pojedyczych slowek

        words.insert(Word(lineList[0], lineList[1], lineList[2], lineList[3]));
    }

    file->close();
}

/* zapisanie słówek do pliku */
void Words::saveWords()
{
    QFile * file = new QFile(filename);

    if(!file->open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Uwaga", "Problem z otworzeniem pliku.");
        return;
    }

    QTextStream outstream(file);

    for(std::multiset<Words::Word>::iterator iElement = words.begin(); iElement != words.end(); ++iElement )
    {
       outstream << iElement->firstForm << "\t" << iElement->secondForm << "\t" << iElement->thirdForm << "\t" << iElement->translation << "\n";
    }

    file->close();
}

/* wylosowanie słówka  */
std::multiset<Words::Word>::iterator Words::randomWord()
{
    int randomWord = qrand() % ((words.size() + 1) - 1) + 1; //losowanie slowa

    int i = 1;
    std::multiset<Word>::iterator random = words.begin();

    while(i != randomWord)
    {
        random++; //przesuwanie iteratora
        i++;
    }

    return random;
}

/* losowanie pola formularza */
int Words::randomForm()
{
    return qrand() % ((4 + 1) - 1) + 1;
}

/* losowanie słowa jak i formy */
void Words::randomAll()
{
    if(words.size() > 0)
    {
        randomed = randomWord();
        randomedForm = randomForm();
    }
}

/* edycja słowa z listy
@param number numer słówka
@param form edytowana forma słówka
@param newValue nowa wartość pola form
*/
 void Words::editWord(int number, int form, QString newValue)
 {
     std::multiset<Words::Word>::iterator changed = words.begin();
     int i = 0;
    //ustawienie itaratora na odpowiednim elemencie listy
     while(number != i+1)
     {
         ++changed;
         ++i;
     }
    //w zależności od zmienianej formy dodajemy nowe słówko
    switch(form)
     {
     case 1:
         words.insert(Word(newValue, changed->secondForm, changed->thirdForm, changed->translation));
         break;

     case 2:
         words.insert(Word(changed->firstForm, newValue, changed->thirdForm, changed->translation));
         break;

     case 3:
         words.insert(Word(changed->firstForm, changed->secondForm, newValue, changed->translation));
         break;

     case 4:
         words.insert(Word(changed->firstForm, changed->secondForm, changed->thirdForm, newValue));
         break;
     }

     words.erase(changed); //z listy usuwamy stare słówko
 }

 /* usunięcie słówka
@param number numer usuwanego słówka
*/
 void Words::deleteWord(int number)
 {
     //ustawienie iteratora na odpowiednim słowie w liście
    std::multiset<Words::Word>::iterator deleted = words.begin();
    int i = 0;

    while(number != i+1)
    {
        ++deleted;
        ++i;
    }

    //usunięcie wybranego słowa z listy
    words.erase(deleted);
 }

 Words::~Words()
 {}



