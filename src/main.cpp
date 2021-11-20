#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QTextStream>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    QFile f(":styles/adaptic");
    if (f.exists())
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        //qApp->setStyleSheet(ts.readAll());
    }

    qDebug() << "Start TestQT";

    MainWindow w;
    w.show();
    return a.exec();
}
