workspace(name = "aio")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("//bazel:repositories.bzl", "aio_deps")

aio_deps()

load(
    "@rules_foreign_cc//foreign_cc:repositories.bzl",
    "rules_foreign_cc_dependencies",
)

rules_foreign_cc_dependencies(
    register_built_tools = False,
    register_default_tools = False,
    register_preinstalled_tools = True,
)
