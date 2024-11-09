#ifndef CAMERALISTMANAGER_H
#define CAMERALISTMANAGER_H

#include <QObject>

#include <Point.h>

class QNetworkAccessManager;
class QNetworkReply;

enum class CardinalDirection
{
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW
};

struct TrafficCamera
{
    QString name;
    Esri::ArcGISRuntime::Point location;
};

// A camera pole may have multiple traffic cams on it.
// Each traffic cam is associated with a cardinal direction.
struct CameraPole
{
    QString name;
    std::unordered_map<CardinalDirection, TrafficCamera> cameras;
};

std::optional<CameraPole> cameraPoleFromJson(const QJsonObject& cameraJson);
QVector<CameraPole> cameraPolesFromJson(const QJsonDocument& jsonDocument);

class CameraListManager : public QObject
{
    Q_OBJECT
public:
    explicit CameraListManager(QObject *parent = nullptr);

    void loadCameraList();

signals:
     void cameraListChanged(QVector<CameraPole>);

private slots:
    void onNetworkReplyReceived(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkManager;

};

#endif // CAMERALISTMANAGER_H
