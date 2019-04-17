#include "simulatorstate.h"
#include "simulatorstatehistory.h"

SimulatorStateHistory::SimulatorStateHistory()
    : QObject(nullptr), currentState(nullptr)
{
}

void SimulatorStateHistory::Add(const SimulatorState& state)
{
    std::unique_ptr<SimulatorState> newState = std::make_unique<SimulatorState>();
    newState->CopyFrom(state);
    stateList.append(newState.get());
    currentState = newState.get(); // This has to be done before moving ownership
    container.push_back(std::move(newState));

    // signal history change.
    emit historyChanged();
}
