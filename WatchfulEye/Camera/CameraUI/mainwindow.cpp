#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	style();
	//on click
	connect(ui->startCameraBtn, &QPushButton::clicked, this, &MainWindow::on_startCamera_clicked);
	connect(ui->stopCameraBtn, &QPushButton::clicked, this, &MainWindow::on_stopCamera_clicked);
	connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &MainWindow::cleanupOnExit);
}

MainWindow::~MainWindow()
{
	on_stopCamera_clicked();
	QThreadPool::globalInstance()->waitForDone();
	delete ui;
}

void MainWindow::on_startCamera_clicked() {
	if (!runCameraThreadRunning && !displayThreadRunning) {
		ui->stopCameraBtn->setVisible(true);
		ui->label_display_frames->setVisible(true);
		runCameraThreadRunning = true;
		displayThreadRunning = true;
		
		StartCameraClass* startCameraInstance = new StartCameraClass();
		if(!startCameraTask)
		startCameraTask = std::make_unique<StartCameraClass>([startCameraInstance]() { startCameraInstance->startCamera(); });
		if(!displayFramesTask)
		displayFramesTask = std::make_unique<StartCameraClass>([startCameraInstance]() { startCameraInstance->displayFrames(); });

		connect(startCameraInstance, &StartCameraClass::toDisplayFrame, this, &MainWindow::onDisplayFrame);
		connect(startCameraInstance, &StartCameraClass::finishedRunCamera, this, &MainWindow::onFinishedCamera);
		connect(startCameraInstance, &StartCameraClass::finishedDisplay, this, &MainWindow::onFinishedDisplay);
		
		QThreadPool::globalInstance()->start(startCameraTask.release());
		QThreadPool::globalInstance()->start(displayFramesTask.release());
	}
}

void MainWindow::on_stopCamera_clicked()
{
	StartCameraClass* toStopCamera = new StartCameraClass();
	toStopCamera->stoped();
	ui->stopCameraBtn->setVisible(false);
}

void MainWindow::onDisplayFrame(QPixmap pixel)
{
	ui->label_display_frames->setPixmap(pixel);
	//ui->label_background->stackUnder(ui->label_display_frames);
}

void MainWindow::onFinishedCamera()
{
	ui->stopCameraBtn->setVisible(false);
	runCameraThreadRunning = false;
}

void MainWindow::onFinishedDisplay()
{
	ui->label_display_frames->setVisible(false);
	displayThreadRunning = false;
}

void MainWindow::cleanupOnExit()
{
	on_stopCamera_clicked();
	QThreadPool::globalInstance()->waitForDone();
}

void MainWindow::style()
{
	setWindowTitle("Camera");
	QMovie* movie = new QMovie("../../CameraUI/pictures/background2.gif");
	movie->setScaledSize(QSize(800, 600));
	ui->label_background->setMovie(movie);
	movie->start();
	ui->label_background->stackUnder(ui->startCameraBtn);
	ui->label_background->stackUnder(ui->stopCameraBtn);
	ui->label_background->stackUnder(ui->label_name);
	ui->stopCameraBtn->setVisible(false);
}
