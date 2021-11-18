/*
 *  Copyright (C) 2020 Embedded AMS B.V. - All Rights Reserved
 *
 *  This file is part of Embedded Proto.
 *
 *  Embedded Proto is open source software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation, version 3 of the license.
 *
 *  Embedded Proto  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Embedded Proto. If not, see <https://www.gnu.org/licenses/>.
 *
 *  For commercial and closed source application please visit:
 *  <https://EmbeddedProto.com/license/>.
 *
 *  Embedded AMS B.V.
 *  Info:
 *    info at EmbeddedProto dot com
 *
 *  Postal adress:
 *    Johan Huizingalaan 763a
 *    1066 VH, Amsterdam
 *    the Netherlands
 */

#include "CompressorWriteBuffer.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "cmsis_os.h"

void CompressorWriteBuffer::clear()
{
    inIDX = 0;
    outIDX = 0;
}

uint32_t CompressorWriteBuffer::get_size() const
{
    return inIDX;
}

uint32_t CompressorWriteBuffer::get_max_size() const
{
    return UINT32_MAX;
}

uint32_t CompressorWriteBuffer::get_available_size() const
{
    return UINT32_MAX;
}

bool CompressorWriteBuffer::push(const uint8_t byte)
{
    if (inIDX < inbuff.size())
    {
        inbuff[inIDX++] = byte;
    }

    if (inIDX >= CHUNK_SIZE)
    {
        compressBlock();
    }

    return true;
}


bool CompressorWriteBuffer::push(const uint8_t *bytes, const uint32_t length)
{
    for (uint32_t remLen = length; remLen > 0;)
    {
        auto start = bytes + (length - remLen);
        uint32_t btc = 512 - inIDX;
        btc = std::min(btc, remLen);
        std::copy_n(start, btc, inbuff.data() + inIDX);
        inIDX += btc;
        if (inIDX >= CHUNK_SIZE)
        {
            compressBlock();
        }
        remLen -= btc;
    }

    return true;
}

static void *zcalloc(void *opaque, uint16_t items, uint16_t size)
{
    (void) opaque;
    void *ptr = pvPortMalloc(items * size);
    return memset(ptr, 0, items * size);
}

static void zcfree(void *opaque, void *ptr)
{
    (void) opaque;
    vPortFree(ptr);
}


CompressorWriteBuffer::CompressorWriteBuffer(FIL *output) :
        fout(output), strm{}, header{}
{
    header.name = (Bytef *) "hourly.pb";
    header.text = false;
    header.time = 0; // Don't know if I care about time or not
    header.xflags = 0;
    header.os = 0xFF;
    header.extra = nullptr;
    header.extra_len = 0;
    header.extra_max = 0;
    header.name_max = 10;
    header.comment = nullptr;
    header.hcrc = true;
    header.done = false;


    strm.next_in = nullptr;
    strm.avail_in = 0;
    strm.total_in = 0;
    strm.next_out = nullptr;
    strm.avail_out = 0;
    strm.total_out = 0;
    strm.msg = nullptr;
    strm.state = nullptr;
    strm.data_type = Z_BINARY;
    strm.adler = 0;
    strm.reserved = 0;


    /* allocate deflate state */
    strm.zalloc = zcalloc;
    strm.zfree = zcfree;
    strm.opaque = Z_NULL;
    deflateInit(&strm, Z_DEFAULT_COMPRESSION);

    deflateSetHeader(&strm, &header);
}

void CompressorWriteBuffer::finish()
{
    compressBlock(true);
}

void CompressorWriteBuffer::compressBlock(bool last)
{
    int flush;
    unsigned have;

    strm.avail_in = inIDX;
    strm.next_in = inbuff.data();
    flush = last ? Z_FINISH : Z_NO_FLUSH;
    uint8_t out[512];

    do
    {
        strm.avail_out = 512;
        strm.next_out = out;
        deflate(&strm, flush);    /* no bad return value */
        have = CHUNK_SIZE - strm.avail_out;


        for (uint32_t remLen = have; remLen > 0;)
        {
            auto start = out + (have - remLen);
            uint32_t btc = 512 - outIDX;
            btc = std::min(btc, remLen);
            std::copy_n(start, btc, outbuff.data() + outIDX);
            outIDX += btc;
            if (outIDX >= 512)
            {
            	f_write(fout, outbuff.data(), 512, nullptr);
                outIDX -= 512;
            }
            remLen -= btc;
        }

    } while (strm.avail_out == 0);

    inIDX = 0;

    if (last)
    {
        (void) deflateEnd(&strm);
        if(outIDX > 0)
        {
        	f_write(fout, outbuff.data(), outIDX, nullptr);
            outIDX = 0;
        }
        f_sync(fout);
    }
}
