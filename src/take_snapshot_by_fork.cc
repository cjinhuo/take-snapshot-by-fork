#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <node.h>
#include <v8-profiler.h>
#include <chrono>

using namespace std::chrono;

namespace addon
{

  using v8::Context;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  // https://github.com/bnoordhuis/node-heapdump
  class FileOutputStream : public v8::OutputStream
  {
  public:
    explicit FileOutputStream(FILE *stream) : stream_(stream) {}

    virtual int GetChunkSize()
    {
      return 65536; // big chunks == faster
    }

    virtual void EndOfStream() {}

    virtual WriteResult WriteAsciiChunk(char *data, int size)
    {
      const size_t len = static_cast<size_t>(size);
      size_t off = 0;
      while (off < len && !feof(stream_) && !ferror(stream_))
        off += fwrite(data + off, 1, len - off, stream_);

      return off == len ? kContinue : kAbort;
    }

  private:
    FILE *stream_;
  };

  void TakeSnapshotByFork(const FunctionCallbackInfo<Value> &args)
  {
    high_resolution_clock::time_point fork_t1 = high_resolution_clock::now();
    Isolate *isolate = args.GetIsolate();
    String::Utf8Value filename(isolate, args[0]);
    isolate->LowMemoryNotification();

    pid_t pid = fork();
    std::cout << "fork pid " << pid << std::endl;
    switch (pid)
    {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
    {
      std::cout << "child process" << getpid() << std::endl;
      std::cout << "child process ----" << getppid() << std::endl;

      std::cout << "isolate" << isolate << std::endl;
      std::cout << "filename" << *filename << std::endl;

      v8::HeapStatistics heap_stats;
      isolate->GetHeapStatistics(&heap_stats);
      std::cout << "Heap size limit: " << heap_stats.heap_size_limit() << std::endl;
      std::cout << "Total heap size: " << heap_stats.total_heap_size() << std::endl;
      std::cout << "Used heap size: " << heap_stats.used_heap_size() << std::endl;
      std::cout << "Physical memory: " << heap_stats.total_physical_size() << std::endl;

      // 在生成快照前先触发一次完整的垃圾回收
      high_resolution_clock::time_point gc_t1 = high_resolution_clock::now();
      std::cout << "Starting manual GC..." << std::endl;

      // isolate->LowMemoryNotification(); // 通知 V8 进行完整的 GC

      // high_resolution_clock::time_point gc_t2 = high_resolution_clock::now();
      // duration<double, std::milli> gc_time = gc_t2 - gc_t1;
      // std::cout << "Manual GC completed in " << gc_time.count() << " milliseconds." << std::endl;

      FILE *fp = fopen(*filename, "w");
      if (fp == NULL)
      {
        std::cout << "fopen failed" << std::endl;
        perror("fopen");
        exit(EXIT_FAILURE);
      }

      // std::cout << "taking snapshot after GC" << std::endl;

      high_resolution_clock::time_point take_snapshot_t1 = high_resolution_clock::now();
      const v8::HeapSnapshot *snapshot = nullptr;
      try
      {
        snapshot = isolate->GetHeapProfiler()->TakeHeapSnapshot(nullptr);
        if (!snapshot)
        {
          std::cout << "Failed to take heap snapshot" << std::endl;
          fclose(fp);
          exit(EXIT_FAILURE);
        }
      }
      catch (const std::exception &e)
      {
        std::cout << "Exception while taking heap snapshot: " << e.what() << std::endl;
        fclose(fp);
        exit(EXIT_FAILURE);
      }

      FileOutputStream stream(fp);
      std::cout << "taking snapshot done" << std::endl;
      snapshot->Serialize(&stream, v8::HeapSnapshot::kJSON);
      std::cout << "taking snapshot done" << std::endl;
      high_resolution_clock::time_point take_snapshot_t2 = high_resolution_clock::now();
      duration<double, std::milli> time_span = take_snapshot_t2 - take_snapshot_t1;
      std::cout << "taking snapshot cost " << time_span.count() << " milliseconds." << std::endl;
      std::cout << "take snapshot done !\n"
                << std::endl;
      fclose(fp);
      const_cast<v8::HeapSnapshot *>(snapshot)->Delete();
      exit(EXIT_SUCCESS);
    }
    default:
      high_resolution_clock::time_point fork_t2 = high_resolution_clock::now();
      duration<double, std::milli> time_span = fork_t2 - fork_t1;
      std::cout << "fork cost " << time_span.count() << " milliseconds." << std::endl;
      break;
    }
  }

  void Initialize(Local<Object> exports, Local<Value> module, Local<Context> context)
  {
    NODE_SET_METHOD(exports, "takeSnapshotByFork", TakeSnapshotByFork);
  }

  NODE_MODULE_CONTEXT_AWARE(NODE_GYP_MODULE_NAME, Initialize)

}
