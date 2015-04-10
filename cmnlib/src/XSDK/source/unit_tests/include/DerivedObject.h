
#ifndef _DERIVED_OBJECT_H_
#define _DERIVED_OBJECT_H_

#include "XSDK/XBaseObject.h"

namespace XSDK
{
    //=========================================================================
    // Test interface.
    //=========================================================================
    class IBaseObjectTestInterface : public XSDK::IXUnknown
    {
        //=====================================================================
        // {236614B8-15AC-44e2-BCBC-64AF0B0A1A83}
        //=====================================================================
        DEFINE_INTERFACE_ID(IBaseObjectTestInterface,0x23,0x66,0x14,0xB8,0x15,0xAC,0x44,0xe2,0xBC,0xBC,0x64,0xAF,0x0B,0x0A,0x1A,0x83);

        virtual int Increment(int value) = 0;

    }; // class IBaseObjectTestInterface

    //=========================================================================
    /// The class ID of the object
    //
    // {7E7E96EC-7847-4831-A5A8-2BB21782BAF3}
    //=========================================================================
    const X_UUID kClsIdDerivedObject = {{0x7E,0x7E,0x96,0xEC,0x78,0x47,0x48,0x31,0xA5,0xA8,0x2B,0xB2,0x17,0x82,0xBA,0xF3}};

    //=========================================================================
    // A class to test inheriting from BaseObject.
    //=========================================================================
    class DerivedObject :
        public XSDK::XBaseObject,
        public IBaseObjectTestInterface
    {
    public:

        //=================================================================
        // Constructors/Destructor
        //=================================================================
        DerivedObject(int* pDeleteCount);
        DerivedObject(bool* pDeleted, bool* pCleanedUp = 0);
        DerivedObject(bool* pDeleted, const std::string& name, bool* pCleanedUp = 0);
        virtual ~DerivedObject();

        //=================================================================
        // Declare IXUnknown
        //=================================================================
        XSDK_DECLARE_IXUNKNOWN(kClsIdDerivedObject)
        XSDK_QUERY_INTERFACE_BEGIN()
            XSDK_QUERY_INTERFACE_ENTRY(IBaseObjectTestInterface)
        XSDK_QUERY_INTERFACE_END(XBaseObject)

        //=================================================================
        // IBaseObjectTestInterface
        //=================================================================
        virtual int Increment(int value);

        void EnableDoubleDelete() { _doubleDelete = true; }

    private:

        // XBaseObject CleanUp() method override.
        virtual void CleanUp();

        bool* _pDeleted;     // We set this to true in the destructor.
        int*  _pDeleteCount; // Number of times the destructor was called.
        bool  _doubleDelete; // MPF-1195 Causes destructor to AddRef/Release itself
        bool* _pCleanedUp;   // We set this to true in the CleanUp method.

    }; // class DerivedObject

} // namespace XSDK

#endif // _DERIVED_OBJECT_H_

