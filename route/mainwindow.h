#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "routing.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
        void reDraw(void);
        void savePic(void);
        void zoomIn(void);
        void zoomOut(void);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void DrawEvent();
    void setLines(vector<vector<Line>>);
    void setPinList(vector<vector<int>>);
    void settrackCount(int);


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    vector<vector<Line>> Lines;
    bool *check;
    vector<vector<int>> PinList;
    void addText(QString,int, int, int);
    void autoAdjust();
    int trackCount;


};
#endif // MAINWINDOW_H
