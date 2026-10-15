cmake_minimum_required(VERSION 3.24)
project(DarksidersGenesisASIMod VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
if(BUILD_TESTING)
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
  add_executable(ResourceGainTests tests/ResourceGainTests.cpp)
  target_include_directories(ResourceGainTests PRIVATE src)
  add_test(NAME ResourceGains COMMAND ResourceGainTests)

endif()

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
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


# Windows x64 post-exit helper embedded as RT_RCDATA (no extra ZIP root file).
enable_language(RC)
set(DG_CSC "$ENV{WINDIR}/Microsoft.NET/Framework64/v4.0.30319/csc.exe")
if(NOT EXISTS "${DG_CSC}")
  message(FATAL_ERROR "Embedded updater requires the Windows .NET Framework x64 C# compiler")
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
  src/CameraTraceFeature.cpp
  src/ChaosPoseFeature.cpp
  src/ResourceGainFeature.cpp
  src/ConfigStore.cpp
  src/DarksidersGenesisMod.cpp
  src/EngineIniFeature.cpp
  src/GaugeChargeFeature.cpp
  src/ManualGaugeFeature.cpp
  src/HorseFeature.cpp
  src/OverlayUi.cpp
  src/RuntimeSettings.cpp
  src/ShaderAimFeature.cpp
  src/SkipLogosFeature.cpp
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

if(MSVC)
  target_compile_options(DarksidersGenesisMod PRIVATE /W4 /EHsc /permissive-)
  target_compile_options(dxgi_proxy PRIVATE /W4 /EHsc /permissive-)
endif()
