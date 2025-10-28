#include "SingleInstanceGuard.h"

#include <QLocalServer>
#include <QLocalSocket>

SingleInstanceGuard::SingleInstanceGuard(const QString &serverName, QObject *parent)
    : QObject(parent)
    , m_serverName(serverName)
    , m_server(nullptr)
    , m_isPrimary(false)
{
}

SingleInstanceGuard::~SingleInstanceGuard()
{
    if (m_server)
    {
        m_server->close();
        QLocalServer::removeServer(m_serverName);
    }
}

bool SingleInstanceGuard::tryToRun()
{
    if (m_server)
    {
        return m_isPrimary;
    }

    m_server = new QLocalServer(this);
    if (!m_server->listen(m_serverName))
    {
        QLocalServer::removeServer(m_serverName);
        if (!m_server->listen(m_serverName))
        {
            delete m_server;
            m_server = nullptr;
            m_isPrimary = false;
            return false;
        }
    }

    connect(m_server, &QLocalServer::newConnection, this, &SingleInstanceGuard::handleNewConnection);
    m_isPrimary = true;
    return true;
}

void SingleInstanceGuard::notifyExistingInstance() const
{
    QLocalSocket socket;
    socket.connectToServer(m_serverName);
    if (socket.waitForConnected(1000))
    {
        socket.write("activate");
        socket.flush();
        socket.waitForBytesWritten(1000);
        socket.disconnectFromServer();
        socket.waitForDisconnected(1000);
    }
}

void SingleInstanceGuard::handleNewConnection()
{
    if (!m_server)
    {
        return;
    }

    while (m_server->hasPendingConnections())
    {
        QLocalSocket *socket = m_server->nextPendingConnection();
        if (!socket)
        {
            continue;
        }

        connect(socket, &QLocalSocket::disconnected, socket, &QLocalSocket::deleteLater);
        socket->readAll();
        emit activationRequested();
        socket->disconnectFromServer();
    }
}
