#include "forest.h"
#include "ui_forest.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    mainWindow z;

   z.resize(1000,480);

   z.show();
    return a.exec();

}
