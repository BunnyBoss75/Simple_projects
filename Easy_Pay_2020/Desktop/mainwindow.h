#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "EthRequester.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update();

    void on_balanceEthButton_clicked();
    void updateEthBalance();

    void on_balanceButton_clicked();
    void updateBalance();

    void on_buyTokenButton_clicked();
    void buyToken();

    void updateTokenRate();

    void on_tokenRateButton_clicked();

    void specifyTokenRate();
    void specifyBalance();

    void setSpecifyTokenRate(QString& response);
    void setSpecifyBalance(QString& response);

    void on_sellTokenButton_clicked();
    void sellToken();

    EthReply* unlockAccount();

    void on_transferButton_clicked();
    void transferToken();
    void setWindowEnable(bool);

private:
    QTimer* timer;
    Ui::MainWindow *ui;
    EthRequester* ethRequester;
    int timeBetweenUpdate;
};
#endif // MAINWINDOW_H
