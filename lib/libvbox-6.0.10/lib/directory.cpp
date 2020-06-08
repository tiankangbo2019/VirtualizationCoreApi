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

COM_WRAP_IFC(IDirectory)
COM_WRAP_IFC(IGuestDirectory)

std::u16string VBox::IDirectory::directoryName() const
{
    std::u16string result;
    COM_GetString(get_IFC(), DirectoryName, result);
    return result;
}

std::u16string VBox::IDirectory::filter() const
{
    std::u16string result;
    COM_GetString(get_IFC(), Filter, result);
    return result;
}

void VBox::IDirectory::close()
{
    auto rc = get_IFC()->Close();
    COM_ERROR_CHECK(rc);
}

VBox::COMPtr<VBox::IFsObjInfo> VBox::IDirectory::read()
{
    ::IFsObjInfo *cResult = nullptr;

    auto rc = get_IFC()->Read(&cResult);
    COM_ERROR_CHECK(rc);

    return COMPtr<IFsObjInfo>::wrap(cResult);
}
