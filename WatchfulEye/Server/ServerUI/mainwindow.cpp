#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	style();
	//on click
	connect(ui->startServerBtn, &QPushButton::clicked, this, &MainWindow::on_startServerBtn_clicked);
	connect(ui->stopServerBtn, &QPushButton::clicked, this, &MainWindow::on_stopServerBtn_clicked);
	connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &MainWindow::cleanupOnExit);
}

MainWindow::~MainWindow()
{
	on_stopServerBtn_clicked();
	QThreadPool::globalInstance()->waitForDone();
	delete ui;
}

void MainWindow::on_startServerBtn_clicked() {
	if (!runServerThreadRunning && !displayThreadRunning) {
		ui->label_running->setText("Running the server...");
		ui->stopServerBtn->setVisible(true);
		ui->label_display_frame->setVisible(true);
		runServerThreadRunning = true;
		displayThreadRunning = true;

		StartServerClass* startServerInstance = new StartServerClass();
		if(!startServerTask)
		startServerTask = std::make_unique<StartServerClass>([startServerInstance]() { startServerInstance->startServer(); });
		if(!displayTheDetectionTask)
		displayTheDetectionTask = std::make_unique<StartServerClass>([startServerInstance]() { startServerInstance->displayTheDetection(); });
		
		connect(startServerInstance, &StartServerClass::toDisplayTheDetection, this, &MainWindow::onDisplayTheDetection);
		connect(startServerInstance, &StartServerClass::finishedRunServer, this, &MainWindow::onFinishedRunServer);
		connect(startServerInstance, &StartServerClass::finishedDisplay, this, &MainWindow::onFinishedDisplay);

		QThreadPool::globalInstance()->start(startServerTask.release());
		QThreadPool::globalInstance()->start(displayTheDetectionTask.release());
	}
}

void MainWindow::on_stopServerBtn_clicked()
{
	StartServerClass* toStopServer = new StartServerClass();
	toStopServer->stoped();
	ui->label_running->setText("stops...");
	ui->stopServerBtn->setVisible(false);
}

void MainWindow::onDisplayTheDetection(QPixmap pixel)
{
	ui->label_display_frame->setPixmap(pixel);
}

void MainWindow::onFinishedRunServer()
{
	ui->stopServerBtn->setVisible(false);
	runServerThreadRunning = false;
	ui->label_running->setText("The server is finished!");
}

void MainWindow::onFinishedDisplay()
{
	ui->label_display_frame->setVisible(false);
	displayThreadRunning = false;
}

void MainWindow::cleanupOnExit()
{
	on_stopServerBtn_clicked();
	QThreadPool::globalInstance()->waitForDone();
}

void MainWindow::style()
{
	setWindowTitle("Server");
	ui->stopServerBtn->setVisible(false);
	ui->label_running->setStyleSheet("color: white");
	QPixmap pix("../../ServerUI/pictures/background.png");
	pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
	QPalette palette;
	palette.setBrush(QPalette::Window, pix);
	this->setPalette(palette);
	ui->startServerBtn->setStyleSheet("background-color: transparent");
	ui->stopServerBtn->setStyleSheet("QPushButton {"
		"color: white;"
		"background-color: transparent;"
		"border-style: none;"
		"border: 1px solid #9c447c;"
		"border-radius: 19.5px;"
		"}"
		"QPushButton:hover {"
		"border: 2px solid #712f59;"
		"text-shadow: 0 0 4px #000000cc;"
		"font-weight: 700;"
		"}");
}
