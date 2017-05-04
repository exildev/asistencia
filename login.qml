import QtQuick 2.3
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2

Window {
    minimumWidth: 800
    minimumHeight: 600
    maximumWidth: 800
    maximumHeight: 600
    height:600
    width: 800
    visible: true
    color: "#D0DAD4"
    FontLoader{id:robotoThin; source: "media/Roboto-Light.ttf"; name:"Roboto Thin"}
    Rectangle{
        id:loginContainer
        width: 400
        height: 350
        anchors.centerIn: parent
        Rectangle{
            id:header
            anchors.top: parent.top
            width: parent.width
            height: 50
            color:"#999"
            Text {
                text: qsTr("INICIAR SESIÓN")
                font.family: robotoThin.name
                font.pointSize: 20
                color:"white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5
            }
        }
        Rectangle{
            id:body
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            color:"#e1e1e1"
            Rectangle{
                id:logo
                width: 230
                height: 110
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                color:parent.color
                Image {
                    source: "media/logo.png"
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                }
            }
            Rectangle{
                id:errorMessage
                anchors.top: logo.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                visible: false
                height: 20
                width: 230
                color:parent.color
                Text{
                    text: "Usuario y/o contraseña no valido"
                    anchors.centerIn: parent
                    font.pointSize: 12
                    font.family: robotoThin.name
                    color:"#fc5400"
                }
            }
            Rectangle{
                id:username
                anchors.top: logo.bottom
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                width: 230
                height: 29
                color: parent.color
                TextField{
                    id:usernameField
                    focus: true
                    textColor: "#555"
                    font.family: robotoThin.name
                    font.pointSize: 14
                    placeholderText: "Usuario"
                    anchors.fill: parent
                    anchors.margins: 2
                    style: TextFieldStyle{
                        background: Rectangle{
                            color: "#000";
                            opacity: 0.0;
                        }
                    }
                    Keys.onReturnPressed: {
                        password.focus = true
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 2
                    anchors.bottom: parent.bottom
                    color:"#999"
                }
                Rectangle{
                    height: parent.height
                    width: 2
                    anchors.right: parent.right
                    color:"#999"
                }
            }
            Rectangle{
                id:password
                anchors.top: username.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter
                width: 230
                height: 29
                color:parent.color
                TextField{
                    id:passwordField
                    textColor: "#555"
                    font.family: robotoThin.name
                    font.pointSize: 14
                    placeholderText: "Contraseña"
                    echoMode: TextInput.Password
                    anchors.fill: parent
                    anchors.margins: 2
                    style: TextFieldStyle{
                        background: Rectangle{
                            color: "#000";
                            opacity: 0.0;
                        }
                    }
                    Keys.onReturnPressed: {
                        loadingDialog.open();
                        timer.start()
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 2
                    anchors.bottom: parent.bottom
                    color:"#999"
                }
                Rectangle{
                    height: parent.height
                    width: 2
                    anchors.right: parent.right
                    color:"#999"
                }
            }
            Rectangle{
                width: 230
                height: 40
                anchors.top: password.bottom
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                color: {
                    if(!submit.pressed){
                        "#4FA372"
                    }else{
                        "#3C825C"
                    }
                }
                Text{

                    text: "INGRESAR"
                    anchors.centerIn: parent
                    color: "#f9f9f9"
                    font.family: robotoThin.name
                    font.pointSize: 20
                    style: Text.Raised
                    styleColor: "#14291D"
                }
                Rectangle{
                    width: parent.width
                    height: 2
                    anchors.bottom: parent.bottom
                    color: "#3C825C"
                }
                MouseArea{
                    id:submit
                    anchors.fill: parent
                    onClicked: {
                        loadingDialog.open();
                        timer.start()
                    }
                }
            }
        }
    }
    Item {
        id: funcs
        objectName: "funcs"
        signal login(string user, string pass);
        function loginError(){
            errorMessage.visible = true
        }
        Dialog {
            id:loadingDialog
            title: "Cargando"
            contentItem: Rectangle {
                color: "#D0DAD4"
                implicitWidth: 200
                implicitHeight: 100
                Text {
                    text: "Iniciando sesion"
                    color: "#555"
                    font.family: robotoThin.name
                    font.pointSize: 14
                    anchors.centerIn: parent
                }
            }
        }
        Timer {
            id:timer
            interval: 100;
            running: false;
            repeat: false;
            onTriggered: {
                funcs.login(usernameField.text,passwordField.text);
                loadingDialog.close();
            }
        }
    }
}

