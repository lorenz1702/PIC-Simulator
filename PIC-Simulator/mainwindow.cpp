#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>


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



}

