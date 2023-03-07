#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ping_timer = new QTimer();
    connect(ping_timer, SIGNAL(timeout()),
            this, SLOT(on_pushButton_ping_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ping_clicked()
{
    // newPing();
    auto lambda = [&](){this->newPing();};
    QFuture<void> future = QtConcurrent::run(lambda);
}

void MainWindow::on_checkBox_ping_stateChanged(int arg1)
{
    if(arg1)
    {
        on_pushButton_ping_clicked();
        ping_timer->start(ui->spinBox_freq->value());
    }
    else
    {
        ping_timer->stop();
    }
}

void MainWindow::on_spinBox_freq_valueChanged(int)
{
    if(ui->checkBox_ping->isChecked())
    {
        ping_timer->start(ui->spinBox_freq->value());
    }
}

void MainWindow::newPing()
{
    long time = this->ping();

    ui->lcdNumber->display((int)time);
}

long MainWindow::ping()
{
    long time = 0;
    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32];
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

    QString ip = QString::number(ui->spinBox_ip_1->value()) + "." +
                 QString::number(ui->spinBox_ip_2->value()) + "." +
                 QString::number(ui->spinBox_ip_3->value()) + "." +
                 QString::number(ui->spinBox_ip_4->value());

    ipaddr = inet_addr(ip.toStdString().c_str());
    if (ipaddr == INADDR_NONE)
    {
        qDebug() << "IP";
        return -1;
    }

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE)
    {
        qDebug() << "IcmpCreatefile";
        return -1;
    }

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);
    if (ReplyBuffer == NULL)
    {
        qDebug() << "ReplyBuffer : unable to allocate memory";
        return -1;
    }

    dwRetVal = IcmpSendEcho(hIcmpFile,
                            ipaddr,
                            SendData,
                            sizeof(SendData),
                            NULL,
                            ReplyBuffer,
                            ReplySize,
                            1000);
    if (dwRetVal != 0)
    {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;
        if(pEchoReply->Status != 0)
            qDebug() << "pEchoReply->Status" << pEchoReply->Status;
        time = (long)pEchoReply->RoundTripTime;
    }
    else
    {
        qDebug() << "IcmpSendEcho";
        return -1;
    }

    return time;
}
