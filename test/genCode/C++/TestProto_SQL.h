 
#ifndef _TESTPROTOSQL_H_ 
#define _TESTPROTOSQL_H_ 
 
 
inline std::vector<std::string> TestIntegerData_BUILD() 
{ 
    std::vector<std::string> ret; 
    ret.push_back("desc `tb_TestIntegerData`"); 
    ret.push_back("CREATE TABLE `tb_TestIntegerData` (        `_char` bigint(20) NOT NULL DEFAULT '0' ,        `_uchar` bigint(20) unsigned NOT NULL DEFAULT '0' ,        `_short` bigint(20) NOT NULL DEFAULT '0' ,        `_ushort` bigint(20) unsigned NOT NULL DEFAULT '0' ,        `_int` bigint(20) NOT NULL DEFAULT '0' ,        `_uint` bigint(20) unsigned NOT NULL DEFAULT '0' ,        `_i64` bigint(20) NOT NULL DEFAULT '0'         PRIMARY KEY(`_uint`,`_i64`)  ) ENGINE = MyISAM DEFAULT CHARSET = utf8"); 
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
    return std::move(ret); 
} 
 
inline std::vector<std::string> TestIntegerData_DCL() 
{ 
    ret.push_back("select `_char`,`_uchar`,`_short`,`_ushort`,`_int`,`_uint`,`_i64` from `tb_TestIntegerData` where `_uint` = ? and `_i64` = ? "); 
    ret.push_back("insert into `tb_TestIntegerData`(`_char`,`_uchar`,`_short`,`_ushort`,`_int`,`_uint`,`_i64`) values(?,?,?,?,?,?,?)"); 
    ret.push_back("delete from `tb_TestIntegerData` where `_uint` = ?,`_i64` = ? "); 
    ret.push_back("insert into `tb_TestIntegerData`(_uint,_i64) values(?,? ) on duplicate key update `_char` = ?,`_uchar` = ?,`_short` = ?,`_ushort` = ?,`_int` = ? "); 
    return std::move(ret); 
} 
 
inline bool TestIntegerData_FETCH(zsummer::mysql::DBResultPtr ptr, TestIntegerData & info) 
{ 
    if (ptr->getErrorCode() != zsummer::mysql::QEC_SUCCESS) 
    { 
        LOGE("fetch info from db found error. ErrorCode="  <<  ptr->getErrorCode() << ", Error=" << ptr->getErrorMsg()); 
        return false; 
    } 
    try 
    { 
        if (ptr->haveRow()) 
        { 
            *ptr >> info._char; 
            *ptr >> info._uchar; 
            *ptr >> info._short; 
            *ptr >> info._ushort; 
            *ptr >> info._int; 
            *ptr >> info._uint; 
            *ptr >> info._i64; 
            return true;  
        } 
    } 
    catch(std::runtime_error e) 
    { 
        LOGE("fetch info catch one runtime error. what=" << e.what()); 
        return false; 
    } 
    return false; 
} 
 
 
 
#endif 
