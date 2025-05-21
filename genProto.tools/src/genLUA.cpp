


#include "genLUA.h"

std::string GenLUA::genRealContent(const std::list<AnyData> & stores)
{
    std::string text;
    for (auto &info : stores)
    {
        if (info.type_ == GT_DataConstValue)
        {
            text += LFCR;
            text += genDataConst(info.const_);
        }
        else if (info.type_ == GT_DataEnum)
        {
            text += LFCR;
            text += genDataEnum(info.enum_);
        }
        else if (info.type_ == GT_DataArray)
        {
            text += LFCR;
            text += genDataArray(info.array_);
        }
        else if (info.type_ == GT_DataMap)
        {
            text += LFCR;
            text += genDataMap(info.map_);
        }
        else if (info.type_ == GT_DataPacket)
        {
            text += LFCR;
            text += genDataPacket(info.proto_);
        }
    }

    return text;
}



std::string GenLUA::genDataConst(const DataConstValue & dc)
{
    std::string text;
    text += "Proto4z." + dc.name_ + " = " + dc.value_;
    if (!dc.desc_.empty())
    {
        text += "--" + dc.desc_;
    }
    return text;
}

std::string GenLUA::genDataEnum(const DataEnum & de)
{
    std::string text;
    for (const auto & m : de.members_)
    {
        text += "Proto4z." + m.name_ + " = " + m.value_;
        if (!m.desc_.empty())
        {
            text += "--" + m.desc_;
        }
        text += LFCR;
    }
    return text;
}
std::string GenLUA::genDataArray(const DataArray & da)
{
    std::string text;
    text += "Proto4z." + da.arrayName_ + " = {} ";
    if (!da.desc_.empty())
    {
        text += "--" + da.desc_;
    }
    text += LFCR;

    text += "Proto4z." + da.arrayName_ + ".__protoName = \"" + da.arrayName_ + "\"" + LFCR;
    text += "Proto4z." + da.arrayName_ + ".__protoDesc = \"array\"" + LFCR;
    text += "Proto4z." + da.arrayName_ + ".__protoTypeV = \"" + da.type_ + "\"" + LFCR;
    return text;
}
std::string GenLUA::genDataMap(const DataMap & dm)
{
    std::string text;
    text += "Proto4z." + dm.mapName_ + " = {} ";
    if (!dm.desc_.empty())
    {
        text += "--" + dm.desc_;
    }
    text += LFCR;

    text += "Proto4z." + dm.mapName_ + ".__protoName = \"" + dm.mapName_ + "\"" + LFCR;
    text += "Proto4z." + dm.mapName_ + ".__protoDesc = \"map\"" + LFCR;
    text += "Proto4z." + dm.mapName_ + ".__protoTypeK = \"" + dm.typeKey_ + "\"" + LFCR;
    text += "Proto4z." + dm.mapName_ + ".__protoTypeV = \"" + dm.typeValue_ + "\"" + LFCR;
    return text;
}
std::string GenLUA::genDataPacket(const DataPacket & dp)
{
    std::string text;

    text += "Proto4z.register(" + dp.const_.value_ + ",\"" + dp.struct_.name_ + "\")" + LFCR;


    text += "Proto4z." + dp.struct_.name_ + " = {} ";
    if (!dp.struct_.desc_.empty())
    {
        text += "--" + dp.struct_.desc_;
    }
    text += LFCR;


    text += "Proto4z." + dp.struct_.name_ + ".__protoID = " + dp.const_.value_ + "" + LFCR;
    text += "Proto4z." + dp.struct_.name_ + ".__protoName = \"" + dp.struct_.name_ + "\"" + LFCR;

    for (size_t i = 0; i < dp.struct_.members_.size(); ++i)
    {
        text += "Proto4z." + dp.struct_.name_ + "[" + toString(i + 1)
            + "] = {name=\"" + dp.struct_.members_[i].name_ + "\", type=\"" + dp.struct_.members_[i].type_ + "\"";
        text += " } ";
        if (!dp.struct_.members_[i].desc_.empty())
        {
            text += "--" + dp.struct_.members_[i].desc_;
        }
        text += LFCR;
    }
    return text;
}


