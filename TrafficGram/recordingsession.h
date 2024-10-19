#ifndef RECORDINGSESSION_H
#define RECORDINGSESSION_H

#include <QObject>

class RecordingSession : public QObject
{
    Q_OBJECT
public:
    explicit RecordingSession(QObject *parent = nullptr);

signals:

};

#endif // RECORDINGSESSION_H
