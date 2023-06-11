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

private slots:
    void on_open_file_triggered();
    void markNextLine();
    void clearMarkedLine();
    void on_start_button_clicked();

    void on_step_in_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
