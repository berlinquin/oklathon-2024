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

#include "TrafficGram.h"

#include "Map.h"
#include "MapTypes.h"
#include "MapQuickView.h"

#include "Point.h"
#include "Viewpoint.h"
#include "SpatialReference.h"
#include <QFuture>

using namespace Esri::ArcGISRuntime;

TrafficGram::TrafficGram(QObject* parent /* = nullptr */):
    QObject(parent),
    m_map(new Map(BasemapStyle::ArcGISStreets, this))
{
}

TrafficGram::~TrafficGram()
{
}

MapQuickView* TrafficGram::mapView() const
{
    return m_mapView;
}

void TrafficGram::setupViewpoint()
{

    const Point center(-118.80543, 34.02700, SpatialReference::wgs84());
    const Viewpoint viewpoint(center, 100000.0);
    m_mapView->setViewpointAsync(viewpoint);
}

// Set the view (created in QML)
void TrafficGram::setMapView(MapQuickView* mapView)
{
    if (!mapView || mapView == m_mapView)
    {
        return;
    }

    m_mapView = mapView;
    m_mapView->setMap(m_map);

    setupViewpoint();

    emit mapViewChanged();
}
