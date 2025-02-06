const addon = require('./build/Debug/take_snapshot_by_fork.node');


const os = require('os')
console.log(addon.takeSnapshotByFork)
console.log(addon.takeSnapshotByFork())
console.log(os.arch(), os.platform())


module.exports = {
    takeSnapshotByFork: addon.takeSnapshotByFork
};