#include <QtWidgets>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bardelegate.h"
#include "spinboxdelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);
    QAction *saveAction = fileMenu->addAction(tr("&Save As..."));
    saveAction->setShortcuts(QKeySequence::SaveAs);
    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcuts(QKeySequence::Quit);

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    menuBar()->addMenu(fileMenu);
    statusBar();

    model = new QStandardItemModel(8,2,this);
    model->setHeaderData(0, Qt::Horizontal, tr("Product"));
    model->setHeaderData(1, Qt::Horizontal, tr("Quantity"));

    openFile(":/Charts/qtdata.cht");


    myBarDelegate = new BarDelegate(this);
    mySpinBoxDelegate = new SpinBoxDelegate(this);
    QSplitter *splitter = new QSplitter;
    QTableView *tableView = new QTableView;
    QTableView *tableView2 = new QTableView;
    QTableView *barView = new QTableView;
    barView->setItemDelegate(myBarDelegate);
    splitter->addWidget(tableView);
    splitter->addWidget(tableView2);
    splitter->addWidget(barView);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setStretchFactor(2, 2);


    tableView->setModel(model);
    tableView->verticalHeader()->setVisible(false);
    tableView->setColumnHidden(1,1);
    tableView->setEditTriggers(0);
    tableView->setFixedWidth(125);
    tableView->setFixedHeight(235);



    tableView2->setModel(model);
    tableView2->verticalHeader()->setVisible(false);
    tableView2->setColumnHidden(0,1);
    tableView2->setItemDelegate(mySpinBoxDelegate);
    tableView2->setFixedWidth(125);
    tableView2->setFixedHeight(235);



    barView->setModel(model);
    barView->setColumnHidden(0,1);
    barView->setEditTriggers(0);
    barView->verticalHeader()->setVisible(false);
    barView->setFixedWidth(225);
    barView->setFixedHeight(235);



    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    tableView->setSelectionModel(selectionModel);
    tableView2->setSelectionModel(selectionModel);
    barView->setSelectionModel(selectionModel);


    setWindowTitle(tr("Inventory"));

    tableView->setAutoScrollMargin(1);
    QHeaderView *headerView = tableView->horizontalHeader();
    headerView->setStretchLastSection(true);
    QHeaderView *headerView2 = tableView2->horizontalHeader();
    headerView2->setStretchLastSection(true);
    QHeaderView *headerView3 = barView->horizontalHeader();
    headerView3->setStretchLastSection(true);

    setCentralWidget(splitter);

}

void MainWindow::openFile(const QString &path)
{
    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"), "", "*.cht");
    else
        fileName = path;

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;

    QTextStream stream(&file);
    QString line;

    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

    int row = 0;
    do {
        line = stream.readLine();
        if (!line.isEmpty()) {
            model->insertRows(row, 1, QModelIndex());

            QStringList pieces = line.split(",", QString::SkipEmptyParts);
            model->setData(model->index(row, 0, QModelIndex()),
                           pieces.value(0));
            model->setData(model->index(row, 1, QModelIndex()),
                           pieces.value(1));
            model->setData(model->index(row, 0, QModelIndex()),
                           QColor(pieces.value(2)), Qt::DecorationRole);
            row++;
        }
    } while (!line.isEmpty());

    file.close();
    statusBar()->showMessage(tr("Loaded %1").arg(fileName), 2000);
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save file as"), "", "*.cht");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;

    QTextStream stream(&file);
    for (int row = 0; row < model->rowCount(QModelIndex()); ++row) {

        QStringList pieces;

        pieces.append(model->data(model->index(row, 0, QModelIndex()),
                                  Qt::DisplayRole).toString());
        pieces.append(model->data(model->index(row, 1, QModelIndex()),
                                  Qt::DisplayRole).toString());
        pieces.append(model->data(model->index(row, 0, QModelIndex()),
                                  Qt::DecorationRole).toString());

        stream << pieces.join(',') << "\n";
    }

    file.close();
    statusBar()->showMessage(tr("Saved %1").arg(fileName), 2000);
}


MainWindow::~MainWindow()
{
    delete ui;
}
