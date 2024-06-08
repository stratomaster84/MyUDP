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

    UdpTrans _udp;

public slots:
    void slotSendDatagram();
    void slotChangePort();
    void slotDataRead(const QByteArray &, const QHostAddress &);

signals:
    void signalSendDatagram(const QByteArray &, const QHostAddress &);
    void signalChangePort(quint16);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
