#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

server::server(uint16_t nPort, QObject *parent) : QObject(parent)
{
    mng = new QNetworkAccessManager;
    connect(mng, &QNetworkAccessManager::finished, [=](QNetworkReply *reply){ sendToClient(reply->readAll()); });

    m_server =  new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    if (!m_server->listen(QHostAddress::Any, nPort)) {
        qDebug() << "Unable to start the server: " + m_server->errorString();
        m_server->close();
        return;
    }
}

void server::slotNewConnection()
{
    m_socket = m_server->nextPendingConnection();
    connect(m_socket, SIGNAL(disconnected()), m_socket, SLOT(deleteLater()));
    connect(m_socket, SIGNAL(readyRead()),this, SLOT(slotReadClient()));
}

void server::slotReadClient()
{
    m_socket = (QTcpSocket*)sender();
    QString socket_content(m_socket->readAll());
    if (socket_content.left(3) == "mac") // http://192.168.1.20/cgi-bin/ramon/mac.pl?a=mac&ip=250.213
        mng->get((QNetworkRequest(QUrl("http://192.168.1.20/cgi-bin/ramon/mac.pl?a=mac&ip=" + socket_content.mid(3)))));
    if (socket_content.left(3) == "cab")
        mng->get((QNetworkRequest(QUrl("http://192.168.1.20/cgi-bin/ramon/mac.pl?a=cab&ip=" + socket_content.mid(3)))));
}

void server::handleResults(const QString res)
{
    sendToClient(res);
}

void server::sendToClient(const QString& str)
{
    m_socket->write(str.toUtf8());
}

server::~server()
{
    m_server->deleteLater();
    mng->deleteLater();
}
