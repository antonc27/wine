/*
 * Copyright 2000 Lionel Ulmer
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __WINE_DLLS_DINPUT_DINPUT_PRIVATE_H
#define __WINE_DLLS_DINPUT_DINPUT_PRIVATE_H

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "dinput.h"
#include "dinputd.h"
#include "wine/list.h"

extern HINSTANCE DINPUT_instance;

/* Implementation specification */
typedef struct IDirectInputImpl IDirectInputImpl;
struct IDirectInputImpl
{
    IDirectInput7A              IDirectInput7A_iface;
    IDirectInput7W              IDirectInput7W_iface;
    IDirectInput8A              IDirectInput8A_iface;
    IDirectInput8W              IDirectInput8W_iface;
    IDirectInputJoyConfig8      IDirectInputJoyConfig8_iface;

    LONG                        ref;

    BOOL                        initialized;
    struct list                 entry;          /* entry into list of all IDirectInputs */

    DWORD                       evsequence;     /* unique sequence number for events */
    DWORD                       dwVersion;      /* direct input version number */
    struct list                 device_players; /* device instance guid to player name */
};

extern const IDirectInput7AVtbl dinput7_a_vtbl DECLSPEC_HIDDEN;
extern const IDirectInput8AVtbl dinput8_a_vtbl DECLSPEC_HIDDEN;

/* Function called by all devices that Wine supports */
struct dinput_device {
    const char *name;
    HRESULT (*enum_device)(DWORD dwDevType, DWORD dwFlags, LPDIDEVICEINSTANCEW lpddi, DWORD version, int id);
    HRESULT (*create_device)(IDirectInputImpl *dinput, REFGUID rguid, IDirectInputDevice8W **out);
};

struct DevicePlayer {
    GUID instance_guid;
    WCHAR username[MAX_PATH];
    struct list entry;
};

extern const struct dinput_device mouse_device DECLSPEC_HIDDEN;
extern const struct dinput_device keyboard_device DECLSPEC_HIDDEN;
extern const struct dinput_device joystick_hid_device DECLSPEC_HIDDEN;

extern void dinput_hooks_acquire_device(LPDIRECTINPUTDEVICE8W iface);
extern void dinput_hooks_unacquire_device(LPDIRECTINPUTDEVICE8W iface);
extern int dinput_mouse_hook( IDirectInputDevice8W *iface, WPARAM wparam, LPARAM lparam );
extern int dinput_keyboard_hook( IDirectInputDevice8W *iface, WPARAM wparam, LPARAM lparam );
extern void dinput_mouse_rawinput_hook( IDirectInputDevice8W *iface, WPARAM wparam, LPARAM lparam,
                                        RAWINPUT *raw );

extern void check_dinput_hooks(LPDIRECTINPUTDEVICE8W, BOOL) DECLSPEC_HIDDEN;
extern void check_dinput_events(void) DECLSPEC_HIDDEN;

extern HRESULT _configure_devices(IDirectInput8W *iface, LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSW lpdiCDParams, DWORD dwFlags, LPVOID pvRefData) DECLSPEC_HIDDEN;

extern WCHAR* get_mapping_path(const WCHAR *device, const WCHAR *username) DECLSPEC_HIDDEN;
extern DWORD get_device_type(DWORD version, BOOL is_joystick) DECLSPEC_HIDDEN;

#define IS_DIPROP(x)    (((ULONG_PTR)(x) >> 16) == 0)

#define DIKEYBOARD_MASK    0x81000000
#define DIMOUSE_MASK       0x82000000
#define DIGENRE_ANY        0xFF000000

#endif /* __WINE_DLLS_DINPUT_DINPUT_PRIVATE_H */
