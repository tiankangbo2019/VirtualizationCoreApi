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

#ifndef _LIBVBOX_VBOXSDK_VERSION_H
#define _LIBVBOX_VBOXSDK_VERSION_H

#define VBox_MAKE_VERSION(major, minor, patch) \
            ((major) << 16 | (minor) << 8 | (patch))

#define VirtualBoxSDK_VERSION VBox_MAKE_VERSION(6, 0, 8)

#define VBOX_MSCOM

#ifdef _WIN32
#   ifdef BUILDING_LIBVBOX
#       define LIBVBOX_API __declspec(dllexport)
#   else
#       define LIBVBOX_API __declspec(dllimport)
#   endif
#else
#   define LIBVBOX_API
#endif

#endif /* _LIBVBOX_VBOXSDK_VERSION_H */
