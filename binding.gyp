{
  "targets": [{
    "target_name": "addon",
    "sources": [ "src/take_snapshot_by_fork.cc" ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
      "/Users/bytedance/Library/Caches/node-gyp/20.9.0/include/node"
    ]
  }]
}
