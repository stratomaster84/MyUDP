#include "mainwindow.h"
#include "ui_mainwindow.h"

//---------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

// кнопка на высылку сообщения
    connect(ui->send_but, SIGNAL(clicked()), this, SLOT(slotSendDatagram()));
    connect(this, SIGNAL(signalSendDatagram(const QByteArray &, const QHostAddress &)), &_udp, SLOT(slotSendDatagram(const QByteArray &, const QHostAddress &)));

// кнопка замены порта
    connect(ui->port_but, SIGNAL(clicked()), this, SLOT(slotChangePort()));
    connect(this, SIGNAL(signalChangePort(quint16)), &_udp, SLOT(slotChangePort(quint16)));

// отклик на принятие данных
    connect(&_udp,SIGNAL(signalDataRead(const QByteArray &, const QHostAddress &)), this, SLOT(slotDataRead(const QByteArray &, const QHostAddress &)));

    //slotChangePort();
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
        ui->receive_txt->append("ERROR: NO SUCH IP-ADDRESS");
        return;
    }

    QByteArray datagram(ui->send_txt->toPlainText().toLatin1());
    if(datagram.isNull()){
        ui->receive_txt->append("ERROR: INVALID DATA");
        return;
    }

    ui->receive_txt->append("Вы: " + ui->send_txt->toPlainText());

    emit signalSendDatagram(datagram, address);
    ui->send_txt->clear();
}
void MainWindow::slotChangePort()
{
    bool ok = false;
    uint port = ui->port_edit->text().toUInt(&ok,10);

    if(ok && port < 65536)
        emit signalChangePort(port);
}
void MainWindow::slotDataRead(const QByteArray &datagram, const QHostAddress &address)
{
    if(datagram.isNull() || address.isNull())
        return;

    ui->receive_txt->append(address.toString() + ": " + QString::fromLatin1(datagram));
}
