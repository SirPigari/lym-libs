#define MAX_CAMERAS 32
#define MAX_MODELS 512

static Camera cameras[MAX_CAMERAS];
static bool cameras_used[MAX_CAMERAS] = {0};

static Model models[MAX_MODELS];
static bool models_used[MAX_MODELS] = {0};

int get_free_camera_slot() {
    for (int i = 0; i < MAX_CAMERAS; i++) {
        if (!cameras_used[i]) {
            return i;
        }
    }
    return -1;
}

RLAPI int bind_CreateCamera(float position_x, float position_y, float position_z,
                            float target_x, float target_y, float target_z,
                            float up_x, float up_y, float up_z,
                            float fovy) {
    Camera camera = { 0 };
    camera.position = (Vector3){ position_x, position_y, position_z };
    camera.target = (Vector3){ target_x, target_y, target_z };
    camera.up = (Vector3){ up_x, up_y, up_z };
    camera.fovy = fovy;
    
    int id = get_free_camera_slot();
    if (id == -1) return -1;
    cameras[id] = camera;
    cameras_used[id] = true;
    return id;
}

RLAPI void bind_SetCameraParameters(int id, float position_x, float position_y, float position_z,
                                float target_x, float target_y, float target_z,
                                float up_x, float up_y, float up_z,
                                float fovy) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_used[id]) return;
    cameras[id].position = (Vector3){ position_x, position_y, position_z };
    cameras[id].target = (Vector3){ target_x, target_y, target_z };
    cameras[id].up = (Vector3){ up_x, up_y, up_z };
    cameras[id].fovy = fovy;
}

RLAPI void bind_SetCameraProjection(int id, int projection) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_used[id]) return;
    cameras[id].projection = projection;
}

RLAPI bool bind_IsCameraValid(int id) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_used[id]) return false;
    return true;
}

RLAPI void bind_BeginMode3D(int id) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_used[id]) return;
    BeginMode3D(cameras[id]);
}

int alloc_id_model() {
    for (int i = 0; i < MAX_MODELS; i++) {
        if (!models_used[i]) {
            models_used[i] = true;
            return i;
        }
    }
    return -1;
}

void free_id_model(int id) {
    models_used[id] = false;
    if (IsModelValid(models[id])) {
        UnloadModel(models[id]);
    }
}

RLAPI int bind_LoadModel(const char* fileName) {
    int idx = alloc_id_model();
    if (idx == -1) return -1;
    models[idx] = LoadModel(fileName);
    if (!IsModelValid(models[idx])) {
        free_id_model(idx);
        return -1;
    }
    return idx;
}

RLAPI void bind_UnloadModel(int id) {
    if (id < 0 || id >= MAX_MODELS || !models_used[id]) return;
    free_id_model(id);
}

RLAPI void bind_UnloadAllModels() {
    for (int i = 0; i < MAX_MODELS; i++) {
        if (models_used[i]) {
            free_id_model(i);
        }
    }
}

RLAPI bool bind_IsModelValid(int id) {
    if (id < 0 || id >= MAX_MODELS || !models_used[id]) return false;
    return IsModelValid(models[id]);
}

RLAPI void bind_DrawModel(int id, float pos_x, float pos_y, float pos_z, float scale, int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_MODELS || !models_used[id]) return;
    Vector3 position = { pos_x, pos_y, pos_z };
    Color tint = { (unsigned char)r, g, b, a };
    DrawModel(models[id], position, scale, tint);
}

RLAPI void bind_DrawModelEx(int id, float pos_x, float pos_y, float pos_z,
                            float rot_axis_x, float rot_axis_y, float rot_axis_z,
                            float rot_angle,
                            float scale_x, float scale_y, float scale_z,
                            int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_MODELS || !models_used[id]) return;
    Vector3 position = { pos_x, pos_y, pos_z };
    Vector3 rotationAxis = { rot_axis_x, rot_axis_y, rot_axis_z };
    Vector3 scale = { scale_x, scale_y, scale_z };
    Color tint = { (unsigned char)r, g, b, a };
    DrawModelEx(models[id], position, rotationAxis, rot_angle, scale, tint);
}

RLAPI void bind_DrawModelWires(int id, float pos_x, float pos_y, float pos_z, float scale, int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_MODELS || !models_used[id]) return;
    Vector3 position = { pos_x, pos_y, pos_z };
    Color tint = { (unsigned char)r, g, b, a };
    DrawModelWires(models[id], position, scale, tint);
}

RLAPI void bind_DrawModelWiresEx(int id, float pos_x, float pos_y, float pos_z,
                                float rot_axis_x, float rot_axis_y, float rot_axis_z,
                                float rot_angle,
                                float scale_x, float scale_y, float scale_z,
                                int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_MODELS || !models_used[id]) return;
    Vector3 position = { pos_x, pos_y, pos_z };
    Vector3 rotationAxis = { rot_axis_x, rot_axis_y, rot_axis_z };
    Vector3 scale = { scale_x, scale_y, scale_z };
    Color tint = { (unsigned char)r, g, b, a };
    DrawModelWiresEx(models[id], position, rotationAxis, rot_angle, scale, tint);
}

RLAPI void bind_DrawModelPoints(int id, float pos_x, float pos_y, float pos_z, float scale, int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_MODELS || !models_used[id]) return;
    Vector3 position = { pos_x, pos_y, pos_z };
    Color tint = { (unsigned char)r, g, b, a };
    DrawModelPoints(models[id], position, scale, tint);
}

RLAPI void bind_DrawModelPointsEx(int id, float pos_x, float pos_y, float pos_z,
                                float rot_axis_x, float rot_axis_y, float rot_axis_z,
                                float rot_angle,
                                float scale_x, float scale_y, float scale_z,
                                int r, unsigned char g, unsigned char b, unsigned char a) {
    if (id < 0 || id >= MAX_MODELS || !models_used[id]) return;
    Vector3 position = { pos_x, pos_y, pos_z };
    Vector3 rotationAxis = { rot_axis_x, rot_axis_y, rot_axis_z };
    Vector3 scale = { scale_x, scale_y, scale_z };
    Color tint = { (unsigned char)r, g, b, a };
    DrawModelPointsEx(models[id], position, rotationAxis, rot_angle, scale, tint);
}
