#include "addnew.h"
#include "ui_addnew.h"
#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>
#include "managewordlist.h"

addNew::addNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addNew)
{
    ui->setupUi(this);
}

addNew::addNew(QWidget *parent, Words * words) :
    QDialog(parent),
    ui(new Ui::addNew),
    words(words)
{
    ui->setupUi(this); 
    this->setWindowTitle("Dodawanie nowego słowa");
}

void addNew::on_pushButton_released()
{
    //pobieramy wpisane słowa
    QString firstForm = ui->firstForm->displayText();
    QString secondForm = ui->secondForm->displayText();
    QString thirdForm = ui->thirdForm->displayText();
    QString translation =  ui->translation->displayText();

    //sprawdzamy czy wszystkie zostały zapisane
    if(firstForm.isEmpty() || secondForm.isEmpty() || thirdForm.isEmpty() || translation.isEmpty())
    {
        QMessageBox::warning(this, "Uwaga", "Jedno z wymaganych pól jest puste!");
    } else {
        words->words.insert(Words::Word(firstForm, secondForm, thirdForm, translation)); //dodanie słówka do listy słówek
        resetForm(); //wyczyszczenie formularza
        QMessageBox::information(this, "Dodano", "Wpisane słówko zostało dodane!");  
        emit added(); //wysłanie sygnału potwierdzającego dodanie słowa
    } 
}

/* czyszczenie formularza */
void addNew::resetForm()
{
    ui->firstForm->setText("");
    ui->secondForm->setText("");
    ui->thirdForm->setText("");
    ui->translation->setText("");
}

addNew::~addNew()
{
    delete ui;
}
