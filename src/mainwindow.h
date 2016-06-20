#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include "bardelegate.h"
#include "spinboxdelegate.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile(const QString &path = QString());
    void saveFile();


private:
    Ui::MainWindow *ui;
    QAbstractItemModel *model;
    QItemSelectionModel *selectionModel;
    BarDelegate *myBarDelegate;
    SpinBoxDelegate *mySpinBoxDelegate;

};

#endif // MAINWINDOW_H
