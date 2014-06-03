#ifndef MANAGEWORDLIST_H
#define MANAGEWORDLIST_H

#include <QDialog>
#include <QTreeWidget>
#include "words.h"
#include "addnew.h"

namespace Ui {
class manageWordList;
}

class manageWordList : public QDialog
{
    Q_OBJECT

public:
    Words * words;
    QTreeWidgetItem * lastSelectedItem = NULL;

    explicit manageWordList(QWidget *parent = 0);
    manageWordList(QWidget *parent = 0, Words * words = NULL);
    ~manageWordList();

    int printMessage();

private:
    Ui::manageWordList *ui;
    addNew * newWord = NULL;

private slots:
    void changeItem(QTreeWidgetItem * item, int column);
    void setLastClickedItem(QTreeWidgetItem * item);
    void paintTree();
    void on_addItem_released();
    void on_deleteItem_released();
};

#endif // MANAGEWORDLIST_H
