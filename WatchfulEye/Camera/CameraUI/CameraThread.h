#ifndef STARTCAMERACLASS_H
#define STARTCAMERACLASS_H
#include <QMainWindow>
#include <QRunnable>
#include <QThreadPool>
#include "Camera.h"

class StartCameraClass : public QObject, public QRunnable
{
	Q_OBJECT
private:
	std::function<void()> task;
	Camera& camera = Camera::getInstance();
public:
	StartCameraClass(const std::function<void()>& task);
	StartCameraClass(){}
	~StartCameraClass() {}
	void run() override;
	void startCamera();
	void displayFrames();
	void stoped();
signals:
	void finishedRunCamera();
	void finishedDisplay();
	void toDisplayFrame(QPixmap pixel);
};
#endif // STARTCAMERACLASS_H