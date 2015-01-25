
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

    class i8 : IProtoObject
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
    }

    class ui8 : IProtoObject
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
    }

    class i16 : IProtoObject
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
    }

    class ui16 : IProtoObject
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
    }

    class i32 : IProtoObject
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
    }

    class ui32 : IProtoObject
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
    }

    class i64 : IProtoObject
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
    }

    class ui64 : IProtoObject
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
    }

    class Float : IProtoObject
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
    }

    class Double : IProtoObject
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
    }

    class String : IProtoObject
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
    }
}

