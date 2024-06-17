#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>

#include "mapview.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    MapView* _mapView;
    QToolBar* _toolBar;
    QPushButton* _openButton;
    QPushButton* _zoomInButton;
    QPushButton* _zoomOutButton;

    QLabel* _zoomLabel;

    void askForFilename();
    void openFile(const QString& filename);
};

#endif // MAINWINDOW_H
