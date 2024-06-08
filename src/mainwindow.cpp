#include "mainwindow.h"
#include "ui_mainwindow.h"

//---------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

// кнопка на высылку сообщения (кнопка - this, this - сервер)
    connect(ui->send_but, SIGNAL(clicked()), this, SLOT(slotSendDatagram()));
    connect(this, SIGNAL(signalSendDatagram(const QByteArray &, const QHostAddress &)), &_udp, SLOT(slotSendDatagram(const QByteArray &, const QHostAddress &)));

// кнопка замены порта (кнопка - this, this - клиент/сервер)
    connect(ui->port_but, SIGNAL(clicked()), this, SLOT(slotChangePort()));
    connect(this, SIGNAL(signalChangePort(quint16)), &_udp, SLOT(slotChangePort(quint16)));

// отклик на принятие данных    (клиент - this)
    connect(&_udp,SIGNAL(signalDataRead(const QByteArray &, const QHostAddress &)), this, SLOT(slotDataRead(const QByteArray &, const QHostAddress &)));

    slotChangePort();   // меняет порт (должен быть после connect, т.к. кидает сигнал)
}

//---------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------СЛОТЫ-----------------------------
void MainWindow::slotSendDatagram()
{
    QHostAddress address(ui->host_edit->text());
    if(address.isNull()){
        ui->receive_txt->append("ОШИБКА:\tНЕВЕРНЫЙ IP-АДРЕС ПОЛУЧАТЕЛЯ");
        return;
    }

    QByteArray datagram(ui->send_txt->toPlainText().toUtf8());
    if(datagram.isNull()){
        ui->receive_txt->append("ОШИБКА:\tНЕИЗВЕСТНЫЙ ФОРМАТ ДАННЫХ");
        return;
    }

    ui->receive_txt->append("Вы: " + ui->send_txt->toPlainText()); // печать своего сообщения

    emit signalSendDatagram(datagram, address);     // сигнал серверу на отправку сообщения
    ui->send_txt->clear();                          // стираем сообщение на форме
}
void MainWindow::slotChangePort()
{
    bool ok = false;
    uint port = ui->port_edit->text().toUInt(&ok,10);

    if(ok && port < 65536)
        emit signalChangePort(port);    // если новый порт в порядке - меняем, посылая сигнал в клиент/сервер
    else{
        ui->receive_txt->append("ОШИБКА:\tНЕВЕРНЫЙ СЕТЕВОЙ ПОРТ:\n\t" +
                                ui->port_edit->text() +
                                "\n\tТРЕБУЕТСЯ ПОРТ В ДИАПАЗОНЕ 0-65535"
                                "\n\tВОЗВРАЩАЮСЬ К ПРЕДУДЫЩЕМУ ПОРТУ:\n\t" +
                                QString::number(_udp.getPort(),10));
        ui->port_edit->setText(QString::number(_udp.getPort(),10));
    }
}
void MainWindow::slotDataRead(const QByteArray &datagram, const QHostAddress &address)
{
    if(datagram.isNull())
        return;
    if(address.isNull()){
        ui->receive_txt->append("UNKNOWN: " + QString::fromUtf8(datagram));
    }
    else{
        ui->receive_txt->append(address.toString() + ": " + QString::fromUtf8(datagram));
    }
}
