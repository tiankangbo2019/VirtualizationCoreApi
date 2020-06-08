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

COM_WRAP_IFC(IVirtualBoxClient)

VBox::COMPtr<VBox::IVirtualBox> VBox::IVirtualBoxClient::virtualBox() const
{
    COMPtr<IVirtualBox> result;
    COM_GetValue_Wrap(get_IFC(), VirtualBox, result);
    return result;
}

VBox::COMPtr<VBox::ISession> VBox::IVirtualBoxClient::session() const
{
    COMPtr<ISession> result;
    COM_GetValue_Wrap(get_IFC(), Session, result);
    return result;
}

VBox::COMPtr<VBox::IEventSource> VBox::IVirtualBoxClient::eventSource() const
{
    COMPtr<IEventSource> result;
    COM_GetValue_Wrap(get_IFC(), EventSource, result);
    return result;
}

void VBox::IVirtualBoxClient::checkMachineError(const COMPtr<IMachine> &machine)
{
    auto rc = get_IFC()->CheckMachineError(machine->get_IFC());
    COM_ERROR_CHECK(rc);
}
