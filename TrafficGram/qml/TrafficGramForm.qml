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

import QtQuick
import QtQuick.Controls
import Esri.TrafficGram

Item {

    // Create MapQuickView here, and create its Map etc. in C++ code
    MapView {
        id: view
        anchors.fill: parent
        // set focus to enable keyboard navigation
        focus: true
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    TrafficGram {
        id: model
        mapView: view
    }

    Button {
        id: startSimulatedLocation
        text: "Start Simulated Location"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 5
        onClicked: model.startSimulatedLocation()

        background: Rectangle {
            border.color: "black"                                 // Set the border color
            color: parent.pressed ? "light gray" : "black"        // Background color of the button
            radius: 5                                             // Optional: rounded corners
        }
    }
}
