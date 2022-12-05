#pragma once

#define MD5_F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | (~z)))
#define ROTLEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define MD5_FF(a, b, c, d, x, s, ac)                            \
    {                                                           \
        (a) += MD5_F((b), (c), (d)) + (x) + (unsigned int)(ac); \
        (a) = ROTLEFT((a), (s));                                \
        (a) += (b);                                             \
    }

#define MD5_GG(a, b, c, d, x, s, ac)                            \
    {                                                           \
        (a) += MD5_G((b), (c), (d)) + (x) + (unsigned int)(ac); \
        (a) = ROTLEFT((a), (s));                                \
        (a) += (b);                                             \
    }

#define MD5_HH(a, b, c, d, x, s, ac)                            \
    {                                                           \
        (a) += MD5_H((b), (c), (d)) + (x) + (unsigned int)(ac); \
        (a) = ROTLEFT((a), (s));                                \
        (a) += (b);                                             \
    }

#define MD5_II(a, b, c, d, x, s, ac)                            \
    {                                                           \
        (a) += MD5_I((b), (c), (d)) + (x) + (unsigned int)(ac); \
        (a) = ROTLEFT((a), (s));                                \
        (a) += (b);                                             \
    }

namespace AoC
{
    enum
    {
        _S11 = 7,
        _S12 = 12,
        _S13 = 17,
        _S14 = 22,
        _S21 = 5,
        _S22 = 9,
        _S23 = 14,
        _S24 = 20,
        _S31 = 4,
        _S32 = 11,
        _S33 = 16,
        _S34 = 23,
        _S41 = 6,
        _S42 = 10,
        _S43 = 15,
        _S44 = 21,
    };

    static const unsigned char PADDING[64] = {0x80};

    unsigned char NibbleToTChar(unsigned char val)
    {
        if (val > 9)
        {
            return 'A' + (val - 10);
        }
        return '0' + val;
    }
    std::string ByteToHex(unsigned char c)
    {
        std::string result;
        result += NibbleToTChar(c >> 4);
        result += NibbleToTChar(c & 15);
        return result;
    }

    class MD5
    {
    private:
        struct FContext
        {
            unsigned int state[4];
            unsigned int count[2];
            unsigned char buffer[64];
        };

    public:
        MD5()
        {
            Context.count[0] = Context.count[1] = 0;
            Context.state[0] = 0x67452301;
            Context.state[1] = 0xefcdab89;
            Context.state[2] = 0x98badcfe;
            Context.state[3] = 0x10325476;
        }

        void Update(const unsigned char *input, unsigned long long inputLen)
        {
            unsigned long long i, index, partLen;
            index = (int)((Context.count[0] >> 3) & 0x3F);
            unsigned long long inputBitCount = inputLen << 3;
            unsigned long long newBitCount = (static_cast<unsigned long long>(this->Context.count[0]) | (static_cast<unsigned long long>(Context.count[1]) << 32)) + inputBitCount;
            Context.count[0] = static_cast<unsigned int>(newBitCount);
            Context.count[1] = static_cast<unsigned int>(newBitCount >> 32);
            partLen = 64 - index;
            if (inputLen >= partLen)
            {
                std::memcpy(&Context.buffer[index], input, partLen);
                Transform(Context.state, Context.buffer);
                for (i = partLen; i + 63 < inputLen; i += 64)
                {
                    Transform(Context.state, &input[i]);
                }
                index = 0;
            }
            else
            {
                i = 0;
            }
            std::memcpy(&Context.buffer[index], &input[i], inputLen - i);
        }

        void Final(unsigned char *digest)
        {
            unsigned char bits[8];
            int index, padLen;
            Encode(bits, Context.count, 8);
            index = static_cast<int>((Context.count[0] >> 3) & 0x3f);
            padLen = (index < 56) ? (56 - index) : (120 - index);
            Update(PADDING, padLen);
            Update(bits, 8);
            Encode(digest, Context.state, 16);
            std::memset(&Context, 0, sizeof(Context));
        }

        static std::string HashBytes(const unsigned char *input, unsigned long long inputLen)
        {
            unsigned char Digest[16];

            MD5 Md5Gen;

            Md5Gen.Update(input, inputLen);
            Md5Gen.Final(Digest);

            std::string MD5;
            for (int i = 0; i < 16; i++)
            {
                MD5 += ByteToHex(Digest[i]);
            }
            return MD5;
        }

    private:
        void Transform(unsigned int *state, const unsigned char *block)
        {
            unsigned int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

            Decode(x, block, 64);

            // Round 1
            MD5_FF(a, b, c, d, x[0], _S11, 0xd76aa478);  /* 1 */
            MD5_FF(d, a, b, c, x[1], _S12, 0xe8c7b756);  /* 2 */
            MD5_FF(c, d, a, b, x[2], _S13, 0x242070db);  /* 3 */
            MD5_FF(b, c, d, a, x[3], _S14, 0xc1bdceee);  /* 4 */
            MD5_FF(a, b, c, d, x[4], _S11, 0xf57c0faf);  /* 5 */
            MD5_FF(d, a, b, c, x[5], _S12, 0x4787c62a);  /* 6 */
            MD5_FF(c, d, a, b, x[6], _S13, 0xa8304613);  /* 7 */
            MD5_FF(b, c, d, a, x[7], _S14, 0xfd469501);  /* 8 */
            MD5_FF(a, b, c, d, x[8], _S11, 0x698098d8);  /* 9 */
            MD5_FF(d, a, b, c, x[9], _S12, 0x8b44f7af);  /* 10 */
            MD5_FF(c, d, a, b, x[10], _S13, 0xffff5bb1); /* 11 */
            MD5_FF(b, c, d, a, x[11], _S14, 0x895cd7be); /* 12 */
            MD5_FF(a, b, c, d, x[12], _S11, 0x6b901122); /* 13 */
            MD5_FF(d, a, b, c, x[13], _S12, 0xfd987193); /* 14 */
            MD5_FF(c, d, a, b, x[14], _S13, 0xa679438e); /* 15 */
            MD5_FF(b, c, d, a, x[15], _S14, 0x49b40821); /* 16 */

            // Round 2
            MD5_GG(a, b, c, d, x[1], _S21, 0xf61e2562);  /* 17 */
            MD5_GG(d, a, b, c, x[6], _S22, 0xc040b340);  /* 18 */
            MD5_GG(c, d, a, b, x[11], _S23, 0x265e5a51); /* 19 */
            MD5_GG(b, c, d, a, x[0], _S24, 0xe9b6c7aa);  /* 20 */
            MD5_GG(a, b, c, d, x[5], _S21, 0xd62f105d);  /* 21 */
            MD5_GG(d, a, b, c, x[10], _S22, 0x2441453);  /* 22 */
            MD5_GG(c, d, a, b, x[15], _S23, 0xd8a1e681); /* 23 */
            MD5_GG(b, c, d, a, x[4], _S24, 0xe7d3fbc8);  /* 24 */
            MD5_GG(a, b, c, d, x[9], _S21, 0x21e1cde6);  /* 25 */
            MD5_GG(d, a, b, c, x[14], _S22, 0xc33707d6); /* 26 */
            MD5_GG(c, d, a, b, x[3], _S23, 0xf4d50d87);  /* 27 */
            MD5_GG(b, c, d, a, x[8], _S24, 0x455a14ed);  /* 28 */
            MD5_GG(a, b, c, d, x[13], _S21, 0xa9e3e905); /* 29 */
            MD5_GG(d, a, b, c, x[2], _S22, 0xfcefa3f8);  /* 30 */
            MD5_GG(c, d, a, b, x[7], _S23, 0x676f02d9);  /* 31 */
            MD5_GG(b, c, d, a, x[12], _S24, 0x8d2a4c8a); /* 32 */

            // Round 3
            MD5_HH(a, b, c, d, x[5], _S31, 0xfffa3942);  /* 33 */
            MD5_HH(d, a, b, c, x[8], _S32, 0x8771f681);  /* 34 */
            MD5_HH(c, d, a, b, x[11], _S33, 0x6d9d6122); /* 35 */
            MD5_HH(b, c, d, a, x[14], _S34, 0xfde5380c); /* 36 */
            MD5_HH(a, b, c, d, x[1], _S31, 0xa4beea44);  /* 37 */
            MD5_HH(d, a, b, c, x[4], _S32, 0x4bdecfa9);  /* 38 */
            MD5_HH(c, d, a, b, x[7], _S33, 0xf6bb4b60);  /* 39 */
            MD5_HH(b, c, d, a, x[10], _S34, 0xbebfbc70); /* 40 */
            MD5_HH(a, b, c, d, x[13], _S31, 0x289b7ec6); /* 41 */
            MD5_HH(d, a, b, c, x[0], _S32, 0xeaa127fa);  /* 42 */
            MD5_HH(c, d, a, b, x[3], _S33, 0xd4ef3085);  /* 43 */
            MD5_HH(b, c, d, a, x[6], _S34, 0x4881d05);   /* 44 */
            MD5_HH(a, b, c, d, x[9], _S31, 0xd9d4d039);  /* 45 */
            MD5_HH(d, a, b, c, x[12], _S32, 0xe6db99e5); /* 46 */
            MD5_HH(c, d, a, b, x[15], _S33, 0x1fa27cf8); /* 47 */
            MD5_HH(b, c, d, a, x[2], _S34, 0xc4ac5665);  /* 48 */

            // Round 4
            MD5_II(a, b, c, d, x[0], _S41, 0xf4292244);  /* 49 */
            MD5_II(d, a, b, c, x[7], _S42, 0x432aff97);  /* 50 */
            MD5_II(c, d, a, b, x[14], _S43, 0xab9423a7); /* 51 */
            MD5_II(b, c, d, a, x[5], _S44, 0xfc93a039);  /* 52 */
            MD5_II(a, b, c, d, x[12], _S41, 0x655b59c3); /* 53 */
            MD5_II(d, a, b, c, x[3], _S42, 0x8f0ccc92);  /* 54 */
            MD5_II(c, d, a, b, x[10], _S43, 0xffeff47d); /* 55 */
            MD5_II(b, c, d, a, x[1], _S44, 0x85845dd1);  /* 56 */
            MD5_II(a, b, c, d, x[8], _S41, 0x6fa87e4f);  /* 57 */
            MD5_II(d, a, b, c, x[15], _S42, 0xfe2ce6e0); /* 58 */
            MD5_II(c, d, a, b, x[6], _S43, 0xa3014314);  /* 59 */
            MD5_II(b, c, d, a, x[13], _S44, 0x4e0811a1); /* 60 */
            MD5_II(a, b, c, d, x[4], _S41, 0xf7537e82);  /* 61 */
            MD5_II(d, a, b, c, x[11], _S42, 0xbd3af235); /* 62 */
            MD5_II(c, d, a, b, x[2], _S43, 0x2ad7d2bb);  /* 63 */
            MD5_II(b, c, d, a, x[9], _S44, 0xeb86d391);  /* 64 */

            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;

            // Zeroize sensitive information.
            std::memset(x, 0, sizeof(x)); //-V597
        }

        void Encode(unsigned char *output, const unsigned int *input, int len)
        {
            int i, j;

            for (i = 0, j = 0; j < len; i++, j += 4)
            {
                output[j] = (unsigned char)(input[i] & 0xff);
                output[j + 1] = (unsigned char)((input[i] >> 8) & 0xff);
                output[j + 2] = (unsigned char)((input[i] >> 16) & 0xff);
                output[j + 3] = (unsigned char)((input[i] >> 24) & 0xff);
            }
        }

        void Decode(unsigned int *output, const unsigned char *input, int len)
        {
            int i, j;

            for (i = 0, j = 0; j < len; i++, j += 4)
            {
                output[i] = ((unsigned int)input[j]) | (((unsigned int)input[j + 1]) << 8) |
                            (((unsigned int)input[j + 2]) << 16) | (((unsigned int)input[j + 3]) << 24);
            }
        }

    private:
        FContext Context;
    };
}