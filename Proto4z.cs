
/*
* proto4z License
* -----------
*
* proto4z is licensed under the terms of the MIT license reproduced below.
* This means that proto4z is free software and can be used for both academic
* and commercial purposes at absolutely no cost.
*
*
* ===============================================================================
*
* Copyright (C) 2013 YaweiZhang <yawei_zhang@foxmail.com>.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* ===============================================================================
*
* (end of COPYRIGHT)
*/



namespace Proto4z
{
    class RC4Encryption //rc4 encrypt
    {
        private int m_x;
	    private int m_y;
        private int[] m_box;
	    public void makeSBox(string obscure)
	    {
            m_box = new int[256];
		    m_x = 0;
		    m_y = 0;
		    for (int i = 0; i < 256; i++)
		    {
			    m_box[i] = i;
		    }
		    if (obscure.Length > 0)
		    {
			    int j = 0;
			    int k = 0;
                char[] obs = obscure.ToCharArray();
			    int lenth = obs.Length;
			    for (int i = 0; i < 256; i++)
			    {
				    int tmp = m_box[i];
				    j = (byte)(j + tmp + (byte)obs[k]);
				    m_box[i] = m_box[j];
				    m_box[j] = tmp;
				    if (++k >= lenth)
				    {
					    k = 0;
				    }
			    }
		    }
	    }

	    public void encryption(byte[] data, int length)
	    {
		    int  x, y;

		    x = m_x;
		    y = m_y;

		    for (int i = 0; i < length; i++)
		    {
			    x = (byte)(x + 1);
			    int a = m_box[x];
			    y = (byte)(y + a);
			    int b = m_box[x] = m_box[y];
			    m_box[y] = a;
                data[i] ^= (byte)m_box[(byte)(a + b)];
		    }

		    m_x = x;
		    m_y = y;
	    }



	   
    }

    interface IProtoObject
    {
        System.Collections.Generic.List<byte> __encode();
        System.Int32 __decode(byte[] binData, ref System.Int32 pos);
    }

    class i8 : IProtoObject, System.Collections.Generic.IEqualityComparer<i8>
    {
        private char _val;
        public i8() { _val = '\0'; }
        public i8(char v) { _val = v; }
        public char val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            return new System.Collections.Generic.List<byte>(_val);
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            _val = System.BitConverter.ToChar(binData, pos);
            pos += 1;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            i8 child = obj as i8;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(i8 x, i8 y)
        {
            return x == y;
        }

        public int GetHashCode(i8 obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class ui8 : IProtoObject, System.Collections.Generic.IEqualityComparer<ui8>
    {
        private byte _val;
        public ui8() { _val = 0; }

        public ui8(byte v) { _val = v; }
        public byte val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            return new System.Collections.Generic.List<byte>(_val);
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            _val = (byte)System.BitConverter.ToChar(binData, pos);
            pos += 1;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            ui8 child = obj as ui8;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(ui8 x, ui8 y)
        {
            return x == y;
        }

        public int GetHashCode(ui8 obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class i16 : IProtoObject, System.Collections.Generic.IEqualityComparer<i16>
    {
        private System.Int16 _val;
        public i16() { _val = 0; }

        public i16(System.Int16 v) { _val = v; }
        public System.Int16 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            if(System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToInt16(binData, pos);
            }
            else
            {
                byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToInt16(reverBin, 0);
            }
            pos += 2;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            i16 child = obj as i16;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(i16 x, i16 y)
        {
            return x == y;
        }

        public int GetHashCode(i16 obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class ui16 : IProtoObject, System.Collections.Generic.IEqualityComparer<ui16>
    {
        private System.UInt16 _val;
        public ui16() { _val = 0; }
        public ui16(System.UInt16 v) { _val = v; }
        public System.UInt16 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToUInt16(binData, pos);
            }
            else
            {
                byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToUInt16(reverBin, 0);
            }
            pos += 2;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            ui16 child = obj as ui16;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(ui16 x, ui16 y)
        {
            return x == y;
        }

        public int GetHashCode(ui16 obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class i32 : IProtoObject, System.Collections.Generic.IEqualityComparer<i32>
    {
        private System.Int32 _val;
        public i32() { _val = 0; }
        public i32(System.Int32 v) { _val = v; }
        public System.Int32 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToInt32(binData, pos);
            }
            else
            {
                byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToInt32(reverBin, 0);
            }
            pos += 4;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            i32 child = obj as i32;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(i32 x, i32 y)
        {
            return x == y;
        }

        public int GetHashCode(i32 obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class ui32 : IProtoObject, System.Collections.Generic.IEqualityComparer<ui32>
    {
        private System.UInt32 _val;
        public ui32() { _val = 0; }
        public ui32(System.UInt32 v) { _val = v; }
        public System.UInt32 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToUInt32(binData, pos);
            }
            else
            {
                byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToUInt32(reverBin, 0);
            }
            pos += 4;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            ui32 child = obj as ui32;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(ui32 x, ui32 y)
        {
            return x == y;
        }

        public int GetHashCode(ui32 obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class i64 : IProtoObject, System.Collections.Generic.IEqualityComparer<i64>
    {
        private System.Int64 _val;
        public i64() { _val = 0; }
        public i64(System.Int64 v) { _val = v; }
        public System.Int64 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToInt64(binData, pos);
            }
            else
            {
                byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToInt64(reverBin, 0);
            }
            pos += 8;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            i64 child = obj as i64;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(i64 x, i64 y)
        {
            return x == y;
        }

        public int GetHashCode(i64 obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class ui64 : IProtoObject, System.Collections.Generic.IEqualityComparer<ui64>
    {
        private System.UInt64 _val;
        public ui64() { _val = 0; }
        public ui64(System.UInt64 v) { _val = v; }
        public System.UInt64 val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToUInt64(binData, pos);
            }
            else
            {
                byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToUInt64(reverBin, 0);
            }
            pos += 8;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            ui64 child = obj as ui64;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(ui64 x, ui64 y)
        {
            return x == y;
        }

        public int GetHashCode(ui64 obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class Float : IProtoObject, System.Collections.Generic.IEqualityComparer<Float>
    {
        private float _val;
        public Float() { _val = 0; }
        public Float(float v) {_val = v;}
        public float val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToSingle(binData, pos);
            }
            else
            {
                byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToSingle(reverBin, 0);
            }
            pos += 4;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            Float child = obj as Float;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(Float x, Float y)
        {
            return x == y;
        }

        public int GetHashCode(Float obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class Double : IProtoObject, System.Collections.Generic.IEqualityComparer<Double>
    {
        private double _val;
        public Double() { _val = 0; }
        public Double(double v) { _val = v; }

        public double val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            byte[] bin = System.BitConverter.GetBytes(_val);
            var ret = new System.Collections.Generic.List<byte>(bin);
            if (!System.BitConverter.IsLittleEndian)
                ret.Reverse();
            return ret;
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            if (System.BitConverter.IsLittleEndian)
            {
                _val = System.BitConverter.ToDouble(binData, pos);
            }
            else
            {
                byte[] reverBin = { binData[pos + 1], binData[pos] };
                _val = System.BitConverter.ToDouble(reverBin, 0);
            }
            pos += 8;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            Double child = obj as Double;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(Double x, Double y)
        {
            return x == y;
        }

        public int GetHashCode(Double obj)
        {
            return obj.val.GetHashCode();
        }
    }

    class String : IProtoObject, System.Collections.Generic.IEqualityComparer<Proto4z.String>
    {
        private string _val;
        public String() { _val = ""; }
        public String(string v) { _val = v; }
        public string val
        {
            get { return _val; }
            set { _val = value; }
        }
        public System.Collections.Generic.List<byte> __encode()
        {
            var ret = new System.Collections.Generic.List<byte>();
            ret.AddRange(new ui16((System.UInt16)_val.Length).__encode());
            foreach (char ch in _val)
            {
                ret.Add((byte)ch);
            }
            return ret;
        }
        public System.Int32 __decode(byte[] binData, ref System.Int32 pos)
        {
            _val = "";
            ui16 len = new ui16(0);
            len.__decode(binData, ref pos);
            for (int i = pos; i < pos + len.val; i++)
            {
                _val += (char)binData[i];
            }
            pos += _val.Length;
            return pos;
        }
        public override bool Equals(System.Object obj)
        {
            if (obj == null) return false;
            String child = obj as Proto4z.String;
            if (child == null) return false;
            return child.val == val;
        }
        public override int GetHashCode()
        {
            return val.GetHashCode();
        }

        public bool Equals(String x, String y)
        {
            return x == y;
        }

        public int GetHashCode(String obj)
        {
            return obj.val.GetHashCode();
        }
    }
}

