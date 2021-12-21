#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProcess>
#include <QLocalServer>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void load();
    void on_ready_read();
    void newConnection();
    void disconnected();

private:
    QProcess* mProcess = nullptr;
    QLocalServer* mServer = nullptr;
};
#endif // DIALOG_H
