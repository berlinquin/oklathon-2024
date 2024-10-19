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

#include "Graphic.h"
#include "GraphicListModel.h"
#include "GraphicsOverlay.h"
#include "GraphicsOverlayListModel.h"
#include "PolylineBuilder.h"
#include "PolygonBuilder.h"
#include "SimpleFillSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleMarkerSymbol.h"
#include "SymbolTypes.h"

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
    const Point center(-97.51689, 35.49325, SpatialReference::wgs84());
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

    GraphicsOverlay* overlay = new GraphicsOverlay(this);
    createGraphics(overlay);
    m_mapView->graphicsOverlays()->append(overlay);

    emit mapViewChanged();
}

void TrafficGram::createGraphics(GraphicsOverlay *overlay)
{
    // I-44 & Broadway Ext
    const Point i_44_broadway_ext(-97.51602381374889, 35.529033242757436, SpatialReference::wgs84());

    // I-44 & Lincoln
    const Point i_44_lincoln(-97.50353703164697, 35.52921431832528, SpatialReference::wgs84());

    // Create symbols for the point
    SimpleLineSymbol* point_outline = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor("blue"), 3, this);
    SimpleMarkerSymbol* point_symbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor("red"), 15, this);
    point_symbol->setOutline(point_outline);

    {
        // Create a graphic to display the point with its symbology
        Graphic* point_graphic = new Graphic(i_44_broadway_ext, point_symbol, this);
        // Add point graphic to the graphics overlay
        overlay->graphics()->append(point_graphic);
    }
    {
        // Create a graphic to display the point with its symbology
        Graphic* point_graphic = new Graphic(i_44_lincoln, point_symbol, this);
        // Add point graphic to the graphics overlay
        overlay->graphics()->append(point_graphic);
    }
}
