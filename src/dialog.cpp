#include "dialog.h"
#include <QDir>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QProcess>
#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTextStream>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QTextEdit* textEdit = new QTextEdit(this);
    textEdit->setObjectName("logBox");
    layout->addWidget(textEdit, 1);

    QMetaObject::invokeMethod(this, "load", Qt::QueuedConnection);
}

Dialog::~Dialog()
{
}

void Dialog::load()
{
    QTextEdit* textEdit = findChild<QTextEdit*>("logBox", Qt::FindDirectChildrenOnly);
    textEdit->append("Loading TestQT.exe ...");
    QString mDir = QCoreApplication::applicationDirPath();
    QString mExe = mDir + QDir::separator() + "TestQT.exe";

    mServer = new QLocalServer(this);
    connect(mServer, &QLocalServer::newConnection, this, &Dialog::newConnection);
    mServer->listen("36663904-E0E6-4DAB-A8EC-83C98B4C48AC");

    mProcess = new QProcess(this);
    connect(mProcess, &QProcess::readyRead, this, &Dialog::on_ready_read);
    mProcess->setProgram(mExe);
    mProcess->startDetached();
}

void Dialog::newConnection()
{
    QTextEdit* textEdit = findChild<QTextEdit*>("logBox", Qt::FindDirectChildrenOnly);
    textEdit->append("Stub new connection.");
    QLocalSocket* socket = mServer->nextPendingConnection();
    if (socket && QAbstractSocket::ConnectedState == socket->state())
    {
        connect(socket, &QLocalSocket::disconnected, this, &Dialog::disconnected);
        connect(socket, &QLocalSocket::disconnected, socket, &QLocalSocket::deleteLater);
        connect(socket, &QLocalSocket::readyRead, this, &Dialog::on_ready_read);
    }
}

void Dialog::disconnected()
{
    QTextEdit* textEdit = findChild<QTextEdit*>("logBox", Qt::FindDirectChildrenOnly);
    textEdit->append("Stub disconnected.");
    accept();
}

void Dialog::on_ready_read()
{
    QTextEdit* textEdit = findChild<QTextEdit*>("logBox", Qt::FindDirectChildrenOnly);
    textEdit->append("Stub ready_read.");

    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    if (socket)
    {
        QTextStream ins(socket);
        textEdit->append(ins.readAll());
    }
}

