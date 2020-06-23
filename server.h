#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class QTcpServer;
class QTcpSocket;
class QNetworkAccessManager;

class server : public QObject
{
    Q_OBJECT
    QTcpServer *m_server;
    QTcpSocket *m_socket;
    QNetworkAccessManager *mng;

public:
    explicit server(uint16_t nPort, QObject *parent = nullptr);
public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void handleResults(const QString );
signals:
    void operate(const QString&);
    void finished();
public:
     void sendToClient(const QString& str);
     ~server();
};

#endif // SERVER_H
