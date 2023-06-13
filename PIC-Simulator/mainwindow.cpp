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
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("QMainWindow { background-color: rgba(50, 62, 98, 255); }");
    currentIndex = "0000 ";

    connect(ui->displayfile, &QTextBrowser::cursorPositionChanged, this, &MainWindow::handleCursorPositionChanged);

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

    breakpoints = QSet<int>();

}


void MainWindow::on_start_button_clicked()
{
    //trigger signal for scanner
    emit fileProcessed();
}

void MainWindow::markNextLine(std::string pcurrentIndex)
{
    int currentLine = ui->displayfile->textCursor().blockNumber();
    int lineCount = ui->displayfile->document()->blockCount();
    if (currentLine >= 0 && currentLine < lineCount)
    {
        // Clear mark from previous line
        clearMarkedLine();

        // Mark next line that starts with "000x" followed by a space
        for (int i = currentLine + 1; i < lineCount; i++)
        {
            QTextBlock block = ui->displayfile->document()->findBlockByLineNumber(i);
            QString lineText = block.text().trimmed();

            if (lineText.startsWith(QString::fromUtf8(currentIndex.c_str())) && lineText.length() > 5)
            {
                QTextCursor cursor(block);
                cursor.select(QTextCursor::LineUnderCursor);

                // Change background color to yellow
                QTextCharFormat format;
                format.setBackground(QColor(Qt::yellow));
                cursor.mergeCharFormat(format);

                int tempcurrentIndex;
                std::istringstream iss(pcurrentIndex);
                iss >> std::hex >> tempcurrentIndex;

                // Inkrementiere die Zahl
                tempcurrentIndex++;

                // Wandle die Zahl zurück in einen String um
                std::ostringstream oss;
                    oss << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << tempcurrentIndex;
                currentIndex = oss.str();

                // Füge das Leerzeichen am Ende hinzu
                currentIndex += " ";
                break;  // Stop marking after finding the first matching line
            }
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

void MainWindow::handleCursorPositionChanged()
{
    QTextCursor cursor = ui->displayfile->textCursor();
    int lineNumber = cursor.blockNumber();

    // Wenn die Zeile bereits einen Breakpoint hat, entferne ihn
    if (hasBreakpoint(lineNumber))
    {
        removeBreakpoint(lineNumber);
    }
    else
    {
        // Sonst füge einen Breakpoint hinzu
        QString lineText = cursor.block().text().trimmed();

        QRegularExpression regex("^\\d{4}\\s");
        if (regex.match(lineText).hasMatch())
        {
            addBreakpoint(lineNumber);
        }
    }
}



void MainWindow::addBreakpoint(int plineNumber)
{
    QTextBlock block = ui->displayfile->document()->findBlockByLineNumber(plineNumber);
    QTextCursor cursor(block);
    cursor.select(QTextCursor::LineUnderCursor);

    // Change background color to indicate the breakpoint
    QTextCharFormat format;
    format.setBackground(QColor(Qt::red));
    cursor.mergeCharFormat(format);
}

void MainWindow::removeBreakpoint(int plineNumber)
{
    QTextBlock block = ui->displayfile->document()->findBlockByLineNumber(plineNumber);
    QTextCursor cursor(block);
    cursor.select(QTextCursor::LineUnderCursor);

    // Change background color back to the default
    QTextCharFormat format;
    format.setBackground(QColor(Qt::white));
    cursor.mergeCharFormat(format);
}

bool MainWindow::hasBreakpoint(int plineNumber) const
{
    QTextBlock block = ui->displayfile->document()->findBlockByLineNumber(plineNumber);
    QTextCursor cursor(block);
    cursor.select(QTextCursor::LineUnderCursor);

    QTextCharFormat format = cursor.charFormat();
    return format.background().color() == Qt::red;
}


void MainWindow::on_step_in_button_clicked()
{
    markNextLine(currentIndex);
    emit nextCommand();

}

void MainWindow::on_step_out_button_clicked()
{
    //
}


void MainWindow::on_reset_button_clicked()
{
    //
}

