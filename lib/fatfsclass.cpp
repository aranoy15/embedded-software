#include <fatfsclass.h>
#include <sdio.h>
#include <cstring>

extern "C" int sd_status()
{
    return Sdio::instance()->checkStatus(); 
}

extern "C" int sd_initialize()
{
    return Sdio::instance()->initialize(); 
}

extern "C" int sd_disk_read(uint8_t* buff, uint32_t sector, uint32_t count)
{
    return Sdio::instance()->read(buff, sector, count);
}

extern "C" int sd_disk_write(uint8_t* buff, uint32_t sector, uint32_t count)
{
    return Sdio::instance()->write(buff, sector, count);
}

extern "C" int sd_disk_ioctl(uint8_t cmd, void* buff)
{
    return Sdio::instance()->ioctl(cmd, buff);
}

/*
extern "C" DWORD get_fattime(void)
{
    auto now = DateTimeManager::instance()->now();

	return ((DWORD)(now.year() - 1980) << 25 | (DWORD)now.month() << 21 |
	        (DWORD)now.date() << 16);
}
*/

extern "C" void* ff_memalloc(UINT msize)
{
	return malloc(msize);
}

extern "C" void ffmemfree(void* mblock)
{
    free(mblock);
}

/* ===========================================================

                    FatFsClass functions

   =========================================================== */

FatFsClass::FatFsClass() : ffsResult(0), ffs(new FATFS()), mHaveNewCard(true) {}

bool FatFsClass::init()
{
    if (mHaveNewCard) {
        ffsResult = 0;    
        ffs.reset(new FATFS());

        ffsResult = f_mount(ffs.get(), "", 1);

        if (ffsResult == FR_OK)
            mHaveNewCard = false;
    } else {
        ffsResult = FR_OK;
    }

    return ffsResult == FR_OK;
}

int32_t FatFsClass::capacity()
{
    return (ffs->n_fatent - 2) * ffs->csize >> 1;
}


int32_t FatFsClass::free()
{
    uint32_t freClust;
    FATFS *fs;

    if (f_getfree("0:", reinterpret_cast<DWORD*>(&freClust), &fs) != 0)
        return -1;

    return freClust * ffs->csize >> 1;
}

uint8_t FatFsClass::error()
{
    return ffsResult;
}

bool FatFsClass::mkdir(char* path) 
{
    ffsResult = f_mkdir(path);
    return ffsResult == FR_OK;
}

bool FatFsClass::rmdir(char* path)
{
    return remove(path);
}

bool FatFsClass::remove(char* path)
{
    ffsResult = f_unlink(path);
    return ffsResult == FR_OK;
}

bool FatFsClass::rename(char* oldName, char* newName)
{
    char* oldNameTemp = oldName;
    char* newNameTemp = newName;
    ffsResult = f_rename(oldNameTemp, newNameTemp);
    return ffsResult == FR_OK;
}

bool FatFsClass::dirExists(char* path)
{
    std::string strPath = path;

    if (strPath == "/")
        return true;

    FILINFO finfo;

	return (f_stat(path, &finfo) == FR_OK) && (finfo.fattrib & AM_DIR);
}

bool FatFsClass::fileExists(char* path)
{
    FILINFO finfo;
    return (f_stat(path, &finfo) == FR_OK) and (finfo.fattrib & AM_ARC);
}

bool FatFsClass::timeStamp(char* path, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    FILINFO finfo;

    finfo.fdate = (year - 1980) << 9 | month << 5 | day;
    finfo.ftime = hour << 11 | minute << 5 | second >> 1;
    ffsResult = f_utime(path, &finfo);
    return ffsResult == FR_OK;
}

bool FatFsClass::getFileModTime(char* path, uint16_t* pdate, uint16_t* ptime)
{
    FILINFO finfo;

    if (f_stat(path, &finfo) != FR_OK)
        return false;
    
    *pdate = finfo.fdate;
    *ptime = finfo.ftime;

    return true;
}

/* ===========================================================

                    DirFs functions

   =========================================================== */

bool DirFs::openDir(char* dirPath)
{
    fatFs().ffsResult = f_opendir(&dir, dirPath);
    return fatFs().ffsResult == FR_OK;
}

bool DirFs::closeDir()
{
    fatFs().ffsResult = f_closedir(&dir);
    return fatFs().ffsResult == FR_OK;
}

bool DirFs::nextFile()
{
    fatFs().ffsResult = f_readdir(&dir, &finfo);
    return fatFs().ffsResult == FR_OK && finfo.fname[0] != 0;
}

bool DirFs::rewind()
{
    fatFs().ffsResult = f_readdir(&dir, NULL);
    return fatFs().ffsResult == FR_OK;
}

bool DirFs::isDir()
{
    return finfo.fattrib & AM_DIR;
}

char* DirFs::fileName()
{
    return finfo.fname;
}

uint32_t DirFs::fileSize()
{
    return finfo.fsize;
}

uint16_t DirFs::fileModDate()
{
    return finfo.fdate;
}

uint16_t DirFs::fileModTime()
{
    return finfo.ftime;
}

/* ===========================================================

                    FileFs functions

   =========================================================== */


bool FileFs::open(char* fileName, uint8_t mode)
{
    fatFs().ffsResult = f_open(&ffile, fileName, mode);
    return fatFs().ffsResult == FR_OK;
}

bool FileFs::close()
{
    fatFs().ffsResult = f_close(&ffile);
    return fatFs().ffsResult == FR_OK;
}

uint32_t FileFs::write(void* buf, uint32_t size)
{
	uint32_t lb, nwrt0, nwrt = 0;

	fatFs().ffsResult = FR_OK;
	while (nwrt < size && fatFs().ffsResult == FR_OK) {
		nwrt0 = 0;
		lb = size - nwrt;
		if (lb > FF_MIN_SS) lb = FF_MIN_SS;
		fatFs().ffsResult = f_write(&ffile, (((BYTE*)buf) + nwrt), lb, (UINT*)&nwrt0);
		nwrt += nwrt0;
	}
	return nwrt;
}

int FileFs::writeString(char* str)
{
    return f_puts(str, &ffile);
}

bool FileFs::writeChar(char ch)
{
    return f_putc(ch, &ffile) == 1;
}

uint32_t FileFs::read(void *buf, uint32_t size)
{
	uint32_t lb, nrd0, nrd;

	nrd = 0;
	do {
		nrd0 = 0;
		lb = size - nrd;
		if (lb > FF_MIN_SS) lb = FF_MIN_SS;
		fatFs().ffsResult = f_read(&ffile, (((BYTE*)buf) + nrd), lb, (UINT*)&nrd0);
		nrd += nrd0;
	} while (nrd0 > 0 && nrd < size && fatFs().ffsResult == FR_OK);
	return nrd;
}


int16_t FileFs::readString(char* str, int len)
{
	str = f_gets(str, len, &ffile);
	if (str == NULL) return -1;
	uint16_t lstr = strlen(str) - 1;
	while (lstr > 0 && (str[lstr] == '\n' || str[lstr] == '\r'))
		str[lstr--] = 0;
	return lstr;
}

char FileFs::readChar()
{
    char ch;
    uint32_t nrd;

    fatFs().ffsResult = f_read(&ffile, &ch, 1, reinterpret_cast<UINT*>(&nrd));
    return nrd == 1 ? ch : -1;
}

uint16_t FileFs::readInt()
{
	uint16_t i = 0;
    char c;

	do
		c = readChar();
	while (c != -1 && !isdigit(c));
	while (isdigit(c)) {
		i = 10 * i + c - '0';
		c = readChar();
	}
	return i;
}

uint16_t FileFs::readHex()
{
	uint16_t i = 0;
	char c;
	// skip characters they are not hexadecimal
	do
		c = readChar();
	while (c != -1 && !isxdigit(c));
	while (isxdigit(c)) {
		i = 16 * i + c;
		if (isdigit(c))
			i -= '0';
		else {
			i += 10;
			if (c < 'F')
				i -= 'A';
			else
				i -= 'a';
		}
		c = readChar();
	}
	return i;
}

uint32_t FileFs::curPosition()
{
	return f_tell(&ffile);
}

bool FileFs::seekSet( uint32_t cur )
{
  fatFs().ffsResult = f_lseek( & ffile, cur );
  return fatFs().ffsResult == FR_OK;
}

uint32_t FileFs::fileSize()
{
	return f_size(&ffile);
}