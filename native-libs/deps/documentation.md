# Overview

The build system consists of recipe files and classes. Recipe files typically contain instructions for building a specific piece of software, while classes typically contain more general information like "how to build something for Android." Both recipes and classes are written in the shell scripting language.

Recipes exist to define "tasks", which are simply functions that can be invoked from the outer build script. Tasks can have dependencies, which the build script will run before running the task itself. The script remembers which tasks have been run successfully, and will not run them again unless the recipe has changed, the task has been cleaned, or a dependency has been updated.

The `inherit` keyword allows a recipe to pull in a class. Any code that exists in the class becomes part of the recipe. Most recipes do not include task definitions themselves, but instead rely on one or more classes to provide common tasks such as `download`, `clean`, or `build-android-arm`. The recipes then customize these tasks with various variable or function definitions. For example, defining a `source` variable tells the `download` task which files to get.

# Running the script

The build script integrates with the standard `make` tool. The command for building a specific library is often just as simple as `make library-name`. The script will automatically download the necessary source files, set up the toolchains, and build the library along with any other libraries it depends on.

To run a specific task defined in a recipe, the syntax is `make recipe.task`. Omitting the task name runs the default task for the recipe, if there is one. It is also possible to specify more than one task at once. Standard make options like `-j` should also work as expected.

To change the directory where the build script places its work, define the environment variable `BUILD_DIR`. For more information, see the "Common directory names" section.

# Writing Recipes

## Keywords

### inherit

The `inherit` keyword pulls in a class, similar to how the shell's  `source` keyword works. The `inherit` keyword provides built-in multiple-include protection to avoid reading a common base class more than once.

### task

The `task` keyword defines a task to be run. The first parameter is the name of the task, and the recipe must provide a shell function with the same name. It is traditional for task names to use hyphens, but these are not valid characters for shell function names. Therefore, the build script automatically converts hyphens to underscores when it looks for the corresponding function.

The remaining parameters to the `task` keyword are a list of dependencies. To specify a task from a specific recipe, use the "recipe.task" naming convention. Dependencies without a recipe name are assumed to come from the current task.

## Common directory names

The build script provides variables with these paths:

* base_dir - The location "make" is run from.
* build_dir - The "build" directory where all work takes place.
* download_dir - The "download" directory where all sources are downloaded.
* work_dir - The per-recipe working directory. The run script switches to this directory before running tasks. The unpack task writes its output here, and the "build" tasks should run inside this directory.
* recipe_dir - The directory where the recipe file is located.

Every recipe has a work directory. The top-level script is responsible for creating this directory, and `cd`'s into the directory prior to running any recipe tasks. The build script also places its own per-recipe state information into the work directory, including logs, task-done state files, and makefile fragments. These files have the extensions `.log`, `.done`, and `.mk`, respectively, so the recipe itself should avoid creating files with similar names. Instead, the recipe should probably create its own working directory under the work directory, where it unpacks and builds its source code.

The per-recipe work directories live inside the build directory. By default, the build directory is called `build` an lives in the same directory as the build script. Setting the `BUILD_DIR` environment variable moves the work directory, making it possible to place the build artifacts in a different location than the build script. The `BUILD_DIR` environment variable must be an absolute path name if it is set; a relative path name will not work.

The `common` class provides a `download` task, which places its output in the `download_dir` directory.

## Common variables

* default - If defined, this sets the default task for the current recipe.

## Specific tasks

### download

The download task expects a `source` variable to be defined, which contains a space-separated list of URL's to download. The class defines a variable called `download_dir`, which contains the location of the downloaded files. To change the download directory, define an environment variable called `DOWNLOAD_DIR`.

If the URL ends with `.git`, the download task will use git to download the source code.

### unpack

The unpack task will search the same `source` variable used by the `download` task for files named `*.tar.*`. If it finds any, it unpacks them inside the work directory.

Likewise, if the unpack task finds any URL's ending with `.git`, it will checkout the contents into the work directory.

### clean

The clean task simply deletes the work directory and all its contents.

### build-*

There are a whole family of `build` tasks for different platform and architecture combinations:

* android-arm
* android-arm64
* android-x86
* android-x86_64
* ios-armv7
* ios-armv7s
* ios-arm64
* ios-i386
* ios-x86_64

All these tasks expect the recipe to define a single `build` function, which should contain platform-independent build instructions. If some platform-specific adjustments are necessary, the build tasks define a `target` variable corresponding to one of the names in the list above.

Besides the `target` variable, the build tasks also define a `cross` variable, which contains a target triplet suitable for passing to GNU autoconf.

After building, the build function should install its files to the location given in the `install_dir` variable.

The `depends` variable lists the names of recipes which must be built first before building the current recipe.

The build function can expect common environment variables such as `CFLAGS` to be correctly configured for the current platform.

### ios-universal

The ios-universal task combines iOS libraries for the various platforms into a single universal library. The list of libraries to combine in this way should be given in the `lib` variable.
