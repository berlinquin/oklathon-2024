#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "CameraListManager.h"

CameraListManager::CameraListManager(QObject *parent)
    : QObject{parent}
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &CameraListManager::cameraListReceived);
}

void CameraListManager::loadCameraList()
{
    const QUrl cameraPoleEndpoint{"https://oktraffic.org/api/CameraPoles"};
    QNetworkRequest cameraPoleRequest{cameraPoleEndpoint};
    m_networkManager->get(cameraPoleRequest);
}

void CameraListManager::cameraListReceived(QNetworkReply *reply)
{
    auto json = QJsonDocument::fromJson(reply->readAll());
    if (!json.isArray())
    {
        emit cameraListReady(QVector<TrafficCamera>{});
        return;
    }

    QVector<TrafficCamera> out;

    auto jsonArray = json.array();
    for (const auto& jsonDoc : jsonArray)
    {
        // TODO handle each item in the array
    }
    emit cameraListReady(std::move(out));
}
