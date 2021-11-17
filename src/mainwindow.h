#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QGraphicsScene;
class QGraphicsPixmapItem;
class QActionGroup;
class QTranslator;
class QToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    
    void retranslateUi();

private slots:
    void selectEnglish(bool);
    void selectChinese(bool);

public:
    QMenu* menuLang;
    QAction* actionLangEng;
    QAction* actionLangChs;
    QActionGroup* langGroup;
    QActionGroup* themeGroup;
    QActionGroup* toolboxGroup;
    QAction* actionUndo;
    QAction* actionRedo;
    QAction* actionFitImage;
    QAction* actionFitView;
    QAction* actionFitImage;
    QAction* actionZoomImage11;
    QAction* actionZoomImageIn;
    QAction* actionZoomImageOut;
    QToolBar* mainToolBar;
    std::unique_ptr<QTranslator> chsTranslator;

private:
    Ui::MainWindow *ui;

private:
    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *imageItem = nullptr;
};
#endif // MAINWINDOW_H
