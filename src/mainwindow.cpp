#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets>
#include <QtOpenGL>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    QGraphicsRectItem *item1 = new QGraphicsRectItem(0, 0, 100, 100);
    item1->setBrush(QBrush(Qt::transparent));
    item1->setPen(QPen(Qt::red));
    item1->setFlag(QGraphicsItem::ItemIsMovable);
    item1->setFlag(QGraphicsItem::ItemIsSelectable);
    item1->setZValue(1);
    scene->addItem(item1);

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
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
