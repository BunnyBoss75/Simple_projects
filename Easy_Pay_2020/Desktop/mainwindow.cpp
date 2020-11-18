#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QFile>
#include <QMap>
#include <QMetaEnum>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {

    QFile configFile("config.json");
    if (configFile.exists() && configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject data = QJsonDocument::fromJson(configFile.readAll()).object();
        QUrl url = data["params"].toObject()["url"].toString();
        QString contractAddress = data["params"].toObject()["contractAddress"].toString();
        ethRequester = new EthRequester(url, contractAddress, this);
        timeBetweenUpdate = data["params"].toObject()["timeBetweenUpdate"].toString().toInt();
    }
    else {
        ethRequester = new EthRequester(QUrl("http://127.0.0.1:8545/"), "0xe578f2cf7d7ea7e9329ad399a7e9d4eadc2a967b", this);
        timeBetweenUpdate = 10000;
    }

    ui->setupUi(this);

    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(timeBetweenUpdate);
}

MainWindow::~MainWindow()
{
    delete ethRequester;
    delete ui;
}

void MainWindow::on_balanceEthButton_clicked() {
    setWindowEnable(false);
    updateEthBalance();
    setWindowEnable(true);
}

void MainWindow::on_balanceButton_clicked() {
    setWindowEnable(false);
    updateBalance();
    setWindowEnable(true);
}

void MainWindow::on_buyTokenButton_clicked() {
    setWindowEnable(false);
    EthReply* ethReply = unlockAccount();
    QObject::connect(ethReply, &EthReply::endRequestWithoutData, this, &MainWindow::buyToken);
}
void MainWindow::buyToken() {
    QString* parameters = new QString[2];
    parameters[0] = ui->accountLineEdit->text();
    parameters[1] = ui->buyTokenLineEdit->text();
    EthReply* ethReply = ethRequester->sendRequest(EthMethod::buyToken, parameters);
    QObject::connect(ethReply, &EthReply::endRequestWithoutData, this, &MainWindow::update);
    delete[] parameters;
    setWindowEnable(true);
}
void MainWindow::setWindowEnable(bool flag) {
    ui->balanceButton->setEnabled(flag);
    ui->buyTokenButton->setEnabled(flag);
    ui->balanceEthButton->setEnabled(flag);
    ui->tokenRateButton->setEnabled(flag);
    ui->sellTokenLineEdit->setEnabled(flag);
    ui->transferButton->setEnabled(flag);
    ui->accountLineEdit->setEnabled(flag);
    ui->balanceLineEdit->setEnabled(flag);
    ui->sellTokenButton->setEnabled(flag);
    ui->buyTokenLineEdit->setEnabled(flag);
    ui->passwordLineEdit->setEnabled(flag);
    ui->tokenRateLineEdit->setEnabled(flag);
    ui->balanceEthLineEdit->setEnabled(flag);
    ui->transferToLineEdit->setEnabled(flag);
    ui->transferValueLineEdit->setEnabled(flag);
}

void MainWindow::on_tokenRateButton_clicked() {
    setWindowEnable(false);
    updateTokenRate();
    setWindowEnable(true);
}
void MainWindow::updateBalance() {
    QString* parameters = new QString(ui->accountLineEdit->text());
    EthReply* ethReply = ethRequester->sendRequest(EthMethod::getBalance, parameters);
    QObject::connect(ethReply, &EthReply::endRequest, ui->balanceLineEdit, &QLineEdit::setText);
    QObject::connect(ethReply, &EthReply::endRequest, this, &MainWindow::specifyBalance);
    delete parameters;
}
void MainWindow::specifyBalance() {
    EthReply* ethReply = ethRequester->sendRequest(EthMethod::getDecimals, nullptr);
    QObject::connect(ethReply, &EthReply::endRequest, this, &MainWindow::setSpecifyBalance);
}
void MainWindow::setSpecifyBalance(QString& response) {
    if (response.toInt() == 0) {
        return;
    }
    QString temp = ui->balanceLineEdit->text();
    int size = temp.size();
    temp.insert(size - response.toInt(), ',');
    ui->balanceLineEdit->setText(temp);
}
void MainWindow::updateTokenRate() {
    EthReply* ethReply = ethRequester->sendRequest(EthMethod::getTokenRate, nullptr);
    QObject::connect(ethReply, &EthReply::endRequest, ui->tokenRateLineEdit, &QLineEdit::setText);
    QObject::connect(ethReply, &EthReply::endRequest, this, &MainWindow::specifyTokenRate);
}
void MainWindow::specifyTokenRate() {
    EthReply* ethReply = ethRequester->sendRequest(EthMethod::getTokenRateShift, nullptr);
    QObject::connect(ethReply, &EthReply::endRequest, this, &MainWindow::setSpecifyTokenRate);
}
void MainWindow::setSpecifyTokenRate(QString& response) {
    if (response.toInt() == 0) {
        return;
    }
    QString temp = ui->tokenRateLineEdit->text();
    int size = temp.size();
    temp.insert(size - response.toInt(), ',');
    ui->tokenRateLineEdit->setText(temp);
}
void MainWindow::updateEthBalance() {
    QString* parameters = new QString(ui->accountLineEdit->text());
    EthReply* ethReply = ethRequester->sendRequest(EthMethod::getBalanceEth, parameters);
    QObject::connect(ethReply, &EthReply::endRequest, ui->balanceEthLineEdit, &QLineEdit::setText);
    delete parameters;
}
void MainWindow::update() {
    updateBalance();
    updateTokenRate();
    updateEthBalance();
}
void MainWindow::on_sellTokenButton_clicked() {
    setWindowEnable(false);
    EthReply* ethReply = unlockAccount();
    QObject::connect(ethReply, &EthReply::endRequestWithoutData, this, &MainWindow::sellToken);
}
void MainWindow::sellToken() {
    QString* parameters = new QString[2];
    parameters[0] = ui->accountLineEdit->text();
    parameters[1] = ui->sellTokenLineEdit->text();
    EthReply* ethReply = ethRequester->sendRequest(EthMethod::sellToken, parameters);
    QObject::connect(ethReply, &EthReply::endRequestWithoutData, this, &MainWindow::update);
    delete[] parameters;
    setWindowEnable(true);
}

EthReply* MainWindow::unlockAccount() {
    QString* parameters = new QString[2];
    parameters[0] = ui->accountLineEdit->text();
    parameters[1] = ui->passwordLineEdit->text();
    EthReply* ethReply = ethRequester->sendRequest(EthMethod::unlockAccount, parameters);
    return ethReply;
}

void MainWindow::on_transferButton_clicked() {
    setWindowEnable(false);
    EthReply* ethReply = unlockAccount();
    QObject::connect(ethReply, &EthReply::endRequestWithoutData, this, &MainWindow::transferToken);
}
void MainWindow::transferToken() {
    QString* parameters = new QString[3];
    parameters[0] = ui->accountLineEdit->text();
    parameters[1] = ui->transferToLineEdit->text();
    parameters[2] = ui->transferValueLineEdit->text();
    EthReply* ethReply = ethRequester->sendRequest(EthMethod::transferToken, parameters);
    QObject::connect(ethReply, &EthReply::endRequestWithoutData, this, &MainWindow::update);
    setWindowEnable(true);
}
