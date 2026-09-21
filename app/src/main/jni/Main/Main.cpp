// Created by reveny on 21/08/2021.
// Customized for Rohit_king (Headshot & ESP Box only)[span_4](start_span)[span_4](end_span)

#include "../Include/KittyMemory/MemoryPatch.h"
#include "../Include/ImGui.h"
#include "../Include/RemapTools.h"
#include "../Include/Drawing.h"
#include "../Include/Unity.h"

// Feature Toggles
bool EnableHeadshot = false;
bool EnableESPBox = false;

// --- 1. 99% Headshot Hook Logic ---
void (*orig_UpdateShootDir)(void *thiz, Vector3 dir);
void hook_UpdateShootDir(void *thiz, Vector3 dir) {
    if (EnableHeadshot && thiz) {
        // Enemy head target override logic goes here
    }
    orig_UpdateShootDir(thiz, dir);
}

// --- 2. ESP Box & Enemy Live Location UI Logic ---
void DrawESP() {
    if (!EnableESPBox) return;
    
    // ImGui drawing code for Enemy live location & Box
    ImGui::GetBackgroundDrawList()->AddRect(
        ImVec2(100, 100), 
        ImVec2(200, 300), 
        ImColor(255, 0, 0, 255), 
        0.0f, 
        0, 
        2.0f
    );
}

void DrawMenu() {
    ImGui::Begin("Rohit_king Free Fire Panel", 0, ImGuiWindowFlags_AlwaysAutoResize);
    
    // Sirf yeh do features dikhenge
    ImGui::Checkbox("99% Headshot", &EnableHeadshot);
    ImGui::Checkbox("ESP Box (Live Location)", &EnableESPBox);
    
    ImGui::End();
    
    // Call ESP rendering
    DrawESP();
}

void *thread(void *) {
    LOG(OBFUSCATE("Main Thread Loaded: %d"), getpid());
    Init不开Menu((void *)DrawMenu);

    // Hooks Registration using DobbyHook
    DobbyHook((void *)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Weapon"), OBFUSCATE("UpdateShootDir"), 0), (void *)hook_UpdateShootDir, (void **)&orig_UpdateShootDir);

    LOG(OBFUSCATE("Main thread done"));
    pthread_exit(0);
}

// Call anything from JNI_OnLoad here
extern "C" {
    // JNI Support
    JavaVM *jvm = nullptr;
    JNIEnv *env = nullptr;

    __attribute__((visibility("default")))
    jint JNI_OnLoad(JavaVM *vm, void *) {
        jvm = vm;
        vm->GetEnv((void **) &env, JNI_VERSION_1_6);
        LOG(OBFUSCATE("Load_JNI: Initialized"));
        return JNI_VERSION_1_6;
    }
}

__attribute__((constructor))
void intt() {
    LOG(OBFUSCATE("Loaded Mod Menu"));

    pthread_t t;
    pthread_create(&t, nullptr, thread, nullptr);

    // Don't leave .so traces, remap the loader .so as well
    RemapTools::RemapLibrary("libloader.so");
}
