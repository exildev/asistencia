import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1

Rectangle {
    id:main
    visible: true
    height:600
    width: 800
    y:0
    anchors.centerIn: parent
    SystemPalette {id: syspal}
    color: "#D0DAD4"

    Rectangle {
        id: toolbar
        width: parent.width
        height: 27
        anchors.top: parent.top
        color: "#D0DAD4"
        TextField{
            id:searchField
            placeholderText: "Buscar..."
            anchors.right: buscarBoton.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            height: buscarBoton.height - 2
        }
        Button{
            id:buscarBoton
            text: ""
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            width: 25
            Image {
                source: "media/lupa.png"
                height: parent.height - 6
                anchors.left: parent.left
                anchors.leftMargin: 3
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
            }
        }
        Rectangle{
            height: 2
            width: parent.width
            anchors.bottom: parent.bottom
            color: "#999"
        }
    }

    Column{
        id:huellaContainer
        width: parent.width * 0.3
        anchors.top: toolbar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 8
        Rectangle{
            id:huellaHeader
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 20
            height: 30
            border.color: syspal.dark
            color: syspal.window
            Text{
                anchors.centerIn: parent
                text: "Coloque su dedo en el huellero"
            }
        }
        Rectangle{
            id: huella
            anchors.top: huellaHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 10
            height: 350
            border.color: syspal.dark
            color: "#fff"
            Image {
                id:imgHuella
                source: "media/huella.png"
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                anchors.margins: 2
                opacity: 0.4
            }
            Text {
                id: conteo
                text: qsTr("-")
                font.pointSize: 150
                anchors.centerIn: parent
                opacity: 0.8
            }
        }
        Button{
            id: guardarHuella
            text: ""
            height: 30
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: huella.bottom
            anchors.topMargin: 10
            enabled: false
            Item{
                height: parent.height - 8
                anchors.centerIn: parent
                width: 70
                Image{
                    id:imgGuardar
                    source: "media/guardar.png"
                    height: parent.height - 6
                    anchors.left: parent.left
                    anchors.leftMargin: 3
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter:parent.verticalCenter

                }
                Text{
                    id:textGuardar
                    text: "Guardar"
                    anchors.left: imgGuardar.right
                    anchors.leftMargin: 3
                    anchors.verticalCenter: parent.verticalCenter
                    color:syspal.dark
                }
            }
            onClicked: {
                funcs.saveTemplate(templateDialog.pk)
            }
        }
        Button{
            id:reset
            text: ""
            height: 30
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: guardarHuella.bottom
            anchors.topMargin: 10
            enabled: false
            onClicked: funcs.resetTemplate()
            Item{
                height: parent.height - 8
                anchors.centerIn: parent
                width: 74
                Image{
                    id:imgReset
                    source: "media/reintentar.png"
                    height: parent.height - 6
                    anchors.left: parent.left
                    anchors.leftMargin: 3
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter:parent.verticalCenter
                }
                Text{
                    id:textReset
                    text: "Reintentar"
                    anchors.left: imgReset.right
                    anchors.leftMargin: 3
                    anchors.verticalCenter: parent.verticalCenter
                    color: syspal.dark
                }
            }
        }
    }

    Column {
        id:tabsContainer
        anchors.top: toolbar.bottom
        anchors.right: huellaContainer.left
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 8
        TabView {
            id:frame
            focus:true
            enabled: true

            property int margins: Qt.platform.os === "osx" ? 16 : 0

            height: parent.height - 34
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.margins: margins
        }
    }

    Item {
        id: funcs
        objectName: "funcs"
        signal saveValidable(int id, bool flag)
        signal createTemplate(int id)
        signal saveTemplate(int id)
        signal resetTemplate()
        function activarHuella(){
            reset.enabled = true
            textReset.color = syspal.windowText
            guardarHuella.enabled = false
            imgHuella.opacity = 1
            conteo.text = "4"
        }

        function successTemplate(){
            messageDialog.show("guardado  correctamente");
            imgHuella.opacity = 0.4
            guardarHuella.enabled = false
            textGuardar.color = syspal.dark
            reset.enabled = false
            textGuardar.color = syspal.dark
            conteo.text = "-"
        }

        function finishTemplate(){
            guardarHuella.enabled = true
            textGuardar.color = syspal.windowText
            reset.enabled = false
            textReset.color = syspal.dark
            conteo.text = "-"
        }

        function updateHuella(num){
            conteo.text = num
        }
        function noTemplate(id){
            templateDialog.show(id)
        }

        function getCurrentId(){
            return templateDialog.pk
        }

        function savedValidable(s){
            if(s){
                messageDialog.show("guardado  correctamente");
            }else{
                messageDialog.show("fallo al guardar");
            }
        }
        signal getValidables()
        function renderValidables(list){
            for(var i in list){
                var tab = "import QtQuick 2.2;
                           import QtQuick.Window 2.1;
                           import QtQuick.Controls 1.2;
                           Tab{ title:\""+ i +"\";";
                tab +='TableView {
                   anchors.fill: parent;';
                   for(var key in list[i][0]){
                       if(key==="activo"){
                           tab +='TableViewColumn{ role: "activo" ; title: "activo" ; width: 51
                               delegate: CheckBox{
                                   id:check'+ i +'
                                   checked: model'+ i +'.get(styleData.row).activo
                                   objectName:"model'+ i +'"
                                   anchors.centerIn: parent
                                   onClicked: {funcs.saveValidable(model'+ i +'.get(styleData.row).pk, checked);}
                               }
                           }';
                       }else if(!(key==="id")){
                           tab += 'TableViewColumn{ role: "'+ key +'"  ; title: "'+key+'" ; width: 150 }';
                       }
                   }

                   tab+='model: ListModel {
                       id: model'+ i +';'
                   for(var j in list[i]){
                       tab+='ListElement{';
                       for(var k in list[i][j]){
                           if(k==="activo"){
                               tab += ''+k+': '+list[i][j][k]+' ; ';
                           }else if(k==="id"){
                               tab += 'pk: "'+list[i][j][k]+'" ; ';
                           }else{
                               tab += ''+k+': "'+list[i][j][k]+'" ; ';
                           }
                       }

                       tab+='}'
                   }
                   tab+='}
                }';
                tab += "}";
                Qt.createQmlObject(tab,frame,'log');
            }
        }
    }
    Timer {
        id:timer
        interval: 800;
        running: false;
        repeat: false;
        onTriggered: messageDialog.visible = false;
    }
    MessageDialog {
        id: messageDialog
        title: qsTr("Mensaje")
        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
            timer.start();
        }
    }

    MessageDialog{
        property int pk: 0
        id:templateDialog
        text:"No se a asigando una huella a este usuario ¿desea hacerlo ahora?"
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            funcs.createTemplate(templateDialog.pk)
            funcs.activarHuella()
        }
        onNo: {
            console.log("dijeron que no ")
            for(var i = frame.__tabs.count - 1; i >= 0; i--){
                frame.removeTab(i);
            }
            funcs.getValidables();
        }
        function show(id) {
            templateDialog.pk = id
            templateDialog.open();
        }
    }
}
