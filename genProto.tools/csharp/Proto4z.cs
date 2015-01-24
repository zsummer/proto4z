using System;


namespace Proto4z
{
    class EncryRC4 //rc4 encrypt
    {
        
    }
    class Serialize
    {

    }


    interface IProtoObject
    {
        System.Collections.Generic.List<Byte> __encode();
        Int32 __decode(byte[] binData, ref Int32 pos);
    }

    class i8 : IProtoObject
    {
        private Char _val;
        public i8(Char v) {_val = v;}
        public Char val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            return new System.Collections.Generic.List<Byte>(_val);
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
        {
            _val = System.BitConverter.ToChar(binData, pos);
            pos += 1;
            return pos;
        }
    }

    class ui8 : IProtoObject
    {
        private Byte _val;
        public ui8(Byte v) {_val = v;}
        public Byte val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            return new System.Collections.Generic.List<Byte>(_val);
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
        {
            _val = (Byte)System.BitConverter.ToChar(binData, pos);
            pos += 1;
            return pos;
        }
    }

    class i16 : IProtoObject
    {
        private Int16 _val;
        public i16(Int16 v) {_val = v;}
        public Int16 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
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

    class ui16 : IProtoObject
    {
        private UInt16 _val;
        public ui16(UInt16 v) {_val = v;}
        public UInt16 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
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

    class i32 : IProtoObject
    {
        private Int32 _val;
        public i32(Int32 v) {_val = v;}
        public Int32 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
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

    class ui32 : IProtoObject
    {
        private UInt32 _val;
        public ui32(UInt32 v) {_val = v;}
        public UInt32 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
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

    class i64 : IProtoObject
    {
        private Int64 _val;
        public i64(Int64 v) {_val = v;}
        public Int64 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
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

    class ui64 : IProtoObject
    {
        private UInt64 _val;
        public ui64(UInt64 v) {_val = v;}
        public UInt64 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
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

    class Float : IProtoObject
    {
        private float _val;
        public Float(float v) {_val = v;}
        public float val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
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

    class Double : IProtoObject
    {
        private double _val;
        public Double(double v) { _val = v; }

        public double val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<Byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
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
    }

    class String : IProtoObject
    {
        private string _val;
        public String(string v) { _val = v; }
        public string val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<Byte> __encode()
        {
            var ret = new System.Collections.Generic.List<Byte>();
            ret.AddRange(new ui16((UInt16)_val.Length).__encode());
            foreach (Char ch in _val)
            {
                ret.Add((Byte)ch);
            }
            return ret;
        }
        public Int32 __decode(byte[] binData, ref Int32 pos)
        {
            _val = "";
            ui16 len = new ui16(0);
            len.__decode(binData, ref pos);
            for (int i = pos; i < pos + len.val; i++)
            {
                _val += (Char)binData[i];
            }
            pos += _val.Length;
            return pos;
        }
    }
}

