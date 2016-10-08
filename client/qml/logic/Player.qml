import QtQuick 2.0

Item {
    property var mans: []
    property string color

    function addMan(man){
        mans+=man
    }
}
