
#include <QtGui>
#include "UdpTrans.h"

// ----- конструктор (передаём родителя и порт, на котором работает клиент/сервер)------
UdpTrans::UdpTrans(QObject* parent /*=nullptr*/, quint16 port /*=0*/) :
    QObject(parent), // родительский конструктор
    _port(port)      // порт на котором работает клиент/сервер
{
    _server = new QUdpSocket(this); //  сокет сервера

    _client = new QUdpSocket(this); //  сокет клиента
    _client->bind(QHostAddress::AnyIPv4, _port);  // клиент может принимать со всех адресов (IPv4)
    connect(_client, SIGNAL(readyRead()), this, SLOT(slotProcessDatagrams()));  // сигнал от клиента "готов к чтению" - на слот
}
// ----слот передачи сервером конкретной датаграммы по заданному адресу (порт = _port)
void UdpTrans::slotSendDatagram(const QByteArray &datagram, const QHostAddress &address)
{
    _server->writeDatagram(datagram, address, _port);
}
// -----слот принятия клиентом датаграмм с любого адреса по порту = _port
void UdpTrans::slotProcessDatagrams()
{
    QByteArray datagram;
    QHostAddress SenderAddress;

    do {
        datagram.resize(_client->pendingDatagramSize());    // изменяем размер буфера для считывания очередной датаграммы
        _client->readDatagram(datagram.data(), datagram.size(),&SenderAddress); // считываем в буфер данные и адрес отправителя
    } while(_client->hasPendingDatagrams());    // сохраняем только последнюю датаграмму, т.к. она самая актуальная

    emit signalDataRead(datagram, SenderAddress);   // высылаем сигнал в основной Widget
}
// ----------------------------------------------------------------------
void UdpTrans::slotChangePort(quint16 port){
    _port = port;   // меняем порт внутри класса

    _client->abort();   // моментально разрываем соединение

//ALTERNATIVE (IF) YOU HAVE YOU WAIT

    //_client->close();
    //_client->waitForDisconnected(100);

    _client->bind(QHostAddress::AnyIPv4, _port);    // клиент может принимать со всех адресов (IPv4)

}
// ----------------------------------------------------------------------
