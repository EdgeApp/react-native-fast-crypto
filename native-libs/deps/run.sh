#!/bin/bash

set -e

# Command-line options:
if [ $# -ne 2 ]; then
    echo "Usage:"
    echo "    $0 recipe task"
fi
recipe=$1
task=$2

# Essential directories:
base_dir=$(pwd)

build_dir=${BUILD_DIR:-build}
mkdir -p $build_dir
build_dir=$(cd $build_dir; pwd)

work_dir=$build_dir/$recipe
mkdir -p $work_dir

################################################################################
# Utility functions:

# Returns true if the string is found in a list.
# $1 the string to search for
# $@ the list to search
contains() {
    local string=$1 s
    shift
    for s; do
        [ $s = $string ] && return 0
    done
    return 1
}

# Takes an absolute path, and makes it relative to the base directory.
# For example, `$(relative $build_dir/foo)` might return `build/foo`.
relative() {
    echo ${1#$base_dir/}
}

# Returns the name of a recipe source file.
# $1 recipe name
recipe_file() {
    echo $base_dir/recipes/$1/$1.recipe
}

# Returns the name of a class source file.
# $1 class name
class_file() {
    echo $base_dir/classes/$1
}

# Returns the name of the makefile fragment for the given recipe.
# $1 recipe name
task_file() {
    echo $build_dir/$1/tasks.mk
}

# Returns the name of the state file associated with the given task.
# $1 recipe name
# $2 task name
done_file() {
    echo $build_dir/$1/$2.done
}

# Returns the name of the state file associated with the given task.
# $1 recipe and task name, in dot format
done_file_dot() {
    local task=$1
    local raw_task=${task#*.} raw_recipe
    if [ $raw_task = $task ]; then
        raw_recipe=$recipe
    else
        raw_recipe=${task%%.*}
    fi
    echo $(done_file $raw_recipe $raw_task)
}

################################################################################
# Recipe commands:

# Pulls in a class file. Like "source", but with double-include guards.
# $1 class name
recipe_files=$(relative $(recipe_file $recipe))
inherit() {
    local class_file=$(class_file $1)
    if ! contains $(relative $class_file) $recipe_files; then
        recipe_files="$recipe_files $(relative $class_file)"
        source $class_file
    fi
}

# Defines a task to be performed
# $1 the name of the new task
# $@ any prerequisites for the task. If a prerequisite comes from another
# recipe, it should be prefixed with the recipe name and a dot.
recipe_tasks=write-tasks
task() {
    local task=$1
    if contains $task $recipe_tasks; then
        echo >&2 "error: Task '$task' defined more than once in recipe '$recipe'"
        exit 1
    fi
    recipe_tasks="$recipe_tasks $task"
}

# The one built-in task, which is needed to bootstrap the whole thing.
write_tasks() {
    out=$(task_file $recipe)
    echo >$out "# Generated from $(relative $(recipe_file $recipe))"

    # The write-tasks rule itself:
    echo >>$out "$recipe.write-tasks: $(relative $(task_file $recipe))"
    echo >>$out "$(relative $(task_file $recipe)): $recipe_files run.sh"
    echo >>$out "	@./run.sh $recipe write-tasks" # Caution - literal tab character!

    # Redefine the task function to actually write tasks to disk:
    recipe_files=$(relative $(recipe_file $recipe))
    task() {
        local task=$1
        shift 1

        local dep deps="\$(wildcard $(relative $recipe_dir)/*)"
        for dep; do
            deps="$deps $(relative $(done_file_dot $dep))"
        done

        echo >>$out
        echo >>$out "$recipe.$task: $(relative $(done_file $recipe $task))"
        echo >>$out "$(relative $(done_file $recipe $task)): $deps"
        echo >>$out "	@./run.sh $recipe $task" # Caution - literal tab character!
    }
    source $(recipe_file $recipe)

    # The default rule, if any:
    if [ -n "$default" ]; then
        echo >>$out
        echo >>$out "$recipe: $recipe.$default"
    fi
}

################################################################################
# Task runner:

recipe_dir=$(dirname $(recipe_file $recipe))
log=$work_dir/$task.log
echo "Running $recipe.$task"

source $(recipe_file $recipe)

if ! contains $task $recipe_tasks; then
    echo >&2 "error: Task '$task' is not defined in recipe '$recipe'"
    exit 1
fi

# Run the task:
set +e
(
    set -e
    cd $work_dir
    $(echo $task | sed s/-/_/g)
) > $log 2>&1
status=$?
set -e

# Handle errors:
if [ $status -ne 0 ]; then
    echo "Task failed (see $(relative $log) for full logs):"
    echo "================================"
    tail $log
    echo "================================"
    exit $status
fi

# The clean rule deletes the build directory, so only update the
# task-done file if the directory still exists:
if [ -d $work_dir ]; then
    touch $(done_file $recipe $task)
fi
