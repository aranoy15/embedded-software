#pragma once

#include <ff.h>
#include <diskio.h>

#include <singleton.h>
#include <string>
#include <bsp.h>
#include <memory>

extern "C" void EXTI15_10_IRQHandler(void);
extern "C" void EXTI9_5_IRQHandler(void);

class FatFsClass : public Singleton<FatFsClass> 
{
    friend class DirFs;
    friend class FileFs;
    friend void EXTI15_10_IRQHandler(void);
    friend void EXTI9_5_IRQHandler(void);

private:
    uint8_t ffsResult;
    std::unique_ptr<FATFS> ffs;
    bool mHaveNewCard;

public:
    FatFsClass();

    bool init();

	bool haveNewCard() const { return mHaveNewCard; }

	int32_t capacity();
    int32_t free();
    uint8_t error();

	bool mkdir(char*);
    bool rmdir(char*);
    bool remove(char*);
    bool rename(char*, char*);
    bool dirExists(char*);
    bool fileExists(char*);
    bool isDir(char*);
	bool timeStamp(char*, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t,
	               uint8_t);
    bool getFileModTime(char*, uint16_t*, uint16_t*);

	bool cardDetected() { return not bsp::sdio::sdioCd::state(); }
};

class DirFs
{
private:
    FILINFO finfo;
    DIR dir;

	FatFsClass& fatFs() { return *FatFsClass::instance(); }

public:

    DirFs() : finfo(), dir() {}
	~DirFs() { f_closedir(&dir); }

    bool openDir(char*);
    bool closeDir();
    bool nextFile();
    bool rewind();
    bool isDir();
    char* fileName();
    uint32_t fileSize();
    uint16_t fileModDate();
    uint16_t fileModTime();
};

class FileFs
{
private:
    FIL ffile;

	FatFsClass& fatFs() { return *FatFsClass::instance(); }

public:
	FileFs() : ffile() {}

	bool open(char* fileName, uint8_t mode = FA_OPEN_EXISTING);
	bool close();

	uint32_t write(void* buf, uint32_t size);
	int writeString(char* str);
	bool writeChar(char ch);

	uint32_t read(void* buf, uint32_t size);
	int16_t readString(char* buf, int len);
	char readChar();
	uint16_t readInt();
	uint16_t readHex();

	uint32_t curPosition();
	bool seekSet(uint32_t cur);

	uint32_t fileSize();

};

inline bool legalChar(char c)
{
	if (c == '"' || c == '*' || c == '?' || c == ':' || c == '<' || c == '>' ||
	    c == '|')
		return false;
	return true;
}