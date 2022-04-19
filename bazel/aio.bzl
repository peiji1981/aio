"""
warpper bazel cc_xx to modify flags.
"""

load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

WARNING_FLAGS = [
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wno-unused-parameter",
    "-Wnon-virtual-dtor",
] + select({
    "@bazel_tools//src/conditions:darwin": ["-Wunused-const-variable"],
    "//conditions:default": ["-Wunused-const-variable=1"],
})
DEBUG_FLAGS = ["-O0", "-g"]
RELEASE_FLAGS = ["-O2"]
FAST_FLAGS = ["-O1"]

def _aio_linkopts():
    return select({
        "//conditions:default": [],
    })

def _aio_copts():
    return select({
        "@aio//bazel:aio_build_as_release": RELEASE_FLAGS,
        "@aio//bazel:aio_build_as_debug": DEBUG_FLAGS,
        "@aio//bazel:aio_build_as_fast": FAST_FLAGS,
        "//conditions:default": FAST_FLAGS,
    }) + select({
        "//conditions:default": [],
    }) + WARNING_FLAGS

def aio_cc_library(
        linkopts = [],
        copts = [],
        deps = [],
        **kargs):
    cc_library(
        linkopts = linkopts + _aio_linkopts(),
        copts = copts + _aio_copts(),
        deps = deps + [
            "@com_github_gabime_spdlog//:spdlog",
        ],
        **kargs
    )

def aio_cmake_external(**attrs):
    if "generate_args" not in attrs:
        attrs["generate_args"] = ["-GNinja"]
    return cmake(**attrs)

def aio_cc_test(
        linkopts = [],
        copts = [],
        deps = [],
        linkstatic = True,
        **kwargs):
    cc_test(
        # -lm for tcmalloc
        linkopts = linkopts + _aio_linkopts() + ["-lm"],
        copts = copts + _aio_copts(),
        deps = deps + [
            # use tcmalloc same as release bins. make them has same behavior on mem.
            "@com_google_googletest//:gtest_main",
        ],
        # static link for tcmalloc
        linkstatic = True,
        **kwargs
    )
