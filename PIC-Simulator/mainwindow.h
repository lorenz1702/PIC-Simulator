#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include "Engine.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString filename;

    void getEngine(Engine *pEngine);




signals:
    void fileProcessed(); // signal that emits when file was selected
    void nextCommand(); // signal to control command execution
    void resetEngine();

private slots:

    //buttons
    void on_open_file_triggered();
    void on_start_button_clicked();
    void on_step_in_button_clicked();
    void on_step_out_button_clicked();
    void on_reset_button_clicked();

    void markNextLine(std::string pcurrentIndex);
    void clearMarkedLine();

    // breakpoints
    void handleCursorPositionChanged();
    void addBreakpoint(int plineNumber);
    void removeBreakpoint(int plineNumber);
    bool hasBreakpoint(int lineNumber) const;


    void on_reset_runtime_button_clicked();

private:
    Ui::MainWindow *ui;
    std::string currentIndex; // string to mark next line
    QSet<int> breakpoints;  // list of breakpoints
    Engine *engine;         // engine
    double runtime;
    int lineoffset;

    void updateCurrentIndex(int pIP);
    void setDefaultValues();
    void setDefaultValues2();
    void toggleValue(int pRow, int pColumn);
    void updateTableFromMemory();
    void syncArrayToTable();
    int findLineNumber(QTextBrowser* textBrowser, const QString& searchText);

};
#endif // MAINWINDOW_H
