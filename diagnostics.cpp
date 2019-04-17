#include "diagnostics.h"

void Diagnostics::StringReceived(QDataStream &stream)
{
    QString text;
    stream>>text;
    emit messageReceived(text);
    qDebug()<<"recv message";
    qDebug()<<text;
}

void Diagnostics::DiagDataReceived(QDataStream &stream)
{
    int counter = 0;
    qDebug() << "diagnostics data:";
    qint32 diagType;
    stream>>diagType;
    qDebug() << "diag type:" << diagType;
    QVector<float> diagData;
    diagData.resize(DiagDataSize(diagType));
    qDebug() << "diag size:" << diagData.size();
    stream.readRawData(reinterpret_cast<char*>(diagData.data()), diagData.size() * static_cast<int>(sizeof(float)));

    if (diagType & static_cast<int>(DiagType::DISTANCE))
    {
        emit distanceChanged(diagData[counter + 0], diagData[counter + 1]);
        counter += 2;
    }

    if (diagType & static_cast<int>(DiagType::LIGHT))
    {

        emit colourChanged(
                diagData[counter + 0] / 5.0f,
                diagData[counter + 1] / 5.0f,
                diagData[counter + 2] / 5.0f,
                diagData[counter + 3] / 5.0f,
                diagData[counter + 4] / 5.0f,
                diagData[counter + 5] / 5.0f);
        counter += 6;
    }

    if (diagType & static_cast<int>(DiagType::POSITION))
    {
        emit positionChanged(diagData[counter + 0] + 25, diagData[counter + 1] + 25);
        counter += 2;
    }

    if (diagType & static_cast<int>(DiagType::MOVEMENT))
    {
        emit carMoved(diagData[counter + 0], diagData[counter + 1]);
        counter += 2;
    }
}

Diagnostics::Diagnostics(QObject *parent)
    :QObject(parent)
{
    server.Start(213);
    QObject::connect(&timer, &QTimer::timeout, this, &Diagnostics::timeoutHandler);
    QObject::connect(&server, SIGNAL(dataReady(QDataStream&)), this, SLOT(recvMessageHandler(QDataStream&)));
}

void Diagnostics::StartDiagnostics(int interval)
{
    timer.setInterval(interval);
    timer.start();
    qDebug()<<"diagnostics started";
}

void Diagnostics::EndDiagnostics()
{
    timer.stop();
    qDebug()<<"diagnostics stopped";
}

void Diagnostics::StartSelfTest()
{
    qDebug()<<"self test button pressed";
    server.Send(CommandType::SELFTEST);
}

void Diagnostics::ToggleAutoControl()
{
    qDebug()<<"auto button pressed";
    server.Send(CommandType::AUTOMODE);
}

void Diagnostics::ResetSimulator()
{
    qDebug()<<"reset button pressed";
    server.Send(CommandType::RESET);
}

void Diagnostics::DriveForward()
{
    qDebug()<<"moving forward";
    server.Send(CommandType::FORWARD);
}

void Diagnostics::DriveBackward()
{
    qDebug()<<"moving backward";
    server.Send(CommandType::BACKWARD);
}

void Diagnostics::DontDrive()
{
    qDebug()<<"moving stopped";
    server.Send(CommandType::NOMOVE);
}

void Diagnostics::SteerRight()
{
    qDebug()<<"steering right";
    server.Send(CommandType::RIGHT);
}

void Diagnostics::SteerLeft()
{
    qDebug()<<"steering left";
    server.Send(CommandType::LEFT);
}

void Diagnostics::DontSteer()
{
    qDebug()<<"going straight";
    server.Send(CommandType::NOSTEERING);
}

void Diagnostics::timeoutHandler()
{
    qDebug()<<"timer tick";
    int diagType =  //these will be monitored
            static_cast<int>(DiagType::DISTANCE) |
            static_cast<int>(DiagType::LIGHT) |
            static_cast<int>(DiagType::POSITION) |
            static_cast<int>(DiagType::MOVEMENT);
    server.AskForDiagData(diagType);
}

void Diagnostics::recvMessageHandler(QDataStream& stream)
{
    int msgType;
    qDebug()<<"recv message";
    stream>>msgType;
    qDebug()<<"type: "<<msgType;
    switch (msgType)
    {
    case static_cast<int>(MessageType::STRING):
        StringReceived(stream);
        break;
    case static_cast<int>(MessageType::CARDIAG):
        DiagDataReceived(stream);
        break;
    }
}
