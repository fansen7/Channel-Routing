#include "mainwindow.h"
#include <QGraphicsTextItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    trackCount = 0;
    check = new bool[4];
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    connect(ui->checkBox,SIGNAL(clicked(bool)), this, SLOT(reDraw(void)));
    connect(ui->checkBox_2,SIGNAL(clicked(bool)), this, SLOT(reDraw(void)));
    connect(ui->checkBox_3,SIGNAL(clicked(bool)), this, SLOT(reDraw(void)));
    connect(ui->checkBox_via,SIGNAL(clicked(bool)), this, SLOT(reDraw(void)));
    connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(savePic(void)));
    connect(ui->pushButton_plus,SIGNAL(clicked()), this, SLOT(zoomIn(void)));
    connect(ui->pushButton_minus,SIGNAL(clicked()), this, SLOT(zoomOut(void)));

    //    ui->graphicsView->resize(500, 100);
    //    QBrush greenBrush(Qt::green);
    //    QBrush blueBrush(Qt::blue);
    //    QPen outlinePen(Qt::white);
    //    outlinePen.setWidth(5);
    //    scene->addLine(0,0,100,300,outlinePen);

}


void MainWindow::DrawEvent(){

    QBrush blackBrush(Qt::black);
    QPen outlinePen(Qt::white);
    int penWidth = 10;
    outlinePen.setWidth(penWidth);

    QColor colors[3] = {
        QColor(255,255,255,192),
        QColor(255,0,0,192),
        QColor(0,0,255,192),
    };

    int order[3] = {1, 3, 2};
    int offset = penWidth / 2;
    int rate = penWidth * 10;
    int fontSize = penWidth  * 6 ;
    if(PinList[0].size() > 1000) fontSize /= 2;

    if(trackCount > 0){
        scene->addRect(-rate / 2,   // draw Bounding Box
                       rate / 2,
                       PinList[0].size() * rate,
                       (trackCount) * rate,
                       QPen(QColor(105,105,105,128), 8),
                       QBrush(Qt::black));
    }

    for(int i = 0; i < PinList[0].size();i++){ //Pin Number Text
        addText(QString::number(PinList[0][i]),i * rate, -1 * rate, fontSize);
    }
    for(int i = 0; i <(int) PinList[1].size();i++){ //Pin Number Text
        int y = (trackCount + 1) * rate;
        addText(QString::number(PinList[1][i]),i * rate, y, fontSize);
    }
    for (int k = 1;k < (int)Lines.size();++k){ // Line
        int j = order[k - 1];
        outlinePen.setColor(colors[j - 1]);
        if(check[k]){
            for(int i = 0 ;i <(int) Lines[j].size();++i){
                int x1 = Lines[j][i].x1 * rate;
                int y1 = Lines[j][i].y1 * rate;
                int x2 = Lines[j][i].x2 * rate;
                int y2 = Lines[j][i].y2 * rate;
                if (j != 1){
                    x1 += offset;
                    x2 += offset;
                }
                scene->addLine(x1, y1,
                               x2, y2,
                               outlinePen);
            }
        }
    }


    if(check[0]) {
        QPen viaPen(QColor(255,255,255,192));
        viaPen.setWidth(16);
        for(int i = 0;i <Lines[1].size();++i){ // via 1
              int x = Lines[1][i].x2 * rate -  0.05 * rate;
              int y = Lines[1][i].y2 * rate -  0.05 * rate;
              scene->addEllipse(x,
                                y,
                                0.1 * rate,
                                0.1 * rate,
                                viaPen, blackBrush);
        }
        viaPen.setColor(QColor(0 ,0 ,255, 192));
        for(int i = 0;i <Lines[3].size();++i){ // via 3
                int x = Lines[3][i].x1 * rate -  0.05 * rate;
                int y = Lines[3][i].y1 * rate -  0.05 * rate;
                scene->addEllipse(x + offset,
                                  y,
                                  0.1 * rate,
                                  0.1 * rate,
                                  viaPen, blackBrush);
        }
    }
    static bool first = true;
    if(first)
    {
        autoAdjust();
        savePic();
    }
    first = false;

}

void MainWindow::setLines(vector<vector<Line>> Lines_){
    Lines =  Lines_;
}

void MainWindow::setPinList(vector<vector<int>> PinList_){
    PinList = PinList_;
}
void MainWindow::settrackCount(int trackCount_){
    trackCount = trackCount_;
}
void MainWindow::reDraw(){
    check[0] = ui->checkBox_via->isChecked();
    check[1] = ui->checkBox->isChecked();
    check[2] = ui->checkBox_3->isChecked();
    check[3] = ui->checkBox_2->isChecked();

    scene->clear();
    //  cout << "reDraw";
    DrawEvent();
}

void MainWindow::savePic(void){
    const QPixmap pixmap = ui->graphicsView->grab();
    const QString fileName("./route.png");
    pixmap.save(fileName);
}

void MainWindow::zoomIn(void){
    ui->graphicsView->scale(1.10, 1.10);
}

void MainWindow::zoomOut(void){
    ui->graphicsView->scale(0.90, 0.90);
}

void MainWindow::addText(QString content = "",int x = 0, int y = 0, int fontsize = 20){
    QFont QF;
    QF.setPixelSize(fontsize);

    QGraphicsTextItem *Text = scene->addText(content, QF);

    Text->setDefaultTextColor(QColor(Qt::white));
    Text->setPos(x - Text->boundingRect().width() / 2, y);
}

void MainWindow::autoAdjust(){
    int w = scene->width();
    int h = scene->height();
    while(w > 1600 || h > 900){
        zoomOut();
        w *= 0.9;
        h *= 0.9;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

