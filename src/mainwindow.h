#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QGraphicsScene;
class QGraphicsPixmapItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private:
    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *imageItem = nullptr;
};
#endif // MAINWINDOW_H
