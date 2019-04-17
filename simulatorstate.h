#pragma once
#ifndef SIMULATORSTATE_H
#define SIMULATORSTATE_H
#include <QDataStream>
#include <QString>

/**
 * @brief Simulator state description at a specific time
 * History is a series of this object.
 */
class SimulatorState : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    SimulatorState();

    /**
     * @brief Constructor with given parameters.
     * @param x,y Position
     */
    SimulatorState(float sp, float st);

    ~SimulatorState() = default;

    /** Position */
    Q_PROPERTY(float sp READ sp WRITE setSp MEMBER _sp NOTIFY spChanged)
    float sp() const { return _sp; }
    void setSp(float sp) { _sp = sp; }
    Q_PROPERTY(float st READ st WRITE setSt MEMBER _st NOTIFY stChanged)
    float st() const { return _st; }
    void setSt(float st) { _st = st; }

    /** Serialize to stream. */
    void WriteTo(QDataStream& stream) const;

    /** Read from stream */
    void ReadFrom(QDataStream& stream);

    /** QObject copy function */
    void CopyFrom(const SimulatorState& other);


signals:
    void spChanged();
    void stChanged();

private:
    float _sp;   /** Position */
    float _st;
};

/** Write to stream. */
QDataStream &operator<<(QDataStream &, const SimulatorState &);

/** Read from stream */
QDataStream &operator>>(QDataStream &, SimulatorState &);

#endif // SIMULATORSTATE_H
