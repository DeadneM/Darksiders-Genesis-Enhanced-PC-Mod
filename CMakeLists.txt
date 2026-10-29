cmake_minimum_required(VERSION 3.24)
project(DarksidersGenesisASIMod VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include(CTest)
if(BUILD_TESTING)
  add_executable(AimAssistTests tests/AimAssistTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(AimAssistTests PRIVATE src)
  target_compile_definitions(AimAssistTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(AimAssistTests PRIVATE user32)
  add_test(NAME AimAssist COMMAND AimAssistTests)
  add_executable(CastReturnTests tests/CastReturnTests.cpp)
  target_include_directories(CastReturnTests PRIVATE tests/stubs src)
  target_compile_definitions(CastReturnTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME CastReturn COMMAND CastReturnTests)
  add_executable(EffectAppearanceTests tests/EffectAppearanceTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(EffectAppearanceTests PRIVATE tests/stubs src)
  target_compile_definitions(EffectAppearanceTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME EffectAppearance COMMAND EffectAppearanceTests)
  add_executable(StoneSkinVisualTests tests/StoneSkinVisualTests.cpp)
  target_include_directories(StoneSkinVisualTests PRIVATE tests/stubs src)
  target_compile_definitions(StoneSkinVisualTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME StoneSkinVisual COMMAND StoneSkinVisualTests)
  add_executable(PlayerPresentationTests tests/PlayerPresentationTests.cpp tests/NativeCursorConsumer.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(PlayerPresentationTests PRIVATE tests/stubs src)
  target_compile_definitions(PlayerPresentationTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(PlayerPresentationTests PRIVATE user32)
  add_test(NAME PlayerPresentation COMMAND PlayerPresentationTests)
  add_executable(ForeignMontageTests tests/ForeignMontageTests.cpp)
  target_include_directories(ForeignMontageTests PRIVATE tests/stubs src)
  target_compile_definitions(ForeignMontageTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME ForeignMontage COMMAND ForeignMontageTests)
  add_executable(ForeignAudioTests tests/ForeignAudioTests.cpp)
  target_include_directories(ForeignAudioTests PRIVATE tests/stubs src)
  target_compile_definitions(ForeignAudioTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME ForeignAudio COMMAND ForeignAudioTests)
  add_executable(StoneTransitionFxTests tests/StoneTransitionFxTests.cpp)
  target_include_directories(StoneTransitionFxTests PRIVATE tests/stubs src)
  target_compile_definitions(StoneTransitionFxTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME StoneTransitionFx COMMAND StoneTransitionFxTests)
  add_executable(ForeignWrathTests tests/ForeignWrathTests.cpp src/StoneSkinVisual.cpp src/StoneTransitionFx.cpp src/ForeignCaltropsFeature.cpp src/ForeignPowerFeature.cpp src/ForeignBeamFeature.cpp src/ForeignRampageFeature.cpp src/RuntimeSettings.cpp)
  target_include_directories(ForeignWrathTests PRIVATE tests/stubs src)
  target_compile_definitions(ForeignWrathTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME ForeignWrath COMMAND ForeignWrathTests)
  add_executable(ForeignBeamTests tests/ForeignBeamTests.cpp)
  target_include_directories(ForeignBeamTests PRIVATE tests/stubs src)
  target_compile_definitions(ForeignBeamTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME ForeignBeam COMMAND ForeignBeamTests)
  add_executable(ForeignRampageTests tests/ForeignRampageTests.cpp)
  target_include_directories(ForeignRampageTests PRIVATE tests/stubs src)
  target_compile_definitions(ForeignRampageTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME ForeignRampage COMMAND ForeignRampageTests)
  add_executable(ForeignRampageMontageTests tests/ForeignRampageMontageTests.cpp)
  target_include_directories(ForeignRampageMontageTests PRIVATE tests/stubs src)
  target_compile_definitions(ForeignRampageMontageTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME ForeignRampageMontage COMMAND ForeignRampageMontageTests)
  add_executable(ForeignPowerTests tests/ForeignPowerTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(ForeignPowerTests PRIVATE tests/stubs src)
  target_compile_definitions(ForeignPowerTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME ForeignPower COMMAND ForeignPowerTests)
  add_executable(ForeignCaltropsTests tests/ForeignCaltropsTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(ForeignCaltropsTests PRIVATE tests/stubs src)
  target_compile_definitions(ForeignCaltropsTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME ForeignCaltrops COMMAND ForeignCaltropsTests)
  add_executable(ExecutionCameraTests tests/ExecutionCameraTests.cpp)
  target_include_directories(ExecutionCameraTests PRIVATE src)
  target_compile_definitions(ExecutionCameraTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME ExecutionCamera COMMAND ExecutionCameraTests)
  add_executable(MeleeEffectTests tests/MeleeEffectTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(MeleeEffectTests PRIVATE tests/stubs src)
  target_compile_definitions(MeleeEffectTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME MeleeEffect COMMAND MeleeEffectTests)
  add_executable(LavaMuzzleTests tests/LavaMuzzleTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(LavaMuzzleTests PRIVATE tests/stubs src)
  target_compile_definitions(LavaMuzzleTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
  add_test(NAME LavaMuzzle COMMAND LavaMuzzleTests)
  add_executable(MountedJumpTests tests/MountedJumpTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(MountedJumpTests PRIVATE tests/stubs src)
  target_compile_definitions(MountedJumpTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME MountedJump COMMAND MountedJumpTests)
  add_executable(WrathSlotTests tests/WrathSlotTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(WrathSlotTests PRIVATE tests/stubs src)
  target_compile_definitions(WrathSlotTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(WrathSlotTests PRIVATE user32)
  add_test(NAME WrathSlots COMMAND WrathSlotTests)
  add_executable(PistolVisualTests tests/PistolVisualTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(PistolVisualTests PRIVATE tests/stubs src)
  target_compile_definitions(PistolVisualTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(PistolVisualTests PRIVATE user32)
  add_test(NAME PistolVisual COMMAND PistolVisualTests)
  add_executable(LeviathanVisualTests tests/LeviathanVisualTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(LeviathanVisualTests PRIVATE src)
  target_compile_definitions(LeviathanVisualTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(LeviathanVisualTests PRIVATE user32)
  add_test(NAME LeviathanVisual COMMAND LeviathanVisualTests)
  add_executable(CosmeticUnlockTests tests/CosmeticUnlockTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(CosmeticUnlockTests PRIVATE tests/stubs src)
  target_compile_definitions(CosmeticUnlockTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(CosmeticUnlockTests PRIVATE user32)
  add_test(NAME CosmeticUnlock COMMAND CosmeticUnlockTests)
  add_executable(MountedHolsterTests tests/MountedHolsterTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(MountedHolsterTests PRIVATE tests/stubs src)
  target_compile_definitions(MountedHolsterTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(MountedHolsterTests PRIVATE user32)
  add_test(NAME MountedHolsters COMMAND MountedHolsterTests)
  add_executable(UltimateDurationTests tests/UltimateDurationTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(UltimateDurationTests PRIVATE tests/stubs src)
  target_compile_definitions(UltimateDurationTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(UltimateDurationTests PRIVATE user32)
  add_test(NAME UltimateDuration COMMAND UltimateDurationTests)
  add_executable(GrappleRangeTests tests/GrappleRangeTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(GrappleRangeTests PRIVATE tests/stubs src)
  target_compile_definitions(GrappleRangeTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME GrappleRange COMMAND GrappleRangeTests)
  add_executable(AirJumpTests tests/AirJumpTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(AirJumpTests PRIVATE tests/stubs src)
  target_compile_definitions(AirJumpTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME AirJump COMMAND AirJumpTests)
  add_executable(StandardAmmoTests tests/StandardAmmoTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(StandardAmmoTests PRIVATE tests/stubs src)
  target_compile_definitions(StandardAmmoTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME StandardAmmo COMMAND StandardAmmoTests)
  add_executable(GameplayExtensionsConfigTests tests/GameplayExtensionsConfigTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(GameplayExtensionsConfigTests PRIVATE tests/stubs src)
  target_compile_definitions(GameplayExtensionsConfigTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(GameplayExtensionsConfigTests PRIVATE user32)
  add_test(NAME GameplayExtensionsConfig COMMAND GameplayExtensionsConfigTests)
  add_executable(NativeRadialTests tests/NativeRadialTests.cpp)
  target_include_directories(NativeRadialTests PRIVATE tests/stubs src)
  target_compile_definitions(NativeRadialTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME NativeRadial COMMAND NativeRadialTests)
  add_executable(NativeObjectIdentityTests tests/NativeObjectIdentityTests.cpp)
  target_include_directories(NativeObjectIdentityTests PRIVATE src)
  add_test(NAME NativeObjectIdentity COMMAND NativeObjectIdentityTests)
  add_executable(DashCombatFeatureTests tests/DashCombatFeatureTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(DashCombatFeatureTests PRIVATE tests/stubs src)
  target_compile_definitions(DashCombatFeatureTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME DashCombatFeature COMMAND DashCombatFeatureTests)
  add_executable(InteractionHudTests tests/InteractionHudTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(InteractionHudTests PRIVATE tests/stubs src)
  target_compile_definitions(InteractionHudTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(InteractionHudTests PRIVATE user32)
  add_test(NAME InteractionHud COMMAND InteractionHudTests)
  add_executable(ManualActionBindingTests tests/ManualActionBindingTests.cpp)
  target_include_directories(ManualActionBindingTests PRIVATE src)
  add_test(NAME ManualActionBindings COMMAND ManualActionBindingTests)
  add_executable(RecoveryTailTests tests/RecoveryTailTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(RecoveryTailTests PRIVATE src)
  target_compile_definitions(RecoveryTailTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(RecoveryTailTests PRIVATE user32)
  add_test(NAME RecoveryTail COMMAND RecoveryTailTests)
  add_executable(HeavyLandingTests tests/HeavyLandingTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(HeavyLandingTests PRIVATE src)
  target_compile_definitions(HeavyLandingTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(HeavyLandingTests PRIVATE user32)
  add_test(NAME HeavyLanding COMMAND HeavyLandingTests)
  add_executable(ManualAimTests tests/ManualAimTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(ManualAimTests PRIVATE tests/stubs src)
  target_compile_definitions(ManualAimTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(ManualAimTests PRIVATE user32)
  add_test(NAME ManualAim COMMAND ManualAimTests)
  add_executable(WeaponSheathingTests tests/WeaponSheathingTests.cpp src/RuntimeSettings.cpp src/ConfigStore.cpp)
  target_include_directories(WeaponSheathingTests PRIVATE src)
  target_compile_definitions(WeaponSheathingTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(WeaponSheathingTests PRIVATE user32)
  add_test(NAME WeaponSheathing COMMAND WeaponSheathingTests)
  add_executable(DashCombatTests tests/DashCombatTests.cpp)
  target_include_directories(DashCombatTests PRIVATE src)
  add_test(NAME DashCombat COMMAND DashCombatTests)
  add_executable(TpsCameraFocusTests tests/TpsCameraFocusTests.cpp)
  target_include_directories(TpsCameraFocusTests PRIVATE src)
  add_test(NAME TpsCameraFocus COMMAND TpsCameraFocusTests)
  add_executable(LegacyRecoveryConfigTests tests/LegacyRecoveryConfigTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(LegacyRecoveryConfigTests PRIVATE src)
  target_compile_definitions(LegacyRecoveryConfigTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(LegacyRecoveryConfigTests PRIVATE user32)
  add_test(NAME LegacyRecoveryConfig COMMAND LegacyRecoveryConfigTests)
  add_executable(HotstreakConfigTests tests/HotstreakConfigTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(HotstreakConfigTests PRIVATE src)
  target_compile_definitions(HotstreakConfigTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(HotstreakConfigTests PRIVATE user32)
  add_test(NAME HotstreakConfig COMMAND HotstreakConfigTests)
  add_executable(GlideAndHorseTests tests/GlideAndHorseTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(GlideAndHorseTests PRIVATE tests/stubs src)
  target_compile_definitions(GlideAndHorseTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME GlideAndHorse COMMAND GlideAndHorseTests)
  add_executable(SustainConfigTests tests/SustainConfigTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(SustainConfigTests PRIVATE src)
  target_compile_definitions(SustainConfigTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(SustainConfigTests PRIVATE user32)
  add_test(NAME SustainConfig COMMAND SustainConfigTests)
  add_executable(ProjectileSpawnTests tests/ProjectileSpawnTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(ProjectileSpawnTests PRIVATE tests/stubs src)
  target_compile_definitions(ProjectileSpawnTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME ProjectileSpawn COMMAND ProjectileSpawnTests)
  add_executable(TpsAimTargetTests tests/TpsAimTargetTests.cpp)
  target_include_directories(TpsAimTargetTests PRIVATE src)
  add_test(NAME TpsAimTarget COMMAND TpsAimTargetTests)
  add_executable(LocalGameplayIsolationTests tests/LocalGameplayIsolationTests.cpp src/RuntimeSettings.cpp)
  target_include_directories(LocalGameplayIsolationTests PRIVATE tests/stubs src)
  target_compile_definitions(LocalGameplayIsolationTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME LocalGameplayIsolation COMMAND LocalGameplayIsolationTests)
  add_executable(LocalPlayerOwnershipTests tests/LocalPlayerOwnershipTests.cpp)
  target_include_directories(LocalPlayerOwnershipTests PRIVATE src)
  add_test(NAME LocalPlayerOwnership COMMAND LocalPlayerOwnershipTests)
  add_executable(CoopCameraIsolationTests tests/CoopCameraIsolationTests.cpp src/CoopCameraFeature.cpp src/RuntimeSettings.cpp)
  target_include_directories(CoopCameraIsolationTests PRIVATE src)
  target_compile_definitions(CoopCameraIsolationTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  add_test(NAME CoopCameraIsolation COMMAND CoopCameraIsolationTests)
  add_executable(ControllerReadScopeTests tests/ControllerReadScopeTests.cpp)
  target_include_directories(ControllerReadScopeTests PRIVATE src)
  add_test(NAME ControllerReadScope COMMAND ControllerReadScopeTests)
  add_executable(TpsStickRolesTests tests/TpsStickRolesTests.cpp)
  target_include_directories(TpsStickRolesTests PRIVATE src)
  add_test(NAME TpsStickRoles COMMAND TpsStickRolesTests)
  add_executable(GaugeRequestTests tests/GaugeRequestTests.cpp)
  target_include_directories(GaugeRequestTests PRIVATE src)
  add_test(NAME GaugeRequests COMMAND GaugeRequestTests)
  add_executable(GaugeHotkeyTests tests/GaugeHotkeyTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(GaugeHotkeyTests PRIVATE src)
  target_compile_definitions(GaugeHotkeyTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(GaugeHotkeyTests PRIVATE user32)
  add_test(NAME GaugeHotkeys COMMAND GaugeHotkeyTests)
  add_executable(UltimateCameraConfigTests tests/UltimateCameraConfigTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(UltimateCameraConfigTests PRIVATE src)
  target_compile_definitions(UltimateCameraConfigTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(UltimateCameraConfigTests PRIVATE user32)
  add_test(NAME UltimateCameraConfig COMMAND UltimateCameraConfigTests)
  add_executable(PotionTests tests/PotionTests.cpp)
  target_include_directories(PotionTests PRIVATE src)
  add_test(NAME PotionPolicies COMMAND PotionTests)
  add_executable(PotionConfigTests tests/PotionConfigTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(PotionConfigTests PRIVATE src)
  target_compile_definitions(PotionConfigTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(PotionConfigTests PRIVATE user32)
  add_test(NAME PotionConfig COMMAND PotionConfigTests)
  add_executable(ResourceGainTests tests/ResourceGainTests.cpp)
  target_include_directories(ResourceGainTests PRIVATE src)
  add_test(NAME ResourceGains COMMAND ResourceGainTests)
  add_executable(GaugeUnlimitedTests tests/GaugeUnlimitedTests.cpp)
  target_include_directories(GaugeUnlimitedTests PRIVATE src)
  add_test(NAME GaugeUnlimited COMMAND GaugeUnlimitedTests)
  add_executable(WarningPresentationTests tests/WarningPresentationTests.cpp)
  target_include_directories(WarningPresentationTests PRIVATE src)
  add_test(NAME WarningPresentation COMMAND WarningPresentationTests)
  add_executable(AmmoConsumptionTests tests/AmmoConsumptionTests.cpp)
  target_include_directories(AmmoConsumptionTests PRIVATE src)
  add_test(NAME AmmoConsumption COMMAND AmmoConsumptionTests)
  add_executable(AmmoConfigTests tests/AmmoConfigTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(AmmoConfigTests PRIVATE src)
  target_compile_definitions(AmmoConfigTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(AmmoConfigTests PRIVATE user32)
  add_test(NAME AmmoConfig COMMAND AmmoConfigTests)
  add_executable(ConfigWriteFailureTests tests/ConfigWriteFailureTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(ConfigWriteFailureTests PRIVATE src)
  target_compile_definitions(ConfigWriteFailureTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(ConfigWriteFailureTests PRIVATE user32)
  add_test(NAME ConfigWriteFailures COMMAND ConfigWriteFailureTests)
  add_executable(UpdateVersionTests tests/UpdateVersionTests.cpp)
  target_include_directories(UpdateVersionTests PRIVATE src)
  add_test(NAME UpdateVersion COMMAND UpdateVersionTests)
  add_executable(UserDefaultsConfigTests tests/UserDefaultsConfigTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(UserDefaultsConfigTests PRIVATE src)
  target_compile_definitions(UserDefaultsConfigTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(UserDefaultsConfigTests PRIVATE user32)
  add_test(NAME UserDefaultsConfig COMMAND UserDefaultsConfigTests "${CMAKE_CURRENT_SOURCE_DIR}/config/DarksidersGenesisMod.ini")

endif()

include(FetchContent)

FetchContent_Declare(
  minhook
  GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
  GIT_TAG c3fcafdc10146beb5919319d0683e44e3c30d537
)
FetchContent_MakeAvailable(minhook)

if(TARGET minhook)
  set(DG_MINHOOK_TARGET minhook)
else()
  message(STATUS "MinHook CMake target not exported; using local fallback target")
  add_library(dg_minhook STATIC
    ${minhook_SOURCE_DIR}/src/buffer.c
    ${minhook_SOURCE_DIR}/src/hook.c
    ${minhook_SOURCE_DIR}/src/trampoline.c
    ${minhook_SOURCE_DIR}/src/hde/hde64.c
  )
  target_include_directories(dg_minhook
    PUBLIC ${minhook_SOURCE_DIR}/include
    PRIVATE ${minhook_SOURCE_DIR}/src ${minhook_SOURCE_DIR}/src/hde
  )
  set(DG_MINHOOK_TARGET dg_minhook)
endif()

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG dbb5eeaadffb6a3ba6a60de1290312e5802dba5a
)
FetchContent_MakeAvailable(imgui)

add_library(imgui_static STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
)
target_include_directories(imgui_static PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)
target_compile_definitions(imgui_static PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
target_link_libraries(imgui_static PUBLIC d3d11 dxgi user32 gdi32 imm32 dwmapi)

if(BUILD_TESTING)
  add_executable(OverlayLayoutTests tests/OverlayLayoutTests.cpp)
  target_include_directories(OverlayLayoutTests PRIVATE src)
  target_link_libraries(OverlayLayoutTests PRIVATE imgui_static)
  add_test(NAME OverlayLayout COMMAND OverlayLayoutTests)
  add_executable(OverlayBindingScopeTests tests/OverlayBindingScopeTests.cpp)
  add_executable(MenuAppearanceTests tests/MenuAppearanceTests.cpp src/ConfigStore.cpp src/RuntimeSettings.cpp)
  target_include_directories(MenuAppearanceTests PRIVATE src)
  target_compile_definitions(MenuAppearanceTests PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
  target_link_libraries(MenuAppearanceTests PRIVATE imgui_static)
  add_test(NAME MenuAppearance COMMAND MenuAppearanceTests)
  target_include_directories(OverlayBindingScopeTests PRIVATE src)
  target_link_libraries(OverlayBindingScopeTests PRIVATE imgui_static)
  add_test(NAME OverlayBindingScope COMMAND OverlayBindingScopeTests)
  add_executable(UpdateNoticeTests tests/UpdateNoticeTests.cpp)
  target_include_directories(UpdateNoticeTests PRIVATE src)
  target_link_libraries(UpdateNoticeTests PRIVATE imgui_static)
  add_test(NAME UpdateNotice COMMAND UpdateNoticeTests)
endif()


# Windows x64 post-exit helper embedded as RT_RCDATA (no extra ZIP root file).
enable_language(RC)
set(DG_CSC "$ENV{WINDIR}/Microsoft.NET/Framework64/v4.0.30319/csc.exe")
if(NOT EXISTS "${DG_CSC}")
  message(FATAL_ERROR "Embedded updater requires the Windows .NET Framework x64 C# compiler")
endif()
if(BUILD_TESTING)
  set(DG_UPDATER_TEST "${CMAKE_CURRENT_BINARY_DIR}/UpdateEngineSafetyTests.exe")
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tests/UpdateEngineSafetyTests.cs" DG_CSHARP_TEST)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/updater/UpdateEngine.cs" DG_CSHARP_TEST_ENGINE)
  add_custom_command(OUTPUT "${DG_UPDATER_TEST}"
    COMMAND "${DG_CSC}" /nologo /target:exe /platform:x64 /optimize+
      "/out:${DG_UPDATER_TEST}" /r:System.dll /r:System.Core.dll
      /r:System.IO.Compression.dll /r:System.IO.Compression.FileSystem.dll
      /r:System.Web.Extensions.dll "${DG_CSHARP_TEST}" "${DG_CSHARP_TEST_ENGINE}"
    DEPENDS tests/UpdateEngineSafetyTests.cs updater/UpdateEngine.cs VERBATIM)
  add_custom_target(dg_updater_tests ALL DEPENDS "${DG_UPDATER_TEST}")
  add_test(NAME UpdateEngineSafety COMMAND "${DG_UPDATER_TEST}")
endif()
set(DG_HELPER_EXE "${CMAKE_CURRENT_BINARY_DIR}/DarksidersGenesisUpdaterCLI.exe")
# Native paths avoid C# compiler treating forward slashes as argument switches.
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/updater/Program.cs" DG_CSHARP_PROGRAM)
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/updater/UpdaterForm.cs" DG_CSHARP_FORM)
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/updater/UpdateEngine.cs" DG_CSHARP_ENGINE)
add_custom_command(
  OUTPUT "${DG_HELPER_EXE}"
  COMMAND "${DG_CSC}" /nologo /target:winexe /platform:x64 /optimize+
    "/out:${DG_HELPER_EXE}"
    /r:System.dll /r:System.Core.dll /r:System.Drawing.dll
    /r:System.Windows.Forms.dll /r:System.IO.Compression.dll
    /r:System.IO.Compression.FileSystem.dll /r:System.Web.Extensions.dll
    "${DG_CSHARP_PROGRAM}"
    "${DG_CSHARP_FORM}"
    "${DG_CSHARP_ENGINE}"
  DEPENDS updater/Program.cs updater/UpdaterForm.cs updater/UpdateEngine.cs
  COMMENT "Building post-exit updater to embed in the ASI"
  VERBATIM
)
add_custom_target(dg_embedded_updater DEPENDS "${DG_HELPER_EXE}")
set(DG_UPDATER_RC "${CMAKE_CURRENT_BINARY_DIR}/DgUpdaterHelper.rc")
file(WRITE "${DG_UPDATER_RC}" "101 RCDATA \"DarksidersGenesisUpdaterCLI.exe\"\n")
set_source_files_properties("${DG_UPDATER_RC}" PROPERTIES OBJECT_DEPENDS "${DG_HELPER_EXE}")

add_library(DarksidersGenesisMod SHARED
  src/NativeTpsFeature.cpp
  src/DashCombatFeature.cpp
  src/InteractionHudFeature.cpp
  src/WeaponSheathingFeature.cpp
  src/ManualAimFeature.cpp
  src/HeavyLandingFeature.cpp
  src/RecoveryTailFeature.cpp
  src/MountedHolsterFeature.cpp
  src/PistolVisualFeature.cpp
  src/ExecutionCameraFeature.cpp
  src/MeleeEffectFeature.cpp
  src/WrathSlotFeature.cpp
  src/StoneSkinVisual.cpp
  src/StoneTransitionFx.cpp
  src/ForeignWrathFeature.cpp
  src/ForeignCaltropsFeature.cpp
  src/ForeignMontage.cpp
  src/ForeignPowerFeature.cpp
  src/ForeignBeamFeature.cpp
  src/ForeignRampageFeature.cpp
  src/PlayerPresentationFeature.cpp
  src/CosmeticUnlockFeature.cpp
  src/LeviathanVisualFeature.cpp
  src/EffectAppearanceFeature.cpp
  src/UltimateDurationFeature.cpp
  src/ProjectileAimFeature.cpp
  src/HealthFeature.cpp
  src/GlideFeature.cpp
  src/CameraTraceFeature.cpp
  src/LocalPlayerOwnership.cpp
  src/CoopCameraFeature.cpp
  src/ChaosPoseFeature.cpp
  src/ResourceGainFeature.cpp
  src/ConfigStore.cpp
  src/DarksidersGenesisMod.cpp
  src/EngineIniFeature.cpp
  src/GaugeChargeFeature.cpp
  src/ManualGaugeFeature.cpp
  src/PotionFeature.cpp
  src/AmmoConsumptionFeature.cpp
  src/GrappleRangeFeature.cpp
  src/AirJumpFeature.cpp
  src/MountedJumpFeature.cpp
  src/HotstreakFeature.cpp
  src/HorseFeature.cpp
  src/OverlayUi.cpp
  src/RuntimeSettings.cpp
  src/ShaderAimFeature.cpp
  src/SkipLogosFeature.cpp
  src/SkipWarningFeature.cpp
  src/TargetValidator.cpp
  src/UpdateFeature.cpp
  "${DG_UPDATER_RC}"
)
add_dependencies(DarksidersGenesisMod dg_embedded_updater)
target_compile_definitions(DarksidersGenesisMod PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
target_include_directories(DarksidersGenesisMod PRIVATE ${minhook_SOURCE_DIR}/include)
target_link_libraries(DarksidersGenesisMod PRIVATE
  imgui_static
  ${DG_MINHOOK_TARGET}
  d3d11
  dxgi
  user32
  gdi32
  imm32
  dwmapi
  bcrypt
  winhttp
)
set_target_properties(DarksidersGenesisMod PROPERTIES
  PREFIX ""
  SUFFIX ".asi"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/out"
)

add_library(dxgi_proxy SHARED
  src/dxgi_proxy.cpp
)
target_compile_definitions(dxgi_proxy PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
target_link_libraries(dxgi_proxy PRIVATE user32)
set_target_properties(dxgi_proxy PROPERTIES
  OUTPUT_NAME "dxgi"
  PREFIX ""
  SUFFIX ".dll"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/out"
)

option(DG_ENABLE_ANALYZE "Run MSVC native static analysis" OFF)
if(MSVC AND DG_ENABLE_ANALYZE)
  target_compile_options(DarksidersGenesisMod PRIVATE /analyze)
  target_compile_options(dxgi_proxy PRIVATE /analyze)
endif()

if(MSVC)
  target_compile_options(DarksidersGenesisMod PRIVATE /W4 /EHsc /permissive-)
  target_compile_options(dxgi_proxy PRIVATE /W4 /EHsc /permissive-)
endif()
