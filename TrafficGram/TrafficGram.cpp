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
#include "SimpleLineSymbol.h"
#include "SimpleMarkerSymbol.h"
#include "SymbolTypes.h"

#include "SimulatedLocationDataSource.h"
#include "SimulationParameters.h"
#include "LocationGeotriggerFeed.h"
#include "LocationDisplay.h"
#include "GraphicFenceParameters.h"
#include "FenceGeotrigger.h"
#include "GeotriggersTypes.h"

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

    setupGeotriggers();

    emit mapViewChanged();
}

void TrafficGram::createGraphics(GraphicsOverlay *overlay)
{
    // I-44 & Broadway Ext
    const Point i_44_broadway_ext(-97.51602381374889, 35.529033242757436, SpatialReference::wgs84());

    // I-44 & Lincoln
    const Point i_44_lincoln(-97.50353703164697, 35.52921431832528, SpatialReference::wgs84());

    // Add route from Western to Kelly
    PolylineBuilder* polyline_route = new PolylineBuilder(SpatialReference::wgs84(), this);
    polyline_route->addPoint(-97.52981, 35.52577); // Western
    polyline_route->addPoint(-97.52798, 35.52712); // Start of western curve
    polyline_route->addPoint(-97.52638, 35.52780); // Apex of western curve
    polyline_route->addPoint(-97.52185, 35.52806); // curve towards tracks
    polyline_route->addPoint(-97.51895, 35.52878); // railroad tracks
    polyline_route->addPoint(-97.51744, 35.52920); // before 235
    polyline_route->addPoint(-97.51363, 35.52949); // 235 overpass
    polyline_route->addPoint(-97.50841, 35.52942); // Halfway to Lincoln
    polyline_route->addPoint(-97.50415, 35.52955); // Overpass for Lincoln
    polyline_route->addPoint(-97.49985, 35.53091); // Another between these two
    polyline_route->addPoint(-97.49747, 35.53242); // Halfway to Kelly
    polyline_route->addPoint(-97.49419, 35.53506); // Kelly

    m_routePolyline = polyline_route->toPolyline();

    SimpleLineSymbol* route_symbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor(Qt::green), 1, this);
    Graphic* polyline_graphic = new Graphic(polyline_route->toGeometry(), route_symbol, this);

    overlay->graphics()->append(polyline_graphic);

    // Create symbols for the point
    SimpleLineSymbol* point_outline = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor("blue"), 3, this);
    SimpleMarkerSymbol* point_symbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor("red"), 15, this);
    point_symbol->setOutline(point_outline);

    {
        // Create a graphic to display the point with its symbology
        Graphic* point_graphic = new Graphic(i_44_broadway_ext, point_symbol, this);
        // Add point graphic to the graphics overlay
        overlay->graphics()->append(point_graphic);
        m_graphics.push_back(point_graphic);
    }
    {
        // Create a graphic to display the point with its symbology
        Graphic* point_graphic = new Graphic(i_44_lincoln, point_symbol, this);
        // Add point graphic to the graphics overlay
        overlay->graphics()->append(point_graphic);
        m_graphics.push_back(point_graphic);
    }
}

void TrafficGram::setupGeotriggers()
{
    // Following this guide: https://developers.arcgis.com/qt/device-location/work-with-geotriggers/#geotrigger-feed

    // SETUP SIMULATED LOCATION DATA SOURCE

    // Create a new simulated location data source.
    SimulatedLocationDataSource* simulatedDeviceLocation = new SimulatedLocationDataSource(this);

    // Create a new simulation parameters; set the start time and velocity.
    SimulationParameters* simulatedLocationDataSource = new SimulationParameters(this);
    simulatedLocationDataSource->setStartTime(QDateTime::currentDateTime());
    simulatedLocationDataSource->setVelocity(100); // Meters/Second

    // The simulated location will move across the provided polyline.
    simulatedDeviceLocation->setLocationsWithPolyline(m_routePolyline, simulatedLocationDataSource);

    // Create a new Geotrigger feed with the simulated location source.
    LocationGeotriggerFeed* locationGeotriggerFeed = new LocationGeotriggerFeed(simulatedDeviceLocation, this);

    // Get the location display from the map view.
    LocationDisplay* locationDisplay = m_mapView->locationDisplay();

    // Enable location display on the map view using the same simulated location source.
    locationDisplay->setDataSource(simulatedDeviceLocation);

    // SETUP GEOFENCE
    const auto bufferDistance = 200.0;
    GraphicFenceParameters *graphicFenceParameters = new GraphicFenceParameters(m_graphics, bufferDistance, this);

    // Create a geotrigger with the location feed, "enter" rule type, and the fence parameters.
    FenceGeotrigger* fenceGeotrigger = new FenceGeotrigger(locationGeotriggerFeed, FenceRuleType::EnterOrExit, graphicFenceParameters, this);

    m_geotriggerMonitor = new GeotriggerMonitor(fenceGeotrigger, this);
    connect(m_geotriggerMonitor, &GeotriggerMonitor::geotriggerNotification, this, [this] (GeotriggerNotificationInfo* geotriggerNotificationInfo)
    {
    });

    QFuture<void> ignored = m_geotriggerMonitor->startAsync();
    Q_UNUSED(ignored);
    locationDisplay->start();
}

void TrafficGram::startSimulatedLocation()
{
    LocationDisplay* locationDisplay = m_mapView->locationDisplay();
    locationDisplay->start();
}
