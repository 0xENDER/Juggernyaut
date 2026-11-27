/**
 * @brief
 * Per-thread active compiler task breadcrumb for fatal crash (ICE) diagnostics
**/

#include "include/common/crashBreadcrumb.hpp"

// C++ headers
#include <atomic>
#include <csignal>
#include <cstdlib>
#include <exception>

#ifdef _WIN32
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   ifndef NOMINMAX
#       define NOMINMAX
#   endif
#   include <windows.h>
#else
#   include <unistd.h>
#endif

#if defined(__SANITIZE_ADDRESS__)
#   define JUG_ASAN_ACTIVE 1
#elif defined(__has_feature)
#   if __has_feature(address_sanitizer)
#       define JUG_ASAN_ACTIVE 1
#   endif
#endif

#ifdef JUG_ASAN_ACTIVE
extern "C" void __sanitizer_set_death_callback(void (*callback)(void));
#endif

namespace Common::Debug {
    namespace {
        thread_local ActiveTask threadTask{};
        std::atomic_flag handlerInstalled = ATOMIC_FLAG_INIT;
        std::atomic_flag crashBannerPrinted = ATOMIC_FLAG_INIT;

        // Bounded strlen so a corrupted pointer cannot scan unbounded memory
        [[nodiscard]] size_t boundedLen(const char *str, size_t maxChars = 4096u) noexcept {
            if (str == nullptr) {
                return 0u;
            }
            size_t len = 0u;
            while (len < maxChars && str[len] != '\0') {
                ++len;
            }
            return len;
        }

        // Async-signal-safe unbuffered write to stderr (zero heap, zero CRT locks)
        void writeRawStderr(const char *data, size_t len) noexcept {
            if (data == nullptr || len == 0u) {
                return;
            }
        #ifdef _WIN32
            HANDLE errHandle = ::GetStdHandle(STD_ERROR_HANDLE);
            if (errHandle != nullptr && errHandle != INVALID_HANDLE_VALUE) {
                DWORD written = 0;
                (void)::WriteFile(errHandle, data, static_cast<DWORD>(len), &written, nullptr);
            }
        #else
            size_t offset = 0u;
            while (offset < len) {
                const ssize_t n = ::write(STDERR_FILENO, data + offset, len - offset);
                if (n <= 0) {
                    break;
                }
                offset += static_cast<size_t>(n);
            }
        #endif
        }

        void writeLiteral(const char *text) noexcept {
            writeRawStderr(text, boundedLen(text));
        }

        void emitCrashBannerOnce(const char *reason) noexcept {
            if (crashBannerPrinted.test_and_set(std::memory_order_acq_rel)) {
                return; // Prevent recursive or duplicate banners
            }

            const ActiveTask snap = threadTask;
            writeLiteral("\n================================================================================\n");
            writeLiteral("[FATAL ICE] Juggernyaut terminated unexpectedly: ");
            writeLiteral(reason != nullptr ? reason : "Unknown fatal error");
            writeLiteral("\n  Active Stage : ");
            writeLiteral(snap.stage != nullptr ? snap.stage : "Unknown");
            if (snap.detail != nullptr && snap.detail[0] != '\0') {
                writeLiteral("\n  Active Target: ");
                writeLiteral(snap.detail);
            }
            writeLiteral("\n================================================================================\n");
        }

        [[nodiscard]] const char* signalReason(int sig) noexcept {
            switch (sig) {
                case SIGABRT: return "SIGABRT (Abort / assertion failure)";
                case SIGFPE:  return "SIGFPE (Arithmetic exception)";
                case SIGILL:  return "SIGILL (Illegal instruction)";
                case SIGSEGV: return "SIGSEGV (Segmentation fault)";
            #ifdef SIGBUS
                case SIGBUS:  return "SIGBUS (Bus error)";
            #endif
                default:      return "Fatal signal";
            }
        }

    #ifdef _WIN32
        [[nodiscard]] const char* sehReason(DWORD code) noexcept {
            switch (code) {
                case EXCEPTION_ACCESS_VIOLATION:      return "EXCEPTION_ACCESS_VIOLATION";
                case EXCEPTION_STACK_OVERFLOW:        return "EXCEPTION_STACK_OVERFLOW";
                case EXCEPTION_INT_DIVIDE_BY_ZERO:    return "EXCEPTION_INT_DIVIDE_BY_ZERO";
                case EXCEPTION_ILLEGAL_INSTRUCTION:   return "EXCEPTION_ILLEGAL_INSTRUCTION";
                case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
                default:                              return "Unhandled Win32 SEH exception";
            }
        }

        LONG WINAPI winSehHandler(EXCEPTION_POINTERS *info) noexcept {
            const DWORD code = (info != nullptr && info->ExceptionRecord != nullptr)
                ? info->ExceptionRecord->ExceptionCode
                : 0u;
            emitCrashBannerOnce(sehReason(code));
            return EXCEPTION_CONTINUE_SEARCH;
        }

        void winAbortHandler(int sig) noexcept {
            emitCrashBannerOnce(signalReason(sig));
            std::signal(sig, SIG_DFL);
            std::raise(sig);
        }
    #else
        void posixSignalHandler(int sig) noexcept {
            emitCrashBannerOnce(signalReason(sig));

            struct sigaction dfl{};
            dfl.sa_handler = SIG_DFL;
            sigemptyset(&dfl.sa_mask);
            (void)::sigaction(sig, &dfl, nullptr);
            (void)::raise(sig);
        }

        void registerPosixSignal(int sig) noexcept {
            struct sigaction sa{};
            sa.sa_handler = &posixSignalHandler;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESETHAND | SA_NODEFER;
            (void)::sigaction(sig, &sa, nullptr);
        }
    #endif

    #ifdef JUG_ASAN_ACTIVE
        void sanitizerDeathCallback() noexcept {
            emitCrashBannerOnce("Sanitizer fatal error");
        }
    #endif

        [[noreturn]] void terminateHandler() noexcept {
            emitCrashBannerOnce("Uncaught C++ exception / std::terminate()");
            std::abort();
        }
    }

    ActiveTask& currentActiveTask() noexcept {
        return threadTask;
    }

    void installCrashBreadcrumbHandler() noexcept {
        if (handlerInstalled.test_and_set(std::memory_order_acq_rel)) {
            return;
        }

        std::set_terminate(&terminateHandler);

    #ifdef JUG_ASAN_ACTIVE
        __sanitizer_set_death_callback(&sanitizerDeathCallback);
    #endif

    #ifdef _WIN32
        (void)::SetUnhandledExceptionFilter(&winSehHandler);
        (void)std::signal(SIGABRT, &winAbortHandler);
    #else
        registerPosixSignal(SIGABRT);
        registerPosixSignal(SIGFPE);
        registerPosixSignal(SIGILL);
        #ifndef JUG_ASAN_ACTIVE
        // Leave SIGSEGV/SIGBUS to ASan when sanitizer shadow memory is active
        registerPosixSignal(SIGSEGV);
        #ifdef SIGBUS
        registerPosixSignal(SIGBUS);
        #endif
        #endif
    #endif
    }
}
