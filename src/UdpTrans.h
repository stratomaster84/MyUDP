#pragma once

#include <QtNetwork>

// ======================================================================
class UdpTrans : public QObject {
Q_OBJECT
private:
    QUdpSocket* _server;    //  сокет сервера
    QUdpSocket* _client;    //  сокет клиента
    quint16 _port;          //  порт на котором работает клиент/сервер

public:
    UdpTrans(QObject* parent = nullptr, quint16 port = 3456);  //  конструктор (передаём родителя и порт, на котором работает клиент/сервер)

private slots:
    void slotSendDatagram(const QByteArray &datagram, const QHostAddress &address); // слот передачи сервером конкретной датаграммы по заданному адресу (порт = _port)
    void slotProcessDatagrams();                        // слот принятия клиентом датаграмм с любого адреса по порту = _port
    void slotChangePort(quint16 port);                      // поменять текущий порт

signals:
    void signalDataRead(const QByteArray &, const QHostAddress &);  // сигнал в основной Widget, для принятия им данных и адресса сервера
};
