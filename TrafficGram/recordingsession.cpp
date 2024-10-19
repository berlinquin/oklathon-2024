#include "recordingsession.h"

RecordingSession::RecordingSession(QUrl trafficCamServer, QObject *parent)
    : QObject{parent}
    , m_trafficCamServer{std::move(trafficCamServer)}
{
}

void RecordingSession::startRecording()
{

}

void RecordingSession::stopRecording()
{

}
