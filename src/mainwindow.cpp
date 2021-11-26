#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets>
#include <QActionGroup>
#include <QOpenGLWidget>
#include "icamera.h"

extern std::pair<std::string, bool> PyRunFile(const std::string& strFullPath);
extern void PyClearOutput();
extern std::string PyGetOutput();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    chsTranslator = std::make_unique<QTranslator>();
    if (chsTranslator->load(":langs/TestQT_zh_CN"))
    {
        qApp->installTranslator(chsTranslator.get());
    }

    ui->setupUi(this);
    themeGroup = new QActionGroup(this);
    themeGroup->addAction(ui->actionDark);
    themeGroup->addAction(ui->actionDark_Orange);
    themeGroup->addAction(ui->actionAdaptic);
    themeGroup->addAction(ui->actionIntegrid);
    themeGroup->addAction(ui->actionDefault);
    themeGroup->setExclusive(true);

    toolboxGroup = new QActionGroup(this);
    toolboxGroup->addAction(ui->actionOutput_Window);
    toolboxGroup->addAction(ui->actionTemplate_Window);
    toolboxGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
    ui->actionOutput_Window->setChecked(false);
    ui->actionTemplate_Window->setChecked(false);
    ui->dockWidget->setVisible(false);
    ui->dockWidget_2->setVisible(false);

    menuLang = new QMenu(ui->menubar);
    menuLang->setObjectName(QString::fromUtf8("menuLanguage"));

    actionLangEng = new QAction(this);
    actionLangEng->setObjectName(QString::fromUtf8("actionLangEng"));
    actionLangEng->setCheckable(true);

    actionLangChs = new QAction(this);
    actionLangChs->setObjectName(QString::fromUtf8("actionLangChs"));
    actionLangChs->setCheckable(true);

    ui->menubar->addAction(menuLang->menuAction());
    menuLang->addAction(actionLangEng);
    menuLang->addAction(actionLangChs);

    langGroup = new QActionGroup(this);
    langGroup->addAction(actionLangEng);
    langGroup->addAction(actionLangChs);
    langGroup->setExclusive(true);
    actionLangChs->setChecked(true);
    retranslateUi();
    ui->menubar->adjustSize();

    connect(actionLangEng, &QAction::toggled, this, &MainWindow::selectEnglish);
    connect(actionLangChs, &QAction::toggled, this, &MainWindow::selectChinese);
    connect(ui->actionZoom_Actual, &QAction::triggered, this, &MainWindow::actualImageView);
    connect(ui->actionZoom_Fit, &QAction::triggered, this, &MainWindow::fitImageView);
    connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::zoomInImageView);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::zoomOutImageView);
    connect(ui->actionZoom_Selection, &QAction::triggered, this, &MainWindow::zoomSelectionImageView);

    ui->graphicsView->setViewport(new QOpenGLWidget());
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->viewport()->installEventFilter(this);

    scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(Qt::gray));
    scene->setSceneRect(0, 0, 800, 600);
    ui->graphicsView->setScene(scene);

    QGraphicsRectItem *item1 = new QGraphicsRectItem(100, 100, 100, 100);
    item1->setBrush(QBrush(Qt::transparent));
    item1->setPen(QPen(Qt::red));
    item1->setFlag(QGraphicsItem::ItemIsMovable);
    item1->setFlag(QGraphicsItem::ItemIsSelectable);
    item1->setZValue(1);
    scene->addItem(item1);

    QGraphicsEllipseItem* item2 = new QGraphicsEllipseItem(150, 150, 100, 80);
    item2->setBrush(QBrush(Qt::transparent));
    item2->setPen(QPen(Qt::red));
    item2->setFlag(QGraphicsItem::ItemIsMovable);
    item2->setFlag(QGraphicsItem::ItemIsSelectable);
    item2->setZValue(1);
    scene->addItem(item2);

    pyOutput = scene->addText(QString());
    pyOutput->setFlag(QGraphicsItem::ItemIsMovable);

    connect(ui->actionImport_Image, &QAction::triggered, [&]()
    {
        QFileDialog dialog(this, tr("Open File"));
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());

        QStringList mimeTypeFilters;
        QFileDialog::AcceptMode acceptMode = QFileDialog::AcceptOpen;
        const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
            ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
        for (const QByteArray &mimeTypeName : supportedMimeTypes)
            mimeTypeFilters.append(mimeTypeName);
        mimeTypeFilters.sort();
        dialog.setMimeTypeFilters(mimeTypeFilters);
        dialog.selectMimeTypeFilter("image/jpeg");
        dialog.setAcceptMode(acceptMode);
        if (acceptMode == QFileDialog::AcceptSave)
            dialog.setDefaultSuffix("jpg");

        if (dialog.exec() == QDialog::Accepted)
        {
            QImageReader reader(dialog.selectedFiles().constFirst());
            reader.setAutoTransform(true);
            const QImage newImage = reader.read();
            if (!newImage.isNull())
            {
                if (!imageItem)
                {
                    imageItem = new QGraphicsPixmapItem(QPixmap::fromImage(newImage));
                    scene->addItem(imageItem);
                    imageItem->setZValue(0);
                }
                else
                {
                    imageItem->setPixmap(QPixmap::fromImage(newImage));
                }

                ui->graphicsView->resetTransform();
                scene->setSceneRect(imageItem->boundingRect());
            }
        }
    });

    connect(ui->actionDefault, &QAction::triggered, [&]()
            { qApp->setStyleSheet(""); });

    connect(ui->actionDark, &QAction::triggered, [&]()
            {
                QFile f(":styles/qdarkstyle");
                if (f.exists())
                {
                    f.open(QFile::ReadOnly | QFile::Text);
                    QTextStream ts(&f);
                    qApp->setStyleSheet(ts.readAll());
                }
            });

    connect(ui->actionDark_Orange, &QAction::triggered, [&]()
            {
                QFile f(":styles/darkorange");
                if (f.exists())
                {
                    f.open(QFile::ReadOnly | QFile::Text);
                    QTextStream ts(&f);
                    qApp->setStyleSheet(ts.readAll());
                }
            });

    connect(ui->actionAdaptic, &QAction::triggered, [&]()
            {
                QFile f(":styles/adaptic");
                if (f.exists())
                {
                    f.open(QFile::ReadOnly | QFile::Text);
                    QTextStream ts(&f);
                    qApp->setStyleSheet(ts.readAll());
                }
            });

    connect(ui->actionIntegrid, &QAction::triggered, [&]()
            {
                QFile f(":styles/integrid");
                if (f.exists())
                {
                    f.open(QFile::ReadOnly | QFile::Text);
                    QTextStream ts(&f);
                    qApp->setStyleSheet(ts.readAll());
                }
            });
}

MainWindow::~MainWindow()
{
    delete themeGroup;
    delete langGroup;
    delete ui;

    if (loader && loader->isLoaded())
    {
        loader->unload();
    }
}

void MainWindow::retranslateUi()
{
    actionLangEng->setShortcut(tr("Alt+E"));
    actionLangEng->setStatusTip(tr("Set UI language to English"));
    actionLangChs->setShortcut(tr("Alt+C"));
    actionLangChs->setStatusTip(tr("Set UI language to Chinese"));
    menuLang->setTitle(tr("Language"));
    actionLangEng->setText(tr("English"));
    actionLangChs->setText(tr("Chinese"));
}

bool MainWindow::eventFilter(QObject* obj, QEvent* evt)
{
    if (ui->graphicsView->viewport() == obj && QEvent::MouseButtonPress == evt->type())
    {
        QMouseEvent* me = (QMouseEvent*)evt;
        if (me->button() == Qt::LeftButton)
        {
            qDebug() << "position: " << me->position() << " pos" << me->pos() << "globalPosition" << me->globalPosition();
        }
    }
    return QMainWindow::eventFilter(obj, evt);
}

void MainWindow::selectEnglish(bool)
{
    if (actionLangEng->isChecked())
    {
        qApp->removeTranslator(chsTranslator.get());
        ui->retranslateUi(this);
        retranslateUi();
    }
}

void MainWindow::selectChinese(bool)
{
    if (actionLangChs->isChecked())
    {
        qApp->installTranslator(chsTranslator.get());
        ui->retranslateUi(this);
        retranslateUi();
    }
}

void MainWindow::actualImageView(bool)
{
    if (imageItem)
    {
        qDebug() <<"pos: "<< imageItem->pos() << "bounding rect: " << imageItem->boundingRect();
        ui->graphicsView->resetTransform();
        scene->setSceneRect(imageItem->boundingRect());
    }
}

void MainWindow::fitImageView(bool)
{
    if (imageItem)
    {
        ui->graphicsView->fitInView(imageItem, Qt::KeepAspectRatio);
    }
}

void MainWindow::zoomInImageView(bool)
{
    std::string pyFilePath("C:\\Users\\wwang\\Desktop\\qt.py");
    auto res = PyRunFile(pyFilePath);
    QString text= QString::fromStdString(res.first);
    if (res.second)
    {
        pyOutput->setPlainText(text);
    }
    else
    {
        pyOutput->setPlainText(text);
    }
}

void MainWindow::zoomOutImageView(bool)
{
    if (!loader)
    {
        QDir pluginsDir(QCoreApplication::applicationDirPath());
        loader = new QPluginLoader(pluginsDir.absoluteFilePath(QString("videofile_plugin_d.dll")), this);
        if (loader)
        {
            loader->load();
            if (loader->isLoaded())
            {
                QObject* plugin = loader->instance();
                if (plugin) {
                    ICamera* pICam = qobject_cast<ICamera*>(plugin);
                    if (pICam)
                    {
                        connect(pICam, &ICamera::image_ready, this, &MainWindow::on_image_ready);
                    }
                }
            }
        }
    }
}

void MainWindow::zoomSelectionImageView(bool)
{
    if (loader && loader->isLoaded())
    {
        QObject* plugin = loader->instance();
        if (plugin) {
            ICamera* pICam = qobject_cast<ICamera*>(plugin);
            if (pICam)
            {
                pICam->snap();
            }
        }
    }
}

void MainWindow::on_image_ready(QImage)
{
    qDebug() << "Image ready from camera";
}
