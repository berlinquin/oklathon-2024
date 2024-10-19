#ifndef RECORDINGSESSION_H
#define RECORDINGSESSION_H

#include <QObject>
#include <QUrl>

class QProcess;

class RecordingSession : public QObject
{
    Q_OBJECT
public:
    explicit RecordingSession(QUrl trafficCamServer, QObject *parent = nullptr);

public slots:
    void startRecording();
    void stopRecording();

signals:

private:
    QUrl m_trafficCamServer;
    QProcess* m_ffmpegProcess;
};

#endif // RECORDINGSESSION_H
