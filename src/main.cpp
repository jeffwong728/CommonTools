#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QTextStream>
#include <QTranslator>

std::string InitializePython()
{
    try {
        //PyImport_AppendInittab("spam", &PyInit_spam);
        pybind11::initialize_interpreter();
        pybind11::object mainModule = pybind11::module_::import("__main__");
        pybind11::object mainNamespace = mainModule.attr("__dict__");
        pybind11::exec("import sys", mainNamespace);
        pybind11::exec("import io", mainNamespace);
        pybind11::exec("spam_output = io.StringIO()", mainNamespace);
        pybind11::exec("sys.stdout = spam_output", mainNamespace);
        return std::string();
    }
    catch (pybind11::error_already_set& e) {
        return std::string(e.what());
    }
}

void FinalizePython()
{
    //PyImport_AppendInittab("spam", &PyInit_spam);
    pybind11::finalize_interpreter();
}

void PyClearOutput()
{
    pybind11::object mainModule = pybind11::module_::import("__main__");
    pybind11::object mainNamespace = mainModule.attr("__dict__");
    pybind11::exec("spam_output = io.StringIO()", mainNamespace);
    pybind11::exec("sys.stdout = spam_output", mainNamespace);
}

std::string PyGetOutput()
{
    pybind11::object mainModule = pybind11::module_::import("__main__");
    pybind11::object mainNamespace = mainModule.attr("__dict__");
    pybind11::exec("spamOut = spam_output.getvalue()", mainNamespace);
    std::string stdOut = pybind11::cast<std::string>(mainNamespace["spamOut"]);
    return stdOut;
}

std::pair<std::string, bool> PyRunFile(const std::string& strFullPath)
{
    try
    {
        pybind11::object mainModule = pybind11::module_::import("__main__");
        pybind11::object mainNamespace = mainModule.attr("__dict__");
        PyClearOutput();
        pybind11::eval_file(strFullPath.c_str(), mainNamespace);
        return std::make_pair(PyGetOutput(), true);
    }
    catch (const pybind11::error_already_set& e)
    {
        return std::make_pair(std::string(e.what()), false);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InitializePython();
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
    const auto ir = a.exec();
    FinalizePython();
    return ir;
}
