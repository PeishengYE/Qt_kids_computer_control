#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	MainWindow w;

    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
	w.show();

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "kids desktop warning"));
    // Accepts a single argument.
    parser.addPositionalArgument("timeout", QCoreApplication::translate("main", "the time to shutdown."));

    // -t Timeout / --address
    QCommandLineOption timeOption(QStringList() << "t" << "timeout", QCoreApplication::translate("main", "Specify decimal on timeout (default is 300) 5 min."), QCoreApplication::translate("main", "address"), "300");
    parser.addOption(timeOption);

    parser.process(app);
    // Get address option. TODO: Could add checking here that address is a valid number in the correct range.
       int timeout = parser.value(timeOption).toInt();
    w.setInitSeconds(timeout);
    return app.exec();
}
