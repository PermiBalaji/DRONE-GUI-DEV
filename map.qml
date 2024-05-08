import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Rectangle{
    property double oldlat: 22.326000
    property double oldlng: 87.313499

    Plugin{
        id: mapPlugin
        name: "googlemapsplugin"
        PluginParameter{
            name: "apiKey"
            value: "AIzaSyDKMR8F75jyT3SYioZ0mt16smq8ksE5z8s"
        }
    }
    Map{
        id:mapView
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(oldlat,oldlng);
        zoomLevel: 6
    }
    function setCenter(lat,lng){
        mapView.pan(oldlat-lat,oldlng-lng)
        oldlat=lat
        oldlng=lng
    }
}
