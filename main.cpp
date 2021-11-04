#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
//    QRect screenGeometry = QApplication::desktop()->screen();
//    int x = (screenGeometry.width()- w.width()) / 2;
//    int y = (screenGeometry.height()- w.height()) / 2;
    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
	w.show();

	return a.exec();
}
