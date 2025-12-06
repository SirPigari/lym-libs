#include "load_raylib.c"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#if defined(_WIN32)
    #if defined(__TINYC__)
        #define __declspec(x) __attribute__((x))
    #endif
    #if defined(BUILD_LIBTYPE_SHARED)
        #define RLAPI __declspec(dllexport)
    #elif defined(USE_LIBTYPE_SHARED)
        #define RLAPI __declspec(dllimport)
    #endif
#else
    #if defined(BUILD_LIBTYPE_SHARED)
        #define RLAPI __attribute__((visibility("default")))
    #endif
#endif

#ifndef RLAPI
    #define RLAPI
#endif

// unsafe return buffer
// if fails its your skill issue
static char retbuf[64];

// audio stuff
#include "audio.c"
// image stuff
#include "image.c"

// void SetWindowIcon(Image image);
RLAPI void bind_SetWindowIcon(const char *path) {
    Image img = LoadImage(path);
    SetWindowIcon(img);
    UnloadImage(img);
}

// void SetWindowIcons(Image *images, int count);
RLAPI void bind_SetWindowIcons(const char *paths, int count) {
    Image *images = (Image *)malloc(count * sizeof(Image));
    int i = 0;

    const char *start = paths;
    const char *sep;
    while (i < count && start && *start) {
        sep = strchr(start, '|');
        size_t len = sep ? (size_t)(sep - start) : strlen(start);
        char *tmpPath = (char *)malloc(len + 1);
        memcpy(tmpPath, start, len);
        tmpPath[len] = '\0';

        images[i] = LoadImage(tmpPath);
        free(tmpPath);

        i++;
        start = sep ? sep + 1 : NULL;
    }

    SetWindowIcons(images, i);

    for (int j = 0; j < i; j++) {
        UnloadImage(images[j]);
    }
    free(images);
}

// RLAPI Vector2 GetWindowPosition(void);
RLAPI const char* bind_GetWindowPosition() {
    Vector2 pos_v2 = GetWindowPosition();
    snprintf(retbuf, sizeof(retbuf), "%d|%d", (int)pos_v2.x, (int)pos_v2.y);
    return retbuf;
}
// RLAPI Vector2 GetWindowScaleDPI(void);
RLAPI const char* bind_GetWindowScaleDPI() {
    Vector2 scale_v2 = GetWindowScaleDPI();
    snprintf(retbuf, sizeof(retbuf), "%f|%f", scale_v2.x, scale_v2.y);
    return retbuf;
}

// RLAPI void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
RLAPI void bind_DrawTriangle(const char *vertices_str, int r, unsigned char g, unsigned char b, unsigned char a) {
    float verts[6] = {0};
    char *copy = strdup(vertices_str);
    if (!copy) return;

    char *token = strtok(copy, "|");
    int i = 0;
    while (token && i < 6) {
        verts[i] = (float)atof(token);
        token = strtok(NULL, "|");
        i++;
    }
    free(copy);

    if (i != 6) {
        printf("Invalid vertices string: %s\n", vertices_str);
        return;
    }

    Vector2 v1 = { (float)verts[0], (float)verts[1]};
    Vector2 v2 = { (float)verts[2], (float)verts[3]};
    Vector2 v3 = { (float)verts[4], (float)verts[5]};
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    DrawTriangle(v1, v2, v3, color);
}

// RLAPI void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
RLAPI void bind_DrawTriangleLines(const char *vertices_str, int r, unsigned char g, unsigned char b, unsigned char a) {
    float verts[6] = {0};
    char *copy = strdup(vertices_str);
    if (!copy) return;

    char *token = strtok(copy, "|");
    int i = 0;
    while (token && i < 6) {
        verts[i] = (float)atof(token);
        token = strtok(NULL, "|");
        i++;
    }
    free(copy);

    if (i != 6) {
        printf("Invalid vertices string: %s\n", vertices_str);
        return;
    }

    Vector2 v1 = { (float)verts[0], (float)verts[1]};
    Vector2 v2 = { (float)verts[2], (float)verts[3]};
    Vector2 v3 = { (float)verts[4], (float)verts[5]};
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    DrawTriangleLines(v1, v2, v3, color);
}

// RLAPI void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);
RLAPI void bind_DrawPoly(const char *center_str, int sides, float radius, float rotation, int r, unsigned char g, unsigned char b, unsigned char a) {
    float coords[2] = {0};
    char *copy = strdup(center_str);
    if (!copy) return;

    char *token = strtok(copy, "|");
    int i = 0;
    while (token && i < 2) {
        coords[i] = (float)atof(token);
        token = strtok(NULL, "|");
        i++;
    }
    free(copy);

    if (i != 2) {
        printf("Invalid center string: %s\n", center_str);
        return;
    }

    Vector2 center = { (float)coords[0], (float)coords[1]};
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    DrawPoly(center, sides, radius, rotation, color);
}

// RLAPI void DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color);
RLAPI void bind_DrawPolyLines(const char *center_str, int sides, float radius, float rotation, int r, unsigned char g, unsigned char b, unsigned char a) {
    float coords[2] = {0};
    char *copy = strdup(center_str);
    if (!copy) return;

    char *token = strtok(copy, "|");
    int i = 0;
    while (token && i < 2) {
        coords[i] = (float)atof(token);
        token = strtok(NULL, "|");
        i++;
    }
    free(copy);

    if (i != 2) {
        printf("Invalid center string: %s\n", center_str);
        return;
    }

    Vector2 center = { (float)coords[0], (float)coords[1]};
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    DrawPolyLines(center, sides, radius, rotation, color);
}

// RLAPI Vector2 GetMousePosition(void); 
RLAPI const char* bind_GetMousePosition() {
    Vector2 pos = GetMousePosition();
    snprintf(retbuf, sizeof(retbuf), "%f|%f", pos.x, pos.y);
    return retbuf;
}

// RLAPI Vector2 GetMouseDelta(void);
RLAPI const char* bind_GetMouseDelta() {
    Vector2 delta = GetMouseDelta();
    snprintf(retbuf, sizeof(retbuf), "%f|%f", delta.x, delta.y);
    return retbuf;
}

// RLAPI float GetMouseWheelMove(void); 
RLAPI const char* bind_GetMouseWheelMoveV() {
    float wheel = GetMouseWheelMove();
    snprintf(retbuf, sizeof(retbuf), "%f", wheel);
    return retbuf;
}

// RLAPI Vector2 GetTouchPosition(int index);
RLAPI const char* bind_GetTouchPosition(int index) {
    Vector2 pos = GetTouchPosition(index);
    snprintf(retbuf, sizeof(retbuf), "%f|%f", pos.x, pos.y);
    return retbuf;
}

// RLAPI Vector2 GetGesturePinchVector(void);
RLAPI const char* bind_GetGesturePinchVector() {
    Vector2 pinch = GetGesturePinchVector();
    snprintf(retbuf, sizeof(retbuf), "%f|%f", pinch.x, pinch.y);
    return retbuf;
}

// RLAPI Vector2 GetGestureDragVector(void);
RLAPI const char* bind_GetGestureDragVector() {
    Vector2 drag = GetGestureDragVector();
    snprintf(retbuf, sizeof(retbuf), "%f|%f", drag.x, drag.y);
    return retbuf;
}

RLAPI const char* bind_ColorNormalize(int r, int g, int b, int a) {
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    Vector4 norm = ColorNormalize(color);
    snprintf(retbuf, sizeof(retbuf), "%f|%f|%f|%f", norm.x, norm.y, norm.z, norm.w);
    return retbuf;
}

// RLAPI Color Fade(Color color, float alpha);
RLAPI const char* bind_Fade(int r, int g, int b, int a, float alpha) {
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    Color result = Fade(color, alpha);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

// RLAPI int ColorToInt(Color color);
RLAPI int bind_ColorToInt(int r, int g, int b, int a) {
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    return ColorToInt(color);
}

// RLAPI Color ColorFromNormalized(Vector4 normalized);
RLAPI const char* bind_ColorFromNormalized(float x, float y, float z, float w) {
    Vector4 normalized = { x, y, z, w };
    Color result = ColorFromNormalized(normalized);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

// RLAPI Vector3 ColorToHSV(Color color);
RLAPI const char* bind_ColorToHSV(int r, int g, int b, int a) {
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    Vector3 hsv = ColorToHSV(color);
    snprintf(retbuf, sizeof(retbuf), "%f|%f|%f", hsv.x, hsv.y, hsv.z);
    return retbuf;
}

// RLAPI Color ColorFromHSV(float hue, float saturation, float value);
RLAPI const char* bind_ColorFromHSV(float hue, float saturation, float value) {
    Color result = ColorFromHSV(hue, saturation, value);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

// RLAPI Color ColorTint(Color color, Color tint);
RLAPI const char* bind_ColorTint(int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2) {
    Color color = { (unsigned char)r1, (unsigned char)g1, (unsigned char)b1, (unsigned char)a1 };
    Color tint = { (unsigned char)r2, (unsigned char)g2, (unsigned char)b2, (unsigned char)a2 };
    Color result = ColorTint(color, tint);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

// RLAPI Color ColorBrightness(Color color, float factor);
RLAPI const char* bind_ColorBrightness(int r, int g, int b, int a, float factor) {
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    Color result = ColorBrightness(color, factor);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

// RLAPI Color ColorContrast(Color color, float contrast);
RLAPI const char* bind_ColorContrast(int r, int g, int b, int a, float contrast) {
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    Color result = ColorContrast(color, contrast);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

// RLAPI Color ColorAlpha(Color color, float alpha);
RLAPI const char* bind_ColorAlpha(int r, int g, int b, int a, float alpha) {
    Color color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
    Color result = ColorAlpha(color, alpha);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

// RLAPI Color ColorAlphaBlend(Color dst, Color src, Color tint);
RLAPI const char* bind_ColorAlphaBlend(int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int r3, int g3, int b3, int a3) {
    Color dst = { (unsigned char)r1, (unsigned char)g1, (unsigned char)b1, (unsigned char)a1 };
    Color src = { (unsigned char)r2, (unsigned char)g2, (unsigned char)b2, (unsigned char)a2 };
    Color tint = { (unsigned char)r3, (unsigned char)g3, (unsigned char)b3, (unsigned char)a3 };
    Color result = ColorAlphaBlend(dst, src, tint);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

// RLAPI Color ColorLerp(Color color1, Color color2, float factor);
RLAPI const char* bind_ColorLerp(int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, float factor) {
    Color color1 = { (unsigned char)r1, (unsigned char)g1, (unsigned char)b1, (unsigned char)a1 };
    Color color2 = { (unsigned char)r2, (unsigned char)g2, (unsigned char)b2, (unsigned char)a2 };
    Color result = ColorLerp(color1, color2, factor);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

// RLAPI Color GetColor(unsigned int hexValue);
RLAPI const char* bind_GetColor(unsigned int hexValue) {
    Color result = GetColor(hexValue);
    snprintf(retbuf, sizeof(retbuf), "%d|%d|%d|%d", result.r, result.g, result.b, result.a);
    return retbuf;
}

RLAPI void bind_UnloadAllResources() {
    // Unload all audio
    bind_UnloadAllWaves();
    bind_UnloadAllSounds();
    bind_UnloadAllMusic();
    bind_UnloadAllAudioStreams();
    // Unload all images/textures
    bind_UnloadAllTextures();
}