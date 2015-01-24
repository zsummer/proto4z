using System;

using Proto4z;

namespace Proto4z
{
    class ProtoData
    {
        public virtual System.Collections.Generic.List<Byte> __encode()
        {
            return null;
        }
        public virtual Int32 __decode(ref byte[] binData, Int32 pos)
        {
            return 0;
        }
    }

    class i8 : ProtoData
    {
        private Char _val;
        public Char val
        {
            get { return _val; }
            set { _val = value; }
        }
        public override System.Collections.Generic.List<Byte> __encode()
        {
            return new System.Collections.Generic.List<Byte>(_val);
        }
        public override Int32 __decode(ref byte[] binData, Int32 pos)
        {
            _val = System.BitConverter.ToChar(binData, pos);
            return pos + 1;
        }
    }

    class ui8 : ProtoData
    {
        private Byte _val;
        public Byte val
        {
            get { return _val; }
            set { _val = value; }
        }
        public override System.Collections.Generic.List<Byte> __encode()
        {
            return new System.Collections.Generic.List<Byte>(_val);
        }
        public override Int32 __decode(ref byte[] binData, Int32 pos)
        {
            _val = (Byte)System.BitConverter.ToChar(binData, pos);
            return pos + 1;
        }
    }

    class i16 : ProtoData
    {
        private Int16 _val;
        public Int16 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public override System.Collections.Generic.List<Byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public override Int32 __decode(ref byte[] binData, Int32 pos)
        {
            if(System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToInt16(binData, pos);
                return pos + 2;
            }
            else
            {
                Byte[] reverBin = {binData[pos+1], binData[pos] };
                _val = System.BitConverter.ToInt16(reverBin, 0);
                return pos + 2;
            }
        }
    }



    class BinaryWrite
    {

        public System.Collections.Generic.List<Byte> write(Byte n)
        {
            return new System.Collections.Generic.List<Byte>(n);
        }
        public System.Collections.Generic.List<Byte> write(Char n)
        {
            return new System.Collections.Generic.List<Byte>(n);
        }
        public System.Collections.Generic.List<Byte> write(Int16 n)
        {
            byte[] bin = System.BitConverter.GetBytes(n);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Collections.Generic.List<Byte> write(UInt16 n)
        {
            byte[] bin = System.BitConverter.GetBytes(n);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Collections.Generic.List<Byte> write(Int32 n)
        {
            byte[] bin = System.BitConverter.GetBytes(n);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Collections.Generic.List<Byte> write(UInt32 n)
        {
            byte[] bin = System.BitConverter.GetBytes(n);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Collections.Generic.List<Byte> write(Int64 n)
        {
            byte[] bin = System.BitConverter.GetBytes(n);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Collections.Generic.List<Byte> write(UInt64 n)
        {
            byte[] bin = System.BitConverter.GetBytes(n);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Collections.Generic.List<Byte> write(string n)
        {
            var ret = write((UInt16)n.Length);
            for (int i = 0; i < n.Length; i++ )
            {
                ret.Add((Byte)n[i]);
            }
            return ret;
        }
        public System.Collections.Generic.List<Byte> writeData<T>( T t)
        {
            var ret = new System.Collections.Generic.List<Byte>();
            return ret;
        }

    }
    class Proto
    {
        
    }



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

    class HeroInfoDict : System.Collections.Generic.Dictionary<Int32, HeroInfo> { }
    class HeroInfoArray : System.Collections.Generic.List<HeroInfo> { }
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

            Int32 n = 32;

            byte[] bytes = System.BitConverter.GetBytes(n);
            
            System.Collections.Generic.List<Byte> binData = new  System.Collections.Generic.List<Byte>();
            binData.AddRange( new System.Collections.Generic.List<Byte>(bytes));

            for (int i = 0; i < binData.Count; i++)
            {
                System.Console.WriteLine((int)binData[i]);
            }
            
        }
    }
}
