#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qscrollbar.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>
#include <QMainWindow>
#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextBlock>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("QMainWindow { background-color: rgba(50, 62, 98, 255); }");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_file_triggered()
{
    //set mode for proper view
    ui->displayfile->setLineWrapMode(QTextEdit::NoWrap);
    ui->displayfile->horizontalScrollBar()->setValue(0);

    //open select file menu
    filename=QFileDialog::getOpenFileName(
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
    //trigger signal for scanner
    emit fileProcessed();

}


void MainWindow::on_start_button_clicked()
{
    markNextLine();
}

void MainWindow::markNextLine()
{
    int currentLine = ui->displayfile->textCursor().blockNumber();
    int lineCount = ui->displayfile->document()->blockCount();
    if (currentLine >= 0 && currentLine < lineCount)
    {
        // clear mark from previous line
        clearMarkedLine();
        // mark current block
        QTextBlock currentBlock = ui->displayfile->document()->findBlockByLineNumber(currentLine);
        QTextCursor cursor(currentBlock);
        cursor.select(QTextCursor::LineUnderCursor);
        // change background color to yellow
        QTextCharFormat format;
        format.setBackground(QColor(Qt::yellow));
        cursor.mergeCharFormat(format);
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(format);
        // go to next line
        if (currentBlock.next().isValid())
        {
            QTextBlock nextBlock = currentBlock.next();
            QTextCursor nextCursor(nextBlock);
            ui->displayfile->setTextCursor(nextCursor);
        }
    }
}

void MainWindow::clearMarkedLine()
{

    int lineCount = ui->displayfile->document()->blockCount();
    for (int i = 0; i < lineCount; i++)
    {
        QTextBlock block = ui->displayfile->document()->findBlockByLineNumber(i);
        QTextCursor cursor(block);
        cursor.select(QTextCursor::LineUnderCursor);

        // change backgroundcolor to whiteto remove yellow color
        QTextCharFormat format;
        format.setBackground(QColor(Qt::white));
        cursor.mergeCharFormat(format);
    }
}



void MainWindow::on_step_in_button_clicked()
{
    //
}




void MainWindow::on_step_out_button_clicked()
{
    //
}


void MainWindow::on_reset_button_clicked()
{
    //
}

