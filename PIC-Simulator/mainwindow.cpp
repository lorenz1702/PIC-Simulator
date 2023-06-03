#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qscrollbar.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionDatei_ffnen_triggered()
{
    ui->displayfile->setLineWrapMode(QTextEdit::NoWrap);
    ui->displayfile->horizontalScrollBar()->setValue(0);
    QString filename=QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C://",
        "LST files (*.LST)"
        );


    QFile lstfile(filename);
    if(!lstfile.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",lstfile.errorString());

    QTextStream in(&lstfile);
    QFont font = (QFont("Source Code Pro", 9, QFont::Normal));
    ui->displayfile->setFont(font);
    ui->displayfile->setText(in.readAll());

}

