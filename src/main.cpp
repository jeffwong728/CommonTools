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
        qApp->setStyleSheet(ts.readAll());
    }

    QTranslator t;
    t.load(":langs/TestQT_zh_CN");
    a.installTranslator(&t);

    MainWindow w;
    w.show();
    return a.exec();
}
