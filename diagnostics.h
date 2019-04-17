#pragma once

#include "tcpserver.h"
#include <QtCore/QTimer>
#include <QObject>

/**
 * @brief The Diagnostics class manages diagnostics.
 * UI controls call back to this class which passes it on
 */
class Diagnostics : public QObject
{
    Q_OBJECT

    QTimer timer;
    TCPServer server;

private:
    void StringReceived(QDataStream& stream);
    void DiagDataReceived(QDataStream& stream);

public:
    /**
     * @brief
     * Starts server and connects signals and slots
     * @param parent
     */
    Diagnostics(QObject *parent = nullptr);
    /**
     * @brief
     * Starts up a timer which requests diagnostics data from the simulator
     * @param interval_ms Period in milliseconds
     */
    void StartDiagnostics(int interval_ms = 100);
    /**
     * @brief
     * Stops the timer so no more diagnstics data arrives
     */
    void EndDiagnostics();

    /**
     * @brief
     * @return true if diagnostics are requested periodically
     */
    inline bool OnAir() { return timer.isActive(); }
    /**
     * @brief
     * @return The TCPServer reliable for the communication
     */
    inline TCPServer& getServer() {return server; }

    /**
     * @brief
     * Asks the simulator to run a self test
     */
    void StartSelfTest();
    /**
     * @brief
     * Toggles the simulated car to be controlled automaticly or manually
     */
    void ToggleAutoControl();
    /**
     * @brief
     * Resets the simulator
     */
    void ResetSimulator();
    /**
     * @brief
     * Signals the car to move forward
     */
    void DriveForward();
    /**
     * @brief
     * Signals the car to move backward
     */
    void DriveBackward();
    /**
     * @brief
     * Signals the car to stop moving
     */
    void DontDrive();
    /**
     * @brief
     * Signals the car to turn right
     */
    void SteerRight();
    /**
     * @brief
     * Signals the car to turn left
     */
    void SteerLeft();
    /**
     * @brief
     * Signals the car to move straight
     */
    void DontSteer();

signals:
    /**
     * @brief
     * Shots when string message is received
     * @param msg The message
     */
    void messageReceived(QString msg);
    /**
     * @brief
     * Shots when distance sensor data is received
     * @param d1 One of the distance sensor values
     * @param d2 The other one of the distance sensor values
     */
    void distanceChanged(float d1, float d2);
    /**
     * @brief
     * Shots when colour sensor data is received
     * @param c1-c6 The new monochromatic values sensed by the six sensors
     */
    void colourChanged(float c1, float c2, float c3, float c4, float c5, float c6);
    /**
     * @brief
     * Shots when position data is received
     * @param x position
     * @param y position
     */
    void positionChanged(float x, float y);
    /**
     * @brief
     * Shots when car is controlled
     * @param speed
     * @param steering
     */
    void carMoved(float speed, float steering);

public slots:
    /**
     * @brief
     * Asks for diagnostics data
     */
    void timeoutHandler();
    /**
     * @brief
     * Raw data from the simulator
     * @param stream Data
     */
    void recvMessageHandler(QDataStream& stream);
};
