#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QQuickItem>
#include "diagnostics.h"

class SimulatorStateHistory;

/**
 * @brief The MainWindow is responsible for the business logic of the main view.
 *
 * The class receives and sends commands from/to the UI with the help of the signals and slots mechanism.
 * It can be connected either to the simulator, and the simulator can be controlled through the UI.
*/
class MainWindow :public QObject
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow constructor.
     * @param rootObject A pointer to the root, if the instance has a root object.
     */
    MainWindow(QObject *rootObject, SimulatorStateHistory& history, QQmlContext &qmlContext);
    /**
     * @brief MainWindow destructor.
     */
    ~MainWindow() = default;

signals:
    /**
     * @brief Notifies the UI of a change in the distance sensed by main car of the simulator.
     * @param distance The new distance, sensed by the car.
     */
    void distanceChanged(qreal distance);
    /**
     * @brief Notifies the UI of a change in the colour sensed by main car of the simulator.
     * @param c1-c6 The new monochromatic values sensed by the six sensors.
     */
    void colourChanged(qreal c1, qreal c2, qreal c3, qreal c4, qreal c5, qreal c6);
    /**
     * @brief Notifies the UI of a change in the position of the main car.
     * @param x The new 'X' coordinate of the car
     * @param y The new 'Y' coordinate of the car
     */
    void positionChanged(qreal x, qreal y);
public slots:
    /**
     * @brief Starts / Stops the diagnostics with starting / stopping the timer mentioned above.
     */
    void startBtnHandler();
    /**
     * @brief Sends message to the simulator through a TCP connection.
     */
    void sendOutMessageHandler();
    /**
     * @brief Sends self test command to the simulator through a TCP connection.
     */
    void selfTestBtnHandler();
    /**
     * @brief Sends reset command to the simulator through a TCP connection.
     */
    void resetBtnHandler();
    /**
     * @brief Sends auto control command to the simulator through a TCP connection.
     */
    void autoBtnHandler();
    /**
     * @brief Notifies the simulator that the user requested a change of velocity.
     */
    void velocitySliderValueChangedHandler();
    /**
     * @brief Notifies the simulator that the user requested a change of steering angle.
     */
    void steeringSliderValueChangedHandler();
    /**
     * @brief
     * Writes the message to the textbox
     * @param msg The message
     */
    void messageReceivedHandler(QString msg);
    /**
     * @brief
     * Selects the smaller value and emits distanceChanged
     * @param d1 sensor1 data
     * @param d2 sensor2 data
     */
    void distanceChangedHandler(float d1, float d2);
    /**
     * @brief
     * emits colourChanged with casted values
     * @param c1-c6 The new monochromatic values sensed by the six sensors.
     */
    void colourChangedHandler(float c1, float c2, float c3, float c4, float c5, float c6);
    /**
     * @brief
     * emits positionChanged with casted values
     * @param x position
     * @param y position
     */
    void positionChangedHandler(float x, float y);
    /**
     * @brief
     * Appends changes to history
     * @param speed
     * @param steering
     */
    void carMovedHandler(float speed, float steering);

private:
    /**
     * @brief Finds a qml element by it's name.
     * @param rootObject The root object of the class
     * @param name The name of the qml element we are looking for
     */
    QQuickItem* FindItemByName(QObject *rootObject, const QString& name);
    QQuickItem* FindItemByName(QList<QObject*> nodes, const QString& name);
    Diagnostics diagnostics;
    SimulatorStateHistory& history;
    /** \addtogroup References to data binding
     *  @{
     */
    /** QML context for refreshing simulator data. */
    QQmlContext &qmlContext;
    QQuickItem* startButton;
    QQuickItem* incomingMessage;
    QQuickItem* outgoingMessage;
    QQuickItem* sendButton;
    QQuickItem* selfTestButton;
    QQuickItem* resetButton;
    QQuickItem* autoButton;
    QQuickItem* velocitySlider;
    QQuickItem* steeringSlider;
};

#endif // MAINWINDOW_H
