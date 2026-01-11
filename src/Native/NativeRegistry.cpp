#include "NativeRegistry.h"
#include "Modules/Core.h"
#include "Modules/File.h"
#include "Modules/Net.h"
#include "Modules/String.h"
#include "Modules/Crypto.h"
#include "Modules/Sys.h"

void NativeRegistry::registerAll(const std::shared_ptr<Environment>& env) {
    // Core
    env->define("time", std::make_shared<NativeTime>());
    env->define("run", std::make_shared<NativeRun>());
    env->define("env", std::make_shared<NativeEnv>());
    env->define("cwd", std::make_shared<NativeCwd>());
    env->define("cd", std::make_shared<NativeCd>());
    env->define("include", std::make_shared<NativeInclude>());
    env->define("rand", std::make_shared<NativeRand>());
    env->define("sleep", std::make_shared<NativeSleep>());

    // File
    env->define("read_file", std::make_shared<NativeReadFile>());
    env->define("write_file", std::make_shared<NativeWriteFile>());
    env->define("ls", std::make_shared<NativeLs>());

    // String
    env->define("size", std::make_shared<NativeSize>());
    env->define("trim", std::make_shared<NativeTrim>());
    env->define("split", std::make_shared<NativeSplit>());
    env->define("extract", std::make_shared<NativeExtract>());

    // Net
    env->define("connect", std::make_shared<NativeConnect>());
    env->define("send", std::make_shared<NativeSend>());
    env->define("recv", std::make_shared<NativeRecv>());
    env->define("close", std::make_shared<NativeClose>());
    env->define("http_get", std::make_shared<NativeHttpGet>());
    env->define("http_post", std::make_shared<NativeHttpPost>());
    env->define("listen", std::make_shared<NativeListen>());
    env->define("accept", std::make_shared<NativeAccept>());

    // Crypto
    env->define("hex", std::make_shared<NativeHex>());
    env->define("base64_encode", std::make_shared<NativeBase64Encode>());
    env->define("base64_decode", std::make_shared<NativeBase64Decode>());

    // Sys
    env->define("ps", std::make_shared<NativePs>());
    env->define("kill", std::make_shared<NativeKill>());
}
