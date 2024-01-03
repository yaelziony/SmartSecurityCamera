#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ServerThread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    bool runServerThreadRunning = false;
    bool displayThreadRunning = false;
    std::unique_ptr<StartServerClass> startServerTask = nullptr;
    std::unique_ptr<StartServerClass> displayTheDetectionTask = nullptr;
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
private slots:
    void on_startServerBtn_clicked();
    void on_stopServerBtn_clicked();
    void onDisplayTheDetection(QPixmap pixel);
    void onFinishedRunServer();
    void onFinishedDisplay();
    void cleanupOnExit();
    void style();
private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
