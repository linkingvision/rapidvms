
#include "DerivedObject.h"

using namespace XSDK;
using namespace XSDK;

//=============================================================================
// Constructor
//=============================================================================
DerivedObject::DerivedObject(int* pDeleteCount) :
    XBaseObject("XSDK::UnitTests::DerivedObject"),
    _pDeleted(0),
    _pDeleteCount(pDeleteCount),
    _doubleDelete(false),
    _pCleanedUp(0)
{
    if (_pDeleteCount)
        *_pDeleteCount = 0;
}

//=============================================================================
// Constructor
//=============================================================================
DerivedObject::DerivedObject(bool* pDeleted, bool* pCleanedUp/*=0*/) :
    XBaseObject("XSDK::UnitTests::DerivedObject"),
    _pDeleted(pDeleted),
    _pDeleteCount(0),
    _doubleDelete(false),
    _pCleanedUp(pCleanedUp)
{
    if (_pDeleted)
        *_pDeleted = false;
    if (_pCleanedUp)
        *_pCleanedUp = false;
}

//=============================================================================
// Constructor
//=============================================================================
DerivedObject::DerivedObject(bool* pDeleted, const std::string& name, bool* pCleanedUp/*=0*/) :
    XBaseObject(name),
    _pDeleted(pDeleted),
    _pDeleteCount(0),
    _doubleDelete(false),
    _pCleanedUp(pCleanedUp)
{
    if (_pDeleted)
        *_pDeleted = false;
    if (_pCleanedUp)
        *_pCleanedUp = false;
}

//=============================================================================
// Destructor
//=============================================================================
DerivedObject::~DerivedObject()
{
    if (_pDeleteCount)
        (*_pDeleteCount)++;
    if (_pDeleted)
        *_pDeleted = true;

    if (_doubleDelete)
    {
        AddRef();
        Release();
        _doubleDelete = false;
    }
}

//=============================================================================
// Increment
//=============================================================================
int DerivedObject::Increment(int value)
{
    return ++value;
}

//=============================================================================
// CleanUp
//=============================================================================
void DerivedObject::CleanUp()
{
    if (_pCleanedUp)
        *_pCleanedUp = true;
}
