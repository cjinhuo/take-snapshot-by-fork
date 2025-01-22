{
  "targets": [{
    "target_name": "take_snapshot_by_fork",
    "cflags!": [ "-fno-exceptions" ],
    "cflags_cc!": [ "-fno-exceptions" ],
    "xcode_settings": {
      "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
      "CLANG_CXX_LIBRARY": "libc++",
      "MACOSX_DEPLOYMENT_TARGET": "10.7"
    },
    "msvs_settings": {
      "VCCLCompilerTool": { "ExceptionHandling": 1 },
    },
    # 启用 C++ 异常处理,不然会报错 /napi.h:63:2: error: Exception support not detected.       Define either NODE_ADDON_API_CPP_EXCEPTIONS or NODE_ADDON_API_DISABLE_CPP_EXCEPTIONS. #error Exception support not detected.
    "defines": [ "NODE_ADDON_API_CPP_EXCEPTIONS" ], 
    "sources": [ "./src/take_snapshot_by_fork.cc" ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
    ],
    'dependencies': [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ],
  }]
}
