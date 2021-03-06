#include <ShadowRandom.hpp>
#include <random>

uint64_t lrotate(uint64_t t,size_t s){
    s &=0x3f;
    return t<<s|(t>>(64-s));
}
uint64_t rrotate(uint64_t t,size_t s){
    s &=0x3f;
    return t>>s|(t<<(64-s));
}



const uint64_t ctable[] ={
    0x6a09e667f3bcc908,0xbb67ae8584caa73b,
    0x3c6ef372fe94f82b,0xa54ff53a5f1d36f1,
    0x510e527fade682d1,0x9b05688c2b3e6c1f,
    0x1f83d9abfb41bd6b,0x5be0cd19137e2179,
    0xB7E151628AED2A6B,0x243F6A8885A308D3,
    0x9E3779B97F4A7C15,0x93C467E37DB0C7A4
};

size_t target = 0xB7E15162;

const size_t rotations[] = {
     2,  3,  5,  7, 
    11, 13, 17, 19, 
    23, 29, 31, 37, 
    41, 47, 53, 57,
};

uint64_t ShadowRandom::getConstant(size_t s){
    size_t idx = s%12;
    size_t modifier = (2uLL<<((s/12)&0x3f))-1;
    return lrotate(ctable[idx]*modifier,13);
}

uint64_t ShadowRandom::transform(uint64_t u){
    return lrotate(u,rotations[u&0x0f])^
        getConstant(rrotate(u,rotations[(u+1)&0x0f])&target);
}

uint8_t sbox[256] = 
{
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

uint8_t toByte(uint64_t t){
    uint8_t b = 0;
    for(size_t s=0;s<16;s++)
        b ^= lrotate(t,rotations[s])&0xff;
    
    return sbox[b];
}

void ShadowRandom::initSeed(const unsigned char* buff,size_t s){
    //Lock state
    std::lock_guard<recursive_mutex> sync(lock);
    //Start with 16 fixed values, the first 16 constants
    uint64_t expanded[] = 
    {
        0x6a09e667f3bcc908,0xbb67ae8584caa73b,
        0x3c6ef372fe94f82b,0xa54ff53a5f1d36f1,
        0x510e527fade682d1,0x9b05688c2b3e6c1f,
        0x1f83d9abfb41bd6b,0x5be0cd19137e2179,
        0xB7E151628AED2A6B,0x243F6A8885A308D3,
        0x9E3779B97F4A7C15,0x93C467E37DB0C7A4,
        getConstant(12),getConstant(13),
        getConstant(14),getConstant(15)
    };
    //Construct the fixed array
    uint8_t array[] = 
    {
        0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
    };
    Random::setSeed(1);//Reset the haveNextNextGuassian Flag
    //expand input to main array;
    for(size_t i=0;i<s;i++){
        array[i&0xff] ^= sbox[buff[i]];
        array[i&0xff] ^= sbox[array[i&0xff]];
    }
    //Substitute all values in the array
    for(size_t i=0;i<255;i++)
        array[i] = sbox[array[i]];
    //preform main loop
    for(size_t t=0;t<64;t++){
        for(size_t i=0;i<16;i++){
            //Add each value in the array to the expanded array.
            for(size_t j=0;j<256;j++)
                expanded[i] += array[j]^getConstant((t*4096)+(i*256)+j);
            expanded[i] ^= expanded[(i-1)&0x3f];
            expanded[i] = transform(expanded[i]);
        }
        //Subsitute each element in the array
        for(size_t j=0;j<256;j++)
            array[j] = sbox[array[j]];
        //xor array[2],array[3],array[5], and array[7] with the output of the sbox at it.
        array[2] ^=sbox[array[2]];
        array[3] ^=sbox[array[3]];
        array[5] ^=sbox[array[5]];
        array[7] ^=sbox[array[7]];
    }
    //Preform final loop
    for(size_t i=0;i<16;i++)
        state[i] = rrotate(expanded[i],3*i+1)^getConstant(s)^transform(array[i]);
}

void ShadowRandom::seed(){
    std::random_device dev;
    uint32_t vals[16];
    uint64_t numbers[16];
    for(size_t i = 0;i<16;i++)
        vals[i] = dev();
    
    for(size_t j =0;j<16;j++)
        numbers[j] = (vals[(j-1)&0x0f]+transform(vals[j]))^ getConstant(j);
    initSeed((unsigned char*)numbers,sizeof(numbers));
}

void ShadowRandom::setSeed(uint64_t s){
    uint64_t numbers[32];
    for(size_t j=0;j<32;j++)
        numbers[j] = s^transform(s^getConstant(j));
    initSeed((unsigned char*)numbers,sizeof(numbers));
}

uint32_t ShadowRandom::next(int bits){
    uint64_t s;
    this->nextBytes((uint8_t*)&s,8);
    return (uint32_t)(s>>(64-bits));
}

void ShadowRandom::nextBytes(uint8_t* out,size_t bytes){
    uint64_t block[64];
    for(size_t s=0;s<32;s++)
        block[s] = transform(state[s]);
    for(size_t s=32;s<64;s++){
        uint64_t b0 = getConstant(s)+transform(bytes^getConstant(s));
        uint64_t b1 = transform(b0)^block[s-8]^block[s-16];
        block[s] = b0+b1+block[s-32];
    }
    for(size_t j=0;j<bytes;j++)
        out[j] = sbox[j&0xff];
    for(size_t s=0;s<32;s++)
        state[s] = block[s]+transform(block[s+32]);
    for(size_t s=0;s<64;s++){
        for(size_t j=0;j<bytes;j++){
            out[j] ^=  toByte(block[s]^getConstant(j));
            block[s] ^= transform(out[j]);
        }
    }
}

ShadowRandom::ShadowRandom(){
    this->seed();
}

ShadowRandom::ShadowRandom(uint64_t seed){
    this->setSeed(seed);
}