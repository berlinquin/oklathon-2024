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

std::optional<TrafficCamera> trafficCameraFromJson(const QJsonObject& trafficCameraJson)
{
    auto location = trafficCameraJson["location"];
    if (!location.isString())
    {
        return {};
    }

    auto point = pointFromJson(trafficCameraJson["latitude"], trafficCameraJson["longitude"]);
    if (!point)
    {
        return {};
    }

    return TrafficCamera {
        location.toString(),
        std::move(*point)
    };
}

std::optional<CardinalDirection> cardinalDirectionFromJson(const QJsonValue& cardinalDirectionValue)
{
    if (!cardinalDirectionValue.isString())
    {
        return {};
    }

    auto cardinalDirection = cardinalDirectionValue.toString();
    if (cardinalDirection == "N")
    {
        return CardinalDirection::N;
    }
    if (cardinalDirection == "NE")
    {
        return CardinalDirection::NE;
    }
    if (cardinalDirection == "E")
    {
        return CardinalDirection::E;
    }
    if (cardinalDirection == "SE")
    {
        return CardinalDirection::SE;
    }
    if (cardinalDirection == "S")
    {
        return CardinalDirection::S;
    }
    if (cardinalDirection == "SW")
    {
        return CardinalDirection::SW;
    }
    if (cardinalDirection == "W")
    {
        return CardinalDirection::W;
    }
    if (cardinalDirection == "NW")
    {
        return CardinalDirection::NW;
    }
    return {};
}

std::optional<CameraPole> cameraPoleFromJson(const QJsonObject& cameraPoleJson)
{
    CameraPole out;

    auto name = cameraPoleJson["name"];
    if (!name.isString())
    {
        return {};
    }
    out.name = name.toString();

    auto mapCamerasJson = cameraPoleJson["mapCameras"];
    if (!mapCamerasJson.isArray())
    {
        return {};
    }
    auto mapCamerasArray = mapCamerasJson.toArray();
    std::unordered_map<CardinalDirection, TrafficCamera> mapCameras;
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
        mapCameras.emplace(*cardinalDirection, std::move(*trafficCam));
    }
    out.cameras = std::move(mapCameras);

    return out;
}

QVector<CameraPole> cameraPolesFromJson(const QJsonDocument& json)
{
    if (!json.isArray())
    {
        return {};
    }
    auto jsonArray = json.array();

    QVector<CameraPole> out;
    for (const auto& jsonElement : jsonArray)
    {
        if (!jsonElement.isObject())
        {
            return {};
        }
        auto cameraPole = cameraPoleFromJson(jsonElement.toObject());
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
