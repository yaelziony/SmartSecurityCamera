#include "CameraThread.h"

StartCameraClass::StartCameraClass(const std::function<void()>& task)
	: task(task){}

void StartCameraClass::run()
{
	task();
}

void StartCameraClass::startCamera()
{
	camera.runCamera();
	emit finishedRunCamera();
}

void StartCameraClass::displayFrames()
{
	camera.setCurrentFrame(cv::imread("../../CameraUI/pictures/black.png"));
	QImage img;
	cv::Mat currentFrame;
	cv::Mat frame;
	while (!camera.getCurrentFrame().empty() && camera.getStopCamera() != 27) {
		currentFrame = camera.getCurrentFrame();
		cv::resize(currentFrame, frame, cv::Size(751, 471));
		if (frame.type() == CV_8UC1)
		{
			// Set the color table (used to translate colour indexes to qRgb values)
			QVector<QRgb> colorTable;
			for (int i = 0; i < 256; i++)
				colorTable.push_back(qRgb(i, i, i));
			const uchar* qImageBuffer = (const uchar*)frame.data;
			img = QImage(qImageBuffer, frame.cols, frame.rows, frame.step, QImage::Format_Indexed8);
			img.setColorTable(colorTable);
		}
		else {
			// Copy input Mat
			const uchar* qImageBuffer = (const uchar*)frame.data;
			// Create QImage with same dimensions as input Mat
			img = QImage(qImageBuffer, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
			img = img.rgbSwapped();
		}
		QPixmap pixel = QPixmap::fromImage(img);
		emit toDisplayFrame(pixel);
	}
	emit finishedDisplay();
}

void StartCameraClass::stoped()
{
	camera.setStopCamera(27);
}