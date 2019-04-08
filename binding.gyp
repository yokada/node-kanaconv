{
  "targets": [
    {
      "target_name": "kanaconv",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
          "src/main.cc", "src/convert.cc"
      ],
      "include_dirs": [
          "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "libraries": [],
      "dependencies": [
          "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS", "DEBUG" ]
    }
  ]
}
