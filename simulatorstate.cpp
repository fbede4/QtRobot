#include <QDebug>
#include "simulatorstate.h"

SimulatorState::SimulatorState()
{
}

SimulatorState::SimulatorState(float sp, float st)
    : _sp(sp), _st(st)
{
}

void SimulatorState::WriteTo(QDataStream& stream) const
{
    stream << _sp;
    stream << _st;
}

void SimulatorState::ReadFrom(QDataStream& stream)
{
    stream >> _sp;
    stream >> _st;
}

void SimulatorState::CopyFrom(const SimulatorState &other)
{
    _sp = other._sp;
    _st = other._st;
}

QDataStream &operator<<(QDataStream& stream, const SimulatorState& state)
{
    state.WriteTo(stream);
    return stream;
}

QDataStream &operator>>(QDataStream& stream, SimulatorState& state)
{
    state.ReadFrom(stream);
    return stream;
}
