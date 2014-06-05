#include "modelwords.h"
#include <QDebug>
#include <QStringList>
#include <QString>

modelWords::modelWords()
{}

modelWords::modelWords(QString filename)
{
    int open = sqlite3_open(filename.toStdString().c_str(), &db);
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


/* funkcja odpowiedzialna za zliczanie ilości rekordów w bazie danyc */
int* modelWords::getNumberOfRows()
{
   QString query = "SELECT Count(*) FROM words;";

   char *zErrMsg;
   int * numberOfRows = new int();

   int open = sqlite3_exec(this->db, query.toStdString().c_str(), setNumberOfRows, (void*)(numberOfRows), &zErrMsg);
   getSQLError(open, zErrMsg);

   return numberOfRows;
}

/* funkcja generująca zapytanie do bazy danych - pobranie elementu
@param id id rekordu, ktory chcemy pobrac
*/
QString* modelWords::getWordByID(const int& id)
{
    QString query = "SELECT * FROM words WHERE id = " + QString::number(id) + ";";
    QString* newWord = new QString();
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
void modelWords::editWordByID(const int& id, QString newWord)
{
    QStringList newWordList = newWord.split(QRegExp("\\s+")); //rozbicie formy słowa
    QString query = "UPDATE words SET "\
        "first = \'" + newWordList[1] + "\', " \
        "second = \'" + newWordList[2] + "\', " \
        "third = \'" + newWordList[3] + "\', " \
        "translation = \'" + newWordList[4]+ "\', " \
        "wrong = " + newWordList[5] + ", "\
        "good = " + newWordList[6] + ", "\
        "user_order = " + newWordList[7] + ""\
        " WHERE id = " + QString::number(id) + ";";
   qDebug()<<query;
   char *zErrMsg;

   int open = sqlite3_exec(this->db, query.toStdString().c_str(), NULL, 0, &zErrMsg);
   getSQLError(open, zErrMsg);
}

/* funkcja odpowiedzialna za wygenerowanie zapytania do bazy danych - usuniecie rekordu
@param id id usuwanego rekordu
*/
void modelWords::deleteWordByID(const int& id)
{
    QString query = "DELETE FROM words WHERE id = " + QString::number(id) + ";";

    char *zErrMsg;
    int open = sqlite3_exec(this->db, query.toStdString().c_str(), NULL, 0, &zErrMsg);
    getSQLError(open, zErrMsg);

}

/*dodanie rekordu do bazy danych
@param newWord wartosc nowego rekordu
*/
void modelWords::addWord(QString newWord)
{
    QStringList newWordList = newWord.split(QRegExp("\\s+")); //rozbicie formy słowa

   QString query = "INSERT INTO words (first, second, third, translation, wrong, good, user_order) "\
        "VALUES(\'" + newWordList[0] + "\'," \
        "\'" + newWordList[1] + "\'," \
        "\'" + newWordList[2] + "\'," \
        "\'" + newWordList[3]+ "\'," \
        "0, 0, 0);";

   char *zErrMsg;
   int open = sqlite3_exec(this->db, query.toStdString().c_str(), NULL, 0, &zErrMsg);
   getSQLError(open, zErrMsg);
}

 /* funkcja wywołująca generowanie całego drzewa z rekordami z tabeli
@param treeItems lista elementów drzewa
*/
void modelWords::getAllWords(QList<QTreeWidgetItem *>* treeItems)
{
   QString query = "SELECT * FROM words;";
   char* zErrMsg;

   int open = sqlite3_exec(this->db, query.toStdString().c_str(), printAllWords, (void*)treeItems, &zErrMsg);
   getSQLError(open, zErrMsg);

}

/*funkcja 'callback' dla zliczania ilości rekordów*/
int modelWords::setNumberOfRows(void *NotUsed, int argc, char **argv, char **azColName)
 {
     int* numberOfRows = static_cast<int*>(NotUsed);
     *numberOfRows = atoi(argv[0]);
     return 0;
 }

/* funkcja 'callback' dla zapytania o pobranie rekodu z DB*/
int modelWords::setWord(void *NotUsed, int argc, char **argv, char **azColName)
{
    QString* newWord = reinterpret_cast<QString*>(NotUsed);
    for(int x = 0; x < argc; x++)
    {
        *newWord += QString(argv[x]) + " ";
    }

    return 0;
}

/* funkcja 'callback' dla zapytania o wszystkie rekody z DB*/
int modelWords::printAllWords(void *NotUsed, int argc, char **argv, char **azColName)
{

    QList<QTreeWidgetItem *>* treeItems = static_cast<QList<QTreeWidgetItem *>*>(NotUsed);

    treeItems->push_back(new QTreeWidgetItem()); //dodanie nowego elementu na koniec listy

    QTreeWidgetItem * tmp = treeItems->back(); //bierzący ostatni element listy => ostatnie dodane słówko, ustawiamy je w kolumnach

    tmp->setText(0, QString(argv[0]).remove(" "));
    tmp->setText(1, QString(argv[1]).remove(" "));
    tmp->setText(2, QString(argv[2]).remove(" "));
    tmp->setText(3, QString(argv[3]).remove(" "));
    tmp->setText(4, QString(argv[4]));
    tmp->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);

    return 0;
}

void modelWords::getSQLError(const int& open, const char* zErrMsg)
{
   if (open != SQLITE_OK){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(&zErrMsg);
   }
}

modelWords::~modelWords()
{
    sqlite3_close(this->db);
}
