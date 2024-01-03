#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include "CameraThread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
private:
	bool runCameraThreadRunning = false;
	bool displayThreadRunning = false;
	std::unique_ptr<StartCameraClass> startCameraTask = nullptr;
	std::unique_ptr<StartCameraClass> displayFramesTask = nullptr;
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
private slots:
	void on_startCamera_clicked();
	void on_stopCamera_clicked();
	void style();
	void onDisplayFrame(QPixmap pixel);
	void onFinishedCamera();
	void onFinishedDisplay();
	void cleanupOnExit();
private:
	Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H