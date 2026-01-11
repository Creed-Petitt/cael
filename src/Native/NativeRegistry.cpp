#include "NativeRegistry.h"
#include "Modules/Core.h"
#include "Modules/File.h"
#include "Modules/Net.h"
#include "Modules/String.h"

void NativeRegistry::registerAll(const std::shared_ptr<Environment>& env) {
    // Core
    env->define("clock", std::make_shared<NativeClock>());
    env->define("run", std::make_shared<NativeRun>());
    env->define("env", std::make_shared<NativeEnv>());
    env->define("cwd", std::make_shared<NativeCwd>());
    env->define("cd", std::make_shared<NativeCd>());

    // File
    env->define("read_file", std::make_shared<NativeReadFile>());
    env->define("write_file", std::make_shared<NativeWriteFile>());

    // String
    env->define("size", std::make_shared<NativeSize>());
    env->define("trim", std::make_shared<NativeTrim>());
    env->define("split", std::make_shared<NativeSplit>());

    // Net
    env->define("connect", std::make_shared<NativeConnect>());
    env->define("send", std::make_shared<NativeSend>());
    env->define("recv", std::make_shared<NativeRecv>());
    env->define("close", std::make_shared<NativeClose>());
}
