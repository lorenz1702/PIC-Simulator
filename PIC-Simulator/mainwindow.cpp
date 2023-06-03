#include "mainwindow.h"
#include "./ui_mainwindow.h"
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
    QString filename=QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C://",
        "LST files (*.LST)"
        );


    /*
    auto fileContentReady = [](const QString &fileName, const QByteArray &fileContent) {
        if (fileName.isEmpty()) {
            // No file was selected
        } else {
            // Use fileName and fileContent

        }
    };
    QFileDialog::getOpenFileContent("LST files (*.LST)",  fileContentReady);
*/


    QFile lstfile(filename);
    if(!lstfile.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",lstfile.errorString());

    QTextStream in(&lstfile);

    ui->displayfile->setText(in.readAll());




}

