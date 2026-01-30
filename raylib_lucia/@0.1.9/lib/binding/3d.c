#define MAX_CAMERAS 32
#define MAX_MODELS 512

static Camera3D cameras_3d[MAX_CAMERAS];
static bool cameras_3d_used[MAX_CAMERAS] = {0};

static Camera2D cameras_2d[MAX_CAMERAS];
static bool cameras_2d_used[MAX_CAMERAS] = {0};

static Model models[MAX_MODELS];
static bool models_used[MAX_MODELS] = {0};

int get_free_camera_slot_3d() {
    for (int i = 0; i < MAX_CAMERAS; i++) {
        if (!cameras_3d_used[i]) {
            return i;
        }
    }
    return -1;
}

int get_free_camera_slot_2d() {
    for (int i = 0; i < MAX_CAMERAS; i++) {
        if (!cameras_2d_used[i]) {
            return i;
        }
    }
    return -1;
}

RLAPI int bind_CreateCamera2D(float offset_x, float offset_y,
                            float target_x, float target_y,
                            float rotation,
                            float zoom) {
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ offset_x, offset_y };
    camera.target = (Vector2){ target_x, target_y };
    camera.rotation = rotation;
    camera.zoom = zoom;
    
    int id = get_free_camera_slot_2d();
    if (id == -1) return -1;
    cameras_2d[id] = camera;
    cameras_2d_used[id] = true;
    return id;
}

RLAPI void bind_SetCameraParameters2D(int id, float offset_x, float offset_y,
                                float target_x, float target_y,
                                float rotation,
                                float zoom) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_2d_used[id]) return;
    cameras_2d[id].offset = (Vector2){ offset_x, offset_y };
    cameras_2d[id].target = (Vector2){ target_x, target_y };
    cameras_2d[id].rotation = rotation;
    cameras_2d[id].zoom = zoom;
}

RLAPI bool bind_IsCameraValid2D(int id) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_2d_used[id]) return false;
    return true;
}

RLAPI int bind_CreateCamera3D(float position_x, float position_y, float position_z,
                            float target_x, float target_y, float target_z,
                            float up_x, float up_y, float up_z,
                            float fovy) {
    Camera3D camera = { 0 };
    camera.position = (Vector3){ position_x, position_y, position_z };
    camera.target = (Vector3){ target_x, target_y, target_z };
    camera.up = (Vector3){ up_x, up_y, up_z };
    camera.fovy = fovy;
    
    int id = get_free_camera_slot_3d();
    if (id == -1) return -1;
    cameras_3d[id] = camera;
    cameras_3d_used[id] = true;
    return id;
}

RLAPI void bind_SetCameraParameters3D(int id, float position_x, float position_y, float position_z,
                                float target_x, float target_y, float target_z,
                                float up_x, float up_y, float up_z,
                                float fovy) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_3d_used[id]) return;
    cameras_3d[id].position = (Vector3){ position_x, position_y, position_z };
    cameras_3d[id].target = (Vector3){ target_x, target_y, target_z };
    cameras_3d[id].up = (Vector3){ up_x, up_y, up_z };
    cameras_3d[id].fovy = fovy;
}

RLAPI void bind_SetCameraProjection3D(int id, int projection) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_3d_used[id]) return;
    cameras_3d[id].projection = projection;
}

RLAPI bool bind_IsCameraValid3D(int id) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_3d_used[id]) return false;
    return true;
}

RLAPI void bind_UpdateCamera(int id, int mode) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_3d_used[id]) return;
    UpdateCamera(&cameras_3d[id], mode);
}

RLAPI void bind_UpdateCameraPro(int id,
                                float movement_x, float movement_y, float movement_z,
                                float rotation_x, float rotation_y, float rotation_z,
                                float zoom) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_3d_used[id]) return;
    Vector3 movement = { movement_x, movement_y, movement_z };
    Vector3 rotation = { rotation_x, rotation_y, rotation_z };
    UpdateCameraPro(&cameras_3d[id], movement, rotation, zoom);
}

RLAPI void bind_BeginMode2D(int id) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_2d_used[id]) return;
    BeginMode2D(cameras_2d[id]);
}

RLAPI void bind_BeginMode3D(int id) {
    if (id < 0 || id >= MAX_CAMERAS || !cameras_3d_used[id]) return;
    BeginMode3D(cameras_3d[id]);
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

RLAPI const char* bind_GetModelBoundingBox(int id) {
    if (id < 0 || id >= MAX_MODELS || !models_used[id]) return NULL;
    BoundingBox box = GetModelBoundingBox(models[id]);
    snprintf(retbuf, sizeof(retbuf), "%.6f|%.6f|%.6f|%.6f|%.6f|%.6f",
             box.min.x, box.min.y, box.min.z,
             box.max.x, box.max.y, box.max.z);
    return retbuf;
}

RLAPI void bind_DrawBoundingBox(float min_x, float min_y, float min_z,
                             float max_x, float max_y, float max_z,
                             int r, unsigned char g, unsigned char b, unsigned char a) {
    BoundingBox box = { 0 };
    box.min = (Vector3){ min_x, min_y, min_z };
    box.max = (Vector3){ max_x, max_y, max_z };
    Color color = { (unsigned char)r, g, b, a };
    DrawBoundingBox(box, color);
}

RLAPI bool bind_CheckCollisionBoxes(float box1_min_x, float box1_min_y, float box1_min_z,
                                 float box1_max_x, float box1_max_y, float box1_max_z,
                                 float box2_min_x, float box2_min_y, float box2_min_z,
                                 float box2_max_x, float box2_max_y, float box2_max_z) {
    BoundingBox box1 = { 0 };
    box1.min = (Vector3){ box1_min_x, box1_min_y, box1_min_z };
    box1.max = (Vector3){ box1_max_x, box1_max_y, box1_max_z };
    BoundingBox box2 = { 0 };
    box2.min = (Vector3){ box2_min_x, box2_min_y, box2_min_z };
    box2.max = (Vector3){ box2_max_x, box2_max_y, box2_max_z };
    return CheckCollisionBoxes(box1, box2);
}

RLAPI bool bind_CheckCollisionBoxSphere(float box_min_x, float box_min_y, float box_min_z,
                                 float box_max_x, float box_max_y, float box_max_z,
                                 float center_x, float center_y, float center_z,
                                 float radius) {
    BoundingBox box = { 0 };
    box.min = (Vector3){ box_min_x, box_min_y, box_min_z };
    box.max = (Vector3){ box_max_x, box_max_y, box_max_z };
    Vector3 center = { center_x, center_y, center_z };
    return CheckCollisionBoxSphere(box, center, radius);
}
