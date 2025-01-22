#include <napi.h>
#include <iostream>

// 实现打印函数
Napi::Value Print(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  // 获取第一个参数并打印
  if (info[0].IsString())
  {
    std::string message = info[0].As<Napi::String>().Utf8Value();
    std::cout << message << std::endl;
  }

  return env.Undefined();
}

// 初始化模块
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  // 导出 print 函数
  exports.Set("print", Napi::Function::New(env, Print));
  return exports;
}

// 注册模块
NODE_API_MODULE(addon, Init)
