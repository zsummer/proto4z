 
#ifndef _TESTPROTO_H_ 
#define _TESTPROTO_H_ 
 
 
const unsigned short MAX_SESSIONS = 5000; //max session count  
 
enum ServiceStatus : unsigned short 
{ 
    SS_NONE = 0, //不存在  
    SS_CREATED = 1, //已创建  
    SS_WORKING = 10, //working状态  
    SS_DESTROY = 11, //已销毁  
}; 
 
struct IntegerData //测试  
{ 
    static const unsigned short getProtoID() { return 30000;} 
    static const std::string getProtoName() { return "IntegerData";} 
    char _char;  
    unsigned char _uchar;  
    short _short;  
    unsigned short _ushort;  
    int _int;  
    unsigned int _uint;  
    long long _i64;  
    unsigned long long _ui64;  
    IntegerData() 
    { 
        _char = 0; 
        _uchar = 0; 
        _short = 0; 
        _ushort = 0; 
        _int = 0; 
        _uint = 0; 
        _i64 = 0; 
        _ui64 = 0; 
    } 
    IntegerData(const char & _char, const unsigned char & _uchar, const short & _short, const unsigned short & _ushort, const int & _int, const unsigned int & _uint, const long long & _i64, const unsigned long long & _ui64) 
    { 
        this->_char = _char; 
        this->_uchar = _uchar; 
        this->_short = _short; 
        this->_ushort = _ushort; 
        this->_int = _int; 
        this->_uint = _uint; 
        this->_i64 = _i64; 
        this->_ui64 = _ui64; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const IntegerData & data) 
{ 
    ws << data._char;  
    ws << data._uchar;  
    ws << data._short;  
    ws << data._ushort;  
    ws << data._int;  
    ws << data._uint;  
    ws << data._i64;  
    ws << data._ui64;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, IntegerData & data) 
{ 
    rs >> data._char;  
    rs >> data._uchar;  
    rs >> data._short;  
    rs >> data._ushort;  
    rs >> data._int;  
    rs >> data._uint;  
    rs >> data._i64;  
    rs >> data._ui64;  
    return rs; 
} 
 
struct FloatData //测试  
{ 
    static const unsigned short getProtoID() { return 30001;} 
    static const std::string getProtoName() { return "FloatData";} 
    float _float;  
    double _double;  
    FloatData() 
    { 
        _float = 0.0; 
        _double = 0.0; 
    } 
    FloatData(const float & _float, const double & _double) 
    { 
        this->_float = _float; 
        this->_double = _double; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const FloatData & data) 
{ 
    ws << data._float;  
    ws << data._double;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, FloatData & data) 
{ 
    rs >> data._float;  
    rs >> data._double;  
    return rs; 
} 
 
struct StringData //测试  
{ 
    static const unsigned short getProtoID() { return 30002;} 
    static const std::string getProtoName() { return "StringData";} 
    std::string _string;  
    StringData() 
    { 
    } 
    StringData(const std::string & _string) 
    { 
        this->_string = _string; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const StringData & data) 
{ 
    ws << data._string;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, StringData & data) 
{ 
    rs >> data._string;  
    return rs; 
} 
 
 
typedef std::vector<unsigned int> IntArray;  
 
 
typedef std::vector<IntegerData> IntegerDataArray;  
 
 
typedef std::vector<FloatData> FloatDataArray;  
 
 
typedef std::vector<StringData> StringDataArray;  
 
 
typedef std::map<std::string, IntegerData> IntegerDataMap;  
 
 
typedef std::map<std::string, FloatData> FloatDataMap;  
 
 
typedef std::map<std::string, StringData> StringDataMap;  
 
struct EchoPack 
{ 
    static const unsigned short getProtoID() { return 30003;} 
    static const std::string getProtoName() { return "EchoPack";} 
    IntegerDataArray _iarray;  
    FloatDataArray _farray;  
    StringDataArray _sarray;  
    IntegerDataMap _imap;  
    FloatDataMap _fmap;  
    StringDataMap _smap;  
    EchoPack() 
    { 
    } 
    EchoPack(const IntegerDataArray & _iarray, const FloatDataArray & _farray, const StringDataArray & _sarray, const IntegerDataMap & _imap, const FloatDataMap & _fmap, const StringDataMap & _smap) 
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
 
struct MoneyTree //摇钱树功能模块  
{ 
    static const unsigned short getProtoID() { return 30004;} 
    static const std::string getProtoName() { return "MoneyTree";} 
    unsigned int lastTime; //最后一次执行时间  
    unsigned int freeCount; //今日剩余免费次数  
    unsigned int payCount; //今日已购买次数  
    unsigned int statSum; //历史总和  
    unsigned int statCount; //历史总次数  
    MoneyTree() 
    { 
        lastTime = 0; 
        freeCount = 0; 
        payCount = 0; 
        statSum = 0; 
        statCount = 0; 
    } 
    MoneyTree(const unsigned int & lastTime, const unsigned int & freeCount, const unsigned int & payCount, const unsigned int & statSum, const unsigned int & statCount) 
    { 
        this->lastTime = lastTime; 
        this->freeCount = freeCount; 
        this->payCount = payCount; 
        this->statSum = statSum; 
        this->statCount = statCount; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const MoneyTree & data) 
{ 
    ws << data.lastTime;  
    ws << data.freeCount;  
    ws << data.payCount;  
    ws << data.statSum;  
    ws << data.statCount;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, MoneyTree & data) 
{ 
    rs >> data.lastTime;  
    rs >> data.freeCount;  
    rs >> data.payCount;  
    rs >> data.statSum;  
    rs >> data.statCount;  
    return rs; 
} 
 
struct SimplePack //简单示例  
{ 
    static const unsigned short getProtoID() { return 30005;} 
    static const std::string getProtoName() { return "SimplePack";} 
    inline const std::vector<std::string>  getDBBuild(); 
    inline std::string  getDBInsert(); 
    inline std::string  getDBDelete(); 
    inline std::string  getDBUpdate(); 
    inline std::string  getDBSelect(); 
    inline std::string  getDBSelectPure(); 
    inline bool fetchFromDBResult(zsummer::mysql::DBResult &result); 
    unsigned int id; //id, 对应数据库的结构为自增ID,key  
    std::string name; //昵称, 唯一索引  
    unsigned int createTime; //创建时间, 普通索引  
    MoneyTree moneyTree;  
    SimplePack() 
    { 
        id = 0; 
        createTime = 0; 
    } 
    SimplePack(const unsigned int & id, const std::string & name, const unsigned int & createTime, const MoneyTree & moneyTree) 
    { 
        this->id = id; 
        this->name = name; 
        this->createTime = createTime; 
        this->moneyTree = moneyTree; 
    } 
}; 
 
const std::vector<std::string>  SimplePack::getDBBuild() 
{ 
    std::vector<std::string> ret; 
    ret.push_back("CREATE TABLE IF NOT EXISTS `tb_SimplePack` (        `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT ,        `name` varchar(255) NOT NULL DEFAULT '' ,        `createTime` bigint(20) unsigned NOT NULL DEFAULT '0' ,        PRIMARY KEY(`id`),        UNIQUE KEY `name` (`name`),        KEY `createTime` (`createTime`) ) ENGINE = MyISAM DEFAULT CHARSET = utf8"); 
    ret.push_back("alter table `tb_SimplePack` add `id`  bigint(20) unsigned NOT NULL AUTO_INCREMENT "); 
    ret.push_back("alter table `tb_SimplePack` change `id`  `id`  bigint(20) unsigned NOT NULL AUTO_INCREMENT "); 
    ret.push_back("alter table `tb_SimplePack` add `name`  varchar(255) NOT NULL DEFAULT '' "); 
    ret.push_back("alter table `tb_SimplePack` change `name`  `name`  varchar(255) NOT NULL DEFAULT '' "); 
    ret.push_back("alter table `tb_SimplePack` add `createTime`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_SimplePack` change `createTime`  `createTime`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
    ret.push_back("alter table `tb_SimplePack` add `moneyTree`  blob "); 
    ret.push_back("alter table `tb_SimplePack` change `moneyTree`  `moneyTree`  blob "); 
    return std::move(ret); 
} 
std::string  SimplePack::getDBSelect() 
{ 
    zsummer::mysql::DBQuery q; 
    q.init("select `id`,`name`,`createTime`,`moneyTree` from `tb_SimplePack` where `id` = ? "); 
    q << this->id; 
    return q.pickSQL(); 
} 
std::string  SimplePack::getDBSelectPure() 
{ 
    return "select `id`,`name`,`createTime`,`moneyTree` from `tb_SimplePack` "; 
} 
std::string  SimplePack::getDBInsert() 
{ 
    zsummer::mysql::DBQuery q; 
    q.init("insert into `tb_SimplePack`(`name`,`createTime`,`moneyTree`) values(?,?,?)"); 
    q << this->name; 
    q << this->createTime; 
    q << this->moneyTree; 
    return q.pickSQL(); 
} 
std::string  SimplePack::getDBDelete() 
{ 
    zsummer::mysql::DBQuery q; 
    q.init("delete from `tb_SimplePack` where `id` = ? "); 
    q << this->id; 
    return q.pickSQL(); 
} 
std::string  SimplePack::getDBUpdate() 
{ 
    zsummer::mysql::DBQuery q; 
    q.init("insert into `tb_SimplePack`(id) values(? ) on duplicate key update `name` = ?,`createTime` = ?,`moneyTree` = ? "); 
    q << this->id; 
    q << this->name; 
    q << this->createTime; 
    q << this->moneyTree; 
    return q.pickSQL(); 
} 
bool SimplePack::fetchFromDBResult(zsummer::mysql::DBResult &result) 
{ 
    if (result.getErrorCode() != zsummer::mysql::QEC_SUCCESS) 
    { 
        LOGE("error fetch SimplePack from table `tb_SimplePack` . ErrorCode="  <<  result.getErrorCode() << ", Error=" << result.getErrorMsg() << ", sql=" << result.peekSQL()); 
        return false; 
    } 
    try 
    { 
        if (result.haveRow()) 
        { 
            result >> this->id; 
            result >> this->name; 
            result >> this->createTime; 
            try 
            { 
                std::string blob; 
                result >> blob; 
                if(!blob.empty()) 
                { 
                    zsummer::proto4z::ReadStream rs(blob.c_str(), (zsummer::proto4z::Integer)blob.length(), false); 
                    rs >> this->moneyTree; 
                } 
            } 
            catch(std::runtime_error e) 
            { 
                LOGW("catch one except error when fetch SimplePack.moneyTree  from table `tb_SimplePack` . what=" << e.what() << "  ErrorCode="  <<  result.getErrorCode() << ", Error=" << result.getErrorMsg() << ", sql=" << result.peekSQL()); 
            } 
            return true;  
        } 
    } 
    catch(std::runtime_error e) 
    { 
        LOGE("catch one except error when fetch SimplePack from table `tb_SimplePack` . what=" << e.what() << "  ErrorCode="  <<  result.getErrorCode() << ", Error=" << result.getErrorMsg() << ", sql=" << result.peekSQL()); 
        return false; 
    } 
    return false; 
} 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SimplePack & data) 
{ 
    ws << data.id;  
    ws << data.name;  
    ws << data.createTime;  
    ws << data.moneyTree;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SimplePack & data) 
{ 
    rs >> data.id;  
    rs >> data.name;  
    rs >> data.createTime;  
    rs >> data.moneyTree;  
    return rs; 
} 
 
#endif 
