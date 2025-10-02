// i dont care about your '.h' files
// fuck it we including a c file
// skill issue

#define MAX_WAVES   256
#define MAX_SOUNDS  512
#define MAX_MUSICS  128
#define MAX_STREAMS 128

static Wave waves[MAX_WAVES];
static bool waves_used[MAX_WAVES] = {0};

static Sound sounds[MAX_SOUNDS];
static bool sounds_used[MAX_SOUNDS] = {0};

static Music musics[MAX_MUSICS];
static bool musics_used[MAX_MUSICS] = {0};

static AudioStream streams[MAX_STREAMS];
static bool streams_used[MAX_STREAMS] = {0};

// unsafe return buffer
// if fails its your skill issue
static char retbuf[64];
// helpers
static int alloc_id(bool *used, int max) {
    for (int i = 0; i < max; i++) {
        if (!used[i]) {
            used[i] = true;
            return i;
        }
    }
    return -1;
}

static void free_id(bool *used, int id) {
    used[id] = false;
}

// Waves
RLAPI const char* bind_PlayWave(const char *fileName, float volume, float pitch, float pan) {
    int idx = alloc_id(waves_used, MAX_WAVES);
    if (idx == -1) return "ERR: No free wave slots";
    waves[idx] = LoadWave(fileName);
    if (!IsWaveValid(waves[idx])) return "ERR: Failed to load wave";
    Sound s = LoadSoundFromWave(waves[idx]);
    if (!IsSoundValid(s)) return "ERR: Failed to create sound from wave";
    SetSoundVolume(s, volume);
    SetSoundPitch(s, pitch);
    SetSoundPan(s, pan);
    PlaySound(s);
    UnloadSound(s);
    UnloadWave(waves[idx]);
    free_id(waves_used, idx);
    return "";
}

// Sounds
RLAPI const char* bind_PlaySound(const char *fileName, float volume, float pitch, float pan) {
    int idx = alloc_id(sounds_used, MAX_SOUNDS);
    if (idx == -1) return "ERR: No free sound slots";
    sounds[idx] = LoadSound(fileName);
    if (!IsSoundValid(sounds[idx])) return "ERR: Failed to load sound";
    SetSoundVolume(sounds[idx], volume);
    SetSoundPitch(sounds[idx], pitch);
    SetSoundPan(sounds[idx], pan);
    PlaySound(sounds[idx]);
    snprintf(retbuf, sizeof(retbuf), "OUT: %d", idx);
    return retbuf;
}

RLAPI const char* bind_StopSound(int id) {
    if (id < 0 || id >= MAX_SOUNDS || !sounds_used[id]) return "ERR: Invalid sound id";
    StopSound(sounds[id]);
    return "";
}

RLAPI const char* bind_UnloadSound(int id) {
    if (id < 0 || id >= MAX_SOUNDS || !sounds_used[id]) return "ERR: Invalid sound id";
    UnloadSound(sounds[id]);
    free_id(sounds_used, id);
    return "";
}

RLAPI const char* bind_PauseSound(int id) {
    if (id < 0 || id >= MAX_SOUNDS || !sounds_used[id]) return "ERR: Invalid sound id";
    PauseSound(sounds[id]);
    return "";
}

RLAPI const char* bind_ResumeSound(int id) {
    if (id < 0 || id >= MAX_SOUNDS || !sounds_used[id]) return "ERR: Invalid sound id";
    ResumeSound(sounds[id]);
    return "";
}

RLAPI const char* bind_IsSoundPlaying(int id) {
    if (id < 0 || id >= MAX_SOUNDS || !sounds_used[id]) return "ERR: Invalid sound id";
    return IsSoundPlaying(sounds[id]) ? "TRUE" : "FALSE";
}

// Music
RLAPI const char* bind_PlayMusicStream(const char *fileName, float volume, float pitch, float pan) {
    int idx = alloc_id(musics_used, MAX_MUSICS);
    if (idx == -1) return "ERR: No free music slots";
    musics[idx] = LoadMusicStream(fileName);
    if (!IsMusicValid(musics[idx])) return "ERR: Failed to load music";
    SetMusicVolume(musics[idx], volume);
    SetMusicPitch(musics[idx], pitch);
    SetMusicPan(musics[idx], pan);
    PlayMusicStream(musics[idx]);
    UpdateMusicStream(musics[idx]);
    snprintf(retbuf, sizeof(retbuf), "OUT: %d", idx);
    return retbuf;
}

RLAPI const char* bind_StopMusicStream(int id) {
    if (id < 0 || id >= MAX_MUSICS || !musics_used[id]) return "ERR: Invalid music id";
    StopMusicStream(musics[id]);
    return "";
}

RLAPI const char* bind_UnloadMusicStream(int id) {
    if (id < 0 || id >= MAX_MUSICS || !musics_used[id]) return "ERR: Invalid music id";
    UnloadMusicStream(musics[id]);
    free_id(musics_used, id);
    return "";
}

RLAPI const char* bind_PauseMusicStream(int id) {
    if (id < 0 || id >= MAX_MUSICS || !musics_used[id]) return "ERR: Invalid music id";
    PauseMusicStream(musics[id]);
    return "";
}

RLAPI const char* bind_ResumeMusicStream(int id) {
    if (id < 0 || id >= MAX_MUSICS || !musics_used[id]) return "ERR: Invalid music id";
    ResumeMusicStream(musics[id]);
    return "";
}

RLAPI const char* bind_IsMusicStreamPlaying(int id) {
    if (id < 0 || id >= MAX_MUSICS || !musics_used[id]) return "ERR: Invalid music id";
    return IsMusicStreamPlaying(musics[id]) ? "TRUE" : "FALSE";
}

// AudioStreams
RLAPI const char* bind_PlayAudioStream(const char *name, unsigned int sampleRate, unsigned int sampleSize, unsigned int channels, float volume, float pitch, float pan) {
    int idx = alloc_id(streams_used, MAX_STREAMS);
    if (idx == -1) return "ERR: No free audio stream slots";
    streams[idx] = LoadAudioStream(sampleRate, sampleSize, channels);
    if (!IsAudioStreamValid(streams[idx])) return "ERR: Failed to load audio stream";
    SetAudioStreamVolume(streams[idx], volume);
    SetAudioStreamPitch(streams[idx], pitch);
    SetAudioStreamPan(streams[idx], pan);
    PlayAudioStream(streams[idx]);
    snprintf(retbuf, sizeof(retbuf), "OUT: %d", idx);
    return retbuf;
}

RLAPI const char* bind_StopAudioStream(int id) {
    if (id < 0 || id >= MAX_STREAMS || !streams_used[id]) return "ERR: Invalid audio stream id";
    StopAudioStream(streams[id]);
    return "";
}

RLAPI const char* bind_UnloadAudioStream(int id) {
    if (id < 0 || id >= MAX_STREAMS || !streams_used[id]) return "ERR: Invalid audio stream id";
    UnloadAudioStream(streams[id]);
    free_id(streams_used, id);
    return "";
}

RLAPI const char* bind_PauseAudioStream(int id) {
    if (id < 0 || id >= MAX_STREAMS || !streams_used[id]) return "ERR: Invalid audio stream id";
    PauseAudioStream(streams[id]);
    return "";
}

RLAPI const char* bind_ResumeAudioStream(int id) {
    if (id < 0 || id >= MAX_STREAMS || !streams_used[id]) return "ERR: Invalid audio stream id";
    ResumeAudioStream(streams[id]);
    return "";
}

RLAPI const char* bind_IsAudioStreamPlaying(int id) {
    if (id < 0 || id >= MAX_STREAMS || !streams_used[id]) return "ERR: Invalid audio stream id";
    return IsAudioStreamPlaying(streams[id]) ? "TRUE" : "FALSE";
}

// full cleanup
RLAPI void bind_UnloadAllSounds() {
    for (int i = 0; i < MAX_SOUNDS; i++) {
        if (sounds_used[i]) {
            UnloadSound(sounds[i]);
            sounds_used[i] = false;
        }
    }
}

RLAPI void bind_UnloadAllMusic() {
    for (int i = 0; i < MAX_MUSICS; i++) {
        if (musics_used[i]) {
            UnloadMusicStream(musics[i]);
            musics_used[i] = false;
        }
    }
}

RLAPI void bind_UnloadAllAudioStreams() {
    for (int i = 0; i < MAX_STREAMS; i++) {
        if (streams_used[i]) {
            UnloadAudioStream(streams[i]);
            streams_used[i] = false;
        }
    }
}
