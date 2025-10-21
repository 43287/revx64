# revx64

基于CPP的针对WindowsX64逆向分析与开发的基础库



当前进度：

Process支持基本的读写操作

```cpp
//初始化
static Process initByName(const std::string& exeName);
static Process initByPid(DWORD pid);
static Process initByWindowName(std::string& className);

//读写
// 自定类型读写
template<typename T>
bool read(LPCVOID addr, T& localBuffer);
template<typename T>
bool write(LPVOID addr, T& localBuffer);
// 读
BYTE readByte(LPVOID addr);
WORD readWord(LPVOID addr);
DWORD readDword(LPVOID addr);
DWORD64 readQword(LPVOID addr);
std::vector<BYTE> readVector(LPVOID addr,size_t len);

// 写
void writeByte(LPVOID addr, BYTE data);
void writeWord(LPVOID addr, WORD data);
void writeDword(LPVOID addr, DWORD data);
void writeQword(LPVOID addr, DWORD64 data);
void writeVector(LPVOID addr, std::vector<BYTE>& data);


// 基本信息
uintptr_t getPEB();
uintptr_t getBase();
```


