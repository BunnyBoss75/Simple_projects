#include "EthRequester.h"

EthRequester::EthRequester(QUrl _url, QString _contractAddress, QObject* _parent):
                               manager(new QNetworkAccessManager),
                               request(_url),
                               contractAddress(_contractAddress) {
    request.setRawHeader("Content-Type","application/json");

    methodInd.insert(EthMethod::getBalance, "0xe1ef806b");
    methodInd.insert(EthMethod::buyToken, "0xf7837d44");
    methodInd.insert(EthMethod::getTokenRate, "0x5cf4c97a");
    methodInd.insert(EthMethod::getTokenRateShift, "0x7b672ee6");
    methodInd.insert(EthMethod::getDecimals, "0x313ce567");
    methodInd.insert(EthMethod::sellToken, "0xbb5daf8c");
    methodInd.insert(EthMethod::transferToken, "0xa9059cbb");

    QObject::setParent(_parent);
}

EthRequester::~EthRequester() {
    delete manager;
}

EthReply* EthRequester::sendRequest(EthMethod method, QString* parameters) {
    QByteArray requestData = makeRequestData(method, parameters);
    return new EthReply(manager->post(request, requestData), method, QObject::parent());
}

QString EthRequester::parseAddressTo16(QString address) {
    QString res(24, '0');
    res.append(address.mid(2, 40));
    return res;
}

QString EthRequester::parseNumberTo16(QString number) {
    BigInt::Vin temp(number.toStdString(), 10);
    QString data = QString::fromStdString(temp.toStrHex());
    QString result(64 - data.size(), '0');
    result += data;
    return result;
}
QString EthRequester::parseNumberTo16WithoutZero(QString number){
    BigInt::Vin temp(number.toStdString(), 10);
    return QString::fromStdString(temp.toStrHex());
}
QString EthReply::parseAddressTo10(QString address) {
    address = address.mid(24);
    address.prepend("0x");
    return address;
}

QString EthReply::parseNumberTo10(QString number) {
    BigInt::Vin temp(number.toStdString(), 16);
    return QString::fromStdString(temp.toStrDec());
}

QByteArray EthRequester::makeRequestData(EthMethod method, QString *parameters) {
    QJsonObject result;
    result.insert("jsonrpc", "2.0");
    result.insert("id", "1");
    QJsonArray temp;
    QJsonObject temp1;
    QString tempData;
    switch (method) {
    case unlockAccount:
        result.insert("method", "personal_unlockAccount");
        temp.append(parameters[0]);
        temp.append(parameters[1]);
        temp.append(5);
        result.insert("params", temp);
        break;
    case sellToken:
        result.insert("method", "eth_sendTransaction");
        temp1.insert("to", contractAddress);
        tempData += methodInd[sellToken];
        tempData += parseNumberTo16(parameters[1]);
        temp1.insert("data", tempData);
        temp1.insert("from", parameters[0]);
        temp1.insert("gas", "0xfffff");
        temp.insert(0, temp1);
        result.insert("params", temp);
        break;
    case transferToken:
        result.insert("method", "eth_sendTransaction");
        temp1.insert("to", contractAddress);
        tempData += methodInd[transferToken];
        tempData += parseAddressTo16(parameters[1]);
        tempData += parseNumberTo16(parameters[2]);
        temp1.insert("data", tempData);
        temp1.insert("from", parameters[0]);
        temp1.insert("gas", "0xfffff");
        temp.insert(0, temp1);
        result.insert("params", temp);
        break;
    case getDecimals:
        result.insert("method", "eth_call");
        temp1.insert("to", contractAddress);
        tempData += methodInd[getDecimals];
        temp1.insert("data", tempData);
        temp.insert(0, temp1);
        temp.insert(1, "latest");
        result.insert("params", temp);
        break;
    case getTokenRateShift:
        result.insert("method", "eth_call");
        temp1.insert("to", contractAddress);
        tempData += methodInd[getTokenRateShift];
        temp1.insert("data", tempData);
        temp.insert(0, temp1);
        temp.insert(1, "latest");
        result.insert("params", temp);
        break;
    case getBalanceEth:
        result.insert("method", "eth_getBalance");
        temp.append(parameters[0]);
        temp.append("latest");
        result.insert("params", temp);
        break;
    case getBalance:
        result.insert("method", "eth_call");
        temp1.insert("to", contractAddress);
        tempData += methodInd[getBalance];
        tempData += parseAddressTo16(parameters[0]);
        temp1.insert("data", tempData);
        temp.insert(0, temp1);
        temp.insert(1, "latest");
        result.insert("params", temp);
        break;
    case buyToken:
        result.insert("method", "eth_sendTransaction");
        temp1.insert("to", contractAddress);
        tempData += methodInd[buyToken];
        temp1.insert("data", tempData);
        temp1.insert("from", parameters[0]);
        temp1.insert("gas", "0xfffff");
        temp1.insert("value", parseNumberTo16WithoutZero(parameters[1]).prepend("0x"));
        temp.insert(0, temp1);
        result.insert("params", temp);
        break;
    case getTokenRate:
        result.insert("method", "eth_call");
        temp1.insert("to", contractAddress);
        tempData += methodInd[getTokenRate];
        temp1.insert("data", tempData);
        temp.insert(0, temp1);
        temp.insert(1, "latest");
        result.insert("params", temp);
        break;
    }
    return QJsonDocument(result).toJson();
}
void EthReply::parseReply() {
    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
    QString response;
    switch (method) {
    case EthMethod::getTokenRateShift:
    case EthMethod::getDecimals:
    case EthMethod::getBalance:
    case EthMethod::getTokenRate:
    case EthMethod::getBalanceEth:
        response = parseNumberTo10(data["result"].toString().mid(2));
        endRequest(response);
        break;
    default:
        endRequestWithoutData();
        break;
    }
    delete this;
}

EthReply::EthReply(QNetworkReply* _reply, EthMethod _method, QObject* _parent): reply(_reply),
                                                                                method(_method) {
    QObject::connect(reply, &QNetworkReply::finished, this, &EthReply::parseReply);
    QObject::setParent(_parent);
}

EthReply::~EthReply() {
    reply->deleteLater();
}
