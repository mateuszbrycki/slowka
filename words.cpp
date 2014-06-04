#include "words.h"
#include <QDebug>
#include <QMessageBox>
#include <string>
#include "sqlite/sqlite3.h"



/*konstruktor
 @param filename nazwa pliku, w którym znajdują się słówka
*/
Words::Words(const QString filename) : filename(filename)
{
    int open = sqlite3_open("words.db", &db);
    char *zErrMsg = 0;

    if (open)
    {
        qDebug() << "Problem z otworzeniem bazy danych" << endl;
        qDebug() << sqlite3_errmsg(db) << endl;
    }
    else {
        qDebug() << "Otworzono baze!" << endl;
    }
}

/* wylosowanie słówka  */
Words::Word* Words::randomWord()
{
    int max = getNumberOfRows();
    int randomWord = qrand() % ((max + 1) - 1) + 1; //losowanie slowa
    qDebug()<<randomWord;

    Word* newWord = this->getWordByID(randomWord);
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
     Word * changed = this->getWordByID(number);

     switch(form)
     {
     case 1:
         this->editWordByID(number, Word(newValue, changed->secondForm, changed->thirdForm, changed->translation));
         break;
     case 2:
         this->editWordByID(number, Word(changed->firstForm, newValue, changed->thirdForm, changed->translation));
         break;
     case 3:
         this->editWordByID(number, Word(changed->firstForm, changed->secondForm, newValue, changed->translation));
         break;
     case 4:
         this->editWordByID(number, Word(changed->firstForm, changed->secondForm, changed->thirdForm, newValue));
         break;
     }
 }

 /* usunięcie słówka
@param number numer usuwanego słówka
*/
 void Words::deleteWord(int number)
 {
    this->deleteWordByID(number);
 }

 /* funkcja odpowiedzialna za zliczanie ilości rekordów w bazie danyc */
 int Words::getNumberOfRows()
 {
    QString query = "SELECT Count(*) FROM words;";

    char *zErrMsg;

    int open = sqlite3_exec(this->db, query.toStdString().c_str(), setNumberOfRows, (void*)(this), &zErrMsg);
    getSQLError(open, zErrMsg);

    return this->numberOfRows;
 }

/* funkcja generująca zapytanie do bazy danych - pobranie elementu
@param id id rekordu, ktory chcemy pobrac
*/
 Words::Word* Words::getWordByID(const int& id)
 {
     QString query = "SELECT * FROM words WHERE id = " + QString::number(id) + ";";
     Word* newWord = new Word();
     //doQuery(this->db, query.toStdString().c_str(), setWord, newWord);

     char *zErrMsg;

     int open = sqlite3_exec(this->db, query.toStdString().c_str(), setWord, (void*) newWord, &zErrMsg);
     getSQLError(open, zErrMsg);

     return newWord;
 }

 /*funkcja generująca zapytanie do bazy danych - edycja rekordu
  @param id id rekordu, ktory bedzie edytowany
  @param newWord nowa wartosc
   */
 void Words::editWordByID(const int& id, Words::Word newWord)
 {
     QString query = "UPDATE words SET "\
         "first = \'" + newWord.firstForm + "\'," \
         "second = \'" + newWord.secondForm + "\'," \
         "third = \'" + newWord.thirdForm + "\'," \
         "translation = \'" + newWord.translation + "\'" \
         " WHERE id = " + QString::number(id) + ";";
    char *zErrMsg;

    int open = sqlite3_exec(this->db, query.toStdString().c_str(), editWord, 0, &zErrMsg);
    getSQLError(open, zErrMsg);
 }

 /* funkcja odpowiedzialna za wygenerowanie zapytania do bazy danych - usuniecie rekordu
 @param id id usuwanego rekordu
*/
 void Words::deleteWordByID(const int& id)
 {
     QString query = "DELETE FROM words WHERE id = " + QString::number(id) + ";";

     char *zErrMsg;
     int open = sqlite3_exec(this->db, query.toStdString().c_str(), deleteWord, 0, &zErrMsg);
     getSQLError(open, zErrMsg);

 }

/*dodanie rekordu do bazy danych
@param newWord wartosc nowego rekordu
*/
 void Words::addWord(Words::Word newWord)
 {
     QString query = "INSERT INTO words (first, second, third, translation, wrong, good, user_order) "  \
         "VALUES(\'" + newWord.firstForm + "\'" \
             ", \'" + newWord.secondForm  + "\'" \
             ", \'" + newWord.thirdForm + "\'" \
             ", \'" + newWord.translation + "\'" \
         ", 0, 0, 0); ";
    char *zErrMsg;
    int open = sqlite3_exec(this->db, query.toStdString().c_str(), addWord, 0, &zErrMsg);
    getSQLError(open, zErrMsg);
 }

  /* funkcja wywołująca generowanie całego drzewa z rekordami z tabeli
@param treeItems lista elementów drzewa
*/
 void Words::getAllWords(QList<QTreeWidgetItem *>* treeItems)
 {
    QString query = "SELECT * FROM words;";
    char* zErrMsg;

    int open = sqlite3_exec(this->db, query.toStdString().c_str(), printAllWords, (void*)treeItems, &zErrMsg);
    getSQLError(open, zErrMsg);

 }

 /*funkcja 'callback' dla zliczania ilości rekordów*/
 int Words::setNumberOfRows(void *NotUsed, int argc, char **argv, char **azColName)
  {
      Words* numberOfRows = static_cast<Words*>(NotUsed);
      numberOfRows->numberOfRows = atoi(argv[0]);
      return 0;
  }

 /* funkcja 'callback' dla zapytania o pobranie rekodu z DB*/
int Words::setWord(void *NotUsed, int argc, char **argv, char **azColName)
 {
     Word* newWord = reinterpret_cast<Word*>(NotUsed);
     newWord->firstForm = QString(argv[1]).remove(" ");
     newWord->secondForm = QString(argv[2]).remove(" ");
     newWord->thirdForm = QString(argv[3]).remove(" ");
     newWord->translation = QString(argv[4]);

     return 0;
 }

 /* funkcja 'callback' dla zapytania o edycje z DB*/
int Words::editWord(void *NotUsed, int argc, char **argv, char **azColName)
{
    qDebug() << "Edytowano slowo! "<< argv[0] << endl;
    return 0;
}

 /* funkcja 'callback' dla zapytania o usuniecie rekodu z DB*/
int Words::deleteWord(void *NotUsed, int argc, char **argv, char **azColName)
{
    qDebug() << "Usunięto słowo! " << endl;
    return 0;
}

 /* funkcja 'callback' dla zapytania o dodanie rekodu do DB*/
int Words::addWord(void *NotUsed, int argc, char **argv, char **azColName)
{
    qDebug() << "Dodano słowo! " << endl;
    return 0;
}

 /* funkcja 'callback' dla zapytania o wszystkie rekody z DB*/
int Words::printAllWords(void *NotUsed, int argc, char **argv, char **azColName)
{

  QList<QTreeWidgetItem *>* treeItems = static_cast<QList<QTreeWidgetItem *>*>(NotUsed);

   treeItems->push_back(new QTreeWidgetItem()); //dodanie nowego elementu na koniec listy

   QTreeWidgetItem * tmp = treeItems->back(); //bierzący ostatni element listy => ostatnie dodane słówko, ustawiamy je w kolumnach

   tmp->setText(0, argv[0]);
   tmp->setText(1, argv[1]);
   tmp->setText(2, argv[2]);
   tmp->setText(3, argv[3]);
   tmp->setText(4, argv[4]);
   tmp->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);

   return 0;
}

void Words::getSQLError(const int& open, const char* zErrMsg)
{
    if (open != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(&zErrMsg);
    }
}

 Words::~Words()
 {
     sqlite3_close(this->db);
 }



