 
#ifndef _TESTPROTOSQL_H_ 
#define _TESTPROTOSQL_H_ 
 
 
inline std::vector<std::string> TestIntegerData_BUILD() 
{ 
	std::vector<std::string> ret; 
	ret.second.push_back("CREATE TABLE `tb_TestIntegerData` (`_i64` bigint(20) NOT NULL DEFAULT '0' ,   PRIMARY KEY(`_i64`) ) ENGINE = MyISAM DEFAULT CHARSET = utf8"); 
	ret.second.push_back("alter table `tb_TestIntegerData` add `_char`  bigint(20) NOT NULL DEFAULT '0' "); 
	ret.second.push_back("alter table `tb_TestIntegerData` add `_uchar`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
	ret.second.push_back("alter table `tb_TestIntegerData` add `_short`  bigint(20) NOT NULL DEFAULT '0' "); 
	ret.second.push_back("alter table `tb_TestIntegerData` add `_ushort`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
	ret.second.push_back("alter table `tb_TestIntegerData` add `_int`  bigint(20) NOT NULL DEFAULT '0' "); 
	ret.second.push_back("alter table `tb_TestIntegerData` add `_uint`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
	ret.second.push_back("alter table `tb_TestIntegerData` add `_ui128`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
	ret.second.push_back("alter table `tb_TestIntegerData` add `_ui64`  bigint(20) unsigned NOT NULL DEFAULT '0' "); 
	return std::move(ret); 
} 
 
inline std::string TestIntegerData_LOAD( unsigned long long curLoaded) 
{ 
	zsummer::mysql::DBQuery q(" select `_char`,`_uchar`,`_short`,`_ushort`,`_int`,`_uint`,`_i64`,`_ui128`,`_ui64` from `tb_TestIntegerData` order by `_i64` desc limit ?, 1000 "); 
	q << curLoaded; 
	return q.popSQL(); 
} 
 
inline std::string TestIntegerData_SELECT(long long _i64) 
{ 
	zsummer::mysql::q(" select `_char`,`_uchar`,`_short`,`_ushort`,`_int`,`_uint`,`_i64`,`_ui128`,`_ui64` from `tb_TestIntegerData` where `_i64` = ? "); 
	q << _i64; 
	return q.popSQL(); 
} 
 
inline std::map<long long, TestIntegerData> TestIntegerData_FETCH(zsummer::mysql::DBResultPtr ptr) 
{ 
	std::map<long long, TestIntegerData> ret; 
	if (ptr->getErrorCode() != QEC_SUCCESS) 
	{ 
		LOGE("fetch info from db found error. ErrorCode="  <<  ptr->getErrorCode() << ", Error=" << ptr->getLastError()); 
		return ret; 
	} 
	try 
	{ 
		while (ptr->haveRow()) 
		{ 
			TestIntegerData info; 
			*ptr >> info._char; 
			*ptr >> info._uchar; 
			*ptr >> info._short; 
			*ptr >> info._ushort; 
			*ptr >> info._int; 
			*ptr >> info._uint; 
			*ptr >> info._i64; 
			*ptr >> info._ui128; 
			*ptr >> info._ui64; 
			ret[info._i64] = info; 
		} 
	} 
	catch(std::runtime_error e) 
	{ 
		LOGE("fetch info catch one runtime error. what=" << e.what()); 
		return ret; 
	} 
	} 
	return std::move(ret); 
} 
 
 
inline std::string TestIntegerData_UPDATE( const TestIntegerData & info)  
{ 
	zsummer::mysql:: q(" insert into tb_TestIntegerData(_i64) values(?) on duplicate key set `_char` = ?,`_uchar` = ?,`_short` = ?,`_ushort` = ?,`_int` = ?,`_uint` = ?,`_ui128` = ?,`_ui64` = ? from tb_TestIntegerData where `_i64` = ? "); 
	q << info._i64; 
	q << info._char; 
	q << info._uchar; 
	q << info._short; 
	q << info._ushort; 
	q << info._int; 
	q << info._uint; 
	q << info._ui128; 
	q << info._ui64; 
	return std::move(ret); 
} 
 
 
#endif 
