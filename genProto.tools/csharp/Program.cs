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
        public virtual Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            return 0;
        }
    }

    class i8 : ProtoData
    {
        private Char _val;
        public i8(Char v) {_val = v;}
        public Char val
        {
            get { return _val; }
            set { _val = value; }
        }
        public override System.Collections.Generic.List<Byte> __encode()
        {
            return new System.Collections.Generic.List<Byte>(_val);
        }
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            _val = System.BitConverter.ToChar(binData, pos);
            pos += 1;
            return pos;
        }
    }

    class ui8 : ProtoData
    {
        private Byte _val;
        public ui8(Byte v) {_val = v;}
        public Byte val
        {
            get { return _val; }
            set { _val = value; }
        }
        public override System.Collections.Generic.List<Byte> __encode()
        {
            return new System.Collections.Generic.List<Byte>(_val);
        }
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            _val = (Byte)System.BitConverter.ToChar(binData, pos);
            pos += 1;
            return pos;
        }
    }

    class i16 : ProtoData
    {
        private Int16 _val;
        public i16(Int16 v) {_val = v;}
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
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            if(System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToInt16(binData, pos);
            }
            else
            {
                Byte[] reverBin = {binData[pos+1], binData[pos] };
                _val = System.BitConverter.ToInt16(reverBin, 0);
            }
            pos += 2;
            return pos;
        }
    }

    class ui16 : ProtoData
    {
        private UInt16 _val;
        public ui16(UInt16 v) {_val = v;}
        public UInt16 val
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
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToUInt16(binData, pos);
            }
            else
            {
                Byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToUInt16(reverBin, 0);
            }
            pos += 2;
            return pos;
        }
    }

    class i32 : ProtoData
    {
        private Int32 _val;
        public i32(Int32 v) {_val = v;}
        public Int32 val
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
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToInt32(binData, pos);
            }
            else
            {
                Byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToInt32(reverBin, 0);
            }
            pos += 4;
            return pos;
        }
    }

    class ui32 : ProtoData
    {
        private UInt32 _val;
        public ui32(UInt32 v) {_val = v;}
        public UInt32 val
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
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToUInt32(binData, pos);
            }
            else
            {
                Byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToUInt32(reverBin, 0);
            }
            pos += 4;
            return pos;
        }
    }

    class i64 : ProtoData
    {
        private Int64 _val;
        public i64(Int64 v) {_val = v;}
        public Int64 val
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
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToInt64(binData, pos);
            }
            else
            {
                Byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToInt64(reverBin, 0);
            }
            pos += 8;
            return pos;
        }
    }

    class ui64 : ProtoData
    {
        private UInt64 _val;
        public ui64(UInt64 v) {_val = v;}
        public UInt64 val
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
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToUInt64(binData, pos);
            }
            else
            {
                Byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToUInt64(reverBin, 0);
            }
            pos += 8;
            return pos;
        }
    }

    class Float : ProtoData
    {
        private float _val;
        public Float(float v) {_val = v;}
        public float val
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
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToSingle(binData, pos);
            }
            else
            {
                Byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToSingle(reverBin, 0);
            }
            pos += 4;
            return pos;
        }
    }

    class Double : ProtoData
    {
        private double _val;
        public Double(double v) {_val = v;}

        public double val
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
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToDouble(binData, pos);
            }
            else
            {
                Byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToDouble(reverBin, 0);
            }
            pos += 8;
            return pos;
        }

    class String : ProtoData
    {
        private string _val;
        public String(string v) {_val = v;}
        public string val
        {
            get { return _val; }
            set { _val = value; }
        }
        public override System.Collections.Generic.List<Byte> __encode()
        {
            var ret = new System.Collections.Generic.List<Byte>();
            ret.AddRange(new ui16((UInt16)_val.Length).__encode());
            foreach(Char ch in _val)
            {
                ret.Add((Byte)ch);
            }
            return ret;
        }
        public override Int32 __decode(ref byte[] binData, ref Int32 pos)
        {
            _val.Remove(0);
            ui16 len = new ui16(0);
            len.__decode(ref binData, ref pos);
            for (int i=0; i<len.val; i++)
            {
                _val += (Char) binData[i];
            }
            pos +=2;
            pos += _val.Length;
            return pos;
        }
    }


    class STATIC_EC_SUCCESS //global const 
    {
        public const ui16 value = new ui16(100);
    }
    class STATIC_EC_ERROR //global const 
    {
        public const ui16 value = new ui16(100);
    }
    class HeroInfo : ProtoData//struct
    {
        public i32 id;
        public String name;
        public virtual System.Collections.Generic.List<Byte> __encode()
        {
            var ret = new System.Collections.Generic.List<Byte>();
            ret.AddRange(id.__encode());
            ret.AddRange(name.__encode());
            return ret;
        }
        public virtual Int32 __decode(ref byte[] binData, Int32 pos)
        {
            id.__decode(ref binData, ref pos);
            name.__decode(ref binData, ref pos);
            return pos;
        }
    }

    class HeroInfoDict : System.Collections.Generic.Dictionary<i32, HeroInfo> { }
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
