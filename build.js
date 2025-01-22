const { execSync } = require('child_process');

// 定义要构建的 Node.js 版本
// , '18.0.0', '20.0.0'
// '14.0.0', '16.0.0', '18.0.0'
// 10.0.0, 12.0.0
const VERSIONS = ['12.0.0'];
// darwin, linux
const PLATFORMS = ['darwin']

const NAME = 'async-snapshot'
// 执行构建命令的函数
const executeBuild = (version, platform, arch) => {
  console.log(`Building for Node.js ${version} (${arch})`);

  const baseCommand = 'prebuildify --napi --strip';
  const commonFlags = [
    `--arch ${arch}`,
    `--target node@${version}`,
    `--platform ${platform}`,
    '--libc musl',
    '--tag-uv',
    '--tag-libc',
    '--tag-armv',
    `--name ${NAME}.v${version.split('.')[0]}.${platform}-${arch}`,
  ];


  const command = [
    baseCommand,
    ...commonFlags,
  ].join(' ');

  try {
    execSync(command, { stdio: 'inherit' });
  } catch (error) {
    console.error(`Error building for Node.js ${version} (${arch}):`, error);
    process.exit(1);
  }
};

// 执行所有版本的构建
const main = () => {
  for (const version of VERSIONS) {
    for (const platform of PLATFORMS) {
      // 构建 x64 版本
      executeBuild(version, platform, 'x64');
      // 构建 arm64 版本
      executeBuild(version, platform, 'arm64');
    }
  }
};

main();
