#include "qvalidation.h"
#include <QDebug>

QValidation::QValidation()
{

}

QValidation::~QValidation()
{

}

bool QValidation::Verify(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize){
    FT_BYTE * pVerTemplate = NULL;
    try {
        FT_RETCODE rc = FT_OK;

        // Get recommended length of the Pre-Enrollment feature sets.
        // It actually could be done once at the beginning, but this is just sample code...
        int iRecommendedVerFtrLen = 0;
        rc = FX_getFeaturesLen(FT_VER_FTR, &iRecommendedVerFtrLen, NULL); // Minimal Fingerprint feature set Length - do not use if possible
        if (FT_OK != rc){
            throw ("Can't configure finger print.");
        }
        // Extract Features (i.e. create fingerprint template)
        FT_IMG_QUALITY imgQuality;
        FT_FTR_QUALITY ftrQuality;
        FT_BOOL bEextractOK = FT_FALSE;
        pVerTemplate = new FT_BYTE[iRecommendedVerFtrLen];
        if (pVerTemplate == NULL){
            throw ("Out of Memory");
        }
        rc = FX_extractFeatures(m_fxContext,             // Context of Feature Extraction
                                iFingerprintImageSize,   // Size of the fingerprint image
                                pFingerprintImage,       // Pointer to the image buffer
                                FT_VER_FTR,              // Requested Verification Features
                                iRecommendedVerFtrLen,   // Length of the Features(template) buffer received previously from the SDK
                                pVerTemplate,            // Pointer to the buffer where the template to be stored
                                &imgQuality,             // Returned quality of the image. If feature extraction fails because used did not put finger on the reader well enough, this parameter be used to tell the user how to put the finger on the reader better. See FT_IMG_QUALITY enumeration.
                                &ftrQuality,             // Returned quality of the features. It may happen that the fingerprint of the user cannot be used. See FT_FTR_QUALITY enumeration.
                                &bEextractOK);           // Returned result of Feature extraction. FT_TRUE means extracted OK.

        if (FT_OK <= rc && bEextractOK == FT_TRUE) {
            // Features extracted OK (i.e. fingerprint Verification template was created successfully)
            // Now match this template and the Enrollment template.
            FT_BOOL bRegFeaturesChanged = FT_FALSE;
            FT_VER_SCORE VerScore = FT_UNKNOWN_SCORE;
            double dFalseAcceptProbability = 0.0;
            FT_BOOL bVerified = FT_FALSE;

            rc = MC_verifyFeaturesEx(m_mcContext,              // Matching Context
                                     m_RegTemplate.cbData,     // Pointer to the Enrollment template content
                                     m_RegTemplate.pbData,     // Length of the Enrollment template
                                     iRecommendedVerFtrLen,    // Length of the Verification template
                                     pVerTemplate,             // Pointer to the Verification template content
                                     m_bDoLearning,            // Whether the Learning is desired - got it from checkbox in the dialog
                                     &bRegFeaturesChanged,     // Out: Whether the Learning actually happened
                                     NULL,                     // Reserved, must be NULL
                                     &VerScore,                // Out: Matching score, see score enumeration FT_VER_SCORE
                                     &dFalseAcceptProbability, // Out: Probability to falsely match these fingerprints
                                     &bVerified);              // Returned result of fingerprint match. FT_TRUE means fingerprints matched.
            if (FT_OK <= rc) {
                /*if (FT_OK != rc) {
                    TCHAR buffer[101] = {0};
                    ULONG uSize = 100;
                    _sntprintf_s(buffer, uSize, _TRUNCATE, _T("Warning: %ld (see dpRCodes.h)"), rc);
                    AddStatus(buffer);
                }*/

                if (bVerified == FT_TRUE) {
                    return true;
                    /*SetDlgItemText(IDC_EDIT_PROMPT, _T("Scan another finger to run verification again."));
                    TCHAR buffer[101] = {0};
                    ULONG uSize = 100;
                    _sntprintf_s(buffer, uSize, _TRUNCATE, _T("%lf"), dFalseAcceptProbability);
                    SetDlgItemText(IDE_PROBABILITY, buffer);*/
                }
                else {
                    return false;
                    /*SetDlgItemText(IDC_EDIT_PROMPT, _T("Scan another finger to run verification again."));
                    TCHAR buffer[101] = {0};
                    ULONG uSize = 100;
                    _sntprintf_s(buffer, uSize, _TRUNCATE, _T("%lf"), dFalseAcceptProbability);
                    SetDlgItemText(IDE_PROBABILITY, buffer);*/
                }
            }
            else {
                throw ("Verification Operation failed");
                /*TCHAR buffer[101] = {0};
                ULONG uSize = 100;
                _sntprintf_s(buffer, uSize, _TRUNCATE, _T("Verification Operation failed, Error: %ld."), rc);
                MessageBox(buffer, _T("Fingerprint Verification"), MB_OK | MB_ICONINFORMATION);
                SetDlgItemText(IDC_EDIT_PROMPT, _T("Scan your finger for verification again."));*/
            }
        }
    }
    catch(const char e[]) {
        qDebug() << e;
    }
    delete [] pVerTemplate;
}



LRESULT QValidation::init()
{
    try {
        FT_RETCODE rc = FT_OK;

        // Create Context for Feature Extraction
        if (FT_OK != (rc = FX_createContext(&m_fxContext))) {
            throw ("Cannot create Feature Extraction Context.");
            return TRUE;  // return TRUE  unless you set the focus to a control
        }

        // Create Context for Matching
        if (FT_OK != (rc = MC_createContext(&m_mcContext))) {
            throw ("Cannot create Matching Context.");
            return TRUE;  // return TRUE  unless you set the focus to a control
        }

    }
    catch(const char e [] ) {
        qDebug() << e;
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void QValidation::LoadRegTemplate(DATA_BLOB *rRegTemplate) {
    // Delete the old stuff that may be in the template.
    delete [] m_RegTemplate.pbData;
    m_RegTemplate.pbData = NULL;
    m_RegTemplate.cbData = 0;

    // Copy Enrollment template data into member of this class
    m_RegTemplate.pbData = new BYTE[rRegTemplate->cbData];
    if (!m_RegTemplate.pbData){
        qDebug() << "Out of Memory";
        return;
    }
    ::CopyMemory(m_RegTemplate.pbData, rRegTemplate->pbData, rRegTemplate->cbData);
    m_RegTemplate.cbData = rRegTemplate->cbData;
}
