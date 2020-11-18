#include "mainwindow.h"

#include <QApplication>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QThread>
#include "EthRequester.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
