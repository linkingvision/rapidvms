
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XPath_h
#define XSDK_XPath_h

#include "XSDK/IXIterate.h"
#include "XSDK/XInternalList.h"
#include "XSDK/XString.h"
#include "XSDK/XException.h"

#ifndef WIN32
#define PATH_SLASH "/"
#else
#define PATH_SLASH "\\"
#endif

namespace XSDK
{

/// This class provides cross platform directory listing, supporting standard Unix and
/// Windows shell wildcard expansion (globing). Any code that needs to access directory
/// lists can do so through this class in a platform independant manner.
class XPath : IXIterate<XString>
{
 public:

  /// Constructor, takes optionally globbed path.
  /// \param glob A path and optional shell wildcard glob.
  X_API XPath(const XString& glob);

  /// Copy constructor.
  /// \param obj The object to duplicate ourselves from.
  X_API XPath( const XPath& obj );

  /// Destructor.
  X_API virtual ~XPath() throw();

  /// Assignment operator.
  /// \param obj The object to duplicate ourselves from.
  X_API XPath& operator = ( const XPath& obj );

  /// This class opens the dir (and optional shell wildcards) specified by glob
  /// and populates our internal list of file names.
  /// \param glob A path and optional shell patern.
  X_API void OpenDir(const XString& glob);

  // Returns true if parameter is a path to a directory.
  // \param path The path to check for directoryness.
  X_API static bool IsDir(const XString& path);

  // Returns true if parameter is a path to a regular file.
  // \param path The path to check for regular fileness.
  X_API static bool IsReg(const XString& path);

  // Returns true if parameter is a path that exists.
  // \param path The path to check existence of.
  X_API static bool Exists(const XString& path);

  // Returns the total size on disk of the file or directory specified by argument.
  // \param path The file or directory to get the size of.
  X_API static int64_t FileSize(const XString& path);

  X_API static XString GetCurDir();

  /// Empty this container.
  X_API void Clear();

  /// @brief Returns whether the element count is 0.
  X_API virtual bool Empty() const;

  /// Returns the number of entries in this directory.
  X_API virtual size_t GetElemCount() const;

  /// Reset our internal iterator to the beginning of the set.
  X_API virtual void IterStart( bool forward = true );

  /// Move our internal iterator to the the next position.
  X_API virtual void Iterate();

  /// Get the XString at our current iterator position.
  X_API virtual XString* IterData() const;

  /// Returns true if the iterator is pointing at something valid.
  X_API virtual bool IterValid() const;

 private:

  /// Given seperate path and glob, populate file list.
  /// \param path The path to the path to get the file names from.
  /// \param glob The shell wildcard to apply to path.
  void _GetFileNames(const XSDK::XString& path, const XSDK::XString& glob);

  XInternalList<XString> _names;
};

}

#endif
