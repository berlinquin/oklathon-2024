#include "recordingsession.h"

#include <QProcess>

RecordingSession::RecordingSession(QUrl trafficCamServer, QObject *parent)
    : QObject{parent}
    , m_trafficCamServer{std::move(trafficCamServer)}
{
}

void RecordingSession::startRecording()
{
    static int counter = 0;
    ++counter;

    m_ffmpegProcess = new QProcess(this);
    m_ffmpegProcess->setProgram("~/Downloads/ffmpeg");
    QString outputFilename = QString("~/Downloads/out_").append(QString::number(counter)).append(".mp4");
    m_ffmpegProcess->setArguments({"-i", "https://stream.oktraffic.org/delay-stream/3de90bc6088b412a.stream/playlist.m3u8", "-codec", "copy", outputFilename});
    m_ffmpegProcess->start();
}

void RecordingSession::stopRecording()
{
    m_ffmpegProcess->terminate();
}
