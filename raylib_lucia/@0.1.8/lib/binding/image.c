#define MAX_TEXTURES 1024
#define MAX_PATH_LENGTH 260


static Texture2D textures[MAX_TEXTURES];
static bool textures_used[MAX_TEXTURES] = {0};
static char texture_paths[MAX_TEXTURES][MAX_PATH_LENGTH];

static int alloc_id_img(int max) {
    for (int i = 0; i < max; i++) {
        if (!textures_used[i]) {
            textures_used[i] = true;
            return i;
        }
    }
    return -1;
}

static void free_id_img(int id) {
    textures_used[id] = false;
}

RLAPI const char* bind_LoadTexture(const char* fileName) {
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (textures_used[i] && strcmp(texture_paths[i], fileName) == 0) {
            snprintf(retbuf, sizeof(retbuf), "ID: %d", i);
            return retbuf;
        }
    }

    int idx = alloc_id_img(MAX_TEXTURES);
    if (idx == -1) {
        snprintf(retbuf, sizeof(retbuf), "ERR: No free texture slots, max textures: %d", MAX_TEXTURES);
        return retbuf;
    }

    Texture2D t;

#if defined(_WIN32)
    int wlen = MultiByteToWideChar(CP_UTF8, 0, fileName, -1, NULL, 0);
    if (wlen == 0) return "ERR: UTF-8 conversion failed";

    wchar_t* wfilename = (wchar_t*)malloc(wlen * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, fileName, -1, wfilename, wlen);

    FILE* f = _wfopen(wfilename, L"rb");
    free(wfilename);
    if (!f) return "ERR: Failed to open file";

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char* buffer = (unsigned char*)malloc(fsize);
    fread(buffer, 1, fsize, f);
    fclose(f);

    const char* ext = strrchr(fileName, '.');
    const char* slash1 = strrchr(fileName, '/');
    const char* slash2 = strrchr(fileName, '\\');
    const char* last_slash = (slash1 > slash2) ? slash1 : slash2;
    if (!ext || (last_slash && ext < last_slash)) ext = ".png";

    Image img = LoadImageFromMemory(ext, buffer, fsize);
    free(buffer);

    if (img.data == NULL) return "ERR: Failed to load image";

    t = LoadTextureFromImage(img);
    UnloadImage(img);

#else
    t = LoadTexture(fileName);
#endif

    if (!IsTextureValid(t)) return "ERR: Failed to load texture";

    textures[idx] = t;
    strncpy(texture_paths[idx], fileName, MAX_PATH_LENGTH - 1);
    texture_paths[idx][MAX_PATH_LENGTH - 1] = 0;

    snprintf(retbuf, sizeof(retbuf), "ID: %d", idx);
    return retbuf;
}

RLAPI const char* bind_UnloadTexture(int id) {
    if (id < 0 || id >= MAX_TEXTURES || !textures_used[id]) return "ERR: Invalid texture id";
    UnloadTexture(textures[id]);
    TraceLog(LOG_DEBUG, "Unloading texture ID: %d, Path: %s", id, texture_paths[id]);
    free_id_img(id);
    return "";
}

RLAPI void bind_UnloadAllTextures() {
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (textures_used[i]) {
            TraceLog(LOG_DEBUG, "Unloading texture ID: %d, Path: %s", i, texture_paths[i]);
            UnloadTexture(textures[i]);
            textures_used[i] = false;
        }
    }
}

RLAPI bool bind_IsTextureValid(int id) {
    if (id < 0 || id >= MAX_TEXTURES || !textures_used[id]) return false;
    return IsTextureValid(textures[id]);
}

RLAPI void bind_DrawTexture(int id, int posX, int posY, int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_TEXTURES || !textures_used[id]) return;
    Color tint = { (unsigned char)r, g, b, a };
    DrawTexture(textures[id], posX, posY, tint);
}

RLAPI void bind_DrawTextureEx(int id, int posX, int posY, float rotation, float scale, int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_TEXTURES || !textures_used[id]) return;
    Color tint = { (unsigned char)r, g, b, a };
    DrawTextureEx(textures[id], (Vector2){ (float)posX, (float)posY }, rotation, scale, tint);
}

RLAPI void bind_DrawTextureRec(int id, int srcX, int srcY, int srcW, int srcH, int posX, int posY, int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_TEXTURES || !textures_used[id]) return;
    Color tint = { (unsigned char)r, g, b, a };
    DrawTextureRec(
        textures[id],
        (Rectangle){ (float)srcX, (float)srcY, (float)srcW, (float)srcH },
        (Vector2){ (float)posX, (float)posY },
        tint
    );
}

RLAPI void bind_DrawTexturePro(int id, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH, float orig_x, float orig_y, float rotation, int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_TEXTURES || !textures_used[id]) return;
    Color tint = { (unsigned char)r, g, b, a };
    DrawTexturePro(
        textures[id],
        (Rectangle){ (float)srcX, (float)srcY, (float)srcW, (float)srcH },
        (Rectangle){ (float)destX, (float)destY, (float)destW, (float)destH },
        (Vector2){ orig_x, orig_y },
        rotation,
        tint
    );
}

RLAPI int bind_GetTextureWidth(int id) {
    if (id < 0 || id >= MAX_TEXTURES || !textures_used[id]) return -1;
    return textures[id].width;
}

RLAPI int bind_GetTextureHeight(int id) {
    if (id < 0 || id >= MAX_TEXTURES || !textures_used[id]) return -1;
    return textures[id].height;
}

RLAPI const char* bind_GetTexturePath(int id) {
    if (id < 0 || id >= MAX_TEXTURES || !textures_used[id]) return "";
    return texture_paths[id];
}

RLAPI const char* bind_DrawImage(const char* fileName,
                                 int posX, int posY,
                                 int width, int height,
                                 float rotation,
                                 float origin_x, float origin_y,
                                 int r, int g, int b, int a) {
    const char* tex = bind_LoadTexture(fileName);
    if (strncmp(tex, "ERR:", 4) == 0)
        return tex;

    
    int id = atoi(tex + 4);
    Texture2D t = textures[id];
    
    if (width == 0)  width  = t.width;
    if (height == 0) height = t.height;

    float ox = (width  * 0.5f) + origin_x;
    float oy = (height * 0.5f) + origin_y;

    Color tint = (Color){
        (unsigned char)r,
        (unsigned char)g,
        (unsigned char)b,
        (unsigned char)a
    };

    DrawTexturePro(
        t,
        (Rectangle){ 0, 0, (float)t.width, (float)t.height },
        (Rectangle){ (float)posX, (float)posY, (float)width, (float)height },
        (Vector2){ ox, oy },
        rotation,
        tint
    );

    return "";
}
