#include "routing.cpp"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    string filename = argv[1];
    //  string filename = "../conf/60.pin";

    Routing pinList(filename);
    pinList.Read();
    pinList.SetTrunks();
    pinList.LeftEdge();
    vector<vector<Line>> Lines = pinList.to_coordinate();
    vector<vector<int>> PinList = pinList.getPinList();

    cout << "Number of tracks: " << pinList.trackCount()<<endl;

    MainWindow w;
    w.setLines(Lines);
    w.setPinList(PinList);
    w.settrackCount(pinList.trackCount());
    w.DrawEvent();
    w.show();

    return a.exec();
}
