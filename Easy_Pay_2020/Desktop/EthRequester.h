#ifndef ETHREQUESTER_H
#define ETHREQUESTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMap>
#include "bigInt.h"

enum EthMethod {
    getBalanceEth = 0,
    getBalance = 1,
    buyToken = 2,
    getTokenRate = 3,
    getDecimals = 4,
    getTokenRateShift = 5,
    sellToken = 6,
    transferToken = 7,
    unlockAccount = 8
};
class EthReply : public QObject {
    Q_OBJECT
    Q_ENUM(EthMethod)
public:
    EthReply(QNetworkReply* _reply, EthMethod _method, QObject* _parant);
    ~EthReply();
private:
    QNetworkReply* reply;
    EthMethod method;

    QString parseNumberTo10(QString number);
    QString parseAddressTo10(QString address);
signals:
    void endRequest(QString& response);
    void endRequestWithoutData();
private slots:
    void parseReply ();
};

class EthRequester : public QObject {
    Q_OBJECT
public:
    EthRequester(QUrl _url, QString _contractAddress, QObject* _parent);
    ~EthRequester();
    EthReply* sendRequest (EthMethod method, QString* parameters);
private:
    QString parseNumberTo16(QString number);
    QString parseNumberTo16WithoutZero(QString number);
    QString parseAddressTo16(QString address);
    QByteArray makeRequestData (EthMethod method, QString* parameters);

    QNetworkAccessManager* manager;
    QMap<EthMethod, QString> methodInd;
    QNetworkRequest request;
    QString contractAddress;
};

#endif // ETHREQUESTER_H
