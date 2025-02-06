// const addon = require('./prebuilds/darwin-arm64/async-snapshot.v16.uv1.armv8.musl.node')
// const addon = require('./prebuilds/darwin-x64/async-snapshot.v14.darwin-x64.uv1.musl.node')
const addon = require('./prebuilds/darwin-x64/async-snapshot.v12.darwin-x64.uv1.musl.node')
// const addon = require('./prebuilds/darwin-x64/async-snapshot.v10.uv1.musl.node')
const v8 = require('v8')
const fs = require('fs')
debugger
class MainClass { }
const obj = new MainClass();
global.buf = Buffer.alloc(1024)
const xxxx = {}
for (let i = 0; i < 100; i++) {
    xxxx[i] = i
}
setInterval(() => {
    console.log('main thread setInterval')
}, 2000)

setTimeout(() => {
    const t1 = Date.now();
    // const stream = v8.getHeapSnapshot();
    // stream.pipe(fs.createWriteStream(`./${process.pid}.heapsnapshot`))
    addon.takeSnapshotByFork(`./${process.pid}.heapsnapshot`)
    // console.log(Date.now() - t1)
}, 1000);