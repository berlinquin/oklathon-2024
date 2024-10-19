#include "recordingsession.h"

#include <QProcess>

RecordingSession::RecordingSession(QString trafficCamServer, QObject *parent)
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
    m_ffmpegProcess->setArguments({"-i", m_trafficCamServer, "-codec", "copy", outputFilename});
    m_ffmpegProcess->start();
    auto state = m_ffmpegProcess->state();

    connect(m_ffmpegProcess, &QProcess::stateChanged, [&](auto newState){
        qDebug("newState: %d", newState);
    });
}

void RecordingSession::stopRecording()
{
    auto code = m_ffmpegProcess->exitCode();
    auto status = m_ffmpegProcess->exitStatus();
    auto output = m_ffmpegProcess->readAllStandardOutput();
    QString output_str = output;
    m_ffmpegProcess->write("q");
    m_ffmpegProcess->closeWriteChannel();
}
