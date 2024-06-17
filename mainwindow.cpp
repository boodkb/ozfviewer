#include "mainwindow.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

#include <ozf/exceptions/exception.h>
#include <ozf/ozf_stream.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    _mapView = new MapView(this);
    setCentralWidget(_mapView);

    _toolBar = new QToolBar(this);
    _toolBar->setMovable(false);
    _toolBar->setOrientation(Qt::Vertical);
    addToolBar(Qt::TopToolBarArea, _toolBar);

    _openButton = new QPushButton("Open", this);
    _toolBar->addWidget(_openButton);
    connect(_openButton, &QPushButton::clicked, this, &MainWindow::askForFilename);

    _zoomInButton = new QPushButton("ZoomIn", this);
    _toolBar->addWidget(_zoomInButton);
    connect(_zoomInButton, &QPushButton::clicked, _mapView, &MapView::zoomIn);

    _zoomOutButton = new QPushButton("ZoomOut", this);
    _toolBar->addWidget(_zoomOutButton);
    connect(_zoomOutButton, &QPushButton::clicked, _mapView, &MapView::zoomOut);

    _toolBar->addSeparator();
    _zoomLabel = new QLabel(this);
    _toolBar->addWidget(_zoomLabel);
    connect(_mapView, &MapView::zoomChanged, [this](double zoom) {
        _zoomLabel->setText(QString("Zoom: %1").arg(zoom));
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::askForFilename()
{
    const QString filename
        = QFileDialog::getOpenFileName(this, "Open file", QString(),
            "OZF Images(*.ozf *.ozf2 *.ozf3 *.ozfx3 *.ozf4)");

    if (filename.isEmpty())
        return;

    openFile(filename);
}

void MainWindow::openFile(const QString& filename) try {
    auto stream = std::make_unique<ozf::ozf_stream>(filename.toStdString());
    _mapView->setSource(std::move(stream));
} catch (const ozf::exception& e) {
    QMessageBox::warning(this, "Could not open file", e.what());
}
