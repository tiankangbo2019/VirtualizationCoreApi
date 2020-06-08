/* This file is part of libvbox
 * Copyright (C) 2019  Michael Hansen
 *
 * libvbox is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libvbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libvbox; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "libvbox_p.h"

COM_WRAP_IFC(IGuestFileIOEvent)
COM_WRAP_IFC(IGuestFileOffsetChangedEvent)
COM_WRAP_IFC(IGuestFileReadEvent)
COM_WRAP_IFC(IGuestFileWriteEvent)

int64_t VBox::IGuestFileIOEvent::offset() const
{
    COM_Long64 result;
    COM_GetValue(get_IFC(), Offset, result);
    return static_cast<int64_t>(result);
}

uint32_t VBox::IGuestFileIOEvent::processed() const
{
    COM_ULong result;
    COM_GetValue(get_IFC(), Processed, result);
    return static_cast<uint32_t>(result);
}

std::vector<uint8_t> VBox::IGuestFileReadEvent::data() const
{
    std::vector<uint8_t> result;
    COM_GetArray(get_IFC(), Data, COM_Byte, result);
    return result;
}
