#include "mainwindow.h"
#include "ui_mainwindow.h"

bool mFor, mBack, mLeft, mRight, cUp, cDown, cLeft, cRight, cJump, cFall, spaceDown, delDown;
extern float cameraZ;
extern float cameraX;
extern float cameraY;
extern float cameraRotHor;
extern float cameraRotVer;
int camView;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    animationTimer(NULL)
{
    ui->setupUi(this);

    // we create a simple timer, with the widget being its parent
    animationTimer = new QTimer(this);
    // the timer will send a signal timeout at regular intervals.
    // whenever this timeout signal occurs, we want it to call our drawOpenGL
    // function
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(drawOpenGL()));
    // we start the timer with a timeout interval of 20ms
    animationTimer->start(20);
}

void MainWindow::drawOpenGL()
{
    // whenever drawOpenGL gets called, we only want to repaint our opengl widget
    ui->oglwidget->repaint();
    //ui->oglwidget->animateGL();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->text() =="w")
    {
        mFor = true;
    }

    if(event->text() == "s")
    {
        mBack = true;
    }

    if(event->text() =="a")
    {
        mLeft = true;
    }

    if(event->text() == "d")
    {
        mRight = true;
    }

    if(event->text() == "-") //down
    {
        cFall = true;
    }

    if(event->text() == "=") //up
    {
        cJump = true;
    }

    if(event->key() == Qt::Key_Left) //rot left
    {
        cLeft = true;
    }

    if(event->key() == Qt::Key_Right) //rot right
    {
        cRight = true;
    }

    if(event->key() == Qt::Key_Up) //rot up
    {
        cUp = true;
    }
    if(event->key() == Qt::Key_Space) //rot up
    {
        spaceDown = true;
    }
    if(event->key() == Qt::Key_Backspace) //rot up
    {
        delDown = true;
    }

    if(event->key() == Qt::Key_Down) //rot down
    {
        cDown = true;
    }

    if(event->text() == "r")
    {
        cameraRotHor = 0.0f;
        cameraRotVer = 10.0f;
        cameraX = 0.0f;
        cameraY = -3.0f;
        cameraZ = -10.0f;
    }


}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Space) //rot up
    {
        spaceDown = false;
    }
    if(event->key() == Qt::Key_Backspace) //rot up
    {
        delDown = false;
    }
    if(event->text() == "w")
    {
        mFor = false;
    }

    if(event->text() == "s")
    {
        mBack = false;
    }

    if(event->text() == "a")
    {
        mLeft = false;
    }

    if(event->text() == "d")
    {
        mRight = false;
    }

    if(event->text() == "-") //down
    {
        cFall = false;
    }

    if(event->text() == "=") //up
    {
        cJump = false;
    }

    if(event->key() == Qt::Key_Left) //rot left
    {
        cLeft = false;
    }

    if(event->key() == Qt::Key_Right) //rot right
    {
        cRight = false;
    }

    if(event->key() == Qt::Key_Up) //rot up
    {
        cUp = false;
    }

    if(event->key() == Qt::Key_Down) //rot down
    {
        cDown = false;
    }
}

