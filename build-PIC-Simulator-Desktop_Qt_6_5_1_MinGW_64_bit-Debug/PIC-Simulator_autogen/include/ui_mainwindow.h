/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionDatei_ffnen;
    QWidget *centralwidget;
    QTextBrowser *displayfile;
    QTableView *tableView;
    QListView *listView;
    QListView *listView_2;
    QMenuBar *menubar;
    QMenu *menuDatei;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1276, 647);
        actionDatei_ffnen = new QAction(MainWindow);
        actionDatei_ffnen->setObjectName("actionDatei_ffnen");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        displayfile = new QTextBrowser(centralwidget);
        displayfile->setObjectName("displayfile");
        displayfile->setGeometry(QRect(40, 80, 581, 461));
        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(705, 80, 531, 192));
        listView = new QListView(centralwidget);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(710, 350, 256, 192));
        listView_2 = new QListView(centralwidget);
        listView_2->setObjectName("listView_2");
        listView_2->setGeometry(QRect(980, 350, 256, 192));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1276, 21));
        menuDatei = new QMenu(menubar);
        menuDatei->setObjectName("menuDatei");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuDatei->menuAction());
        menuDatei->addAction(actionDatei_ffnen);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionDatei_ffnen->setText(QCoreApplication::translate("MainWindow", "Datei \303\266ffnen", nullptr));
        menuDatei->setTitle(QCoreApplication::translate("MainWindow", "Datei", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
