#include "qdpwin.h"
#define ENRROLLING 1
#define VALIDATING 2
#include <QDebug>

WNDPROC OldWinProc   = NULL;
QWidget * Window = NULL;
HDPOPERATION op;
QDPWin * QDPWin::instance = NULL;

enum { QDP_EVENT = WM_USER + 1 };
int DeviceArrival = 0;
int DeviceRemoval = 0;

bool QDPWin::init(){
    HRESULT result = DPFPInit();
    return result == S_OK;
}

bool QDPWin::configure(QWidget * w, QObject * context){
    Window = w;
    QObject::connect(this, SIGNAL(onPush(int)), context, SLOT(onPush(int)));
    QObject::connect(this, SIGNAL(onVerify(bool)), context, SLOT(onVerify(bool)));
    enroll.configure(context, this);
    this->hwnd = (HWND)w->effectiveWinId();
    if(!override_window_procedure(this->hwnd))
    {
        return false;
    }
    return true;
}

bool QDPWin::start(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize){
    if (state == ENRROLLING){
        this->addToEnroll(pFingerprintImage, iFingerprintImageSize);
    }else
    if (state == VALIDATING){
        this->validate_roll(pFingerprintImage, iFingerprintImageSize);
    }
}

bool QDPWin::start_enrollment(){
    enroll.init();
    this->state = ENRROLLING;
    GUID * guids = new GUID();
    ULONG u;
    HRESULT result = DPFPEnumerateDevices(&u, &guids);
    if (result != S_OK){
        return false;
    }
    for (int i = 0; i < u; i++) {
        DP_DEVICE_INFO info;
        result = DPFPGetDeviceInfo(guids[i], &info);
        enrollment(this->hwnd, info.DeviceUid, QDP_EVENT);
    }
    return true;
}

bool QDPWin::start_verify(DATA_BLOB * rRegTemplate){
    qDebug() << "start verify";
    this->state = VALIDATING;
    validat.init();
    validat.LoadRegTemplate(rRegTemplate);
    GUID * guids = new GUID();
    ULONG u;
    HRESULT result = DPFPEnumerateDevices(&u, &guids);
    if (result != S_OK){
        return false;
    }
    for (int i = 0; i < u; i++) {
        DP_DEVICE_INFO info;
        result = DPFPGetDeviceInfo(guids[i], &info);
        enrollment(this->hwnd, info.DeviceUid, QDP_EVENT);
    }
    return true;
}

bool QDPWin::enrollment(HWND win, GUID uid, ULONG msg) {
    HRESULT result = DPFPCreateAcquisition(DP_PRIORITY_NORMAL, uid, DP_SAMPLE_TYPE_IMAGE, win, msg, &op);
    if (result != S_OK){
        return false;
    }
    result = DPFPStartAcquisition(op);
    if (result != S_OK){
        return false;
    }
    return true;
}

bool QDPWin::finish(){
    //QObject::disconnect(this, SIGNAL(onPush(int)));
    if (op) {
        DPFPStopAcquisition(op);    // No error checking - what can we do at the end anyway?
        DPFPDestroyAcquisition(op);
        op = 0;
    }else{
        qDebug() << "No borro";
    }
    return true;
}

bool QDPWin::finish_all(){
    this->enroll.finish();
    this->finish();
}

void QDPWin::onComplete(DATA_BLOB * temp_data){
    this->finish();
}

bool QDPWin::addToEnroll(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize){
    int num = enroll.addToEnroll(pFingerprintImage, iFingerprintImageSize);
    emit this->onPush(num);
    return num;
}

bool QDPWin::validate_roll(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize){
    qDebug() << "validating";
    bool resp = validat.Verify(pFingerprintImage, iFingerprintImageSize);
    emit this->onVerify(resp);
    return resp;
}

QDPWin * QDPWin::getInstance(){
    if (QDPWin::instance == NULL){
        instance = new QDPWin();
    }
    return QDPWin::instance;
}

LRESULT CALLBACK NewWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (QDP_EVENT == uMsg){
        //qDebug() << "MSG" << uMsg <<" Param:"<<wParam<<" To:"<<QDP_EVENT<<" lp:"<<lParam;
        dp_fetch(hwnd, uMsg, wParam, lParam);
    }
    return CallWindowProc(OldWinProc,hwnd,uMsg,wParam,lParam);
}

void dp_fetch(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(wParam) {
        case WN_COMPLETED:{
            DATA_BLOB * pImageBlob = reinterpret_cast<DATA_BLOB*>(lParam);
            if (uMsg == QDP_EVENT){
                QDPWin * win = QDPWin::getInstance();
                win->start(pImageBlob->pbData, pImageBlob->cbData);
            }
            break;
        }
        case WN_ERROR:{
            qDebug()<<"Error";
            break;
        }
        case WN_DISCONNECT:{
            qDebug()<<"disconnect";
            break;
        }
        case WN_RECONNECT:{
            qDebug()<<"recnect";
            break;
        }
        case WN_FINGER_TOUCHED:{
            qDebug()<<"touched";
            break;
        }
        case WN_FINGER_GONE:{
            qDebug()<<"Gone";
            break;
        }
        case WN_IMAGE_READY:{
            qDebug()<<"image ready";
            break;
        }
        case WN_OPERATION_STOPPED:{
            qDebug()<<"stop";
            break;
        }
    }
}

int override_window_procedure(HWND & hWnd)
{
    OldWinProc = (WNDPROC)GetWindowLong(hWnd,GWL_WNDPROC);
    if (!OldWinProc){
        return (int)OldWinProc;
    }
    return SetWindowLong(hWnd,GWL_WNDPROC,(LONG)(WNDPROC)NewWndProc);
}

