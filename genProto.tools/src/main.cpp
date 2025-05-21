


#include "any.h"
#include "common.h"
#include "genBase.h"
#include "parseCache.h"
#include "parseProto.h"
std::map<unsigned short, std::string> _cacheKeys;
std::map<unsigned short, std::string> _errCodes;
//#define __WITH_TAG
int main(int argc, char *argv[])
{
    zsummer::log4z::ILog4zManager::getRef().setLoggerFileLine(LOG4Z_MAIN_LOGGER_ID, false);
    zsummer::log4z::ILog4zManager::getRef().setLoggerOutFile(LOG4Z_MAIN_LOGGER_ID, false);
    zsummer::log4z::ILog4zManager::getRef().start();


    std::vector<SearchFileInfo> files;
    if (!searchFiles("./*.xml", files, false))
    {
        LOGE("searchFiles error.");
        return 0;
    }
    try
    {
        bool updateReflection = false;
        for (auto & file : files)
        {
            std::string filename = subString(file.filename, ".", true, true).first;
            ParseCache cache;
            cache.parse(filename);
            if (cache.isNeedUpdate())
            {
                updateReflection = true;
                auto stores = parseProto(filename, cache);
                for (int i = SL_NORMAL + 1; i < SL_END; i++)
                {
                    auto gen = createGenerate((SupportLanguageType)i);
                    if (!gen)
                    {
                        continue;
                    }
                    gen->init(filename, (SupportLanguageType)i);
                    auto content = gen->genRealContent(stores);
                    gen->write(content);
                    destroyGenerate(gen);
                }
                cache.write();
            }
            if (true)
            {
                auto stores = parseProto(filename, cache);
                for (auto & store : stores)
                {
                    if (store.type_ == GT_DataEnum && trim(store.enum_.name_) == "ERROR_CODE")
                    {
                        for (auto & kv : store.enum_.members_)
                        {
                            _errCodes[fromString<unsigned short>(kv.value_, 0)] = kv.desc_;
                        }
                    }
                }
                for (auto & kv : cache._cacheNumber)
                {
                    _cacheKeys[kv.second] = kv.first;
                }
            }
        }
        if (updateReflection)
        {
            writeCSharpReflection(_cacheKeys, _errCodes);
            writeCPPReflection(_cacheKeys, _errCodes);
        }

    }
    catch (const std::exception & e)
    {
        LOGE("catch error: " << e.what());
        return -1;
    }

    
    LOGA("All Success..");

    return 0;
}






