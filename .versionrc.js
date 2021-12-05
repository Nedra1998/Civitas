const VERSION_REGEX = /project\(([^)]+)VERSION ([0-9\.]+)([^)]+)\)/s;
const cmake = {
  filename: "CMakeLists.txt",
  updater: {
    readVersion: (contents) => {
      if ((m = contents.match(VERSION_REGEX)) !== null) {
        return m[2];
      }
    },
    writeVersion: (contents, version) => {
      return contents.replace(
        VERSION_REGEX,
        (_m, pre, _ver, post) => `project(${pre}VERSION ${version}${post})`
      );
    },
  },
};

module.exports = {
  bumpFiles: [cmake],
  packageFiles: [cmake],
  types: [
    {
      type: "build",
      hidden: true,
    },
    {
      type: "ci",
      hidden: true,
    },
    {
      type: "chore",
      hidden: true,
    },
    {
      type: "docs",
      section: "Documentation",
    },
    {
      type: "feat",
      section: "Features",
    },
    {
      type: "fix",
      section: "Bux Fixes",
    },
    {
      type: "perf",
      section: "Features",
    },
    {
      type: "refactor",
      hidden: true,
    },
    {
      type: "revert",
      hidden: true,
    },
    {
      type: "style",
      hidden: true,
    },
    {
      type: "test",
      hidden: true,
    },
  ],
};
