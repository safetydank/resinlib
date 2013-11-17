#pragma once

#include <cstdio>
#include <memory>

namespace RESIN {

typedef std::shared_ptr<class IStream> IStreamRef;

class IStream
{
  public:
    enum Origin
    {
        kBegin = 0,
        kCurrent,
        kEnd
    };

    enum Mode
    {
        kRead      = 1,
        kWrite     = 2,
        kReadWrite = 3
    };

    virtual bool seek(long offset, Origin origin) = 0;
    virtual size_t tell() = 0;
    virtual bool eof() = 0;
    virtual size_t read(void* dst, size_t size) = 0;
    virtual size_t write(void* src, size_t size) = 0;
    virtual Mode mode() const = 0;
};


typedef std::shared_ptr<class FileStream> FileStreamRef;

class FileStream : public IStream
{
  public:
    static FileStreamRef create(const std::string& path, IStream::Mode mode);

    ~FileStream();

  protected:
    FileStream(const std::string& path, IStream::Mode mode);

    virtual bool seek(long offset, Origin origin);
    virtual size_t tell();
    virtual bool eof();
    virtual size_t read(void* dst, size_t size);
    virtual size_t write(void* src, size_t size);
    virtual Mode mode() const;

    FILE* mFile;
    IStream::Mode mMode;
};

typedef std::shared_ptr<class MemoryBuffer> MemoryBufferRef;
typedef std::shared_ptr<class MemoryBufferStream> MemoryBufferStreamRef;

class MemoryBufferStream : public IStream
{
  public:
    static MemoryBufferStreamRef create(MemoryBufferRef buffer);
    static MemoryBufferStreamRef create(void* data, size_t size);

    ~MemoryBufferStream();

  protected:
    MemoryBufferStream(MemoryBufferRef buffer);

    virtual bool seek(long offset, Origin origin);
    virtual size_t tell();
    virtual bool eof();
    virtual size_t read(void* dst, size_t size);
    virtual size_t write(void* src, size_t size);
    virtual Mode mode() const;

    MemoryBufferRef mBuffer;
    int mOffset;
    bool mEof;
};

}
