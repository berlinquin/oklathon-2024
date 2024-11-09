#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "CameraListManager.h"

CameraPole cameraPoleFromJson(const QJsonObject& cameraJson)
{
    CameraPole out;
    if (cameraJson.contains("name"))
    {
        out.name = cameraJson["name"].toString();
    }
    return out;
}

QVector<CameraPole> cameraPolesFromJson(const QJsonDocument& jsonDocument)
{
    if (!jsonDocument.isArray())
    {
        return {};
    }

    QVector<CameraPole> out;

    auto jsonArray = jsonDocument.array();
    for (const auto& jsonDoc : jsonArray)
    {
        auto cameraJson = jsonDoc.toObject();
        out.push_back(cameraPoleFromJson(cameraJson));
    }
    return out;
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

    // TODO set this filter as a request header
    auto filter = R"({"include":[{"relation":"mapCameras","scope":{"include":"streamDictionary","where":{"status":{"neq":"Out Of Service"},"type":"Web","blockAtis":{"neq":"1"}}}},{"relation":"cameraLocationLinks","scope":{"include":["linkedCameraPole","cameraPole"]}}]})";

    m_networkManager->get(cameraPoleRequest);
}

void CameraListManager::onNetworkReplyReceived(QNetworkReply *reply)
{
    auto json = QJsonDocument::fromJson(reply->readAll());
    auto cameraList = cameraPolesFromJson(json);
    emit cameraListChanged(std::move(cameraList));
}
