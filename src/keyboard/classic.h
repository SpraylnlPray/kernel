#ifndef CLASSIC_KEYBOARD_H
#define CLASSIC_KEYBOARD_H

#define PS2_PORT 0x64
#define PS2_COMMAND_ENABLE_FIRST_PORT 0xAE

#define CLASSIC_KEYBOARD_KEY_RELEASED 0x80
#define CLASSIC_KEYBOARD_LSHIFT_PRESSED 0x2A
#define CLASSIC_KEYBOARD_LSHIFT_RELEASED 0xAA
#define CLASSIC_KEYBOARD_RSHIFT_PRESSED 0x36
#define CLASSIC_KEYBOARD_RSHIFT_RELEASED 0xB6
#define ISR_KEYBOARD_INTERRUPT 0x21
#define KEYBOARD_INPUT_PORT 0x60

struct keyboard* classic_init();

#endif