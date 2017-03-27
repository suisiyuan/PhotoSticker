#include "widget.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("XiaoAnBao");
    QCoreApplication::setApplicationName("PhotoSticker");
    QCoreApplication::setApplicationVersion(App_Version);


    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString(":/PhotoSticker_") + locale);
    a.installTranslator(&translator);

    Widget w;
    w.show();

    return a.exec();
}
