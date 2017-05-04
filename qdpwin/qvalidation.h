#ifndef QVALIDATION_H
#define QVALIDATION_H
#include "qdph.h"

class QValidation
{
public:
    QValidation();
    ~QValidation();

    FT_HANDLE      m_fxContext;        // Context for Feature Extraction functions, must be created before calling any of the FT_ functions
    FT_HANDLE      m_mcContext;        // Context for Matching functions, must be created before calling any of the MC_ functions

    BOOL           m_bDoLearning;      // User selects whether to do learning or not via checking checkbox in the dialog
                                       // NOTE: The Enrollment template must be created with Learning allowed
                                       // to use this feature during verification.

    HDPOPERATION   m_hOperationVerify; // Handle of the Operation, must be closed at exit.

    RECT           m_rcDrawArea;       // Rectangle with the desired sizes of the fingerprint image to display

    DATA_BLOB      m_RegTemplate;      // BLOB that keeps Enrollment Template. It is received from the outside (from Enrollment dialog)

    bool Verify(FT_IMAGE_PT pFingerprintImage, int iFingerprintImageSize);
    void LoadRegTemplate(DATA_BLOB *rRegTemplate);

    LRESULT init();

};

#endif // QVALIDATION_H
