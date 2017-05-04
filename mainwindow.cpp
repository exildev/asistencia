#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->view = new QQuickView();
    QWidget *container = QWidget::createWindowContainer(this->view, this);
    container->setMinimumSize(200, 200);
    container->setMaximumSize(200, 200);
    container->setFocusPolicy(Qt::TabFocus);
    view->setSource(QUrl("qrc:/main.qml"));
    ui->formLayout->addWidget(container);
    this->setMaximumSize(800,600);
    this->setMinimumSize(800,600);
    this->setWindowTitle("LAPIX: Control de Asistencia");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QQuickView *MainWindow::getView(){
    return this->view;
}
