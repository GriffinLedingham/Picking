#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include<QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    /**
      * we use this slot to trigger our widget's redraw function
      * with the timer
      */
    void drawOpenGL();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    
private slots:

private:
    Ui::MainWindow *ui;

    QTimer * animationTimer;        /**< this is the only member so far added to the auto-generated mainwindow class. We use the timer for regular OpenGL redraws. */
};

#endif // MAINWINDOW_H
