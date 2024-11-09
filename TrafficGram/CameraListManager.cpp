#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include <SpatialReference.h>

#include "CameraListManager.h"

std::optional<Esri::ArcGISRuntime::Point> pointFromJson(const QJsonValue& latitude, const QJsonValue& longitude)
{
    if (!latitude.isString())
    {
        return {};
    }
    if (!longitude.isString())
    {
        return {};
    }

    bool conversionOk = true;
    auto x = longitude.toString().toDouble(&conversionOk);
    auto y = latitude.toString().toDouble(&conversionOk);
    if (!conversionOk)
    {
        return {};
    }

    return Esri::ArcGISRuntime::Point(x, y, Esri::ArcGISRuntime::SpatialReference::wgs84());
}

std::optional<TrafficCamera> trafficCameraFromJson(const QJsonObject& cameraJson)
{
    auto location = cameraJson["location"];
    if (!location.isString())
    {
        return {};
    }

    auto point = pointFromJson(cameraJson["latitude"], cameraJson["longitude"]);
    if (!point)
    {
        return {};
    }

    return TrafficCamera {
        location.toString(),
        std::move(*point)
    };
}

std::optional<CardinalDirection> cardinalDirectionFromJson(const QJsonValue& value)
{
    if (!value.isString())
    {
        return {};
    }

    auto valueStr = value.toString();
    if (valueStr == "N")
    {
        return CardinalDirection::N;
    }
    if (valueStr == "NE")
    {
        return CardinalDirection::NE;
    }
    if (valueStr == "E")
    {
        return CardinalDirection::E;
    }
    if (valueStr == "SE")
    {
        return CardinalDirection::SE;
    }
    if (valueStr == "S")
    {
        return CardinalDirection::S;
    }
    if (valueStr == "SW")
    {
        return CardinalDirection::SW;
    }
    if (valueStr == "W")
    {
        return CardinalDirection::W;
    }
    if (valueStr == "NW")
    {
        return CardinalDirection::NW;
    }
    return {};
}

std::optional<CameraPole> cameraPoleFromJson(const QJsonObject& cameraJson)
{
    CameraPole out;

    auto name = cameraJson["name"];
    if (!name.isString())
    {
        return {};
    }
    out.name = name.toString();

    auto mapCameras = cameraJson["mapCameras"];
    if (!mapCameras.isArray())
    {
        return {};
    }
    auto mapCamerasArray = mapCameras.toArray();
    std::unordered_map<CardinalDirection, TrafficCamera> mapCamerasOut;
    for (const auto& mapCamera : mapCamerasArray)
    {
        if (!mapCamera.isObject())
        {
            return {};
        }
        auto mapCameraObject = mapCamera.toObject();
        auto cardinalDirection = cardinalDirectionFromJson(mapCameraObject["direction"]);
        if (!cardinalDirection)
        {
            return {};
        }
        auto trafficCam = trafficCameraFromJson(mapCameraObject);
        if (!trafficCam)
        {
            return {};
        }
        mapCamerasOut.emplace(*cardinalDirection, std::move(*trafficCam));
    }
    out.cameras = std::move(mapCamerasOut);

    return out;
}

QVector<CameraPole> cameraPolesFromJson(const QJsonDocument& jsonDocument)
{
    if (!jsonDocument.isArray())
    {
        return {};
    }
    auto jsonArray = jsonDocument.array();

    QVector<CameraPole> out;
    for (const auto& jsonDoc : jsonArray)
    {
        if (!jsonDoc.isObject())
        {
            return {};
        }
        auto cameraPole = cameraPoleFromJson(jsonDoc.toObject());
        if (!cameraPole)
        {
            return {};
        }

        out.push_back(std::move(*cameraPole));
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
