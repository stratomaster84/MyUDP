#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UdpTrans.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    UdpTrans _udp;  // наш клиент/сервер

public slots:
    void slotSendDatagram();    // слот кнопки - отправить датаграмму
    void slotChangePort();      // слот кнопки - поменять порт
    void slotDataRead(const QByteArray &, const QHostAddress &);    // слот получения данных от клиента

signals:
    void signalSendDatagram(const QByteArray &, const QHostAddress &);  // сигнал от слота к серверу - отправить датаграмму
    void signalChangePort(quint16);                                     // сигнал от слота к клиент/серверу - поменять порт

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
