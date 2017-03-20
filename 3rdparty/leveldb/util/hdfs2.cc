// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <assert.h>
#ifdef LEVELDB_PLATFORM_POSIX
#include <dlfcn.h>
#endif

#include "hdfstera.h"
#include "leveldb/hdfs2.h"
#include "util/hash.h"
#include "counter.h"

namespace leveldb {

extern tera::Counter dfs_read_delay_counter;
extern tera::Counter dfs_write_delay_counter;

extern tera::Counter dfs_read_counter;
extern tera::Counter dfs_write_counter;
extern tera::Counter dfs_sync_counter;
extern tera::Counter dfs_flush_counter;
extern tera::Counter dfs_list_counter;
extern tera::Counter dfs_other_counter;
extern tera::Counter dfs_exists_counter;
extern tera::Counter dfs_open_counter;
extern tera::Counter dfs_close_counter;
extern tera::Counter dfs_delete_counter;
extern tera::Counter dfs_tell_counter;
extern tera::Counter dfs_info_counter;

extern tera::Counter dfs_read_hang_counter;
extern tera::Counter dfs_write_hang_counter;
extern tera::Counter dfs_sync_hang_counter;
extern tera::Counter dfs_flush_hang_counter;
extern tera::Counter dfs_list_hang_counter;
extern tera::Counter dfs_other_hang_counter;
extern tera::Counter dfs_exists_hang_counter;
extern tera::Counter dfs_open_hang_counter;
extern tera::Counter dfs_close_hang_counter;
extern tera::Counter dfs_delete_hang_counter;
extern tera::Counter dfs_tell_hang_counter;
extern tera::Counter dfs_info_hang_counter;

static hdfsFS (*hdfsConnect)(const char* nn, tPort port);
static int (*hdfsDisconnect)(hdfsFS fs);

static int (*hdfsCreateDirectoryHdfs)(hdfsFS fs, const char* path);
static hdfsFileInfo* (*hdfsListDirectory)(hdfsFS fs, const char* path,
                                          int *numEntries);
static hdfsFileInfo* (*hdfsGetPathInfo)(hdfsFS fs, const char* path);
static void (*hdfsFreeFileInfo)(hdfsFileInfo *hdfsFileInfo, int numEntries);

static int (*hdfsDelete)(hdfsFS fs, const char* path, int recursive);
static int (*hdfsExists)(hdfsFS fs, const char *path);
static int (*hdfsRename)(hdfsFS fs, const char* oldPath, const char* newPath);
static int (*hdfsCopy)(hdfsFS srcFS, const char* src,
                        hdfsFS dstFS, const char* dst);

static hdfsFile (*hdfsOpenFile)(hdfsFS fs, const char* path, int flags,
                                 int bufferSize, short replication,
                                 tSize blocksize);
static int (*hdfsCloseFile)(hdfsFS fs, hdfsFile file);

static tSize (*hdfsRead)(hdfsFS fs, hdfsFile file, void* buffer, tSize length);
static tSize (*hdfsPread)(hdfsFS fs, hdfsFile file, tOffset position,
                           void* buffer, tSize length);
static tSize (*hdfsWrite)(hdfsFS fs, hdfsFile file, const void* buffer,
                           tSize length);
static int (*hdfsFlush)(hdfsFS fs, hdfsFile file);
static int (*hdfsHSync)(hdfsFS fs, hdfsFile file);
static tOffset (*hdfsTell)(hdfsFS fs, hdfsFile file);
static int (*hdfsSeek)(hdfsFS fs, hdfsFile file, tOffset desiredPos);

bool Hdfs2::LoadSymbol() {
#ifdef LEVELDB_PLATFORM_POSIX
  dlerror();
  void* dl = dlopen("libhdfs.so", RTLD_NOW | RTLD_GLOBAL);
  if (dl == NULL) {
    fprintf(stderr, "dlopen libhdfs.so error: %s\n", dlerror());
    return false;
  }

  dlerror();
  *(void**)(&hdfsConnect) = dlsym(dl, "hdfsConnect");
  *(void**)(&hdfsDisconnect) = dlsym(dl, "hdfsDisconnect");
  *(void**)(&hdfsCreateDirectoryHdfs) = dlsym(dl, "hdfsCreateDirectoryHdfs");
  *(void**)(&hdfsListDirectory) = dlsym(dl, "hdfsListDirectory");
  *(void**)(&hdfsGetPathInfo) = dlsym(dl, "hdfsGetPathInfo");
  *(void**)(&hdfsFreeFileInfo) = dlsym(dl, "hdfsFreeFileInfo");
  *(void**)(&hdfsDelete) = dlsym(dl, "hdfsDelete");
  *(void**)(&hdfsExists) = dlsym(dl, "hdfsExists");
  *(void**)(&hdfsRename) = dlsym(dl, "hdfsRename");
  *(void**)(&hdfsCopy) = dlsym(dl, "hdfsCopy");
  *(void**)(&hdfsOpenFile) = dlsym(dl, "hdfsOpenFile");
  *(void**)(&hdfsCloseFile) = dlsym(dl, "hdfsCloseFile");
  *(void**)(&hdfsRead) = dlsym(dl, "hdfsRead");
  *(void**)(&hdfsPread) = dlsym(dl, "hdfsPread");
  *(void**)(&hdfsWrite) = dlsym(dl, "hdfsWrite");
  *(void**)(&hdfsFlush) = dlsym(dl, "hdfsFlush");
  *(void**)(&hdfsHSync) = dlsym(dl, "hdfsHSync");
  *(void**)(&hdfsTell) = dlsym(dl, "hdfsTell");
  *(void**)(&hdfsSeek) = dlsym(dl, "hdfsSeek");

  const char* error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "resolve symbol from libhdfs.so error: %s\n", error);
    return false;
  }
#endif
#ifdef LEVELDB_PLATFORM_WINDOWS
	void* _libraryInstance;
	_libraryInstance = ::LoadLibraryW(L"hdfs.dll");
        if( _libraryInstance == 0 )
        {
		LPVOID str = 0;
		std::string _libraryName = "hdfs.dll";
		DWORD_PTR args[1] = { (DWORD_PTR)_libraryName.c_str() };

		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		               NULL,
		               GetLastError(),
		               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		               (LPTSTR)&str,
		               0,
		               (va_list*)args );

		LocalFree( str );
    		fprintf(stderr, "resolve symbol from hdfs.dll error\n");
        }

	*(void**)(&hdfsConnect) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsConnect");
	*(void**)(&hdfsDisconnect) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsDisconnect");
	*(void**)(&hdfsCreateDirectoryHdfs) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsCreateDirectoryHdfs");
	*(void**)(&hdfsListDirectory) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsListDirectory");
	*(void**)(&hdfsGetPathInfo) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsGetPathInfo");
	*(void**)(&hdfsFreeFileInfo) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsFreeFileInfo");
	*(void**)(&hdfsDelete) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsDelete");
	*(void**)(&hdfsExists) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsExists");
	*(void**)(&hdfsRename) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsRename");
	*(void**)(&hdfsCopy) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsCopy");
	*(void**)(&hdfsOpenFile) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsOpenFile");
	*(void**)(&hdfsCloseFile) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsCloseFile");
	*(void**)(&hdfsRead) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsRead");
	*(void**)(&hdfsPread) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsPread");
	*(void**)(&hdfsWrite) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsWrite");
	*(void**)(&hdfsFlush) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsFlush");
	*(void**)(&hdfsHSync) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsHSync");
	*(void**)(&hdfsTell) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsTell");
	*(void**)(&hdfsSeek) = ::GetProcAddress( (HMODULE)_libraryInstance, "hdfsSeek");

#endif
  return true;
}

H2File::H2File(void* fs, void* file, const std::string& name)
  : fs_(fs), file_(file), name_(name) {
}
H2File::~H2File() {
  if (file_) {
    CloseFile();
  }
}

int32_t H2File::Write(const char* buf, int32_t len) {
  int64_t s = tera::get_micros();
  tera::AutoCounter ac(&dfs_write_hang_counter, "Write", name_.c_str());
  int32_t retval = (*hdfsWrite)((hdfsFS)fs_, (hdfsFile)file_, buf, len);
  dfs_write_delay_counter.Add(tera::get_micros() - s);
  dfs_write_counter.Inc();
  return retval;
}
int32_t H2File::Flush() {
  tera::AutoCounter ac(&dfs_flush_hang_counter, "Flush", name_.c_str());
  dfs_flush_counter.Inc();
  int32_t retval = 0;
  return (*hdfsFlush)((hdfsFS)fs_, (hdfsFile)file_);
}
int32_t H2File::Sync() {
  tera::AutoCounter ac(&dfs_sync_hang_counter, "Sync", name_.c_str());
  dfs_sync_counter.Inc();
  return (*hdfsHSync)((hdfsFS)fs_, (hdfsFile)file_);
}
int32_t H2File::Read(char* buf, int32_t len) {
  int64_t s = tera::get_micros();
  tera::AutoCounter ac(&dfs_read_hang_counter, "Read", name_.c_str());
  int32_t retval = (*hdfsRead)((hdfsFS)fs_, (hdfsFile)file_, buf, len);
  dfs_read_delay_counter.Add(tera::get_micros() - s);
  dfs_read_counter.Inc();
  return retval;
}
int32_t H2File::Pread(int64_t offset, char* buf, int32_t len) {
  tera::AutoCounter ac(&dfs_read_hang_counter, "Pread", name_.c_str());
  dfs_read_counter.Inc();
  int32_t retval = (*hdfsPread)((hdfsFS)fs_, (hdfsFile)file_, offset, buf, len);
  return retval;
}
int64_t H2File::Tell() {
  tera::AutoCounter ac(&dfs_tell_hang_counter, "Tell", name_.c_str());
  dfs_tell_counter.Inc();
  int64_t retval = (*hdfsTell)((hdfsFS)fs_, (hdfsFile)file_);
  return retval;
}
int32_t H2File::Seek(int64_t offset) {
  tera::AutoCounter ac(&dfs_other_hang_counter, "Seek", name_.c_str());
  dfs_other_counter.Inc();
  return (*hdfsSeek)((hdfsFS)fs_, (hdfsFile)file_, offset);
}

int32_t H2File::CloseFile() {
  tera::AutoCounter ac(&dfs_close_hang_counter, "CloseFile", name_.c_str());
  dfs_close_counter.Inc();
  int32_t retval = 0;
  if (file_ != NULL) {
    retval = (*hdfsCloseFile)((hdfsFS)fs_, (hdfsFile)file_);
  }
  if (retval != 0) {
    fprintf(stderr, "[ClosFile] %s fail: %d\n", name_.c_str(), retval);
  }
  // WARNING: in libhdfs, close file success even retval != 0
  file_ = NULL;
  return retval;
}

port::Mutex Hdfs2::dl_mu_;
bool Hdfs2::dl_init_ = false;

Hdfs2::Hdfs2(const std::string& namenode_list) {
  {
    MutexLock l(&dl_mu_);
    if (!dl_init_) {
      bool r = LoadSymbol();
      assert(r);
      dl_init_ = true;
    }
  }

  assert(!namenode_list.empty());
  size_t begin = 0, end = 0;
  while (begin < namenode_list.size()) {
    end = namenode_list.find(',', begin);
    if (end == std::string::npos) {
      end = namenode_list.size();
    }
    assert(end > begin);
    std::string namenode = namenode_list.substr(begin, end - begin);
    fprintf(stderr, "connect to %s\n", namenode.c_str());
    hdfsFS fs = (*hdfsConnect)(namenode.c_str(), 0);
    assert(fs);
    fs_list_.push_back(fs);
    begin = end + 1;
  }
}

Hdfs2::~Hdfs2() {
  for (uint32_t i = 0; i < fs_list_.size(); i++) {
    (*hdfsDisconnect)((hdfsFS)fs_list_[i]);
  }
}

int32_t Hdfs2::CreateDirectoryHdfs(const std::string& path) {
  tera::AutoCounter ac(&dfs_other_hang_counter, "CreateDirectoryHdfs", path.c_str());
  dfs_other_counter.Inc();
  return (*hdfsCreateDirectoryHdfs)((hdfsFS)GetFSHandle(path), path.c_str());
}

int32_t Hdfs2::DeleteDirectory(const std::string& path) {
  tera::AutoCounter ac(&dfs_delete_hang_counter, "DeleteDirectory", path.c_str());
  dfs_delete_counter.Inc();
  return (*hdfsDelete)((hdfsFS)GetFSHandle(path), path.c_str(), 1);
}

int32_t Hdfs2::Exists(const std::string& filename) {
  tera::AutoCounter ac(&dfs_exists_hang_counter, "Exists", filename.c_str());
  dfs_exists_counter.Inc();
  return (*hdfsExists)((hdfsFS)GetFSHandle(filename), filename.c_str());
}

int32_t Hdfs2::Delete(const std::string& filename) {
  tera::AutoCounter ac(&dfs_delete_hang_counter, "Delete", filename.c_str());
  dfs_delete_counter.Inc();
  return (*hdfsDelete)((hdfsFS)GetFSHandle(filename), filename.c_str(), 1);
}

int32_t Hdfs2::GetFileSize(const std::string& filename, uint64_t* size) {
  tera::AutoCounter ac(&dfs_info_hang_counter, "GetFileSize", filename.c_str());
  dfs_info_counter.Inc();
  hdfsFileInfo* pFileInfo = (*hdfsGetPathInfo)((hdfsFS)GetFSHandle(filename), filename.c_str());
  if (pFileInfo != NULL) {
    *size = pFileInfo->mSize;
    (*hdfsFreeFileInfo)(pFileInfo, 1);
    return 0;
  }
  return -1;
}

int32_t Hdfs2::Rename(const std::string& from, const std::string& to) {
  assert(GetFSHandle(from) == GetFSHandle(to));
  tera::AutoCounter ac(&dfs_other_hang_counter, "Rename", from.c_str());
  dfs_other_counter.Inc();
  return (*hdfsRename)((hdfsFS)GetFSHandle(from), from.c_str(), to.c_str());
}

DfsFile* Hdfs2::OpenFile(const std::string& filename, int32_t flags) {
  tera::AutoCounter ac(&dfs_open_hang_counter, "OpenFile", filename.c_str());
  dfs_open_counter.Inc();
  //fprintf(stderr, "OpenFile %s %d\n", filename.c_str(), flags);
  int32_t hflags = (flags == RDONLY ? O_RDONLY : O_WRONLY);
  hdfsFile file = (*hdfsOpenFile)((hdfsFS)GetFSHandle(filename), filename.c_str(), hflags, 0 ,0 ,0);
  if (!file) {
    return NULL;
  } else {
    return new H2File(GetFSHandle(filename), file, filename);
  }
}

int32_t Hdfs2::Copy(const std::string& from, const std::string& to) {
  tera::AutoCounter ac(&dfs_other_hang_counter, "Copy", from.c_str());
  dfs_other_counter.Inc();
  return (*hdfsCopy)((hdfsFS)GetFSHandle(from), from.c_str(), (hdfsFS)GetFSHandle(to), to.c_str());
}

int32_t Hdfs2::ListDirectory(const std::string& path, std::vector<std::string>* result) {
  tera::AutoCounter ac(&dfs_list_hang_counter, "ListDirectory", path.c_str());
  dfs_list_counter.Inc();
  int numEntries = 0;
  hdfsFileInfo* pHdfsFileInfo = 0;
  pHdfsFileInfo = (*hdfsListDirectory)((hdfsFS)GetFSHandle(path), path.c_str(), &numEntries);
  if (numEntries >= 0) {
    for (int i = 0; i < numEntries; i++) {
      char* pathname = pHdfsFileInfo[i].mName;
#ifdef LEVELDB_PLATFORM_POSIX
	  char* filename = rindex(pathname, '/');
#endif
#ifdef LEVELDB_PLATFORM_WINDOWS
	  char* filename = strrchr(pathname, '/');
#endif
      if (filename != NULL) {
        result->push_back(filename + 1);
      }
    }
    if (pHdfsFileInfo != NULL) {
      (*hdfsFreeFileInfo)(pHdfsFileInfo, numEntries);
    }
  }
  return 0;
}

void* Hdfs2::GetFSHandle(const std::string& path) {
  size_t pos = path.rfind("tablet");
  if (pos == std::string::npos) {
    // fprintf(stderr, "hash path: %s, index: 0\n", path.c_str());
    return fs_list_[0];
  }
  pos = path.find('/', pos);
  if (pos == std::string::npos) {
    pos = path.size();
  }
  std::string hash_path = path.substr(0, pos);
  uint32_t index = Hash(hash_path.c_str(), hash_path.size(), 12345) % fs_list_.size();
  // fprintf(stderr, "hash path: %s, index: %d\n", hash_path.c_str(), index);
  return fs_list_[index];
}

} // namespace leveldb

/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */
