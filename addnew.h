#ifndef ADDNEW_H
#define ADDNEW_H

#include <QDialog>
#include "words.h";

namespace Ui {
class addNew;
}

class addNew : public QDialog
{
    Q_OBJECT

public:
    Words * words;
    explicit addNew(QWidget *parent = 0);
    addNew(QWidget *parent = 0, Words * words = NULL);
    ~addNew();

private slots:
    void on_pushButton_released();

private:
    Ui::addNew *ui;
    void resetForm();

signals:
    void added();
};

#endif // ADDNEW_H
