#include "mainwindow.h"
#include "ui_mainwindow.h"

//---------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    table_model = new TableModel(0,2,this); // модель
    ui->receive_tabl->setModel(table_model);

// кнопка на высылку сообщения (кнопка - this, this - сервер)
    connect(ui->send_but, SIGNAL(clicked()), this, SLOT(slotSendDatagram()));
    connect(this, SIGNAL(signalSendDatagram(const QByteArray &, const QHostAddress &)), &udp_trans, SLOT(slotSendDatagram(const QByteArray &, const QHostAddress &)));

// кнопка замены порта (кнопка - this, this - клиент/сервер)
    connect(ui->port_but, SIGNAL(clicked()), this, SLOT(slotChangePort()));
    connect(this, SIGNAL(signalChangePort(quint16)), &udp_trans, SLOT(slotChangePort(quint16)));

// отклик на принятие данных    (клиент - this)
    connect(&udp_trans,SIGNAL(signalDataRead(const QByteArray &, const QHostAddress &)), this, SLOT(slotDataRead(const QByteArray &, const QHostAddress &)));

// отклик на изменение данных
    connect(table_model,SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), ui->receive_tabl, SLOT(resizeRowsToContents()));
    connect(table_model,SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), ui->receive_tabl, SLOT(resizeColumnsToContents()));

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
        table_model->newStringRow("ОШИБКА!!!","НЕВЕРНЫЙ IP-АДРЕС ПОЛУЧАТЕЛЯ");
        return;
    }

    QByteArray datagram(ui->send_txt->toPlainText().toUtf8());
    if(datagram.isNull()){
        table_model->newStringRow("ОШИБКА!!!","НЕИЗВЕСТНЫЙ ФОРМАТ ДАННЫХ");
        return;
    }

    table_model->newStringRow("ВЫ",ui->send_txt->toPlainText());

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
        table_model->newStringRow("ОШИБКА!!!","НЕВЕРНЫЙ СЕТЕВОЙ ПОРТ:\n" +
                                ui->port_edit->text() +
                                "\nТРЕБУЕТСЯ ПОРТ В ДИАПАЗОНЕ 0-65535"
                                "\nВОЗВРАЩАЮСЬ К ПРЕДУДЫЩЕМУ ПОРТУ:\n" +
                                QString::number(udp_trans.getPort(),10));

        ui->port_edit->setText(QString::number(udp_trans.getPort(),10));
    }
}
void MainWindow::slotDataRead(const QByteArray &datagram, const QHostAddress &address)
{
    if(datagram.isNull())
        return;
    if(address.isNull()){
        table_model->newStringRow("UNKNOWN",QString::fromUtf8(datagram));
    }
    else{
        table_model->newStringRow(address.toString(),QString::fromUtf8(datagram));
    }
}
