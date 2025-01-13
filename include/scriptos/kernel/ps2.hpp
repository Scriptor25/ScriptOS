#pragma once

#include <scriptos/kernel/interrupts.hpp>
#include <scriptos/std/types.hpp>

#define PS2_IO_DATA 0x60
#define PS2_IO_STATUS 0x64
#define PS2_IO_COMMAND 0x64

#define PS2_KEYBOARD_RESPONSE_KEY_DETECTION_ERROR1 0x00
#define PS2_KEYBOARD_RESPONSE_SELF_TEST_PASSED 0xaa
#define PS2_KEYBOARD_RESPONSE_ECHO 0xee
#define PS2_KEYBOARD_RESPONSE_ACK 0xfa
#define PS2_KEYBOARD_RESPONSE_SELF_TEST_FAILED1 0xfc
#define PS2_KEYBOARD_RESPONSE_SELF_TEST_FAILED2 0xfd
#define PS2_KEYBOARD_RESPONSE_RESEND 0xfe
#define PS2_KEYBOARD_RESPONSE_KEY_DETECTION_ERROR2 0xff

#define PS2_KEYBOARD_SCANCODE_SET1 0x43
#define PS2_KEYBOARD_SCANCODE_SET2 0x41
#define PS2_KEYBOARD_SCANCODE_SET3 0x3f

#define PS2_KEYBOARD_CMD_SET_LEDS 0xed
#define PS2_KEYBOARD_DATA_LED_SCROLL_LOCK 0b0000
#define PS2_KEYBOARD_DATA_LED_NUMBER_LOCK 0b0001
#define PS2_KEYBOARD_DATA_LED_CAPS_LOCK 0b0010
#define PS2_KEYBOARD_DATA_LED_KANA_MODE 0b1000

#define PS2_KEYBOARD_CMD_ECHO 0xee

#define PS2_KEYBOARD_CMD_SCANCODE_SET 0xf0
#define PS2_KEYBOARD_SUBCMD_GET_SCANCODE_SET 0b0000
#define PS2_KEYBOARD_SUBCMD_SET_SCANCODE_SET1 0b0001
#define PS2_KEYBOARD_SUBCMD_SET_SCANCODE_SET2 0b0010
#define PS2_KEYBOARD_SUBCMD_SET_SCANCODE_SET3 0b0011

#define PS2_KEYBOARD_CMD_IDENTIFY 0xf2

#define PS2_KEYBOARD_CMD_SET_RATE_DELAY 0xf3

#define PS2_KEYBOARD_CMD_ENABLE_SCANNING 0xf4
#define PS2_KEYBOARD_CMD_DISABLE_SCANNING 0xf5

#define PS2_KEYBOARD_CMD_SET_DEFAULT 0xf6

#define PS2_KEYBOARD_CMD_SET_TYPEMATIC_AUTOREPEAT_ONLY 0xf7
#define PS2_KEYBOARD_CMD_SET_MAKE_RELEASE 0xf8
#define PS2_KEYBOARD_CMD_SET_MAKE_ONLY 0xf9
#define PS2_KEYBOARD_CMD_SET_TYPEMATIC_AUTOREPEAT_MAKE_RELEASE 0xfa
#define PS2_KEYBOARD_CMD_SET_KEY_TYPEMATIC_AUTOREPEAT_ONLY 0xfb
#define PS2_KEYBOARD_CMD_SET_KEY_MAKE_RELEASE 0xfc
#define PS2_KEYBOARD_CMD_SET_KEY_MAKE_ONLY 0xfd

#define PS2_KEYBOARD_CMD_RESEND 0xfe
#define PS2_KEYBOARD_CMD_RESET_SELF_TEST 0xff

void PS2_Enable_Keyboard_Interrupt();
void PS2_Disable_Keyboard_Interrupt();

u8 PS2_Keyboard_Command(u8 cmd);
u8 PS2_Keyboard_Command(u8 cmd, u8 data);
u8 PS2_Keyboard_Command(u8 cmd, u8 *data, usize count);

bool PS2_SetLEDs(u8 state);
bool PS2_Echo();
bool PS2_SetCurrentScancodeSet(u8 value);
u8 PS2_GetCurrentScancodeSet();
u16 PS2_IdentifyKeyboard();
bool PS2_SetTypematicRateAndDelay(u8 rate, u8 delay);
bool PS2_EnableScanning();
bool PS2_DisableScanning();
bool PS2_SetDefaultParameters();
bool PS2_SetAllKeys_Typematic_Autorepeat_Only();
bool PS2_SetAllKeys_Make_Release();
bool PS2_SetAllKeys_Make_Only();
bool PS2_SetAllKeys_Typematic_Autorepeat_Make_Release();
bool PS2_SetKey_Typematic_Autorelease_Only(u8 scancode);
bool PS2_SetKey_Make_Release(u8 scancode);
bool PS2_SetKey_Make_Only(u8 scancode);
u8 PS2_Resend();
u8 PS2_ResetAndSelfTest();
