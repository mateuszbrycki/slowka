#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <set>
#include <algorithm>
#include <QMessageBox>
#include "words.h"
#include <QGlobal.h>
#include <QTime>
#include <QPixmap>
#include <QKeyEvent>
#include <QColor>
#include "managewordlist.h"
#include "words.h"

MainWindow::MainWindow(QWidget *parent) :    QMainWindow(parent), ui(new Ui::MainWindow)
{
    using namespace std;
    ui->setupUi(this);
    this->setWindowTitle("Słówka 1.0");

    //ziarno do losowania
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    //otworzenie pliku i wczytanie słów
    words = new Words("words.db");

    random();
 }

void MainWindow::random()
{
    words->randomAll();
    setRandomedForm();
}

void MainWindow::on_checkWords_released()
{
    //pobieramy wpisane słówka
    QString firstFormIn = ui->firstFormIn->displayText();
    QString secondFormIn = ui->secondFormIn->displayText();
    QString thirdFormIn = ui->thirdFormIn->displayText();
    QString translationIn = ui->translationIn->displayText();

    if(firstFormIn.isEmpty() || secondFormIn.isEmpty() || thirdFormIn.isEmpty() || translationIn.isEmpty())
    {
        QMessageBox::warning(this, "Uwaga", "Jedno z wymaganych pól jest puste!");
    } else {

        bool allRight = true;
        //sprawdzamy wpisane formy słówek i ustawiamy odpowiednie kolory napisów
        if(words->randomed->firstForm != firstFormIn)
        {
            setLabelColor(ui->firstFormRight, QColor("red"));
            allRight = false;
        } else {
            setLabelColor(ui->firstFormRight, QColor("green"));
        }

        if(words->randomed->secondForm != secondFormIn)
        {
            setLabelColor(ui->secondFormRight, QColor("red"));
            allRight = false;
        } else {
            setLabelColor(ui->secondFormRight, QColor("green"));
        }

        if(words->randomed->thirdForm != thirdFormIn)
        {
            setLabelColor(ui->thirdFormRight, QColor("red"));
            allRight = false;
        } else {
            setLabelColor(ui->thirdFormRight, QColor("green"));
        }

        if(!words->randomed->translation.contains(translationIn))
        {
            setLabelColor(ui->translationRight, QColor("red"));
            allRight = false;
        } else {
            setLabelColor(ui->translationRight, QColor("green"));
        }

        //ustawienie ikony w zależności od poprawności odpowiedzi
        ui->iconLabel->setPixmap(QPixmap(":/images/" + ((allRight) ? QString("true") : QString("false")) + ".png"));

        setRightLabels(); //ustawienie dobrych odpowiedzi
        resetForm(); //wyczszczenie formularza
        random(); //losowanie następnych słówek
    }
}

/* wyczszczenie formularza */
void MainWindow::resetForm()
{
    ui->firstFormIn->setText("");
    ui->firstFormIn->setReadOnly(false);

    ui->secondFormIn->setText("");
    ui->secondFormIn->setReadOnly(false);

    ui->thirdFormIn->setText("");
    ui->thirdFormIn->setReadOnly(false);

    ui->translationIn->setText("");
    ui->translationIn->setReadOnly(false);
}

/* ustawienie poprawnych odpowiedzi */
void MainWindow::setRightLabels()
{
    ui->firstFormRight->setText(words->randomed->firstForm);
    ui->secondFormRight->setText(words->randomed->secondForm );
    ui->thirdFormRight->setText(words->randomed->thirdForm);
    ui->translationRight->setText(words->randomed->translation);
}

/* ustawienie wylosowanego formularza */
void MainWindow::setRandomedForm()
{
    switch(words->randomedForm)
     {
     case 1:
         ui->firstFormIn->setText(words->randomed->firstForm);
         ui->firstFormIn->setReadOnly(true);
         break;
     case 2:
         ui->secondFormIn->setText(words->randomed->secondForm);
         ui->secondFormIn->setReadOnly(true);
         break;
     case 3:
         ui->thirdFormIn->setText(words->randomed->thirdForm);
         ui->thirdFormIn->setReadOnly(true);
         break;
     case 4:
         ui->translationIn->setText(words->randomed->translation);
         ui->translationIn->setReadOnly(true);
         break;
     }
}

/* ustawienie koloru label'a podanego jako argument */
void MainWindow::setLabelColor(QLabel* label, QColor color)
{
    label->setStyleSheet("QLabel {color : "+ color.name() + "; }");
}

/* akcja wykonywana po naciśnięciu elementu menu Zarządzaj listą słówek */
void MainWindow::on_actionZarz_dzaj_list_s_wek_triggered()
{
    manageWordList * manage = new manageWordList(this, words);
    manage->exec();
    delete manage;
}

/* akcja na przyciśnięcie klawisza w oknie głównym programu */
void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Enter)
    {
        this->on_checkWords_released();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
