#include "ServerThread.h"

StartServerClass::StartServerClass(const std::function<void()>& task)
	: task(task) {}

void StartServerClass::run()
{
	task();
}

void StartServerClass::startServer()
{
	server.runServer();
	emit finishedRunServer();
}

void StartServerClass::displayTheDetection()
{
	server.setDetectedFrame(cv::imread("../../ServerUI/pictures/black.png"));
	QImage img;
	cv::Mat detected;
	cv::Mat frame;
	server.setCh(0);
	while (server.getCh() != 27) {
		detected = server.getDetectedFrame();
		cv::resize(detected, frame, cv::Size(501, 341));
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
		emit toDisplayTheDetection(pixel);
	}
	emit finishedDisplay();
}

void StartServerClass::stoped()
{
	server.setCh(27);
}
