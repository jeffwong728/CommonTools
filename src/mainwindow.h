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
    bool eventFilter(QObject* object, QEvent* event) override;

private slots:
    void selectEnglish(bool);
    void selectChinese(bool);
    void fitImageView(bool);

public:
    QMenu* menuLang;
    QAction* actionLangEng;
    QAction* actionLangChs;
    QActionGroup* langGroup;
    QActionGroup* themeGroup;
    QActionGroup* toolboxGroup;
    std::unique_ptr<QTranslator> chsTranslator;

private:
    Ui::MainWindow *ui;

private:
    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *imageItem = nullptr;
};
#endif // MAINWINDOW_H
