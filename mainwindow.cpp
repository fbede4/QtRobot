#include "mainwindow.h"
#include "simulatorstate.h"
#include "simulatorstatehistory.h"
#include <QQmlContext>
#include <QQmlProperty>
#include <QSlider>

MainWindow::MainWindow(QObject *rootObject, SimulatorStateHistory& history, QQmlContext& qmlContext)
    : QObject(nullptr),
      history(history),
      qmlContext(qmlContext)
{
    if (!rootObject)
        qDebug() << "rootObject not found";

    incomingMessage = FindItemByName(rootObject, QString("incomingMessage"));
    outgoingMessage = FindItemByName(rootObject, QString("outgoingMessage"));
    sendButton = FindItemByName(rootObject, QString("sendButton"));
    selfTestButton = FindItemByName(rootObject, QString("selfTestButton"));
    resetButton = FindItemByName(rootObject, QString("resetButton"));
    autoButton = FindItemByName(rootObject, QString("autoButton"));
    velocitySlider = FindItemByName(rootObject, QString("velocitySlider"));
    steeringSlider = FindItemByName(rootObject, QString("steeringSlider"));
    startButton = FindItemByName(rootObject, QString("startButton"));

    if (!incomingMessage)
        qDebug() << "not found: incomingMessage";
    if (!outgoingMessage)
        qDebug() << "not found: outgoingMessage";
    if (!sendButton)
        qDebug() << "not found: sendButton";

    // Bind to qml
    QObject::connect(this,SIGNAL(distanceChanged(qreal)),rootObject,SIGNAL(distanceChanged(qreal)));
    QObject::connect(this,SIGNAL(colourChanged(qreal, qreal, qreal, qreal, qreal, qreal)),rootObject,
                     SIGNAL(colourChanged(qreal, qreal, qreal, qreal, qreal, qreal)));
    QObject::connect(this,SIGNAL(positionChanged(qreal, qreal)),rootObject,SIGNAL(positionChanged(qreal, qreal)));

    QObject::connect(startButton, SIGNAL(clicked()), this, SLOT(startBtnHandler()));
    QObject::connect(steeringSlider, SIGNAL(valueChanged()), this, SLOT(steeringSliderValueChangedHandler()));
    QObject::connect(velocitySlider, SIGNAL(valueChanged()), this, SLOT(velocitySliderValueChangedHandler()));
    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(sendOutMessageHandler()));
    QObject::connect(selfTestButton, SIGNAL(clicked()), this, SLOT(selfTestBtnHandler()));
    QObject::connect(resetButton, SIGNAL(clicked()), this, SLOT(resetBtnHandler()));
    QObject::connect(autoButton, SIGNAL(clicked()), this, SLOT(autoBtnHandler()));

    QObject::connect(&diagnostics, SIGNAL(messageReceived(QString)), this, SLOT(messageReceivedHandler(QString)));
    QObject::connect(&diagnostics, SIGNAL(distanceChanged(float, float)), this, SLOT(distanceChangedHandler(float, float)));
    QObject::connect(&diagnostics, SIGNAL(colourChanged(float, float, float, float, float, float)),
                     this, SLOT(colourChangedHandler(float, float, float, float, float, float)));
    QObject::connect(&diagnostics, SIGNAL(positionChanged(float, float)), this, SLOT(positionChangedHandler(float, float)));
    QObject::connect(&diagnostics, SIGNAL(carMoved(float, float)), this, SLOT(carMovedHandler(float, float)));
}

void MainWindow::startBtnHandler()
{
    if (diagnostics.OnAir())
        diagnostics.EndDiagnostics();
    else
        diagnostics.StartDiagnostics(100);
}
void MainWindow::selfTestBtnHandler()
{
    diagnostics.StartSelfTest();
}
void MainWindow::resetBtnHandler()
{
    diagnostics.ResetSimulator();
}
void MainWindow::autoBtnHandler()
{
    diagnostics.ToggleAutoControl();
}
void MainWindow::velocitySliderValueChangedHandler()
{
    int value = velocitySlider->property("value").toInt();
    if (value > 0) diagnostics.DriveForward();
    else if (value < 0) diagnostics.DriveBackward();
    else diagnostics.DontDrive();
}
void MainWindow::steeringSliderValueChangedHandler()
{
    int value = steeringSlider->property("value").toInt();
    if (value < 0) diagnostics.SteerLeft();
    else if (value > 0) diagnostics.SteerRight();
    else diagnostics.DontSteer();
}
void MainWindow::sendOutMessageHandler()
{
    QVariant msg = outgoingMessage->property("text");
    QString str = msg.toString();
    qDebug() << "send message: " << str;
    diagnostics.getServer().Send(str);
}

void MainWindow::messageReceivedHandler(QString msg)
{
    incomingMessage->setProperty("text", QVariant(msg));
}

void MainWindow::distanceChangedHandler(float d1, float d2)
{
    qreal dist1 = static_cast<qreal>(d1);
    qreal dist2 = static_cast<qreal>(d2);
    emit distanceChanged(dist1 < dist2 ? dist1 : dist2);
}

void MainWindow::colourChangedHandler(float c1, float c2, float c3, float c4, float c5, float c6)
{
    emit colourChanged(
                static_cast<qreal>(c1),
                static_cast<qreal>(c2),
                static_cast<qreal>(c3),
                static_cast<qreal>(c4),
                static_cast<qreal>(c5),
                static_cast<qreal>(c6));
}

void MainWindow::positionChangedHandler(float x, float y)
{
    emit positionChanged(static_cast<qreal>(x), static_cast<qreal>(y));
}

void MainWindow::carMovedHandler(float speed, float steering)
{
    history.Add(SimulatorState(speed, steering));
    qmlContext.setContextProperty(QStringLiteral("historyModel"), QVariant::fromValue(history.stateList));
}

QQuickItem* MainWindow::FindItemByName(QObject *rootObject, const QString& name)
{
    Q_ASSERT(rootObject != nullptr);
    if (rootObject->objectName() == name)
    {
        return static_cast<QQuickItem*>(rootObject);
    }
    return FindItemByName(rootObject->children(), name);
}

QQuickItem* MainWindow::FindItemByName(QList<QObject*> nodes, const QString& name)
{
    for(int i = 0; i < nodes.size(); i++)
    {
        // Node keresése
        if (nodes.at(i) && nodes.at(i)->objectName() == name)
        {
            return static_cast<QQuickItem*>(nodes.at(i));
        }
        // Gyerekekben keresés
        else if (nodes.at(i) && nodes.at(i)->children().size() > 0)
        {
            QQuickItem* item = FindItemByName(nodes.at(i)->children(), name);
            if (item)
                return item;
        }
    }
    // Nem találtuk.
    return nullptr;
}
