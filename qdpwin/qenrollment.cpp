#include "qenrollment.h"
#include <QDebug>

QEnrollment::QEnrollment()
{
    m_nRegFingerprint = 0;
    m_NumberOfPreRegFeatures = 0;
}

QEnrollment::~QEnrollment()
{
    finish();
}


bool QEnrollment::finish(){
    //QObject::disconnect(this, SIGNAL(onComplete(DATA_BLOB&)));
    m_nRegFingerprint = 0;
    if (m_fxContext) {
        FX_closeContext(m_fxContext);
        m_fxContext = 0;
    }

    if (m_mcContext) {
        MC_closeContext(m_mcContext);
        m_mcContext = 0;
    }

    if(m_TemplateArray){
        for (int i=0; i<m_NumberOfPreRegFeatures; ++i)
            if(m_TemplateArray[i]) delete [] m_TemplateArray[i], m_TemplateArray[i] = NULL; // Delete Pre-Enrollment feature sets stored in the array.

        delete [] m_TemplateArray; // delete the array itself.
        m_TemplateArray = NULL;
    }

    return true;
}

int QEnrollment::create_enrollment(FT_BYTE * pPreRegTemplate, FT_BYTE * pRegTemplate, int iRecommendedPreRegFtrLen){
    FT_RETCODE rc = FT_OK;
    // We collected enough Pre-Enrollment feature sets, create Enrollment template out of them.
   // Get the recommended length of the fingerprint template (features).
   // It actually could be done once at the beginning, but this is just sample code...
   int iRecommendedRegFtrLen = 0;
   rc = MC_getFeaturesLen(FT_REG_FTR, m_mcRegOptions, &iRecommendedRegFtrLen, NULL);

   if (FT_OK != rc) qDebug() << (E_FAIL);

   if (NULL == (pRegTemplate = new FT_BYTE[iRecommendedRegFtrLen])) qDebug() << (E_OUTOFMEMORY);

   FT_BOOL bRegSucceeded = FT_FALSE;
   rc = MC_generateRegFeatures(m_mcContext,              // Matching Context
                               m_mcRegOptions,           // Options - collected from checkboxes in the dialog
                               m_NumberOfPreRegFeatures, // Number of Pre-Enrollment feature sets submitted. It must be number received previously from the SDK.
                               iRecommendedPreRegFtrLen, // Length of every Pre-Enrollment feature sets in the array
                               m_TemplateArray,          // Array of pointers to the Pre-Enrollment feature sets
                               iRecommendedRegFtrLen,    // Length of the Enrollment Features(template) buffer received previously from the SDK
                               pRegTemplate,             // Pointer to the buffer where the Enrollment template to be stored
                               NULL,                     // Reserved. Must always be NULL.
                               &bRegSucceeded);          // Returned result of Enrollment Template creation. FT_TRUE means extracted OK.

   if (FT_OK <= rc && bRegSucceeded == FT_TRUE) {
       // Enrollment template is generated.
       // Normally it should be saved in some database, etc.
       // Here we just put it into a BLOB to use later for verification.
       m_RegTemplate.pbData = pRegTemplate;
       m_RegTemplate.cbData = iRecommendedRegFtrLen;
       emit this->onComplete(&m_RegTemplate);
       finish();
       pRegTemplate = NULL;   // This prevents deleting at the end of the function

       throw (("Enrollment Template generated successfully"));
       throw (("Close this dialog and run Verification to verify fingerprint"));
   }
   else {
       throw ("Creation of Enrollment Template Failed.");
       throw (("Scan your finger for enrollment."));
       // Enrolment faled, cleanup data.
       m_nRegFingerprint = 0;

       for (int i=0; i<m_NumberOfPreRegFeatures; ++i){
           if(m_TemplateArray[i]) {
               delete [] m_TemplateArray[i], m_TemplateArray[i] = NULL; // Delete Pre-Enrollment feature sets stored in the array.
           }
       }
   }
}

int QEnrollment::create_template(int & m_nRegFingerprint, FT_BYTE * pPreRegTemplate, FT_BYTE * pRegTemplate, FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize){
    // Do not generate more Pre-Enrollment feature sets than needed
    FT_RETCODE rc = FT_OK;

    // Get recommended length of the Pre-Enrollment Fingerprint feature sets.
    // It actually could be done once at the beginning, but this is just sample code...
    int iRecommendedPreRegFtrLen = 0;
    rc = FX_getFeaturesLen(FT_PRE_REG_FTR, &iRecommendedPreRegFtrLen, NULL); // Minimal Fingerprint feature set Length - do not use if possible
    if (FT_OK != rc){
        throw ("Can't configure finger print.");
    }

    // Extract Features (i.e. create fingerprint template)
    FT_IMG_QUALITY imgQuality;
    FT_FTR_QUALITY ftrQuality;
    FT_BOOL bEextractOK = FT_FALSE;
    pPreRegTemplate = new FT_BYTE[iRecommendedPreRegFtrLen];
    if (pPreRegTemplate == NULL){
        throw ("Out of Memory");
    }
    rc = FX_extractFeatures(m_fxContext,              // Context of Feature Extraction
                            iFingerprintImageSize,    // Size of the fingerprint image
                            pFingerprintImage,        // Pointer to the image buffer
                            FT_PRE_REG_FTR,           // Requested Pre-Enrollment Features
                            iRecommendedPreRegFtrLen, // Length of the Features(template) buffer received previously from the SDK
                            pPreRegTemplate,          // Pointer to the buffer where the template to be stored
                            &imgQuality,              // Returned quality of the image. If feature extraction fails because used did not put finger on the reader well enough, this parameter be used to tell the user how to put the finger on the reader better. See FT_IMG_QUALITY enumeration.
                            &ftrQuality,              // Returned quality of the features. It may happen that the fingerprint of the user cannot be used. See FT_FTR_QUALITY enumeration.
                            &bEextractOK);            // Returned result of Feature extraction. FT_TRUE means extracted OK.

    // If feature extraction succeeded, add the pre-Enrollment feature sets
    // to the set of 4 templates needed to create Enrollment template.
    if (FT_OK <= rc && bEextractOK == FT_TRUE) {
        qDebug() << (("Pre-Enrollment feature set generated successfully"));

        m_TemplateArray[m_nRegFingerprint++] = pPreRegTemplate;
        pPreRegTemplate = NULL;   // This prevents deleting at the end of the function

        if (m_nRegFingerprint == m_NumberOfPreRegFeatures) {
            return create_enrollment(pPreRegTemplate, pRegTemplate, iRecommendedPreRegFtrLen);
        }
        else {
            // This is normal cource of events, before all 4 fingerprint images are captured.
            throw ("Scan same finger again.");
        }
    }
    else {
        throw ("Creation of Pre-Enrollment feature set Failed.");
        throw (("Scan your finger for enrollment."));
    }
}

int QEnrollment::addToEnroll(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize){

    FT_BYTE * pPreRegTemplate = NULL;
    FT_BYTE * pRegTemplate    = NULL;
    try{
        if (m_nRegFingerprint < m_NumberOfPreRegFeatures) {
            return create_template(m_nRegFingerprint, pPreRegTemplate, pRegTemplate, pFingerprintImage, iFingerprintImageSize);
        }
        else {
            throw ("An Enrollment Template has already been created.\n\nClose the dialog, then select \"Fingerprint Verification\" or \"Save Fingerprint Enrollment Template\".");
        }
    }
    catch(const char ex[]){
        qDebug() << ex;
    }

    delete [] pPreRegTemplate;
    delete [] pRegTemplate;
    return m_nRegFingerprint;
}



LRESULT QEnrollment::init()
{
    HRESULT hr = S_OK;
    try {
        FT_RETCODE rc = FT_OK;

        // Create Context for Feature Extraction
        if (FT_OK != (rc = FX_createContext(&m_fxContext))) {
            qDebug() << ("Cannot create Feature Extraction Context.");
        }

        // Create Context for Matching
        if (FT_OK != (rc = MC_createContext(&m_mcContext))) {
             qDebug() << ("Cannot create Matching Context.");
        }

        // Get number of Pre-Enrollment feature sets needed to create on Enrollment template
        // allocate array that keeps those Pre-Enrollment and set the first index to 0;
        MC_SETTINGS mcSettings = {0};
        if (FT_OK != (rc = MC_getSettings(&mcSettings))) {
             qDebug() << ("Cannot get number of Pre-Reg feature sets needed to create one Enrollment template.");
        }

        m_NumberOfPreRegFeatures = mcSettings.numPreRegFeatures;
        if (NULL == (m_TemplateArray = new FT_BYTE*[m_NumberOfPreRegFeatures]))
             qDebug() << (E_OUTOFMEMORY);

        ::ZeroMemory(m_TemplateArray, sizeof(FT_BYTE**)*m_NumberOfPreRegFeatures);

        m_nRegFingerprint = 0;  // This is index of the array where the first template is put.

         qDebug() << ("Scan your finger for enrollment.");
    }
    catch(char * E) {
         qDebug() << E;
    }
    catch(...) {
        hr = E_UNEXPECTED;
    }

    if (FAILED(hr)) {
         qDebug() << ("Error happened");
        //ReportError(m_hWnd, hr);
        //EndDialog(IDCANCEL);
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

bool QEnrollment::configure(QObject * context, QObject * q){
    QObject::connect(this, SIGNAL(onComplete(DATA_BLOB*)), context, SLOT(onComplete(DATA_BLOB*)));
    QObject::connect(this, SIGNAL(onComplete(DATA_BLOB*)), q, SLOT(onComplete(DATA_BLOB*)));
}

void QEnrollment::GetRegTemplate(DATA_BLOB& rRegTemplate) const {
    if (m_RegTemplate.cbData && m_RegTemplate.pbData) { // only copy template if it is not empty
        // Delete the old stuff that may be in the template.
        delete [] rRegTemplate.pbData;
        rRegTemplate.pbData = NULL;
        rRegTemplate.cbData = 0;

        // Copy the new template, but only if it has been created.
        rRegTemplate.pbData = new BYTE[m_RegTemplate.cbData];
        if (!rRegTemplate.pbData){
            qDebug() << "Out of Memory";
            return;
        }
        ::CopyMemory(rRegTemplate.pbData, m_RegTemplate.pbData, m_RegTemplate.cbData);
        rRegTemplate.cbData = m_RegTemplate.cbData;
    }
}
