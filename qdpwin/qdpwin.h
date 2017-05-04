#ifndef QDPWIN_H
#define QDPWIN_H
#include "qdph.h"
#include "qenrollment.h"
#include "qvalidation.h"

// Global -- Original Window Procedure

class QDPWin: public QObject{
       Q_OBJECT
private:

    HWND hwnd;
    QEnrollment enroll;
    QValidation validat;
    int state;
public:
    bool init();
    bool configure(QWidget * w, QObject * context);
    bool enrollment(HWND win, GUID uid, ULONG msg);
    bool start(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize);
    bool start_enrollment();
    bool start_verify(DATA_BLOB *rRegTemplate);


    bool addToEnroll(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize);
    bool validate_roll(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize);

    static QDPWin * getInstance();
    static QDPWin * instance;
    bool finish();
    bool finish_all();

public slots:
    void onComplete(DATA_BLOB * temp_data);

signals:
    void onPush(int num);
    void onVerify(bool resp);
};

int override_window_procedure(HWND & hWnd);
void dp_fetch(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewWndPro(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


#endif // QDPWIN_H
