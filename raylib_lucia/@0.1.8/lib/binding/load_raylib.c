#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// explanation: load raylib from already loaded dll in memory because the rlgl context
// didnt notice that it didnt worked before

#if defined(_WIN32)
// because fuck windows
#define Rectangle WIN_Rectangle
#define LoadImageA WIN_LoadImageA
#define DrawText WIN_DrawText
#define DrawTextEx WIN_DrawTextEx
#define PlaySound WIN_PlaySound
#define CloseWindow WIN_CloseWindow
#define ShowCursor WIN_ShowCursor
#include <windows.h>
#undef CloseWindow
#undef ShowCursor
#undef Rectangle
#undef LoadImageA
#undef DrawText
#undef DrawTextEx
#undef PlaySound
#else
#include <dlfcn.h>
#endif

#include "raylib.h"

static void* load_library(const char* name) {
#if defined(_WIN32)
    HMODULE handle = GetModuleHandleA(name);
    if (handle) return handle;
    handle = LoadLibraryA(name);
    return (void*)handle;
#else
    void* handle = dlopen(name, RTLD_LAZY | RTLD_NOLOAD);
    if (handle) return handle;
    handle = dlopen(name, RTLD_LAZY);
    return handle;
#endif
}

static void* get_symbol(void* lib, const char* name) {
#if defined(_WIN32)
    return (void*)GetProcAddress((HMODULE)lib, name);
#else
    return dlsym(lib, name);
#endif
}

static void (*fp_SetWindowIcon)(Image) = NULL;
static void (*fp_SetWindowIcons)(Image*, int) = NULL;
static Vector2 (*fp_GetWindowPosition)(void) = NULL;
static Vector2 (*fp_GetWindowScaleDPI)(void) = NULL;
static void (*fp_InitWindow)(int, int, const char*) = NULL;

static void (*fp_DrawTriangle)(Vector2, Vector2, Vector2, Color) = NULL;
static void (*fp_DrawTriangleLines)(Vector2, Vector2, Vector2, Color) = NULL;
static void (*fp_DrawPoly)(Vector2, int, float, float, Color) = NULL;
static void (*fp_DrawPolyLines)(Vector2, int, float, float, Color) = NULL;

static Vector2 (*fp_GetMousePosition)(void) = NULL;
static Vector2 (*fp_GetMouseDelta)(void) = NULL;
static float (*fp_GetMouseWheelMove)(void) = NULL;
static Vector2 (*fp_GetTouchPosition)(int) = NULL;
static Vector2 (*fp_GetGesturePinchVector)(void) = NULL;
static Vector2 (*fp_GetGestureDragVector)(void) = NULL;

static Image (*fp_LoadImage)(const char*) = NULL;
static void (*fp_UnloadImage)(Image) = NULL;

static Texture2D (*fp_LoadTexture)(const char*) = NULL;
static void (*fp_UnloadTexture)(Texture2D) = NULL;
static bool (*fp_IsTextureValid)(Texture2D) = NULL;
static void (*fp_DrawTexture)(Texture2D, int, int, Color) = NULL;
static void (*fp_DrawTextureEx)(Texture2D, Vector2, float, float, Color) = NULL;
static void (*fp_DrawTextureRec)(Texture2D, Rectangle, Vector2, Color) = NULL;
static void (*fp_DrawTexturePro)(Texture2D, Rectangle, Rectangle, Vector2, float, Color) = NULL;

static Wave (*fp_LoadWave)(const char*) = NULL;
static void (*fp_UnloadWave)(Wave) = NULL;
static bool (*fp_IsWaveValid)(Wave) = NULL;

static Sound (*fp_LoadSound)(const char*) = NULL;
static Sound (*fp_LoadSoundFromWave)(Wave) = NULL;
static void (*fp_UnloadSound)(Sound) = NULL;
static bool (*fp_IsSoundValid)(Sound) = NULL;
static void (*fp_PlaySound)(Sound) = NULL;
static void (*fp_StopSound)(Sound) = NULL;
static void (*fp_PauseSound)(Sound) = NULL;
static void (*fp_ResumeSound)(Sound) = NULL;
static bool (*fp_IsSoundPlaying)(Sound) = NULL;
static void (*fp_SetSoundVolume)(Sound, float) = NULL;
static void (*fp_SetSoundPitch)(Sound, float) = NULL;
static void (*fp_SetSoundPan)(Sound, float) = NULL;

static Music (*fp_LoadMusicStream)(const char*) = NULL;
static void (*fp_UnloadMusicStream)(Music) = NULL;
static bool (*fp_IsMusicValid)(Music) = NULL;
static void (*fp_PlayMusicStream)(Music) = NULL;
static void (*fp_StopMusicStream)(Music) = NULL;
static void (*fp_PauseMusicStream)(Music) = NULL;
static void (*fp_ResumeMusicStream)(Music) = NULL;
static bool (*fp_IsMusicStreamPlaying)(Music) = NULL;
static void (*fp_UpdateMusicStream)(Music) = NULL;
static void (*fp_SetMusicVolume)(Music, float) = NULL;
static void (*fp_SetMusicPitch)(Music, float) = NULL;
static void (*fp_SetMusicPan)(Music, float) = NULL;

static AudioStream (*fp_LoadAudioStream)(unsigned int, unsigned int, unsigned int) = NULL;
static void (*fp_UnloadAudioStream)(AudioStream) = NULL;
static bool (*fp_IsAudioStreamValid)(AudioStream) = NULL;
static void (*fp_PlayAudioStream)(AudioStream) = NULL;
static void (*fp_StopAudioStream)(AudioStream) = NULL;
static void (*fp_PauseAudioStream)(AudioStream) = NULL;
static void (*fp_ResumeAudioStream)(AudioStream) = NULL;
static bool (*fp_IsAudioStreamPlaying)(AudioStream) = NULL;
static void (*fp_SetAudioStreamVolume)(AudioStream, float) = NULL;
static void (*fp_SetAudioStreamPitch)(AudioStream, float) = NULL;
static void (*fp_SetAudioStreamPan)(AudioStream, float) = NULL;

static void (*fp_TraceLog)(int, const char*, ...) = NULL;

// Color functions
static Color (*fp_Fade)(Color, float) = NULL;
static int (*fp_ColorToInt)(Color) = NULL;
static Vector4 (*fp_ColorNormalize)(Color) = NULL;
static Color (*fp_ColorFromNormalized)(Vector4) = NULL;
static Vector3 (*fp_ColorToHSV)(Color) = NULL;
static Color (*fp_ColorFromHSV)(float, float, float) = NULL;
static Color (*fp_ColorTint)(Color, Color) = NULL;
static Color (*fp_ColorBrightness)(Color, float) = NULL;
static Color (*fp_ColorContrast)(Color, float) = NULL;
static Color (*fp_ColorAlpha)(Color, float) = NULL;
static Color (*fp_ColorAlphaBlend)(Color, Color, Color) = NULL;
static Color (*fp_ColorLerp)(Color, Color, float) = NULL;
static Color (*fp_GetColor)(unsigned int) = NULL;

static bool raylib_loaded = false;
static void* raylib_lib = NULL;

static void load_raylib() {
    if (raylib_loaded) return;

#if defined(_WIN32)
    const char* name = "raylib.dll";
#elif defined(__APPLE__)
    const char* name = "libraylib.dylib";
#else
    const char* name = "libraylib.so";
#endif

    raylib_lib = load_library(name);
    if (!raylib_lib) {
        fprintf(stderr, "Failed to load %s\n", name);
        exit(1);
    }

    fp_SetWindowIcon = get_symbol(raylib_lib, "SetWindowIcon");
    fp_SetWindowIcons = get_symbol(raylib_lib, "SetWindowIcons");
    fp_GetWindowPosition = get_symbol(raylib_lib, "GetWindowPosition");
    fp_GetWindowScaleDPI = get_symbol(raylib_lib, "GetWindowScaleDPI");
    fp_InitWindow = get_symbol(raylib_lib, "InitWindow");

    fp_DrawTriangle = get_symbol(raylib_lib, "DrawTriangle");
    fp_DrawTriangleLines = get_symbol(raylib_lib, "DrawTriangleLines");
    fp_DrawPoly = get_symbol(raylib_lib, "DrawPoly");
    fp_DrawPolyLines = get_symbol(raylib_lib, "DrawPolyLines");

    fp_GetMousePosition = get_symbol(raylib_lib, "GetMousePosition");
    fp_GetMouseDelta = get_symbol(raylib_lib, "GetMouseDelta");
    fp_GetMouseWheelMove = get_symbol(raylib_lib, "GetMouseWheelMove");

    fp_GetTouchPosition = get_symbol(raylib_lib, "GetTouchPosition");
    fp_GetGesturePinchVector = get_symbol(raylib_lib, "GetGesturePinchVector");
    fp_GetGestureDragVector = get_symbol(raylib_lib, "GetGestureDragVector");

    fp_LoadImage = get_symbol(raylib_lib, "LoadImage");
    fp_UnloadImage = get_symbol(raylib_lib, "UnloadImage");

    fp_LoadTexture = get_symbol(raylib_lib, "LoadTexture");
    fp_UnloadTexture = get_symbol(raylib_lib, "UnloadTexture");
    fp_IsTextureValid = get_symbol(raylib_lib, "IsTextureValid");
    fp_DrawTexture = get_symbol(raylib_lib, "DrawTexture");
    fp_DrawTextureEx = get_symbol(raylib_lib, "DrawTextureEx");
    fp_DrawTextureRec = get_symbol(raylib_lib, "DrawTextureRec");
    fp_DrawTexturePro = get_symbol(raylib_lib, "DrawTexturePro");

    fp_LoadWave = get_symbol(raylib_lib, "LoadWave");
    fp_UnloadWave = get_symbol(raylib_lib, "UnloadWave");
    fp_IsWaveValid = get_symbol(raylib_lib, "IsWaveValid");

    fp_LoadSound = get_symbol(raylib_lib, "LoadSound");
    fp_LoadSoundFromWave = get_symbol(raylib_lib, "LoadSoundFromWave");
    fp_UnloadSound = get_symbol(raylib_lib, "UnloadSound");
    fp_IsSoundValid = get_symbol(raylib_lib, "IsSoundValid");
    fp_PlaySound = get_symbol(raylib_lib, "PlaySound");
    fp_StopSound = get_symbol(raylib_lib, "StopSound");
    fp_PauseSound = get_symbol(raylib_lib, "PauseSound");
    fp_ResumeSound = get_symbol(raylib_lib, "ResumeSound");
    fp_IsSoundPlaying = get_symbol(raylib_lib, "IsSoundPlaying");
    fp_SetSoundVolume = get_symbol(raylib_lib, "SetSoundVolume");
    fp_SetSoundPitch = get_symbol(raylib_lib, "SetSoundPitch");
    fp_SetSoundPan = get_symbol(raylib_lib, "SetSoundPan");

    fp_LoadMusicStream = get_symbol(raylib_lib, "LoadMusicStream");
    fp_UnloadMusicStream = get_symbol(raylib_lib, "UnloadMusicStream");
    fp_IsMusicValid = get_symbol(raylib_lib, "IsMusicValid");
    fp_PlayMusicStream = get_symbol(raylib_lib, "PlayMusicStream");
    fp_StopMusicStream = get_symbol(raylib_lib, "StopMusicStream");
    fp_PauseMusicStream = get_symbol(raylib_lib, "PauseMusicStream");
    fp_ResumeMusicStream = get_symbol(raylib_lib, "ResumeMusicStream");
    fp_IsMusicStreamPlaying = get_symbol(raylib_lib, "IsMusicStreamPlaying");
    fp_UpdateMusicStream = get_symbol(raylib_lib, "UpdateMusicStream");
    fp_SetMusicVolume = get_symbol(raylib_lib, "SetMusicVolume");
    fp_SetMusicPitch = get_symbol(raylib_lib, "SetMusicPitch");
    fp_SetMusicPan = get_symbol(raylib_lib, "SetMusicPan");

    fp_LoadAudioStream = get_symbol(raylib_lib, "LoadAudioStream");
    fp_UnloadAudioStream = get_symbol(raylib_lib, "UnloadAudioStream");
    fp_IsAudioStreamValid = get_symbol(raylib_lib, "IsAudioStreamValid");
    fp_PlayAudioStream = get_symbol(raylib_lib, "PlayAudioStream");
    fp_StopAudioStream = get_symbol(raylib_lib, "StopAudioStream");
    fp_PauseAudioStream = get_symbol(raylib_lib, "PauseAudioStream");
    fp_ResumeAudioStream = get_symbol(raylib_lib, "ResumeAudioStream");
    fp_IsAudioStreamPlaying = get_symbol(raylib_lib, "IsAudioStreamPlaying");
    fp_SetAudioStreamVolume = get_symbol(raylib_lib, "SetAudioStreamVolume");
    fp_SetAudioStreamPitch = get_symbol(raylib_lib, "SetAudioStreamPitch");
    fp_SetAudioStreamPan = get_symbol(raylib_lib, "SetAudioStreamPan");

    fp_TraceLog = get_symbol(raylib_lib, "TraceLog");
    
    // Color functions
    fp_Fade = get_symbol(raylib_lib, "Fade");
    fp_ColorToInt = get_symbol(raylib_lib, "ColorToInt");
    fp_ColorNormalize = get_symbol(raylib_lib, "ColorNormalize");
    fp_ColorFromNormalized = get_symbol(raylib_lib, "ColorFromNormalized");
    fp_ColorToHSV = get_symbol(raylib_lib, "ColorToHSV");
    fp_ColorFromHSV = get_symbol(raylib_lib, "ColorFromHSV");
    fp_ColorTint = get_symbol(raylib_lib, "ColorTint");
    fp_ColorBrightness = get_symbol(raylib_lib, "ColorBrightness");
    fp_ColorContrast = get_symbol(raylib_lib, "ColorContrast");
    fp_ColorAlpha = get_symbol(raylib_lib, "ColorAlpha");
    fp_ColorAlphaBlend = get_symbol(raylib_lib, "ColorAlphaBlend");
    fp_ColorLerp = get_symbol(raylib_lib, "ColorLerp");
    fp_GetColor = get_symbol(raylib_lib, "GetColor");

    raylib_loaded = true;
}

RLAPI void InitWindow(int width, int height, const char* title) { load_raylib(); if(fp_InitWindow) fp_InitWindow(width, height, title); }
RLAPI void SetWindowIcon(Image icon) { load_raylib(); if(fp_SetWindowIcon) fp_SetWindowIcon(icon); }
RLAPI void SetWindowIcons(Image* icons, int count) { load_raylib(); if(fp_SetWindowIcons) fp_SetWindowIcons(icons,count); }
RLAPI Vector2 GetWindowPosition(void) { load_raylib(); return fp_GetWindowPosition?fp_GetWindowPosition(): (Vector2){0,0}; }
RLAPI Vector2 GetWindowScaleDPI(void) { load_raylib(); Vector2 result = {1.0f, 1.0f}; if(fp_GetWindowScaleDPI) result = fp_GetWindowScaleDPI(); return result; }

RLAPI void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) { load_raylib(); if(fp_DrawTriangle) fp_DrawTriangle(v1,v2,v3,color); }
RLAPI void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color) { load_raylib(); if(fp_DrawTriangleLines) fp_DrawTriangleLines(v1,v2,v3,color); }
RLAPI void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color) { load_raylib(); if(fp_DrawPoly) fp_DrawPoly(center,sides,radius,rotation,color); }
RLAPI void DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color) { load_raylib(); if(fp_DrawPolyLines) fp_DrawPolyLines(center,sides,radius,rotation,color); }

RLAPI Vector2 GetMousePosition(void) { load_raylib(); return fp_GetMousePosition?fp_GetMousePosition(): (Vector2){0,0}; }
RLAPI Vector2 GetMouseDelta(void) { load_raylib(); return fp_GetMouseDelta?fp_GetMouseDelta(): (Vector2){0,0}; }
RLAPI float GetMouseWheelMove(void) { load_raylib(); return fp_GetMouseWheelMove?fp_GetMouseWheelMove(): 0.0f; }
RLAPI Vector2 GetTouchPosition(int index) { load_raylib(); return fp_GetTouchPosition?fp_GetTouchPosition(index): (Vector2){0,0}; }
RLAPI Vector2 GetGesturePinchVector(void) { load_raylib(); return fp_GetGesturePinchVector?fp_GetGesturePinchVector(): (Vector2){0,0}; }
RLAPI Vector2 GetGestureDragVector(void) { load_raylib(); return fp_GetGestureDragVector?fp_GetGestureDragVector(): (Vector2){0,0}; }

RLAPI Image LoadImage(const char* fileName) { load_raylib(); return fp_LoadImage?fp_LoadImage(fileName): (Image){0}; }
RLAPI void UnloadImage(Image image) { load_raylib(); if(fp_UnloadImage) fp_UnloadImage(image); }

RLAPI Texture2D LoadTexture(const char* fileName) { load_raylib(); return fp_LoadTexture?fp_LoadTexture(fileName): (Texture2D){0}; }
RLAPI void UnloadTexture(Texture2D texture) { load_raylib(); if(fp_UnloadTexture) fp_UnloadTexture(texture); }
RLAPI bool IsTextureValid(Texture2D texture) { load_raylib(); return fp_IsTextureValid?fp_IsTextureValid(texture): false; }
RLAPI void DrawTexture(Texture2D texture, int posX, int posY, Color tint) { load_raylib(); if(fp_DrawTexture) fp_DrawTexture(texture, posX, posY, tint); }
RLAPI void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint) { load_raylib(); if(fp_DrawTextureEx) fp_DrawTextureEx(texture, position, rotation, scale, tint); }
RLAPI void DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint) { load_raylib(); if(fp_DrawTextureRec) fp_DrawTextureRec(texture, source, position, tint); }
RLAPI void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint) { load_raylib(); if(fp_DrawTexturePro) fp_DrawTexturePro(texture, source, dest, origin, rotation, tint); }

RLAPI Wave LoadWave(const char* fileName) { load_raylib(); return fp_LoadWave?fp_LoadWave(fileName): (Wave){0}; }
RLAPI void UnloadWave(Wave wave) { load_raylib(); if(fp_UnloadWave) fp_UnloadWave(wave); }
RLAPI bool IsWaveValid(Wave wave) { load_raylib(); return fp_IsWaveValid?fp_IsWaveValid(wave): false; }

RLAPI Sound LoadSound(const char* fileName) { load_raylib(); return fp_LoadSound?fp_LoadSound(fileName): (Sound){0}; }
RLAPI Sound LoadSoundFromWave(Wave wave) { load_raylib(); return fp_LoadSoundFromWave?fp_LoadSoundFromWave(wave): (Sound){0}; }
RLAPI void UnloadSound(Sound sound) { load_raylib(); if(fp_UnloadSound) fp_UnloadSound(sound); }
RLAPI bool IsSoundValid(Sound sound) { load_raylib(); return fp_IsSoundValid?fp_IsSoundValid(sound): false; }
RLAPI void PlaySound(Sound sound) { load_raylib(); if(fp_PlaySound) fp_PlaySound(sound); }
RLAPI void StopSound(Sound sound) { load_raylib(); if(fp_StopSound) fp_StopSound(sound); }
RLAPI void PauseSound(Sound sound) { load_raylib(); if(fp_PauseSound) fp_PauseSound(sound); }
RLAPI void ResumeSound(Sound sound) { load_raylib(); if(fp_ResumeSound) fp_ResumeSound(sound); }
RLAPI bool IsSoundPlaying(Sound sound) { load_raylib(); return fp_IsSoundPlaying?fp_IsSoundPlaying(sound): false; }
RLAPI void SetSoundVolume(Sound sound, float volume) { load_raylib(); if(fp_SetSoundVolume) fp_SetSoundVolume(sound, volume); }
RLAPI void SetSoundPitch(Sound sound, float pitch) { load_raylib(); if(fp_SetSoundPitch) fp_SetSoundPitch(sound, pitch); }
RLAPI void SetSoundPan(Sound sound, float pan) { load_raylib(); if(fp_SetSoundPan) fp_SetSoundPan(sound, pan); }

RLAPI Music LoadMusicStream(const char* fileName) { load_raylib(); return fp_LoadMusicStream?fp_LoadMusicStream(fileName): (Music){0}; }
RLAPI void UnloadMusicStream(Music music) { load_raylib(); if(fp_UnloadMusicStream) fp_UnloadMusicStream(music); }
RLAPI bool IsMusicValid(Music music) { load_raylib(); return fp_IsMusicValid?fp_IsMusicValid(music): false; }
RLAPI void PlayMusicStream(Music music) { load_raylib(); if(fp_PlayMusicStream) fp_PlayMusicStream(music); }
RLAPI void StopMusicStream(Music music) { load_raylib(); if(fp_StopMusicStream) fp_StopMusicStream(music); }
RLAPI void PauseMusicStream(Music music) { load_raylib(); if(fp_PauseMusicStream) fp_PauseMusicStream(music); }
RLAPI void ResumeMusicStream(Music music) { load_raylib(); if(fp_ResumeMusicStream) fp_ResumeMusicStream(music); }
RLAPI bool IsMusicStreamPlaying(Music music) { load_raylib(); return fp_IsMusicStreamPlaying?fp_IsMusicStreamPlaying(music): false; }
RLAPI void UpdateMusicStream(Music music) { load_raylib(); if(fp_UpdateMusicStream) fp_UpdateMusicStream(music); }
RLAPI void SetMusicVolume(Music music, float volume) { load_raylib(); if(fp_SetMusicVolume) fp_SetMusicVolume(music, volume); }
RLAPI void SetMusicPitch(Music music, float pitch) { load_raylib(); if(fp_SetMusicPitch) fp_SetMusicPitch(music, pitch); }
RLAPI void SetMusicPan(Music music, float pan) { load_raylib(); if(fp_SetMusicPan) fp_SetMusicPan(music, pan); }

RLAPI AudioStream LoadAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels) { load_raylib(); return fp_LoadAudioStream?fp_LoadAudioStream(sampleRate, sampleSize, channels): (AudioStream){0}; }
RLAPI void UnloadAudioStream(AudioStream stream) { load_raylib(); if(fp_UnloadAudioStream) fp_UnloadAudioStream(stream); }
RLAPI bool IsAudioStreamValid(AudioStream stream) { load_raylib(); return fp_IsAudioStreamValid?fp_IsAudioStreamValid(stream): false; }
RLAPI void PlayAudioStream(AudioStream stream) { load_raylib(); if(fp_PlayAudioStream) fp_PlayAudioStream(stream); }
RLAPI void StopAudioStream(AudioStream stream) { load_raylib(); if(fp_StopAudioStream) fp_StopAudioStream(stream); }
RLAPI void PauseAudioStream(AudioStream stream) { load_raylib(); if(fp_PauseAudioStream) fp_PauseAudioStream(stream); }
RLAPI void ResumeAudioStream(AudioStream stream) { load_raylib(); if(fp_ResumeAudioStream) fp_ResumeAudioStream(stream); }
RLAPI bool IsAudioStreamPlaying(AudioStream stream) { load_raylib(); return fp_IsAudioStreamPlaying?fp_IsAudioStreamPlaying(stream): false; }
RLAPI void SetAudioStreamVolume(AudioStream stream, float volume) { load_raylib(); if(fp_SetAudioStreamVolume) fp_SetAudioStreamVolume(stream, volume); }
RLAPI void SetAudioStreamPitch(AudioStream stream, float pitch) { load_raylib(); if(fp_SetAudioStreamPitch) fp_SetAudioStreamPitch(stream, pitch); }
RLAPI void SetAudioStreamPan(AudioStream stream, float pan) { load_raylib(); if(fp_SetAudioStreamPan) fp_SetAudioStreamPan(stream, pan); }

RLAPI void TraceLog(int logLevel, const char* text, ...) { 
    load_raylib(); 
    if(fp_TraceLog) {
        va_list args;
        va_start(args, text);
        char buffer[512];
        vsnprintf(buffer, sizeof(buffer), text, args);
        va_end(args);
        fp_TraceLog(logLevel, "%s", buffer);
    }
}

// Color functions
RLAPI Color Fade(Color color, float alpha) { load_raylib(); return fp_Fade ? fp_Fade(color, alpha) : color; }
RLAPI int ColorToInt(Color color) { load_raylib(); return fp_ColorToInt ? fp_ColorToInt(color) : 0; }
RLAPI Vector4 ColorNormalize(Color color) { load_raylib(); return fp_ColorNormalize ? fp_ColorNormalize(color) : (Vector4){0, 0, 0, 0}; }
RLAPI Color ColorFromNormalized(Vector4 normalized) { load_raylib(); return fp_ColorFromNormalized ? fp_ColorFromNormalized(normalized) : (Color){0, 0, 0, 0}; }
RLAPI Vector3 ColorToHSV(Color color) { load_raylib(); return fp_ColorToHSV ? fp_ColorToHSV(color) : (Vector3){0, 0, 0}; }
RLAPI Color ColorFromHSV(float hue, float saturation, float value) { load_raylib(); return fp_ColorFromHSV ? fp_ColorFromHSV(hue, saturation, value) : (Color){0, 0, 0, 0}; }
RLAPI Color ColorTint(Color color, Color tint) { load_raylib(); return fp_ColorTint ? fp_ColorTint(color, tint) : color; }
RLAPI Color ColorBrightness(Color color, float factor) { load_raylib(); return fp_ColorBrightness ? fp_ColorBrightness(color, factor) : color; }
RLAPI Color ColorContrast(Color color, float contrast) { load_raylib(); return fp_ColorContrast ? fp_ColorContrast(color, contrast) : color; }
RLAPI Color ColorAlpha(Color color, float alpha) { load_raylib(); return fp_ColorAlpha ? fp_ColorAlpha(color, alpha) : color; }
RLAPI Color ColorAlphaBlend(Color dst, Color src, Color tint) { load_raylib(); return fp_ColorAlphaBlend ? fp_ColorAlphaBlend(dst, src, tint) : dst; }
RLAPI Color ColorLerp(Color color1, Color color2, float factor) { load_raylib(); return fp_ColorLerp ? fp_ColorLerp(color1, color2, factor) : color1; }
RLAPI Color GetColor(unsigned int hexValue) { load_raylib(); return fp_GetColor ? fp_GetColor(hexValue) : (Color){0, 0, 0, 0}; }