 
#ifndef _TESTPROTO_H_ 
#define _TESTPROTO_H_ 
 
 
const unsigned short MAX_SESSIONS = 5000; //max session count  
 
enum GAME_TYPE : unsigned short 
{ 
    GTYPE_INVALID = 0, //无效  
    GTYPE_WAITING = 1, //等待  
    GTYPE_INGAME = 10, //游戏中  
    GTYPE_LEAVE = 11, //离开  
}; 
 
struct TestIntegerData //测试  
{ 
    static const unsigned short getProtoID() { return 30002;} 
    static const std::string getProtoName() { return "ID_TestIntegerData";} 
    inline const std::vector<std::string>  getDBBuild(); 
    inline std::string  getDBInsert(); 
    inline std::string  getDBDelete(); 
    inline std::string  getDBUpdate(); 
    inline std::string  getDBSelect(); 
    inline bool fetchFromDBResult(zsummer::mysql::DBResult &result); 
    char _char;  
    unsigned char _uchar;  
    short _short;  
    unsigned short _ushort;  
    int _int;  
    unsigned int _uint;  
    long long _i64;  
    unsigned long long _ui128;  
    unsigned long long _ui64;  
    TestIntegerData() 
    { 
        _char = 0; 
        _uchar = 0; 
        _short = 0; 
        _ushort = 0; 
        _int = 0; 
        _uint = 0; 
        _i64 = 0; 
        _ui128 = 0; 
        _ui64 = 0; 
    } 
    TestIntegerData(const char & _char, const unsigned char & _uchar, const short & _short, const unsigned short & _ushort, const int & _int, const unsigned int & _uint, const long long & _i64, const unsigned long long & _ui128, const unsigned long long & _ui64) 
    { 
        this->_char = _char; 
        this->_uchar = _uchar; 
        this->_short = _short; 
        this->_ushort = _ushort; 
        this->_int = _int; 
        this->_uint = _uint; 
        this->_i64 = _i64; 
        this->_ui128 = _ui128; 
        this->_ui64 = _ui64; 
    } 
}; 
 
const std::vector<std::string>  TestIntegerData::getDBBuild() 
{ 
    std::vector<std::string> ret; 
    ret.push_back("desc `tb_TestIntegerData`"); 
    ret.push_back("CREATE TABLE `tb_TestIntegerData` (        `_char` bigint(20) NOT NULL DEFAULT '0' ,        `_uchar` bigint(20) unsigned NOT NULL DEFAULT '0' ,        `_short` bigint(20) NOT NULL DEFAULT '0' ,        `_ushort` bigint(20) unsigned NOT NULL DEFAULT '0' ,        `_int` bigint(20) NOT NULL DEFAULT '0' ,        `_uint` bigint(20) unsigned NOT NULL DEFAULT '0' ,        `_i64` bigint(20) NOT NULL DEFAULT '0' ,        `_ui128` bigint(20) unsigned NOT NULL DEFAULT '0' ,        PRIMARY KEY(`_uint`,`_i64`)  ) ENGINE = MyISAM DEFAULT CHARSET = utf8"); 
    ret.push_back("alter table `tb_TestIntegerData` add `_char`  bigint(20) NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` change `_char`  `_char`  bigint(20) NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` add `_uchar`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` change `_uchar`  `_uchar`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` add `_short`  bigint(20) NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` change `_short`  `_short`  bigint(20) NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` add `_ushort`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` change `_ushort`  `_ushort`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` add `_int`  bigint(20) NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` change `_int`  `_int`  bigint(20) NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` add `_uint`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` change `_uint`  `_uint`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` add `_i64`  bigint(20) NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` change `_i64`  `_i64`  bigint(20) NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` add `_ui128`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_TestIntegerData` change `_ui128`  `_ui128`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    return std::move(ret); 
} 
std::string  TestIntegerData::getDBSelect() 
{ 
    zsummer::mysql::DBQuery q; 
    q.init("select `_char`,`_uchar`,`_short`,`_ushort`,`_int`,`_uint`,`_i64`,`_ui128` from `tb_TestIntegerData` where `_uint` = ? and `_i64` = ? "); 
    q << this->_uint; 
    q << this->_i64; 
    return q.pickSQL(); 
} 
std::string  TestIntegerData::getDBInsert() 
{ 
    zsummer::mysql::DBQuery q; 
    q.init("insert into `tb_TestIntegerData`(`_char`,`_uchar`,`_short`,`_ushort`,`_int`,`_uint`,`_i64`,`_ui128`) values(?,?,?,?,?,?,?,?)"); 
    q << this->_char; 
    q << this->_uchar; 
    q << this->_short; 
    q << this->_ushort; 
    q << this->_int; 
    q << this->_uint; 
    q << this->_i64; 
    q << this->_ui128; 
    return q.pickSQL(); 
} 
std::string  TestIntegerData::getDBDelete() 
{ 
    zsummer::mysql::DBQuery q; 
    q.init("delete from `tb_TestIntegerData` where `_uint` = ?,`_i64` = ? "); 
    q << this->_uint; 
    q << this->_i64; 
    return q.pickSQL(); 
} 
std::string  TestIntegerData::getDBUpdate() 
{ 
    zsummer::mysql::DBQuery q; 
    q.init("insert into `tb_TestIntegerData`(_uint,_i64) values(?,? ) on duplicate key update `_char` = ?,`_uchar` = ?,`_short` = ?,`_ushort` = ?,`_int` = ?,`_ui128` = ? "); 
    q << this->_char; 
    q << this->_uchar; 
    q << this->_short; 
    q << this->_ushort; 
    q << this->_int; 
    q << this->_ui128; 
    return q.pickSQL(); 
} 
bool TestIntegerData::fetchFromDBResult(zsummer::mysql::DBResult &result) 
{ 
    if (result.getErrorCode() != zsummer::mysql::QEC_SUCCESS) 
    { 
        LOGE("error fetch TestIntegerData from table `tb_TestIntegerData` . ErrorCode="  <<  result.getErrorCode() << ", Error=" << result.getErrorMsg() << ", sql=" << result.peekSQL()); 
        return false; 
    } 
    try 
    { 
        if (result.haveRow()) 
        { 
            result >> this->_char; 
            result >> this->_uchar; 
            result >> this->_short; 
            result >> this->_ushort; 
            result >> this->_int; 
            result >> this->_uint; 
            result >> this->_i64; 
            result >> this->_ui128; 
            return true;  
        } 
    } 
    catch(std::runtime_error e) 
    { 
        LOGE("catch one except error when fetch TestIntegerData from table `tb_TestIntegerData` . what=" << e.what() << "  ErrorCode="  <<  result.getErrorCode() << ", Error=" << result.getErrorMsg() << ", sql=" << result.peekSQL()); 
        return false; 
    } 
    return false; 
} 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const TestIntegerData & data) 
{ 
    ws << data._char;  
    ws << data._uchar;  
    ws << data._short;  
    ws << data._ushort;  
    ws << data._int;  
    ws << data._uint;  
    ws << data._i64;  
    ws << data._ui128;  
    ws << data._ui64;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, TestIntegerData & data) 
{ 
        rs >> data._char;  
        rs >> data._uchar;  
        rs >> data._short;  
        rs >> data._ushort;  
        rs >> data._int;  
        rs >> data._uint;  
        rs >> data._i64;  
        rs >> data._ui128;  
        rs >> data._ui64;  
    return rs; 
} 
 
struct TestFloatData //测试  
{ 
    static const unsigned short getProtoID() { return 30003;} 
    static const std::string getProtoName() { return "ID_TestFloatData";} 
    float _float;  
    double _double;  
    TestFloatData() 
    { 
        _float = 0.0; 
        _double = 0.0; 
    } 
    TestFloatData(const float & _float, const double & _double) 
    { 
        this->_float = _float; 
        this->_double = _double; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const TestFloatData & data) 
{ 
    ws << data._float;  
    ws << data._double;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, TestFloatData & data) 
{ 
        rs >> data._float;  
        rs >> data._double;  
    return rs; 
} 
 
struct TestStringData //测试  
{ 
    static const unsigned short getProtoID() { return 30004;} 
    static const std::string getProtoName() { return "ID_TestStringData";} 
    std::string _string;  
    TestStringData() 
    { 
    } 
    TestStringData(const std::string & _string) 
    { 
        this->_string = _string; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const TestStringData & data) 
{ 
    ws << data._string;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, TestStringData & data) 
{ 
        rs >> data._string;  
    return rs; 
} 
 
 
typedef std::vector<unsigned int> IntArray;  
 
 
typedef std::vector<TestIntegerData> TestIntegerDataArray;  
 
 
typedef std::vector<TestFloatData> TestFloatDataArray;  
 
 
typedef std::vector<TestStringData> TestStringDataArray;  
 
 
typedef std::map<std::string, TestIntegerData> TestIntegerDataMap;  
 
 
typedef std::map<std::string, TestFloatData> TestFloatDataMap;  
 
 
typedef std::map<std::string, TestStringData> TestStringDataMap;  
 
struct EchoPack 
{ 
    static const unsigned short getProtoID() { return 30000;} 
    static const std::string getProtoName() { return "ID_EchoPack";} 
    TestIntegerDataArray _iarray;  
    TestFloatDataArray _farray;  
    TestStringDataArray _sarray;  
    TestIntegerDataMap _imap;  
    TestFloatDataMap _fmap;  
    TestStringDataMap _smap;  
    EchoPack() 
    { 
    } 
    EchoPack(const TestIntegerDataArray & _iarray, const TestFloatDataArray & _farray, const TestStringDataArray & _sarray, const TestIntegerDataMap & _imap, const TestFloatDataMap & _fmap, const TestStringDataMap & _smap) 
    { 
        this->_iarray = _iarray; 
        this->_farray = _farray; 
        this->_sarray = _sarray; 
        this->_imap = _imap; 
        this->_fmap = _fmap; 
        this->_smap = _smap; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const EchoPack & data) 
{ 
    ws << data._iarray;  
    ws << data._farray;  
    ws << data._sarray;  
    ws << data._imap;  
    ws << data._fmap;  
    ws << data._smap;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, EchoPack & data) 
{ 
        rs >> data._iarray;  
        rs >> data._farray;  
        rs >> data._sarray;  
        rs >> data._imap;  
        rs >> data._fmap;  
        rs >> data._smap;  
    return rs; 
} 
 
struct Pulse 
{ 
    static const unsigned short getProtoID() { return 30001;} 
    static const std::string getProtoName() { return "ID_Pulse";} 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const Pulse & data) 
{ 
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, Pulse & data) 
{ 
    return rs; 
} 
 
#endif 
