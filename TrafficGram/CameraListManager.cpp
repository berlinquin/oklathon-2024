#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "CameraListManager.h"

QVector<TrafficCamera> cameraListFromJson(const QJsonDocument& jsonDocument)
{
    if (!jsonDocument.isArray())
    {
        return {};
    }

    QVector<TrafficCamera> out;

    auto jsonArray = jsonDocument.array();
    for (const auto& jsonDoc : jsonArray)
    {
        // TODO handle each item in the array
    }
}

CameraListManager::CameraListManager(QObject *parent)
    : QObject{parent}
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &CameraListManager::onNetworkReplyReceived);
}

void CameraListManager::loadCameraList()
{
    const QUrl cameraPoleEndpoint{"https://oktraffic.org/api/CameraPoles"};
    QNetworkRequest cameraPoleRequest{cameraPoleEndpoint};
    m_networkManager->get(cameraPoleRequest);
}

void CameraListManager::onNetworkReplyReceived(QNetworkReply *reply)
{
    auto json = QJsonDocument::fromJson(reply->readAll());
    auto cameraList = cameraListFromJson(json);
    emit cameraListChanged(std::move(cameraList));
}
