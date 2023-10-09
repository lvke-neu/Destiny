#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::button);
	connect(this, &MainWindow::signalTest, this, &MainWindow::signalTestTrigger);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::button()
{
	ui->label->setText("hahahah");
	emit signalTest();
}

void MainWindow::signalTestTrigger()
{
	ui->label_2->setText("signalTestTrigger");
}
