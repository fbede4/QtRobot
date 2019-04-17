#pragma once
#ifndef SIMULATORSTATEHISTORY_H
#define SIMULATORSTATEHISTORY_H
#include <QtCore>
#include <memory>
#include <vector>
#include "simulatorstate.h"

/**
 * @brief Stores the previous states of the simulator,
 */
class SimulatorStateHistory : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     */
    SimulatorStateHistory();
    virtual ~SimulatorStateHistory() = default;

    /**
     * List of the controller states, can be bound to qml
     * The list stores pointers pointing to the elements in the container.
     *
     * @warning This must be a QObject* list. Derived classes won't work with qml binding
     */
    QList<QObject*> stateList;

    /** Pointer to the last valid state. The Add() function updates it. */
    SimulatorState *currentState;

    /** Actual container of the simulator states. */
    std::vector<std::unique_ptr<SimulatorState>> container;

    /** Appends the state to the history list, and make it the current one. */
    void Add(const SimulatorState& state);

signals:
    /** signal released after Add() */
    void historyChanged();
};

#endif // SIMULATORSTATEHISTORY_H
