// Copyright 2024 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef TRAFFICGRAM_H
#define TRAFFICGRAM_H

#include <QObject>

#include "Polyline.h"
#include "GeotriggerMonitor.h"

#include "CameraListManager.h"

namespace Esri::ArcGISRuntime {
class Graphic;
class Map;
class MapQuickView;
class GraphicsOverlay;
} // namespace Esri::ArcGISRuntime

class CameraListManager;

Q_MOC_INCLUDE("MapQuickView.h")

class TrafficGram : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)

public:
    explicit TrafficGram(QObject* parent = nullptr);
    ~TrafficGram() override;

signals:
    void showVideo(const QUrl urlString, QString stationName);
    void hideVideo();

public slots:
    void loadCameraList();
    void handleCameraListChanged(QVector<CameraPole>);
    void startSimulatedLocation();

signals:
    void mapViewChanged();

private:
    Esri::ArcGISRuntime::MapQuickView* mapView() const;
    void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

    void setupViewpoint();
    void createGraphics(Esri::ArcGISRuntime::GraphicsOverlay* overlay);
    void setupGeotriggers();

    Esri::ArcGISRuntime::GraphicsOverlay* m_graphicsOverlay;
    Esri::ArcGISRuntime::Polyline m_routePolyline;
    QList<Esri::ArcGISRuntime::Graphic*> m_graphics;
    CameraListManager *m_cameraListManager;

    Esri::ArcGISRuntime::Map* m_map = nullptr;
    Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;

    Esri::ArcGISRuntime::GeotriggerMonitor* m_geotriggerMonitor = nullptr;
};

#endif // TRAFFICGRAM_H
