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

#ifndef INC_COMPWRITEBUFFER_H_
#define INC_COMPWRITEBUFFER_H_

#include <cstdint>
#include "WriteBufferInterface.h"
#include <array>
#include "../zlib/zlib.h"
#include "fatfs.h"

class CompressorWriteBuffer : public ::EmbeddedProto::WriteBufferInterface
{
    //! Store a maximum of MAX_SIZE bytes in the buffer

  public:
    CompressorWriteBuffer(FIL *output);
    ~CompressorWriteBuffer() override = default;

    //! \see ::EmbeddedProto::WriteBufferInterface::clear()
    virtual void clear() override;

    //! \see ::EmbeddedProto::WriteBufferInterface::get_size()
    virtual uint32_t get_size() const override;

    //! \see ::EmbeddedProto::WriteBufferInterface::get_max_size()
    virtual uint32_t get_max_size() const override;

    //! \see ::EmbeddedProto::WriteBufferInterface::get_available_size()
    virtual uint32_t get_available_size() const override;

    //! \see ::EmbeddedProto::WriteBufferInterface::push()
    virtual bool push(const uint8_t byte) override;

    //! \see ::EmbeddedProto::WriteBufferInterface::push()
    virtual bool push(const uint8_t* bytes, const uint32_t length) override;

    void finish();

  private:

    void compressBlock(bool last=false);

    const int CHUNK_SIZE = 512;
    std::array<uint8_t, 512> inbuff;
    std::array<uint8_t, 512> outbuff;
    uint16_t inIDX = 0, outIDX = 0;
    FIL * fout;
    z_stream strm;
    gz_header header;
};


#endif /* INC_COMPWRITEBUFFER_H_ */
