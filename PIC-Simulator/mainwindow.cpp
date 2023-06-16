#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qscrollbar.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>
#include <QMainWindow>
#include <QTextBrowser>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextBlock>
#include <charconv>
#include <iostream>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QItemDelegate>
#include <QTimer>

// class for Hex-Validation for Datamemory

class HexDelegate : public QItemDelegate
{
public:
    HexDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QLineEdit *editor = new QLineEdit(parent);
        QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("[0-9A-Fa-f]{1,2}"), editor); // only allow one or two hex digits
        editor->setValidator(validator);
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
        lineEdit->setText(value);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
        QString value = lineEdit->text();
        model->setData(index, value, Qt::EditRole);
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set background color
    this->setStyleSheet("QMainWindow { background-color: rgba(50, 62, 98, 255); }");
    //set connection for breakpoints
    connect(ui->displayfile, &QTextBrowser::cursorPositionChanged, this, &MainWindow::handleCursorPositionChanged);
    //set connection for toggle function for RB changes
    connect(ui->tableWidget_2, &QTableWidget::cellClicked, this, &MainWindow::toggleValue);

    connect(this, &MainWindow::resetEngine, this, [this]() {
        engine->initializeEngine();
        emit fileProcessed();
        setDefaultValues();
        setDefaultValues2();
        syncArrayToTable();
        clearMarkedLine();
        updateCurrentIndex(engine->IP);

        int totalLines = ui->displayfile->document()->blockCount();

        for (int lineNumber = 0; lineNumber < totalLines; lineNumber++) {
            if (hasBreakpoint(lineNumber)) {
                removeBreakpoint(lineNumber);
            }
        }
    });

    //set Validator for Datamemory (only hex values)
    HexDelegate *hexDelegate = new HexDelegate(ui->tableWidget);
    ui->tableWidget->setItemDelegate(hexDelegate);
    //set Validator for second tableWidget (only 0 or 1)
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(QRegularExpression("[01]"), this);
    ui->tableWidget_2->setItemDelegate(new QItemDelegate(this));

    //initialize Datamemory tablewidget
    setDefaultValues();
    //initialize RB Pins
    setDefaultValues2();


}

// initialize Datamemory with 00 in all cells
void MainWindow::setDefaultValues()
{
    int rowCount = ui->tableWidget->rowCount();
    int columnCount = ui->tableWidget->columnCount();

    for (int row = 0; row < rowCount; ++row)
    {
        for (int column = 0; column < columnCount; ++column)
        {
            QTableWidgetItem* item = new QTableWidgetItem("00");
            ui->tableWidget->setItem(row, column, item);
        }
    }

}
// initialize RB Pins
void MainWindow::setDefaultValues2()
{

    // set all cells to 0
    for (int row = 0; row < ui->tableWidget_2->rowCount(); ++row)
    {
        for (int col = 0; col < ui->tableWidget_2->columnCount(); ++col)
        {
            QTableWidgetItem* item = new QTableWidgetItem("0");

            // check if row should be editable, only following will be editable
            if (!(row == 2 || row == 5 || row == 8 || row == 11 || row == 14))
                item->setFlags(item->flags() & ~Qt::ItemIsEditable); // set not editable

            ui->tableWidget_2->setItem(row, col, item);
        }
    }

    // set i's in certain rows
    for (int col = 0; col < ui->tableWidget_2->columnCount(); ++col)
    {
        QTableWidgetItem* item = new QTableWidgetItem("i");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget_2->setItem(1, col, item);
        item = new QTableWidgetItem("i");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget_2->setItem(4, col, item);
    }
    // set o's in certain rows
    for (int col = 0; col < ui->tableWidget_2->columnCount(); ++col)
    {
        QTableWidgetItem* item = new QTableWidgetItem("o");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget_2->setItem(7, col, item);
        item = new QTableWidgetItem("o");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget_2->setItem(10, col, item);
        item = new QTableWidgetItem("o");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget_2->setItem(13, col, item);

    }

    QTableWidgetItem* item = new QTableWidgetItem("7");
    // set not editable
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(0, 0, item);

    item = new QTableWidgetItem("7");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(3, 0, item);

    item = new QTableWidgetItem("7");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(6, 0, item);

    item = new QTableWidgetItem("7");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(9, 0, item);

    item = new QTableWidgetItem("7");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(12, 0, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(0, 1, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(3, 1, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(6, 1, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(9, 1, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(12, 1, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(0, 2, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(3, 2, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(6, 2, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(9, 2, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(12, 2, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(0, 3, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(3, 3, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(6, 3, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(9, 3, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(12, 3, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(0, 4, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(3, 4, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(6, 4, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(9, 4, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(12, 4, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(0, 5, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(3, 5, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(6, 5, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(9, 5, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(12, 5, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(0, 6, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(3, 6, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(6, 6, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(9, 6, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(12, 6, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(0, 7, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(3, 7, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(6, 7, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(9, 7, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget_2->setItem(12, 7, item);

}

void MainWindow::toggleValue(int pRow, int pColumn)
{
    // check if cell is editable
    QTableWidgetItem* item = ui->tableWidget_2->item(pRow, pColumn);
    if (!item || !(item->flags() & Qt::ItemIsEditable))
        return;

    // get value of cell
    QString currentValue = item->text();

    // toggle value
    if (currentValue == "0")
        item->setText("1");
    else
        item->setText("0");

    // if RB cells are edited calculate new value
    if(pRow == 5)
    {
        int newValue = 0;
        for(int c = 0; c <= 7; c++)
        {
            QTableWidgetItem* item = ui->tableWidget_2->item(pRow, c);
            currentValue = item->text();
            if(currentValue == "1")
            {
                switch(c)
                {
                    case 0: newValue = newValue + 128; break;
                    case 1: newValue = newValue + 64; break;
                    case 2: newValue = newValue + 32; break;
                    case 3: newValue = newValue + 16; break;
                    case 4: newValue = newValue + 8; break;
                    case 5: newValue = newValue + 4; break;
                    case 6: newValue = newValue + 2; break;
                    case 7: newValue = newValue + 1; break;
                }
            }
        }
        engine->Datamemory[0][6] = newValue; // set new Value
        syncArrayToTable(); // update Table for Datamemory
    }
}

// if any value in Datamemory changes, signal "valuechanged" can be emitted in Engine
void MainWindow::syncArrayToTable()
{
    // first 16 rows relate to Datamemory[0]
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(engine->Datamemory[0][row * 8 + col], 16).rightJustified(2, '0')); //convert to hex
            ui->tableWidget->setItem(row, col, item);
        }
    }

    // other rows relate to Datamemory[1]
    for (int row = 16; row < 32; ++row) {
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(engine->Datamemory[1][(row - 16) * 8 + col], 16).rightJustified(2, '0'));
            ui->tableWidget->setItem(row, col, item);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// get engine
void MainWindow::getEngine(Engine* pEngine)
{
    engine = pEngine;
    updateCurrentIndex(engine->IP);
    connect(engine, &Engine::valueChanged, this, &MainWindow::syncArrayToTable);
    syncArrayToTable();
}
// if button open file is pressed
void MainWindow::on_open_file_triggered()
{
    //set mode for proper view for file
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

    //trigger signal for scanner
    emit fileProcessed();

}
// if start button is pressed
void MainWindow::on_start_button_clicked()
{

    ui->time_for_command_label->setText(QString::number(((float)4 / (float)ui->quarzfrequenz_spinBox->value()), 'f', 2));
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer]() {
        // quarzfrequenz
        runtime = ui->time_for_command_label->text().toDouble() * engine->RunTime;
        ui->runtime_label->setText(QString::number(runtime));
        ui->step_in_button->click();

        if (engine->programmemory[engine->IP + 1] == 0 && engine->programmemory[engine->IP + 2] == 0) {
            timer->stop();
            timer->deleteLater();
        }
    });

    timer->start(500);

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

                break;  // Stop marking after finding the first matching line
            }
        }
    }

}

void MainWindow::updateCurrentIndex(int pIP)
{
    std::cout << "IP: " << pIP << std::endl;

    // Wandele pIP in einen hexadezimalen String um
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << pIP;
    currentIndex = oss.str();

    // Füge das Leerzeichen am Ende hinzu
    currentIndex += " ";

    std::cout << "currentIndex: " << currentIndex << std::endl;
}


void MainWindow::clearMarkedLine()
{

    int lineCount = ui->displayfile->document()->blockCount();
    for (int i = 0; i < lineCount; i++)
    {

        QTextBlock block = ui->displayfile->document()->findBlockByLineNumber(i);
        QTextCursor cursor(block);
        int lineNumber = cursor.blockNumber();
        cursor.select(QTextCursor::LineUnderCursor);

        if(!hasBreakpoint(lineNumber))
        {
        // change backgroundcolor to whiteto remove yellow color
        QTextCharFormat format;
        format.setBackground(QColor(Qt::white));
        cursor.mergeCharFormat(format);
        }
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
    emit nextCommand();
    updateCurrentIndex(engine->IP);
    markNextLine(currentIndex);

}

void MainWindow::on_step_out_button_clicked()
{
    //
}


void MainWindow::on_reset_button_clicked()
{
    engine->IP = 0;
    emit resetEngine();

}


void MainWindow::on_reset_runtime_button_clicked()
{
    ui->runtime_label->setText("0");
    runtime = 0.00;
    engine->RunTime = 0;
}

