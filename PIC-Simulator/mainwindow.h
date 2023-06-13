#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

signals:
    void fileProcessed();
    void nextCommand();

private slots:
    void on_open_file_triggered();
    void markNextLine(std::string pcurrentIndex);
    void clearMarkedLine();
    void handleCursorPositionChanged();
    void addBreakpoint(int plineNumber);
    void removeBreakpoint(int plineNumber);
    bool hasBreakpoint(int lineNumber) const;
    void on_start_button_clicked();

    void on_step_in_button_clicked();

    void on_step_out_button_clicked();

    void on_reset_button_clicked();

private:
    Ui::MainWindow *ui;
    std::string currentIndex;
    QSet<int> breakpoints;
};
#endif // MAINWINDOW_H
