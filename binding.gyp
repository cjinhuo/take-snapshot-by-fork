{
  "targets": [{
    "target_name": "addon",
    "sources": [ "./src/take_snapshot_by_fork.cc" ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
    ],
    'dependencies': [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ],
  }]
}
