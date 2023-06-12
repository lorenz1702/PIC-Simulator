#include "mainwindow.h"

#include <QApplication>
#include "Scanner.h"
#include "Engine.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>

using namespace std;
//Programm starten:
//-> Im Terminal beim Pfad dieses Ordners "Klassen" mingw32-make eingeben

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Scanner scanner;
    Engine engine;

    // connect putcommandsinprogrammemory with signal from mainwindow
    QObject::connect(&w, &MainWindow::fileProcessed, [&scanner, &engine, &w]() {
        scanner.putcommandsinprogrammemory(engine, w.filename.toStdString());
        engine.controlCommand();
    });


    for (int i = 0; i < 15; i++) {
        cout << "Wert Programmemory an der Stelle " << i << ": " << engine.programmemory[i] << endl;
    }

    w.show();
    return a.exec();
}
