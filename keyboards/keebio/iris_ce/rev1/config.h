// Copyright 2023 Danny Nguyen (danny@keeb.io)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define SPLIT_HAND_PIN GP4
#define USB_VBUS_PIN GP0
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP12
#define SERIAL_USART_RX_PIN GP13
#define SERIAL_USART_PIN_SWAP
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U
#define I2C_DRIVER I2CD2
#define I2C1_SDA_PIN GP10
#define I2C1_SCL_PIN GP11

/*
			┌──────────────────────────────┐
			│      ADDITIONAL OPTIONS      │
			└──────────────────────────────┘

*/

#define SPLIT_LAYER_STATE_ENABLE 								// ensures the current layer state is available on the slave when using the QMK-provided split transport
#define SPLIT_LED_STATE_ENABLE 									// ensures the current host indicator state (caps/num/scroll) is available on the slave when using the QMK-provided split transport
// #define BOTH_SHIFTS_TURNS_ON_CAPS_WORD						// active Caps Word by pressing Left Shift + Right Shift


/*
			┌──────────────────────────────┐
			│      RGB MATRIX OPTIONS      │
			└──────────────────────────────┘

Additional RGB matrix option
*/

#define RGB_MATRIX_TIMEOUT 300000 								// number of milliseconds to wait until rgb automatically turns off
#define RGB_MATRIX_SLEEP										// turn off effects when suspended
// #define RGB_MATRIX_LED_FLUSH_LIMIT 16 						// limits in milliseconds how frequently an animation will update the LEDs. 16 (16ms) is equivalent to limiting to 60fps (increases keyboard responsiveness)
// #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120 					// limits maximum brightness of LEDs to 200 out of 255. If not defined maximum brightness is set to 120
// #define RGB_MATRIX_DEFAULT_ON true 							// Sets the default enabled state, if none has been set
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR 			// Sets the default mode, if none has been set
#define RGB_MATRIX_DEFAULT_HUE 172 								// Sets the default hue value, if none has been set
#define RGB_MATRIX_DEFAULT_SAT 112 								// Sets the default saturation value, if none has been set
// #define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS // Sets the default brightness value, if none has been set
// #define RGB_MATRIX_DEFAULT_SPD 127 							// Sets the default animation speed, if none has been set
#define RGB_MATRIX_HUE_STEP 5 									// The number of steps to cycle through the hue by (default = 8) 
#define RGB_MATRIX_SAT_STEP 10 									// The number of steps to increment the saturation by (default = 16)
#define RGB_MATRIX_VAL_STEP 10 									// The number of steps to increment the brightness by (default = 16)
#define RGB_MATRIX_SPD_STEP 10 									// The number of steps to increment the speed by (default = 16)

/* Disable the animations you don't want/need.*/

#    undef ENABLE_RGB_MATRIX_ALPHAS_MODS        				// Static dual hue, speed is hue for secondary hue
#    undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN    				// Static gradient top to bottom, speed controls how much gradient changes
#    undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT    			// Static gradient left to right, speed controls how much gradient changes
#    undef ENABLE_RGB_MATRIX_BREATHING           				// Single hue brightness cycling animation
// #    undef ENABLE_RGB_MATRIX_BAND_SAT        					// Single hue band fading saturation scrolling left to right
// #    undef ENABLE_RGB_MATRIX_BAND_VAL        					// Single hue band fading brightness scrolling left to right
// #    undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT   				// Single hue 3 blade spinning pinwheel fades saturation
// #    undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL   				// Single hue 3 blade spinning pinwheel fades brightness
// #    undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT     				// Single hue spinning spiral fades saturation
// #    undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL     				// Single hue spinning spiral fades brightness
#    undef ENABLE_RGB_MATRIX_CYCLE_ALL           				// Full keyboard solid hue cycling through full gradient
#    undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT    				// Full gradient scrolling left to right
#    undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN       				// Full gradient scrolling top to bottom
#    undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN        				// Full gradient scrolling out to in
#    undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL   				// Full dual gradients scrolling out to in
#    undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON  			// Full gradient Chevron shapped scrolling left to right
#    undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL      				// Full gradient spinning pinwheel around center of keyboard
#    undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL        				// Full gradient spinning spiral around center of keyboard
#    undef ENABLE_RGB_MATRIX_DUAL_BEACON         				// Full gradient spinning around center of keyboard
#    undef ENABLE_RGB_MATRIX_RAINBOW_BEACON      				// Full tighter gradient spinning around center of keyboard
#    undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS   				// Full dual gradients spinning two halfs of keyboard
// #    undef ENABLE_RGB_MATRIX_RAINDROPS           				// Randomly changes a single key's hue
// #    undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS 				// Randomly changes a single key's hue and saturation
#    undef ENABLE_RGB_MATRIX_HUE_BREATHING       				// Hue shifts up a slight ammount at the same time, then shifts back
#    undef ENABLE_RGB_MATRIX_HUE_PENDULUM        				// Hue shifts up a slight ammount in a wave to the right, then back to the left
#    undef ENABLE_RGB_MATRIX_HUE_WAVE            				// Hue shifts up a slight ammount and then back down in a wave to the right
// #    undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL       				// Single hue fractal filled keys pulsing horizontally out to edges
// #    undef ENABLE_RGB_MATRIX_PIXEL_FLOW          				// Pulsing RGB flow along LED wiring with random hues
// #    undef ENABLE_RGB_MATRIX_PIXEL_RAIN          				// Randomly light keys with random hues
#    undef ENABLE_RGB_MATRIX_TYPING_HEATMAP      				// How hot is your WPM!
#    undef ENABLE_RGB_MATRIX_DIGITAL_RAIN        				// That famous computer simulation
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE   			// Pulses keys hit to hue & value then fades value out
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE      				// Static single hue, pulses keys hit to shifted hue then fades to current hue
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE       			// Hue & value pulse near a single key hit then fades value out
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  			// Hue & value pulse near multiple key hits then fades value out
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS      			// Hue & value pulse the same column and row of a single key hit then fades value out
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS 			// Hue & value pulse the same column and row of multiple key hits then fades value out
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS      			// Hue & value pulse away on the same column and row of a single key hit then fades value out
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS 			// Hue & value pulse away on the same column and row of multiple key hits then fades value out
#    undef ENABLE_RGB_MATRIX_SPLASH              				// Full gradient & value pulse away from a single key hit then fades value out
#    undef ENABLE_RGB_MATRIX_MULTISPLASH         				// Full gradient & value pulse away from multiple key hits then fades value out
#    undef ENABLE_RGB_MATRIX_SOLID_SPLASH        				// Hue & value pulse away from a single key hit then fades value out
#    undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH   				// Hue & value pulse away from multiple key hits then fades value out
