load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def aio_deps():
    _rules_foreign_cc()
    _com_github_gflags_gflags()
    _com_google_googletest()
    _com_google_absl()
    _com_github_fmtlib_fmt()
    _com_github_gabime_spdlog()
    _com_github_google_benchmark()

def _com_google_absl():
    maybe(
        http_archive,
        name = "com_google_absl",
        sha256 = "59b862f50e710277f8ede96f083a5bb8d7c9595376146838b9580be90374ee1f",
        type = "tar.gz",
        strip_prefix = "abseil-cpp-20210324.2",
        urls = [
            "https://github.com/abseil/abseil-cpp/archive/refs/tags/20210324.2.tar.gz",
        ],
    )

def _com_github_fmtlib_fmt():
    maybe(
        http_archive,
        name = "com_github_fmtlib_fmt",
        strip_prefix = "fmt-8.0.1",
        sha256 = "b06ca3130158c625848f3fb7418f235155a4d389b2abc3a6245fb01cb0eb1e01",
        build_file = "@aio//bazel:fmtlib.BUILD",
        urls = [
            "https://github.com/fmtlib/fmt/archive/refs/tags/8.0.1.tar.gz",
        ],
    )

def _com_github_gabime_spdlog():
    maybe(
        http_archive,
        name = "com_github_gabime_spdlog",
        strip_prefix = "spdlog-1.9.1",
        type = "tar.gz",
        sha256 = "9a452cfa24408baccc9b2bc2d421d68172a7630c99e9504a14754be840d31a62",
        build_file = "@aio//bazel:spdlog.BUILD",
        urls = [
            "https://github.com/gabime/spdlog/archive/refs/tags/v1.9.1.tar.gz",
        ],
    )

def _com_google_googletest():
    maybe(
        http_archive,
        name = "com_google_googletest",
        sha256 = "b4870bf121ff7795ba20d20bcdd8627b8e088f2d1dab299a031c1034eddc93d5",
        type = "tar.gz",
        strip_prefix = "googletest-release-1.11.0",
        urls = [
            "https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz",
        ],
    )

def _com_github_google_benchmark():
    maybe(
        http_archive,
        name = "com_github_google_benchmark",
        type = "tar.gz",
        strip_prefix = "benchmark-1.5.5",
        sha256 = "3bff5f237c317ddfd8d5a9b96b3eede7c0802e799db520d38ce756a2a46a18a0",
        urls = [
            "https://github.com/google/benchmark/archive/refs/tags/v1.5.5.tar.gz",
        ],
    )

def _com_github_gflags_gflags():
    maybe(
        http_archive,
        name = "com_github_gflags_gflags",
        strip_prefix = "gflags-2.2.2",
        sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
        type = "tar.gz",
        urls = [
            "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
        ],
    )

def _rules_foreign_cc():
    maybe(
        http_archive,
        name = "rules_foreign_cc",
        sha256 = "bcd0c5f46a49b85b384906daae41d277b3dc0ff27c7c752cc51e43048a58ec83",
        strip_prefix = "rules_foreign_cc-0.7.1",
        urls = [
            "https://github.com/bazelbuild/rules_foreign_cc/archive/0.7.1.tar.gz",
        ],
    )
