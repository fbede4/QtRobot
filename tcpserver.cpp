#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include "tcpserver.h"

int DiagDataSize(int diagType)
{
    int size = 0;
    if (diagType & static_cast<int>(DiagType::DISTANCE))
        size += 2;
    if (diagType & static_cast<int>(DiagType::LIGHT))
        size += 6;
    if (diagType & static_cast<int>(DiagType::POSITION))
        size += 2;
    if (diagType & static_cast<int>(DiagType::MOVEMENT))
        size += 2;
    return size;
}

TCPServer::TCPServer()
    : QObject(nullptr), receiveStream(nullptr), currentMessageSize(0)
{}

void TCPServer::Start(quint16 port)
{
    if (!serverSocket.listen(QHostAddress::Any, port))
    {
        qWarning() << "SocketServer::Start: Could not open server: ";
        qWarning() << serverSocket.errorString();
    }
    else
    {
        qWarning() << "SocketServer::Start: Server socker is waiting for incomming connection...";
    }
    connect(&serverSocket, &QTcpServer::newConnection, this, &TCPServer::newConnection);
}

void TCPServer::newConnection()
{
    qDebug() << "SocketServer::newConnection: Incoming connection...";
    QTcpSocket *newSocket = serverSocket.nextPendingConnection();
    if (newSocket)
    {
        connect(newSocket, &QTcpSocket::disconnected, this, &TCPServer::disconnected);

        if (currentConnectionSocket != nullptr && newSocket != currentConnectionSocket)
        {
            QObject::disconnect(currentConnectionSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(CommunicationTcpSocket::HandleError(QAbstractSocket::SocketError)));
            QObject::disconnect(currentConnectionSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
        }
        currentConnectionSocket = newSocket;
        connect(currentConnectionSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
        connect(currentConnectionSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

        receiveStream = std::make_unique<QDataStream>(currentConnectionSocket);

        qWarning() << "SocketServer::newConnection: New connection has built up.";
    }
}

void TCPServer::disconnected()
{
    qWarning() << "SocketServer::disconnected: Connection closed.";
}

void TCPServer::dataReceived()
{
    QDataStream &inStream = *receiveStream;
    QIODevice *socket = inStream.device();

    if (currentMessageSize == 0) {
        if (socket->bytesAvailable() < static_cast<int>(sizeof(qint32)))
            return;
        inStream >> currentMessageSize;
    }
    if (socket->bytesAvailable() < (currentMessageSize - static_cast<int>(sizeof(qint32))))
        return;

    emit dataReady(inStream);

    currentMessageSize = 0;
    if (socket->bytesAvailable() > 0)
        QTimer::singleShot(0, this, SLOT(dataReceived()));
}

void TCPServer::handleError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "SocketServer::handleError: A socket hibát jelzett: " << socketError;
}

void TCPServer::Send(QString text)
{
    if (currentConnectionSocket == nullptr)
        return;
    QByteArray sendBuffer;
    auto stream = std::unique_ptr<QDataStream>(new QDataStream(&sendBuffer, QIODevice::WriteOnly));
    const qint64 startPos = stream->device()->size();
    qint32 msgSize = 0;
    *stream << msgSize;
    *stream << static_cast<int>(MessageType::STRING);
    *stream << text;
    const qint64 endPos = stream->device()->size();
    stream->device()->seek(startPos);
    msgSize = static_cast<int>(endPos - startPos);
    *stream << msgSize;
    stream->device()->seek(endPos);
    currentConnectionSocket->write(sendBuffer);
    sendBuffer.clear();
}
void TCPServer::Send(CommandType cmd)
{
    if (currentConnectionSocket == nullptr)
        return;
    QByteArray sendBuffer;
    auto stream = std::unique_ptr<QDataStream>(new QDataStream(&sendBuffer, QIODevice::WriteOnly));
    *stream << static_cast<int>(12);
    *stream << static_cast<int>(MessageType::COMMAND);
    *stream << static_cast<int>(cmd);
    currentConnectionSocket->write(sendBuffer);
    sendBuffer.clear();
}
void TCPServer::AskForDiagData(int diagType)
{
    if (currentConnectionSocket == nullptr)
        return;
    QByteArray sendBuffer;
    auto stream = std::unique_ptr<QDataStream>(new QDataStream(&sendBuffer, QIODevice::WriteOnly));
    *stream << static_cast<int>(12);
    *stream << static_cast<int>(MessageType::CARDIAG);
    *stream << diagType;
    currentConnectionSocket->write(sendBuffer);
    sendBuffer.clear();
}
