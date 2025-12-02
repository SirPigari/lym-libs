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
    char *result = (char *)malloc(32);
    if (result != NULL) {
        snprintf(result, 32, "%d|%d", (int)pos_v2.x, (int)pos_v2.y);
    }
    return result;
}
// RLAPI Vector2 GetWindowScaleDPI(void);
RLAPI const char* bind_GetWindowScaleDPI() {
    Vector2 scale_v2 = GetWindowScaleDPI();
    char *result = (char *)malloc(64);
    if (result != NULL) {
        snprintf(result, 64, "%f|%f", scale_v2.x, scale_v2.y);
    }
    return result;
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
    char* result = (char*)malloc(32);
    if (result) snprintf(result, 32, "%f|%f", pos.x, pos.y);
    return result;
}

// RLAPI Vector2 GetMouseDelta(void);
RLAPI const char* bind_GetMouseDelta() {
    Vector2 delta = GetMouseDelta();
    char* result = (char*)malloc(32);
    if (result) snprintf(result, 32, "%f|%f", delta.x, delta.y);
    return result;
}

// RLAPI float GetMouseWheelMove(void); 
RLAPI const char* bind_GetMouseWheelMoveV() {
    float wheel = GetMouseWheelMove();
    char* result = (char*)malloc(16);
    if (result) snprintf(result, 16, "%f", wheel);
    return result;
}

// RLAPI Vector2 GetTouchPosition(int index);
RLAPI const char* bind_GetTouchPosition(int index) {
    Vector2 pos = GetTouchPosition(index);
    char* result = (char*)malloc(32);
    if (result) snprintf(result, 32, "%f|%f", pos.x, pos.y);
    return result;
}

// RLAPI Vector2 GetGesturePinchVector(void);
RLAPI const char* bind_GetGesturePinchVector() {
    Vector2 pinch = GetGesturePinchVector();
    char* result = (char*)malloc(32);
    if (result) snprintf(result, 32, "%f|%f", pinch.x, pinch.y);
    return result;
}

// RLAPI Vector2 GetGestureDragVector(void);
RLAPI const char* bind_GetGestureDragVector() {
    Vector2 drag = GetGestureDragVector();
    char* result = (char*)malloc(32);
    if (result) snprintf(result, 32, "%f|%f", drag.x, drag.y);
    return result;
}
