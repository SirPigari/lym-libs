# Raylib Lucia

Raylib 5.5 bindings for Lucia.  
Currently at version 0.1.8. ([CHANGELOG](#changelog))

## Installation
Install via LYM:

```bash
lym install raylib_lucia  # or just raylib we have an alias
```

## Example Usage

```lucia
import (*) from raylib

InitWindow(800, 600, "Raylib Lucia Example")
SetTargetFPS(60)

while !WindowShouldClose():
    BeginDrawing()
    ClearBackground(RAYWHITE)
    DrawText("Hello, Raylib Lucia!", 190, 200, 20, LIGHTGRAY)
    EndDrawing()
end

CloseWindow()
```

## How it works

We have 2 libraries per platform:
- `raylib.dll/.so/.dylib`: The original Raylib library.
- `raylib_lucia.dll/.so/.dylib`: The binding layer for functions which dont work directly with Lucia. (`libload` doesnt support some complex types like structs or pointers well)

It uses `libload` std lucia library to load both libraries and bind the functions.

The `raylib_lucia` library has a C binding layer ([`raylib_lucia_bindings.c`](./lib/binding/raylib_lucia_bindings.c)) which exposes functions with simple types that `libload` can handle, and internally calls the original Raylib functions.

It loads the already loaded `raylib` library from memory to get the same `rlgl context` so both libraries work together.
The `lym.json` file handles so you dont download the libraries from other platforms, so you only have the ones you need.

### IDs

Raylib-Lucia uses IDs to manage resources like sounds, music, audio streams and textures.  
When you load or create a resource, you get back a struct as an ID that represents that resource.

The IDs are stored in static arrays inside C code, and the library keeps track of which IDs are in use.  
When you want to use a resource, you pass its ID to the relevant function.

Static limits are defined for each resource type:
- MAX_WAVES: 256
- MAX_SOUNDS: 512
- MAX_MUSICS: 128
- MAX_STREAMS: 128
- MAX_TEXTURES: 1024

You should unload resources when done to free up IDs for reuse.

NOTE: `CloseWindow()` will automatically unload all resources, so you dont need to manually unload everything at the end of your program.

## Features

Every binded function follows the format:  

A bind function, private, in snake_case, ends with `_c`, e.g. `init_window_c`.  
A public function that wraps the bind function, in snake_case, without the `_c`, e.g. `init_window`.  
A public alias function in PamelCase, e.g. `InitWindow`.  

In here we list all the public functions in snake_case.  
Even tho in the code they are in snake_case, the alias functions in PamelCase are the recommended way to use them.  
Each function links to its definition in the `defs` folder, and every function includes its documentation directly under its definition in the standard Lucia syntax, so you can see the description, parameters, and return type right there in the code.  

### Audio

Audio device management and audio playing functions.  

 - [init_audio_device](./defs/audio.lc#L38)
 - [close_audio_device](./defs/audio.lc#L48)
 - [is_audio_device_ready](./defs/audio.lc#L58)
 - [set_master_volume](./defs/audio.lc#L67)
 - [get_master_volume](./defs/audio.lc#L82)
 - [play_wave](./defs/audio.lc#L92)
 - [unload_all_waves](./defs/audio.lc#L127)
 - [play_sound](./defs/audio.lc#L168)
 - [stop_sound](./defs/audio.lc#L206)
 - [unload_sound](./defs/audio.lc#L224)
 - [pause_sound](./defs/audio.lc#L242)
 - [resume_sound](./defs/audio.lc#L260)
 - [is_sound_playing](./defs/audio.lc#L278)
 - [unload_all_sounds](./defs/audio.lc#L137)
 - [play_music_stream](./defs/audio.lc#L303)
 - [stop_music_stream](./defs/audio.lc#L341)
 - [unload_music_stream](./defs/audio.lc#L359)
 - [pause_music_stream](./defs/audio.lc#L377)
 - [resume_music_stream](./defs/audio.lc#L395)
 - [is_music_stream_playing](./defs/audio.lc#L413)
 - [unload_all_music](./defs/audio.lc#L147)
 - [play_audio_stream](./defs/audio.lc#L438)
 - [stop_audio_stream](./defs/audio.lc#L491)
 - [unload_audio_stream](./defs/audio.lc#L509)
 - [pause_audio_stream](./defs/audio.lc#L527)
 - [resume_audio_stream](./defs/audio.lc#L545)
 - [is_audio_stream_playing](./defs/audio.lc#L563)
 - [unload_all_audio_streams](./defs/audio.lc#L157)
  
 - [MAX_WAVES](./defs/audio.lc#L32): int     = 256
 - [MAX_SOUNDS](./defs/audio.lc#L33): int    = 512
 - [MAX_MUSICS](./defs/audio.lc#L34): int    = 128
 - [MAX_STREAMS](./defs/audio.lc#L35): int   = 128

**Example**

This example initializes the audio device and plays a sound when the space key is pressed.

```lucia
import (*) from raylib

InitWindow(400, 200, "Audio Example")
InitAudioDevice()

SetTargetFPS(60)

while !WindowShouldClose():
    if IsKeyPressed(KEY_SPACE):
        PlaySound("coin.wav")
    end

    BeginDrawing()
    ClearBackground(RAYWHITE)

    DrawText("Press SPACE to play sound", 20, 80, 20, DARKGREEN)

    EndDrawing()
end

CloseAudioDevice()
CloseWindow()
```

### Colors

Color management functions and constants.

 - [LIGHTGRAY](./defs/colors.lc#L2): tuple[int; 4]  = (200, 200, 200, 255)
 - [GRAY](./defs/colors.lc#L3): tuple[int; 4]       = (130, 130, 130, 255)
 - [DARKGRAY](./defs/colors.lc#L4): tuple[int; 4]   = (80, 80, 80, 255)
 - [YELLOW](./defs/colors.lc#L5): tuple[int; 4]     = (253, 249, 0, 255)
 - [GOLD](./defs/colors.lc#L6): tuple[int; 4]       = (255, 203, 0, 255)
 - [ORANGE](./defs/colors.lc#L7): tuple[int; 4]     = (255, 161, 0, 255)
 - [PINK](./defs/colors.lc#L8): tuple[int; 4]       = (255, 109, 194, 255)
 - [RED](./defs/colors.lc#L9): tuple[int; 4]        = (230, 41, 55, 255)
 - [MAROON](./defs/colors.lc#L10): tuple[int; 4]     = (190, 33, 55, 255)
 - [GREEN](./defs/colors.lc#L11): tuple[int; 4]      = (0, 228, 48, 255)
 - [LIME](./defs/colors.lc#L12): tuple[int; 4]       = (0, 158, 47, 255)
 - [DARKGREEN](./defs/colors.lc#L13): tuple[int; 4]  = (0, 117, 44, 255)
 - [SKYBLUE](./defs/colors.lc#L14): tuple[int; 4]    = (102, 191, 255, 255)
 - [BLUE](./defs/colors.lc#L15): tuple[int; 4]       = (0, 121, 241, 255)
 - [DARKBLUE](./defs/colors.lc#L16): tuple[int; 4]   = (0, 82, 172, 255)
 - [PURPLE](./defs/colors.lc#L17): tuple[int; 4]     = (200, 122, 255, 255)
 - [VIOLET](./defs/colors.lc#L18): tuple[int; 4]     = (135, 60, 190, 255)
 - [DARKPURPLE](./defs/colors.lc#L19): tuple[int; 4] = (112, 31, 126, 255)
 - [BEIGE](./defs/colors.lc#L20): tuple[int; 4]      = (211, 176, 131, 255)
 - [BROWN](./defs/colors.lc#L21): tuple[int; 4]      = (127, 106, 79, 255)
 - [DARKBROWN](./defs/colors.lc#L22): tuple[int; 4]  = (76, 63, 47, 255)
 - [WHITE](./defs/colors.lc#L23): tuple[int; 4]      = (255, 255, 255, 255)
 - [BLACK](./defs/colors.lc#L24): tuple[int; 4]      = (0, 0, 0, 255)
 - [BLANK](./defs/colors.lc#L25): tuple[int; 4]      = (0, 0, 0, 0)
 - [MAGENTA](./defs/colors.lc#L26): tuple[int; 4]    = (255, 0, 255, 255)
 - [RAYWHITE](./defs/colors.lc#L27): tuple[int; 4]   = (245, 245, 245, 255)

 - [fade](./defs/colors.lc#L59)
 - [color_to_int](./defs/colors.lc#L80)
 - [color_normalize](./defs/colors.lc#L99)
 - [color_from_normalized](./defs/colors.lc#L127)
 - [color_to_hsv](./defs/colors.lc#L147)
 - [color_from_hsv](./defs/colors.lc#L174)
 - [color_tint](./defs/colors.lc#L197)
 - [color_brightness](./defs/colors.lc#L226)
 - [color_contrast](./defs/colors.lc#L250)
 - [color_alpha](./defs/colors.lc#L274)
 - [color_alpha_blend](./defs/colors.lc#L295)
 - [color_lerp](./defs/colors.lc#L333)
 - [get_color](./defs/colors.lc#L366)

**Example**

This example demonstrates color manipulation functions.

```lucia
import (*) from raylib

InitWindow(600, 300, "Colors Example")
SetTargetFPS(60)

mutable t: float = 0.0

while !WindowShouldClose():
    t += GetFrameTime()

    // only the blend part is non-trivial
    c := ColorLerp(RED, BLUE, (sin(t) + 1) / 2)

    BeginDrawing()
    ClearBackground(c)

    DrawText("Background Lerp RED <-> BLUE", 40, 120, 20, WHITE)

    EndDrawing()
end

CloseWindow()
```

### Core

Core functionality and utilities.

 - [get_random_value](./defs/core.lc#L26)
 - [set_random_seed](./defs/core.lc#L38)
 - [set_target_fps](./defs/core.lc#L49)
 - [get_fps](./defs/core.lc#L60)
 - [get_frame_time](./defs/core.lc#L70)
 - [get_time](./defs/core.lc#L80)
 - [enable_event_waiting](./defs/core.lc#L90)
 - [disable_event_waiting](./defs/core.lc#L100)
 - [trace_log](./defs/core.lc#L110)
 - [set_trace_log_level](./defs/core.lc#L126)
 - [swap_screen_buffer](./defs/core.lc#L137)
 - [poll_input_events](./defs/core.lc#L147)
 - [wait_time](./defs/core.lc#L157)
 - [take_screenshot](./defs/core.lc#L168)
 - [open_url](./defs/core.lc#L183)

 - [LOG_ALL](./defs/core.lc#L17): int         = 0
 - [LOG_TRACE](./defs/core.lc#L18): int       = 1
 - [LOG_DEBUG](./defs/core.lc#L19): int       = 2
 - [LOG_INFO](./defs/core.lc#L20): int        = 3
 - [LOG_WARNING](./defs/core.lc#L21): int     = 4
 - [LOG_ERROR](./defs/core.lc#L22): int       = 5
 - [LOG_FATAL](./defs/core.lc#L23): int       = 6
 - [LOG_NONE](./defs/core.lc#L24): int        = 7

**Example**

This example creates a window and displays random numbers and elapsed time.

```lucia
import (*) from raylib

InitWindow(400, 200, "Core Example")
SetTargetFPS(60)

SetRandomSeed(1337)

while !WindowShouldClose():
    n := GetRandomValue(0, 100)

    BeginDrawing()
    ClearBackground(RAYWHITE)

    DrawText(f"Random: {n}", 20, 20, 20, DARKBLUE)
    DrawText(f"Time: {GetTime()}", 20, 60, 20, DARKGREEN)

    EndDrawing()
end

CloseWindow()
```

### Drawing

2D drawing functions for shapes and primitives.

 - [begin_drawing](./defs/drawing.lc#L16)
 - [end_drawing](./defs/drawing.lc#L26)
 - [clear_background](./defs/drawing.lc#L36)
 - [draw_rectangle](./defs/drawing.lc#L59)
 - [draw_rectangle_lines](./defs/drawing.lc#L92)
 - [draw_pixel](./defs/drawing.lc#L125)
 - [draw_line](./defs/drawing.lc#L154)
 - [draw_circle](./defs/drawing.lc#L187)
 - [draw_circle_lines](./defs/drawing.lc#L218)
 - [draw_triangle](./defs/drawing.lc#L249)
 - [draw_triangle_lines](./defs/drawing.lc#L280)
 - [draw_poly](./defs/drawing.lc#L311)
 - [draw_poly_lines](./defs/drawing.lc#L344)

**Example**

This example creates a window and draws various shapes.

```lucia
import (*) from raylib

InitWindow(800, 600, "Drawing Example")
SetTargetFPS(60)

while !WindowShouldClose():
    BeginDrawing()
    ClearBackground(RAYWHITE)

    DrawRectangle(50, 50, 200, 100, SKYBLUE)
    DrawCircle(400, 200, 50, PINK)
    DrawLine(300, 400, 600, 500, RED)
    DrawTriangle((600,100), (700,200), (650,300), GREEN)  // in CCW order

    EndDrawing()
end

CloseWindow()
```

### Image

Image loading, texture management and image drawing functions.

 - [MAX_TEXTURES](./defs/image.lc#L17): int = 1024

 - [Texture](./defs/image.lc#L19) - Struct with id, path, width, height fields
 - [Texture2D](./defs/image.lc#L26) - Alias for Texture

 - [load_texture](./defs/image.lc#L38)
 - [unload_texture](./defs/image.lc#L68)
 - [unload_all_textures](./defs/image.lc#L82)
 - [is_texture_valid](./defs/image.lc#L89)
 - [draw_texture](./defs/image.lc#L100)
 - [draw_texture_ex](./defs/image.lc#L127)
 - [draw_texture_rec](./defs/image.lc#L158)
 - [draw_texture_pro](./defs/image.lc#L195)
 - [draw_image](./defs/image.lc#L278)
 - [get_texture_width](./defs/image.lc#L242)
 - [get_texture_height](./defs/image.lc#L253)
 - [get_texture_path](./defs/image.lc#L264)
 - [Texture.get_id](./defs/image.lc#L29)
 - [Texture.get_path](./defs/image.lc#L33)

**Example**

This example loads and displays a texture.

```lucia
import (*) from raylib

InitWindow(800, 600, "Texture Example")
SetTargetFPS(60)

tex: Texture = LoadTexture("my_texture.png")

while !WindowShouldClose():
    BeginDrawing()
    ClearBackground(RAYWHITE)

    if IsTextureValid(tex):
        DrawTexture(tex, 100, 100, WHITE)
    else:
        DrawText("Texture failed to load", 20, 20, 20, RED)
    end

    EndDrawing()
end

CloseWindow()
```

### Input

Input handling functions for keyboard, mouse, gamepad, and gestures.

**Keyboard Input:**
 - [is_key_pressed](./defs/input.lc#L239)
 - [is_key_pressed_repeat](./defs/input.lc#L249)
 - [is_key_down](./defs/input.lc#L259)
 - [is_key_released](./defs/input.lc#L269)
 - [is_key_up](./defs/input.lc#L279)
 - [get_key_pressed](./defs/input.lc#L289)
 - [get_char_pressed](./defs/input.lc#L299)
 - [set_exit_key](./defs/input.lc#L309)
 - [get_key_name](./defs/input.lc#L803)

**Gamepad Input:**
 - [is_gamepad_available](./defs/input.lc#L323)
 - [get_gamepad_name](./defs/input.lc#L333)
 - [is_gamepad_button_pressed](./defs/input.lc#L344)
 - [is_gamepad_button_down](./defs/input.lc#L355)
 - [is_gamepad_button_released](./defs/input.lc#L366)
 - [is_gamepad_button_up](./defs/input.lc#L377)
 - [get_gamepad_button_pressed](./defs/input.lc#L388)
 - [get_gamepad_axis_count](./defs/input.lc#L398)
 - [get_gamepad_axis_movement](./defs/input.lc#L409)
 - [set_gamepad_mappings](./defs/input.lc#L421)
 - [set_gamepad_vibration](./defs/input.lc#L433)

**Mouse Input:**
 - [is_mouse_button_pressed](./defs/input.lc#L450)
 - [is_mouse_button_down](./defs/input.lc#L460)
 - [is_mouse_button_released](./defs/input.lc#L470)
 - [is_mouse_button_up](./defs/input.lc#L480)
 - [get_mouse_x](./defs/input.lc#L490)
 - [get_mouse_y](./defs/input.lc#L500)
 - [get_mouse_position](./defs/input.lc#L736)
 - [get_mouse_delta](./defs/input.lc#L747)
 - [set_mouse_position](./defs/input.lc#L510)
 - [set_mouse_offset](./defs/input.lc#L522)
 - [set_mouse_scale](./defs/input.lc#L534)
 - [get_mouse_wheel_move](./defs/input.lc#L546)
 - [get_mouse_wheel_move_v](./defs/input.lc#L758)
 - [set_mouse_cursor](./defs/input.lc#L556)
 - [show_cursor](./defs/input.lc#L567)
 - [hide_cursor](./defs/input.lc#L577)
 - [is_cursor_hidden](./defs/input.lc#L587)
 - [is_cursor_on_screen](./defs/input.lc#L596)
 - [enable_cursor](./defs/input.lc#L605)
 - [disable_cursor](./defs/input.lc#L615)

**Touch Input:**
 - [get_touch_x](./defs/input.lc#L628)
 - [get_touch_y](./defs/input.lc#L639)
 - [get_touch_position](./defs/input.lc#L769)
 - [get_touch_point_id](./defs/input.lc#L650)
 - [get_touch_point_count](./defs/input.lc#L661)

**Gestures:**
 - [set_gestures_enabled](./defs/input.lc#L674)
 - [is_gesture_detected](./defs/input.lc#L683)
 - [get_gesture_count](./defs/input.lc#L693)
 - [get_gesture_hold_duration](./defs/input.lc#L703)
 - [get_gesture_drag_angle](./defs/input.lc#L713)
 - [get_gesture_drag_vector](./defs/input.lc#L792)
 - [get_gesture_pinch_angle](./defs/input.lc#L723)
 - [get_gesture_pinch_vector](./defs/input.lc#L781)

**Example**

```lucia
import (*) from raylib

InitWindow(600, 300, "Input Example")
SetTargetFPS(60)

mutable x: int = 300
mutable y: int = 150

while !WindowShouldClose():
    if IsKeyDown(KEY_RIGHT):
        x += 3
    end
    if IsKeyDown(KEY_LEFT):
        x -= 3
    end
    if IsKeyDown(KEY_UP):
        y -= 3
    end
    if IsKeyDown(KEY_DOWN):
        y += 3
    end

    BeginDrawing()
    ClearBackground(RAYWHITE)

    DrawCircle(x, y, 20, BLUE)
    DrawText("Use Arrow Keys", 20, 20, 20, DARKGRAY)

    EndDrawing()
end

CloseWindow()
```

### Text

Text rendering and measurement functions.

 - [measure_text](./defs/text.lc#L6)
 - [draw_text](./defs/text.lc#L22)
 - [draw_fps](./defs/text.lc#L59)
 - [set_text_line_spacing](./defs/text.lc#L71)

**Example**

This example displays text and the current FPS.

```lucia
import (*) from raylib

InitWindow(500, 200, "Text Example")
SetTargetFPS(60)

while !WindowShouldClose():
    BeginDrawing()
    ClearBackground(RAYWHITE)

    DrawText("Hello Lucia!", 20, 20, 20, BLACK)
    DrawFPS(20, 60)

    EndDrawing()
end

CloseWindow()
```

### Window

Window management and display functions.

 - [init_window](./defs/window.lc#L63)
 - [close_window](./defs/window.lc#L89)
 - [unload_all_resources](./defs/window.lc#L100)
 - [window_should_close](./defs/window.lc#L110)
 - [is_window_ready](./defs/window.lc#L120)
 - [is_window_minimized](./defs/window.lc#L129)
 - [is_window_maximized](./defs/window.lc#L138)
 - [is_window_focused](./defs/window.lc#L147)
 - [is_window_resized](./defs/window.lc#L156)
 - [is_window_hidden](./defs/window.lc#L165)
 - [is_window_fullscreen](./defs/window.lc#L174)
 - [toggle_fullscreen](./defs/window.lc#L184)
 - [toggle_maximize](./defs/window.lc#L194)
 - [toggle_borderless_windowed](./defs/window.lc#L205)
 - [maximize_window](./defs/window.lc#L215)
 - [minimize_window](./defs/window.lc#L225)
 - [restore_window](./defs/window.lc#L235)
 - [set_window_title](./defs/window.lc#L245)
 - [set_window_position](./defs/window.lc#L260)
 - [set_window_size](./defs/window.lc#L272)
 - [set_window_monitor](./defs/window.lc#L287)
 - [set_config_flags](./defs/window.lc#L298)
 - [set_window_state](./defs/window.lc#L312)
 - [clear_window_state](./defs/window.lc#L326)
 - [set_window_min_size](./defs/window.lc#L471)
 - [set_window_max_size](./defs/window.lc#L483)
 - [set_window_opacity](./defs/window.lc#L495)
 - [set_window_focused](./defs/window.lc#L509)
 - [set_fullscreen](./defs/window.lc#L453)
 - [set_window_icon](./defs/window.lc#L520)
 - [set_window_icons](./defs/window.lc#L535)
 - [get_screen_width](./defs/window.lc#L340)
 - [get_screen_height](./defs/window.lc#L349)
 - [get_render_width](./defs/window.lc#L358)
 - [get_render_height](./defs/window.lc#L367)
 - [get_window_position](./defs/window.lc#L555)
 - [get_window_scale_dpi](./defs/window.lc#L569)
 - [get_monitor_count](./defs/window.lc#L376)
 - [get_current_monitor](./defs/window.lc#L385)
 - [get_monitor_width](./defs/window.lc#L394)
 - [get_monitor_height](./defs/window.lc#L404)
 - [get_monitor_refresh_rate](./defs/window.lc#L414)
 - [get_clipboard_text](./defs/window.lc#L424)
 - [set_clipboard_text](./defs/window.lc#L438)
 - [FLAG_VSYNC_HINT](./defs/window.lc#L48): int           = 0x00000040
 - [FLAG_FULLSCREEN_MODE](./defs/window.lc#L49): int      = 0x00000002
 - [FLAG_WINDOW_RESIZABLE](./defs/window.lc#L50): int     = 0x00000004
 - [FLAG_WINDOW_UNDECORATED](./defs/window.lc#L51): int   = 0x00000008
 - [FLAG_WINDOW_TRANSPARENT](./defs/window.lc#L52): int   = 0x00000010
 - [FLAG_MSAA_4X_HINT](./defs/window.lc#L53): int         = 0x00000020
 - [FLAG_WINDOW_HIDDEN](./defs/window.lc#L54): int        = 0x00000080
 - [FLAG_WINDOW_ALWAYS_RUN](./defs/window.lc#L55): int    = 0x00000100
 - [FLAG_WINDOW_MINIMIZED](./defs/window.lc#L56): int     = 0x00000200
 - [FLAG_WINDOW_MAXIMIZED](./defs/window.lc#L57): int     = 0x00000400
 - [FLAG_WINDOW_UNFOCUSED](./defs/window.lc#L58): int     = 0x00000800
 - [FLAG_WINDOW_TOPMOST](./defs/window.lc#L59): int       = 0x00001000
 - [FLAG_WINDOW_HIGHDPI](./defs/window.lc#L60): int       = 0x00002000
 - [FLAG_INTERLACED_HINT](./defs/window.lc#L61): int      = 0x00004000

**Example**

This example creates a window and displays whether it is focused or not.

```lucia
import (*) from raylib

InitWindow(400, 200, "Window Example: Focus Detection")
SetTargetFPS(60)

while !WindowShouldClose():
    BeginDrawing()
    ClearBackground(RAYWHITE)

    if IsWindowFocused()
        DrawText("Window Focused", 40, 80, 20, GREEN)
    else
        DrawText("Not Focused", 40, 80, 20, RED)
    end

    EndDrawing()
end

CloseWindow()
```

## CHANGELOG

### Version 0.1.8
- Added license field to manifest.json
- Added complete image/texture system with `image.lc` definitions file
- Added image texture management with binding functions (image.c, load_raylib.c)
- Added texture loading, unloading, and validation functions
- Added texture drawing functions (draw_texture, draw_texture_ex, draw_texture_rec, draw_texture_pro)
- Added draw_image function for direct image rendering with transformation options
- Added texture property accessors (get_texture_width, get_texture_height, get_texture_path)
- Added artifacts to `lym.json` to auto build `raylib_lucia.dll/.so/.dylib` on publish

### Version 0.1.7  
- Changed entry point from `__init__.lc` to `main.lc`
- Added `lym.json` configuration file
- Added audio system with `audio.lc` definitions and `audio.c` bindings
- Added input system with `input.lc` definitions (keyboard, mouse, gamepad, touch, gestures)
- Added audio device management functions
- Added Wave, Sound, Music Stream, and Audio Stream support with IDs and error handling
- Added dual library loading support (`lib` and `lib_bind`)

### Version 0.1.6
- Added `raylib_lucia_bindings.c` binding layer
- Moved libraries to structured `lib/` directory with platform-specific files
- Added static library files (.a) for different platforms
- Added `raylib_lucia.dll/.so/.dylib` binding libraries
- Maintained same 5 core definition files as 0.1.4-0.1.5

### Version 0.1.4-0.1.5
- Separated functionality into modular `defs/` directory
- Split single `__init__.lc` into 5 specialized modules:
  - `window.lc` - Window management functions (expanded from basic 3 to ~17 functions)
  - `drawing.lc` - 2D drawing primitives 
  - `text.lc` - Text rendering functions
  - `colors.lc` - Color constants and manipulation functions
  - `core.lc` - Core utilities and system functions
- Changed library path from root directory to `lib/` subdirectory
- Added window state management
- Added color constants (26 predefined colors)
- Added drawing shapes (rectangles, lines, circles, triangles, polygons)

### Version 0.1.0-0.1.3
- Initial implementation with monolithic `__init__.lc` file
- Basic window creation and management (7 core functions)
  - `init_window`, `close_window`, `window_should_close`
  - `begin_drawing`, `end_drawing`, `clear_background`, `draw_text`
- Direct library loading from root directory (`raylib.dll`/`.so`/`.dylib`)
- Cross-platform support (Windows, Linux, macOS)
- Basic error handling and library validation
