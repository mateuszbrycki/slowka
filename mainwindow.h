#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "words.h"
#include <set>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Words * words;

    void keyPressEvent(QKeyEvent * event);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_checkWords_released();
    void resetForm(); //czyszczenie formularza
    void setRightLabels();
    void setRandomedForm();
    void random(); //losowanie
    void setLabelColor(QLabel* label, QColor color);

    void on_actionZarz_dzaj_list_s_wek_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
