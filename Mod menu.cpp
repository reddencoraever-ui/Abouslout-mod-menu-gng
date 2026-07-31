#include <openvr.h>
#include <iostream>

vr::IVROverlay* g_pOverlay = nullptr;
vr::VROverlayHandle_t g_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;

bool InitializeVR() {
    vr::EVRInitError eError = vr::VRInitError_None;
    vr::VR_Init(&eError, vr::VRApplication_Overlay);

    if (eError != vr::VRInitError_None) {
        std::cerr << "OpenVR Initialization Failed: " 
                  << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
        return false;
    }

    g_pOverlay = vr::VROverlay();
    if (!g_pOverlay) return false;

    // Create unique key and display name for the overlay
    g_pOverlay->CreateOverlay("example.overlay.ui", "Custom Overlay", &g_ulOverlayHandle);
    g_pOverlay->SetOverlayWidthInMeters(g_ulOverlayHandle, 1.2f);

    return true;
}

void SetOverlayPosition() {
    if (!g_pOverlay || g_ulOverlayHandle == vr::k_ulOverlayHandleInvalid) return;

    // Define a 3x4 matrix for translation/rotation
    vr::HmdMatrix34_t transform = {
        1.0f, 0.0f, 0.0f, 0.0f,  // X axis
        0.0f, 1.0f, 0.0f, 1.2f,  // Y axis (height)
        0.0f, 0.0f, 1.0f, -1.5f  // Z axis (distance in front of user)
    };

    g_pOverlay->SetOverlayTransformAbsolute(
        g_ulOverlayHandle, 
        vr::TrackingUniverseStanding, 
        &transform
    );
    
    g_pOverlay->ShowOverlay(g_ulOverlayHandle);
}
void UpdateOverlayTexture(uint32_t nativeTextureHandle) {
    if (!g_pOverlay || g_ulOverlayHandle == vr::k_ulOverlayHandleInvalid) return;

    vr::Texture_t vrTexture = {
        reinterpret_cast<void*>(static_cast<uintptr_t>(nativeTextureHandle)),
        vr::TextureType_OpenGL, // Or vr::TextureType_DirectX
        vr::ColorSpace_Auto
    };

    g_pOverlay->SetOverlayTexture(g_ulOverlayHandle, &vrTexture);
}
void PollOverlayEvents() {
    if (!g_pOverlay) return;

    vr::VREvent_t event;
    while (g_pOverlay->PollNextOverlayEvent(g_ulOverlayHandle, &event, sizeof(event))) {
        switch (event.eventType) {
            case vr::VREvent_MouseButtonDown:
                // Pass click input to UI context
                break;
            case vr::VREvent_MouseMove:
                // Update cursor position on UI context
                break;
            default:
                break;
        }
    }
}
