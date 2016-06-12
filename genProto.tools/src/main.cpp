


#include "any.h"
#include "common.h"
#include "genBase.h"
#include "parseCache.h"
#include "parseProto.h"

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
        for (auto & file : files)
        {
            std::string filename = subStringWithoutBack(file.filename, ".");
            ParseCache cache;
            cache.parse(filename);
            if (!cache.isNeedUpdate())
            {
                continue;
            }
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
    }
    catch (const std::exception & e)
    {
        LOGE("catch error: " << e.what());
        return -1;
    }

    
    LOGA("All Success..");

    return 0;
}






