#include "managewordlist.h"
#include "ui_managewordlist.h"
#include <QDebug>
#include "noEditDelegate.h"
#include "addnew.h"

manageWordList::manageWordList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::manageWordList)
{
    ui->setupUi(this);
}

manageWordList::manageWordList(QWidget *parent, Words * words) :
    QDialog(parent),
    ui(new Ui::manageWordList),
    words(words)
{
    ui->setupUi(this);
    this->setWindowTitle("Zarządzanie listą słówek");

    this->paintTree(); //generowanie drzewa

    newWord = new addNew(this, words);

    ui->treeWidget->header()->resizeSection(0, 50); //ustawienie rozmiarów kolumn
    ui->treeWidget->header()->resizeSection(1, 120);
    ui->treeWidget->header()->resizeSection(2, 140);

    bool connectOk;

    connectOk = connect(newWord, SIGNAL(added()), this, SLOT(paintTree())); //sygnał będzie emitowany po dodaniu nowego słówka
    Q_ASSERT(connectOk);

    connectOk = connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(changeItem(QTreeWidgetItem* ,int))); //zmiana słowa
    Q_ASSERT(connectOk);

    connectOk = connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem* ,int)), this, SLOT(setLastClickedItem(QTreeWidgetItem *)));
    Q_ASSERT(connectOk);

}
 /* akcja zmiany słowa */
void manageWordList::changeItem(QTreeWidgetItem* item, int column)
{
    //pobranie zmienionego elementu
    QString text= item->text(column);

    //funkcja zmieniajaca slowo na postawie numeru oraz kolumny, w której zaszła zmiana
    words->editWord(item->text(0).toInt(), column, text); //zmiana danego elementu
}

/* rysowanie drzewa słówek */
void manageWordList::paintTree()
{
    //czyszczenie drzewa
    this->ui->treeWidget->clear();

    QList<QTreeWidgetItem *> treeItems; //kontener na elementy rysowanego drzewa

    words->getAllWords(&treeItems);

    this->ui->treeWidget->setItemDelegateForColumn(0, new NoEditDelegate(this));

    //dodanie wszystkich elementów do drzewa
    this->ui->treeWidget->addTopLevelItems(treeItems);
}

/* wywołania okna dodania słówka*/
void manageWordList::on_addItem_released()
{
    newWord->words = words;
    newWord->exec();
}

/* ustawienie klikniętego elementu w drzewie na ostatnio klinięty w klasie */
void manageWordList::setLastClickedItem(QTreeWidgetItem * item)
{
    this->lastSelectedItem = item;
}

/* akcja usunięcia słówka */
void manageWordList::on_deleteItem_released()
{
    if(lastSelectedItem)
    {
        //akcja usuniecia elementu na podstawie numeru z drzewa => kolumna 0
        words->deleteWord(this->lastSelectedItem->text(0).toInt());

        //ponowne przerysowanie drzewa po usunięciu słowa
        this->paintTree();
    }
}

manageWordList::~manageWordList()
{
    //po zamknięciu okna zapisujemy zmiany do pliku
    //words->saveWords();
    delete ui;
}
