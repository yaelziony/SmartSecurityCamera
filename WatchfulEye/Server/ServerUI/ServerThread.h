#ifndef STARTSERVERCLASS_H
#define STARTSERVERCLASS_H

#include <QMainWindow>
#include <QRunnable>
#include <QThreadPool>
#include "Server.h"

class StartServerClass : public QObject, public QRunnable
{
	Q_OBJECT
private:
	std::function<void()> task;
	Server& server = Server::getInstance();
public:
	StartServerClass(const std::function<void()>& task);
	StartServerClass() {}
	~StartServerClass() {}
	void run() override;
	void startServer();
	void displayTheDetection();
	void stoped();
signals:
	void finishedRunServer();
	void finishedDisplay();
	void toDisplayTheDetection(QPixmap pixel);
};
#endif // STARTSERVERCLASS_H