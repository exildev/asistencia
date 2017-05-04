#ifndef QENROLLMENT_H
#define QENROLLMENT_H
#include "qdph.h"

class QEnrollment: public QObject{
    Q_OBJECT
public:
    QEnrollment();
    ~QEnrollment();
    int addToEnroll(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize);
    LRESULT init();
    bool configure(QObject *context, QObject *q);
    void GetRegTemplate(DATA_BLOB& rRegTemplate) const ;
    int create_enrollment(FT_BYTE *pPreRegTemplate, FT_BYTE *pRegTemplate, int iRecommendedPreRegFtrLen);
    int create_template(int & m_nRegFingerprint, FT_BYTE * pPreRegTemplate, FT_BYTE * pRegTemplate, FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize);
    bool finish();

signals:
    void onComplete(DATA_BLOB * temp_data);
private:
    FT_HANDLE      m_fxContext;              // Context for Feature Extraction functions, must be created before calling any of the FT_ functions
    FT_HANDLE      m_mcContext;              // Context for Matching functions, must be created before calling any of the MC_ functions

    HDPOPERATION   m_hOperationEnroll;     // Handle of the Operation, must be closed at exit.

    RECT           m_rcDrawArea;             // Rectangle with the desired sizes of the fingerprint image to display

    int            m_NumberOfPreRegFeatures; // Number of the pre-Enrollment fingerprint templates needed to create one Enrollment template.
    FT_BYTE**      m_TemplateArray;          // Array that keeps pre-Enrollment fingerprint templates. It is allocated with the size equal to number of pre-Enrollment templetes to create a Enrollment template (usually 4)
    int            m_nRegFingerprint;        // Pre-Enrollment number, index in the above array.
    FT_REG_OPTIONS m_mcRegOptions;           // Enrollment options. Collected from check boxes of the dialog.

    DATA_BLOB      m_RegTemplate;            // BLOB that keeps Enrollment Template.

};

#endif // QENROLLMENT_H
