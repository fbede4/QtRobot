#pragma once
#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
#include <QDataStream>
#include <memory>

class QTcpSocket;

/**
 * @brief
 * Types of messages to be sent or reveived
 */
enum class MessageType :int
{
    STRING = 1,     /// character string to be displayed
    COMMAND = 2,    /// server sends control command which the simulator executes
    CARDIAG = 3     /// server asks for diagnostics data which the simulator sends as a reply
};

/**
 * @brief
 * When COMMAND MessageType is sent, one of these follows
 */
enum class CommandType :int
{
    FORWARD = 1,        /// makes the car move forward
    BACKWARD = 2,       /// makes the car move bavkward
    NOMOVE = 3,         /// stops the car
    LEFT = 4,           /// makes the car turn left
    RIGHT = 5,          /// makes the car turn right
    NOSTEERING = 6,     /// makes the car go straight
    RESET = 7,          /// restarts the simulator
    AUTOMODE = 8,       /// switches the car between automatic and manual control
    SELFTEST = 9        /// starts testing car functions
};

/**
 * @brief
 * Flags showing which data types are to be sent by the simulator
 */
enum class DiagType :int
{
    DISTANCE = 1 << 0,  /// distance sensor data (facing forward)
    LIGHT = 1 << 1,     /// light sensor data (facing towards the floor)
    POSITION = 1 << 2,  /// position of the car
    MOVEMENT = 1 << 3   /// car's speed and steering value
};

/**
 * @brief
 * @param diagType DiagType flags ored
 * @return size of data inf floats
 */
int DiagDataSize(int diagType);

/**
 * @brief
 * Server which accepts connections on TCP.
 * Manages communication between simulator and diagnostics application.
 */
class TCPServer : public QObject
{
    Q_OBJECT

public:
    TCPServer();
    ~TCPServer() = default;

    /**
     * @brief
     * Listens to incomming connection requests
     * @param port The port which accepts connection requests
     */
    void Start(quint16 port);

    /**
     * @brief
     * Sends character string to the connected client if there is one
     * @param text Text to be sent
     */
    void Send(QString text);

    /**
     * @brief
     * Send command which the simulator executes
     * @param cmd Command to execute
     */
    void Send(CommandType cmd);

    /**
     * @brief
     * Asks the simulator to send diagnostics data
     * @param diagType What is expected to be received
     */
    void AskForDiagData(int diagType);

private:

    /**
     * @brief
     * Socket of the server
     */
    QTcpServer serverSocket;

    /**
     * @brief
     * The connected simulator or null
     */
    QTcpSocket *currentConnectionSocket = nullptr;

    /**
     * @brief
     * Here lies all the incoming data
     */
    std::unique_ptr<QDataStream> receiveStream;

    /**
     * @brief
     * Number of bytes already received
     */
    qint32 currentMessageSize;


signals:
    /**
     * @brief
     * Fires when receiveStream fills up
     */
    void dataReady(QDataStream&);

private slots:
    /**
     * @brief
     * Gets called when client tries to connect
     */
    void newConnection();

    /**
     * @brief
     * Gets called when client disconnects
     */
    void disconnected();

    /**
     * @brief
     * Gets called when client tries to connect
     * @param socketError The reason
     */
    void handleError(QAbstractSocket::SocketError socketError);

    /**
     * @brief
     * Gets called when client sends data
     */
    void dataReceived();
};


#endif // TCPSERVER_H
