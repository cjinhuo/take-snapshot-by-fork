const { Worker } = require('worker_threads');

new Worker(`
    const addon = require('.');
    class SubClass {}
    const obj = new SubClass();
    console.log('addon',addon)
    setInterval(() => { obj; }, 1000);
    addon.takeSnapshotByFork('./demo.heapsnapshot')
`, { eval: true });
