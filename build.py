from cpt.packager import ConanMultiPackager

builder = ConanMultiPackager(
    # build_policy="missing",
    remotes=(
        "https://api.bintray.com/conan/bincrafters/public-conan",
        # "https://api.bintray.com/conan/timsimpson/richter"
    )
)

if __name__ == "__main__":

    builder.add_common_builds()
    builder.run()
