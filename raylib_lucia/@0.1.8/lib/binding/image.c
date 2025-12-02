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

RLAPI const char* bind_LoadTexture(const char *fileName) {
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

    Texture2D t = LoadTexture(fileName);
    textures[idx] = t;
    if (!IsTextureValid(textures[idx])) return "ERR: Failed to load texture";

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
