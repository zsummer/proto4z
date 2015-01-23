using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Proto4z;

namespace Proto4z
{
    class STATIC_EC_SUCCESS //global const 
    {
        public const UInt16 value = 100;
    }
    class STATIC_EC_ERROR //global const 
    {
        public const UInt16 value = 100;
    }
    class HeroInfo //struct
    {
        public Int32 id;
        public string name;
    }

    class HeroInfoDict : Dictionary<Int32, HeroInfo> { }
    class HeroInfoArray : List<HeroInfo> { }
    class UserInfo //struct
    {
        public UInt64 uid;
        public HeroInfo hero;
        public HeroInfoDict dictHeros;
        public HeroInfoArray arrayHeros;
    }

    class LS2C_LoginResult
    {
        public UInt16 retCode;
        public UserInfo info;
    }

}



namespace ConsoleApplication2
{
    class Program
    {
        static void Main(string[] args)
        {
            LS2C_LoginResult result = new LS2C_LoginResult();
            result.retCode = STATIC_EC_SUCCESS.value;
            result.info = new UserInfo();
            result.info.uid = 1000;
            result.info.hero = new HeroInfo();
            result.info.hero.id = 100;
            result.info.hero.name = "hero";

            HeroInfo hero1 = new HeroInfo();
            hero1.id = 100;
            hero1.name = "hero";

            HeroInfo hero2 = new HeroInfo();
            hero2.id = 101;
            hero2.name = "hero";


            result.info.dictHeros = new HeroInfoDict();
            result.info.dictHeros.Add(hero1.id, hero1);
            result.info.dictHeros.Add(hero2.id, hero2);

            result.info.arrayHeros = new HeroInfoArray();
            result.info.arrayHeros.Add(hero1);
            result.info.arrayHeros.Add(hero2);

            System.Console.WriteLine(result.retCode);
        }
    }
}
