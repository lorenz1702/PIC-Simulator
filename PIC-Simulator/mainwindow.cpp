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
    connect(ui->pin_table, &QTableWidget::itemClicked, this, [=](QTableWidgetItem* item) {
        int row = item->row();
        int column = item->column();
        toggleValue(row, column, 1);
    });

    connect(ui->status_register, &QTableWidget::itemClicked, this, [=](QTableWidgetItem* item) {
        int row = item->row();
        int column = item->column();
        toggleValue(row, column, 2);
    });

    connect(ui->option_register, &QTableWidget::itemClicked, this, [=](QTableWidgetItem* item) {
        int row = item->row();
        int column = item->column();
        toggleValue(row, column, 3);
    });


    connect(this, &MainWindow::resetEngine, this, [this]() {
        engine->initializeEngine();
        emit fileProcessed();
        setDefaultValues();
        setDefaultValues2();
        syncArrayToTable();
        syncRegisters();
        syncSpecials();
        clearMarkedLine();
        updateCurrentIndex(engine->IP);
        ui->wdt_checkbox->setCheckState(Qt::CheckState(false));
        int totalLines = ui->displayfile->document()->blockCount();

        for (int lineNumber = 0; lineNumber < totalLines; lineNumber++) {
            if (hasBreakpoint(lineNumber)) {
                removeBreakpoint(lineNumber);
            }
        }
    });

    timer = new QTimer(this);

    //set Validator for Datamemory (only hex values)
    HexDelegate *hexDelegate = new HexDelegate(ui->memory_table);
    ui->memory_table->setItemDelegate(hexDelegate);
    //set Validator for second tableWidget (only 0 or 1)
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(QRegularExpression("[01]"), this);
    ui->pin_table->setItemDelegate(new QItemDelegate(this));

    //initialize Datamemory memory_table
    setDefaultValues();
    //initialize RB Pins
    setDefaultValues2();

    for (int column = 0; column < ui->status_register->columnCount(); ++column) {
        QTableWidgetItem* item = new QTableWidgetItem("0");
        ui->status_register->setItem(0, column, item);
    }

    for (int column = 0; column < ui->option_register->columnCount(); ++column) {
        QTableWidgetItem* item = new QTableWidgetItem("0");
        ui->option_register->setItem(0, column, item);
    }


}

// initialize Datamemory with 00 in all cells
void MainWindow::setDefaultValues()
{
    int rowCount = ui->memory_table->rowCount();
    int columnCount = ui->memory_table->columnCount();

    for (int row = 0; row < rowCount; ++row)
    {
        for (int column = 0; column < columnCount; ++column)
        {
            QTableWidgetItem* item = new QTableWidgetItem("00");
            ui->memory_table->setItem(row, column, item);
        }
    }

}
// initialize RB Pins
void MainWindow::setDefaultValues2()
{

    // set all cells to 0
    for (int row = 0; row < ui->pin_table->rowCount(); ++row)
    {
        for (int col = 0; col < ui->pin_table->columnCount(); ++col)
        {
            QTableWidgetItem* item = new QTableWidgetItem("0");

            // check if row should be editable, only following will be editable
            if (!(row == 2 || row == 5 || row == 8 || row == 11 || row == 14))
                item->setFlags(item->flags() & ~Qt::ItemIsEditable); // set not editable

            ui->pin_table->setItem(row, col, item);
        }
    }

    // set i's in certain rows
    for (int col = 0; col < ui->pin_table->columnCount(); ++col)
    {
        QTableWidgetItem* item = new QTableWidgetItem("i");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->pin_table->setItem(1, col, item);
        item = new QTableWidgetItem("i");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->pin_table->setItem(4, col, item);
    }
    // set o's in certain rows
    for (int col = 0; col < ui->pin_table->columnCount(); ++col)
    {
        QTableWidgetItem* item = new QTableWidgetItem("o");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->pin_table->setItem(7, col, item);
        item = new QTableWidgetItem("o");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->pin_table->setItem(10, col, item);
        item = new QTableWidgetItem("o");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->pin_table->setItem(13, col, item);

    }

    QTableWidgetItem* item = new QTableWidgetItem("7");
    // set not editable
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(0, 0, item);

    item = new QTableWidgetItem("7");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(3, 0, item);

    item = new QTableWidgetItem("7");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(6, 0, item);

    item = new QTableWidgetItem("7");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(9, 0, item);

    item = new QTableWidgetItem("7");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(12, 0, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(0, 1, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(3, 1, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(6, 1, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(9, 1, item);

    item = new QTableWidgetItem("6");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(12, 1, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(0, 2, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(3, 2, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(6, 2, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(9, 2, item);

    item = new QTableWidgetItem("5");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(12, 2, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(0, 3, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(3, 3, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(6, 3, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(9, 3, item);

    item = new QTableWidgetItem("4");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(12, 3, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(0, 4, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(3, 4, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(6, 4, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(9, 4, item);

    item = new QTableWidgetItem("3");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(12, 4, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(0, 5, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(3, 5, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(6, 5, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(9, 5, item);

    item = new QTableWidgetItem("2");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(12, 5, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(0, 6, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(3, 6, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(6, 6, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(9, 6, item);

    item = new QTableWidgetItem("1");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(12, 6, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(0, 7, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(3, 7, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(6, 7, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(9, 7, item);

    item = new QTableWidgetItem("0");
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->pin_table->setItem(12, 7, item);

}

void MainWindow::toggleValue(int pRow, int pColumn, int pTable)
{
    // check if cell is editable
    QTableWidgetItem* item = nullptr;
    if(pTable == 1)
    {
        item = ui->pin_table->item(pRow, pColumn);
    } else if(pTable == 2)
    {
        item = ui->status_register->item(pRow, pColumn);
    } else if(pTable == 3)
    {
        item = ui->option_register->item(pRow, pColumn);
    }

    if (!item || !(item->flags() & Qt::ItemIsEditable))
        return;

    // get value of cell
    QString currentValue = item->text();

    // toggle value
    if (currentValue == "0")
        item->setText("1");
    else
        item->setText("0");

    if(pTable == 1)
    {
        // if RB cells are edited calculate new value
        if(pRow == 5)
        {
            int newValue = 0;
            for(int c = 0; c <= 7; c++)
            {
                QTableWidgetItem* rbItem = ui->pin_table->item(pRow, c);
                currentValue = rbItem->text();
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
            syncRegisters();
        }
    } else if(pTable == 2)
    {
        int newValue = 0;
        for(int c = 0; c <= 7; c++)
        {
            QTableWidgetItem* statusitem = ui->status_register->item(0, c);
            currentValue = statusitem->text();
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
        engine->Datamemory[0][3] = newValue; // set new Value
        syncArrayToTable(); // update Table for Datamemory
        syncRegisters();
        ui->status_register_label->setText("Status: " + QString::number(newValue));

    } else if(pTable == 3)
    {
        int newValue = 0;
        for(int c = 0; c <= 7; c++)
        {
            QTableWidgetItem* statusitem = ui->option_register->item(0, c);
            currentValue = statusitem->text();
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
        engine->Datamemory[1][1] = newValue; // set new Value
        syncArrayToTable(); // update Table for Datamemory
        syncRegisters();
        ui->option_register_label->setText("Option: " + QString::number(newValue));
    }
}


// if any value in Datamemory changes, signal "valuechanged" can be emitted in Engine
void MainWindow::syncArrayToTable()
{
    // first 16 rows relate to Datamemory[0]
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(engine->Datamemory[0][row * 8 + col], 16).rightJustified(2, '0')); //convert to hex
            ui->memory_table->setItem(row, col, item);
        }
    }

    // other rows relate to Datamemory[1]
    for (int row = 16; row < 32; ++row) {
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(engine->Datamemory[1][(row - 16) * 8 + col], 16).rightJustified(2, '0'));
            ui->memory_table->setItem(row, col, item);
        }
    }
}

void MainWindow::syncRegisters()
{

    for (int col = 0; col < 8; ++col) {
        int bitmask = 1 << (7 - col);  // Umgekehrte Bitposition
        int value = engine->Datamemory[0][3];
        if ((value & bitmask) != 0) {
            QTableWidgetItem* item = new QTableWidgetItem("1");
            ui->status_register->setItem(0, col, item);
        } else {
            QTableWidgetItem* item = new QTableWidgetItem("0");
            ui->status_register->setItem(0, col, item);
        }
    }


    for (int col = 0; col < 8; ++col) {
        int bitmask = 1 << (7 - col);
        int value = engine->Datamemory[1][1];
        if((value & bitmask) != 0)
        {
            QTableWidgetItem* item = new QTableWidgetItem("1");
            ui->option_register->setItem(0, col, item);
        } else {
            QTableWidgetItem* item = new QTableWidgetItem("0");
            ui->option_register->setItem(0, col, item);
        }
    }
}

void MainWindow::syncSpecials()
{
    ui->w_register_label->setText("W-Reg: " + QString::number(engine->W));
    ui->ip_label->setText("IP: " + QString::number(engine->IP));
    ui->pcl_label->setText("PCL: " + QString::number(engine->PCL));
    ui->pclath_label->setText("PCLATH: " + QString::number(engine->PCLATH));
    ui->fsr_label->setText("FSR: " + QString::number(engine->FSR));
    //if(ui->wdt_checkbox->isChecked())
    //{
    ui->wdt_checkbox->setText("WDT: " + QString::number(engine->WDT * (4 / ui->quarzfrequenz_spinBox->value())));
    //}
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
    connect(engine, &Engine::valueChanged, this, &MainWindow::syncRegisters);
    connect(engine, &Engine::valueChanged, this, &MainWindow::syncSpecials);
    ui->option_register_label->setText("Option: " + QString::number(engine->Datamemory[1][1]));
    ui->status_register_label->setText("Status: " + QString::number(engine->Datamemory[0][3]));
    syncArrayToTable();
    syncRegisters();
    syncSpecials();
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
    // Überprüfen, ob der Timer bereits läuft
    if (timer && timer->isActive()) {
        // Timer stoppen
        timer->stop();
        return;
    }

    ui->time_for_command_label->setText(QString::number(((float)4 / (float)ui->quarzfrequenz_spinBox->value()), 'f', 2));
    ui->option_register_label->setText("Option: " + QString::number(engine->Datamemory[1][1]));
    ui->status_register_label->setText("Status: " + QString::number(engine->Datamemory[0][3]));

    // Erstellen des Timers, falls er noch nicht vorhanden ist
    if (timer) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [this]() {
            ui->step_in_button->click();

            int lineNumber = findLineNumber(ui->displayfile, QString::fromStdString(currentIndex));
            std::cout << "linenumber: " << lineNumber << "currentIndex: " << currentIndex << std::endl;
            if (hasBreakpoint(lineNumber)) {
                timer->stop();
            }
        });
    }

    // Timer starten
    timer->start(500);
}


int MainWindow::findLineNumber(QTextBrowser* textBrowser, const QString& searchText)
{
    QTextDocument* document = textBrowser->document();
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::Start);

    QRegularExpression regex(QString("^") + QRegularExpression::escape(searchText));

    while (!cursor.atEnd())
    {
        cursor.movePosition(QTextCursor::EndOfLine);

        if (regex.match(cursor.block().text()).hasMatch())
        {
            return cursor.blockNumber();
        }

        cursor.movePosition(QTextCursor::NextBlock);
    }

    return -1;
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
    ui->time_for_command_label->setText(QString::number(((float)4 / (float)ui->quarzfrequenz_spinBox->value()), 'f', 2));
    runtime = ui->time_for_command_label->text().toDouble() * engine->RunTime;
    ui->runtime_label->setText(QString::number(runtime));
    emit nextCommand();
    updateCurrentIndex(engine->IP);
    markNextLine(currentIndex);

}

void MainWindow::on_step_out_button_clicked()
{
    ui->start_button->click();
}


void MainWindow::on_reset_button_clicked()
{
    timer->stop();
    engine->IP = 0;
    ui->reset_runtime_button->click();
    emit resetEngine();

}


void MainWindow::on_reset_runtime_button_clicked()
{
    ui->runtime_label->setText("0");
    runtime = 0.00;
    engine->RunTime = 0;
}


void MainWindow::on_wdt_checkbox_pressed()
{
    if(engine->WDTE == 0)
    {
        engine->WDTE = 1;

    } else {
        engine->WDTE = 0;
    }
}

